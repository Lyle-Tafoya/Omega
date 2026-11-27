/*
SDL2 frontend for Omega: tile renderer, text panels, gamepad controls,
radial command menu, on-screen keyboard and option picker.

This file is part of Omega (GPL-3.0-or-later).
*/
#include "frontend.hpp"
#include "../omega_ui.hpp"

#include <SDL.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace
{
constexpr int ESC = 27;

// --------------------------------------------------------------- state ----
SDL_Window *window     = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *tex_tiles = nullptr, *tex_font = nullptr, *tex_disc = nullptr;
int font_w = 19, font_h = 38;   // glyph cell in font texture
int cw = 10, ch = 20;           // text cell on screen
int win_w = 1280, win_h = 800;
int hint_h = 30;
std::string asset_dir;

const std::array<int, 4> TILE_SIZES = {32, 48, 24, 64};
int tile_zoom = 0;

struct sprite
{
  std::vector<SDL_Rect> frames;
};
struct layer
{
  int sprite = -1;
  std::string ref; // "@floor" style reference
  bool tinted = false;
  uint8_t tr = 255, tg = 255, tb = 255;
};
std::vector<sprite> sprites;
std::unordered_map<std::string, int> sprite_index;
std::unordered_map<std::string, std::vector<layer>> tile_keys;
std::unordered_map<std::string, int> variant_counts;

std::deque<int> keyq;
std::deque<MEVENT> mouseq;
std::vector<ui_mode> modes;

bool dirty            = true;
Uint32 last_present   = 0;
Uint32 flash_until    = 0;
Uint32 anim_clock     = 0;

SDL_GameController *pad = nullptr;
enum vbtn
{
  VB_LT = SDL_CONTROLLER_BUTTON_MAX,
  VB_RT,
  VB_COUNT
};
std::array<bool, VB_COUNT> held{};
bool trig_l = false, trig_r = false;

// direction input (dpad/stick) with debounce and repeat
int cur_dir         = -1;
Uint32 dir_fire_at  = 0;
bool dir_fired      = false;
int stick_x = 0, stick_y = 0;

// overlays
bool radial_open = false;
int radial_page  = 0;
int radial_sel   = -1;
bool osk_open    = false;
bool osk_auto    = false; // opened by a text mode
int osk_r = 1, osk_c = 0;
bool osk_shift = false;

struct option
{
  int key;
  std::string label;
  int row = -1; // screen row, -1 = popup only
};
std::vector<option> options;
int opt_sel = 0;
bool options_from_popup = false;

// player facing
int last_px = -1, last_py = -1;
char facing = 's';
std::string player_set = "warrior";

// ---------------------------------------------------------- directions ----
// clockwise from up, matching the radial slices
const int DX[8]       = {0, 1, 1, 1, 0, -1, -1, -1};
const int DY[8]       = {-1, -1, 0, 1, 1, 1, 0, -1};
const char VI[8]      = {'k', 'u', 'l', 'n', 'j', 'b', 'h', 'y'};

int dir_from(int dx, int dy)
{
  for(int i = 0; i < 8; ++i)
  {
    if(DX[i] == dx && DY[i] == dy)
    {
      return i;
    }
  }
  return -1;
}

ui_mode mode()
{
  return modes.empty() ? UI_PROMPT : modes.back();
}

void push_key(int k)
{
  keyq.push_back(k);
}

// -------------------------------------------------------------- assets ----
SDL_Texture *load_rgba(const std::string &path)
{
  std::ifstream in(path, std::ios::binary);
  if(!in)
  {
    fprintf(stderr, "omega: missing asset %s\n", path.c_str());
    return nullptr;
  }
  uint32_t w = 0, h = 0;
  in.read(reinterpret_cast<char *>(&w), 4);
  in.read(reinterpret_cast<char *>(&h), 4);
  std::vector<uint8_t> px(static_cast<size_t>(w) * h * 4);
  in.read(reinterpret_cast<char *>(px.data()), static_cast<std::streamsize>(px.size()));
  SDL_Surface *s = SDL_CreateRGBSurfaceWithFormatFrom(px.data(), w, h, 32, w * 4, SDL_PIXELFORMAT_RGBA32);
  SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
  return t;
}

std::vector<std::string> split(const std::string &s, char sep)
{
  std::vector<std::string> out;
  std::string cur;
  for(char c : s)
  {
    if(c == sep)
    {
      out.push_back(cur);
      cur.clear();
    }
    else
    {
      cur += c;
    }
  }
  out.push_back(cur);
  return out;
}

void load_tile_index(const std::string &path)
{
  std::ifstream in(path);
  std::string line;
  std::vector<std::pair<std::string, std::vector<std::string>>> pending;
  while(std::getline(in, line))
  {
    if(line.empty() || line[0] == '#')
    {
      continue;
    }
    auto f = split(line, '\t');
    if(f[0] == "sprite" && f.size() >= 3)
    {
      sprite s;
      std::istringstream ss(f[2]);
      int x, y;
      while(ss >> x >> y)
      {
        s.frames.push_back({x, y, 16, 16});
      }
      sprite_index[f[1]] = static_cast<int>(sprites.size());
      sprites.push_back(s);
    }
    else if(f[0] == "key" && f.size() >= 3)
    {
      pending.push_back({f[1], std::vector<std::string>(f.begin() + 2, f.end())});
    }
    else if(f[0] == "variants" && f.size() >= 3)
    {
      variant_counts[f[1]] = std::stoi(f[2]);
    }
  }
  for(auto &[key, specs] : pending)
  {
    std::vector<layer> layers;
    for(auto &spec : specs)
    {
      layer l;
      if(!spec.empty() && spec[0] == '@')
      {
        l.ref = spec.substr(1);
      }
      else
      {
        auto at        = spec.find('@');
        std::string nm = spec.substr(0, at);
        auto it        = sprite_index.find(nm);
        if(it == sprite_index.end())
        {
          continue;
        }
        l.sprite = it->second;
        if(at != std::string::npos && spec.size() >= at + 7)
        {
          unsigned rgb = std::stoul(spec.substr(at + 1, 6), nullptr, 16);
          l.tinted     = true;
          l.tr         = (rgb >> 16) & 0xff;
          l.tg         = (rgb >> 8) & 0xff;
          l.tb         = rgb & 0xff;
        }
      }
      layers.push_back(l);
    }
    tile_keys[key] = layers;
  }
}

SDL_Texture *make_disc()
{
  const int n = 64;
  std::vector<uint32_t> px(n * n);
  for(int y = 0; y < n; ++y)
  {
    for(int x = 0; x < n; ++x)
    {
      float dx = x + 0.5f - n / 2.0f, dy = y + 0.5f - n / 2.0f;
      float d  = std::sqrt(dx * dx + dy * dy);
      float a  = std::clamp(n / 2.0f - d, 0.0f, 1.0f);
      px[y * n + x] = (static_cast<uint32_t>(a * 255) << 24) | 0xffffff;
    }
  }
  SDL_Surface *s = SDL_CreateRGBSurfaceWithFormatFrom(px.data(), n, n, 32, n * 4, SDL_PIXELFORMAT_ARGB8888);
  SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
  SDL_SetTextureScaleMode(t, SDL_ScaleModeLinear);
  return t;
}

// ------------------------------------------------------------- layout ----
void compute_grid(int &lines, int &cols)
{
  SDL_GetRendererOutputSize(renderer, &win_w, &win_h);
  int want = win_h / 40;
  if(const char *e = SDL_getenv("OMEGA_TEXT_SIZE"))
  {
    want = std::atoi(e);
  }
  ch = std::clamp(want, 12, 48);
  cw = std::max(6, (ch * font_w + font_h / 2) / font_h);
  hint_h = ch + ch / 2;
  // keep at least 80x24 even on small windows
  while(ch > 8 && ((win_w / cw) < 80 || ((win_h - hint_h) / ch) < 24))
  {
    --ch;
    cw     = std::max(5, (ch * font_w + font_h / 2) / font_h);
    hint_h = ch + ch / 2;
  }
  cols  = std::max(80, win_w / cw);
  lines = std::max(24, (win_h - hint_h) / ch);
}

// ------------------------------------------------------------ drawing ----
void fill(int x, int y, int w, int h, SDL_Color c)
{
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
  SDL_Rect r{x, y, w, h};
  SDL_RenderFillRect(renderer, &r);
}

void outline(int x, int y, int w, int h, SDL_Color c, int t = 2)
{
  fill(x, y, w, t, c);
  fill(x, y + h - t, w, t, c);
  fill(x, y, t, h, c);
  fill(x + w - t, y, t, h, c);
}

void glyph(int x, int y, int w, int h, unsigned char c, SDL_Color col, bool bold = false)
{
  if(c < 32 || c > 126 || c == ' ')
  {
    return;
  }
  SDL_Rect src{(c - 32) * font_w, bold ? font_h : 0, font_w, font_h};
  SDL_Rect dst{x, y, w, h};
  SDL_SetTextureColorMod(tex_font, col.r, col.g, col.b);
  SDL_SetTextureAlphaMod(tex_font, col.a);
  SDL_RenderCopy(renderer, tex_font, &src, &dst);
}

int text(int x, int y, const std::string &s, SDL_Color col, int size = 0, bool bold = false)
{
  int h = size ? size : ch;
  int w = std::max(5, (h * font_w + font_h / 2) / font_h);
  for(char c : s)
  {
    glyph(x, y, w, h, static_cast<unsigned char>(c), col, bold);
    x += w;
  }
  return x;
}

int text_width(const std::string &s, int size = 0)
{
  int h = size ? size : ch;
  int w = std::max(5, (h * font_w + font_h / 2) / font_h);
  return static_cast<int>(s.size()) * w;
}

void disc(int cx_, int cy_, int r, SDL_Color c)
{
  SDL_SetTextureColorMod(tex_disc, c.r, c.g, c.b);
  SDL_SetTextureAlphaMod(tex_disc, c.a);
  SDL_Rect d{cx_ - r, cy_ - r, r * 2, r * 2};
  SDL_RenderCopy(renderer, tex_disc, nullptr, &d);
}

SDL_Color rgb(int idx, uint8_t a = 255)
{
  sdl_rgb c = sdlc_color(idx);
  return {c.r, c.g, c.b, a};
}

void cell_colors(chtype c, SDL_Color &fg, SDL_Color &bg, bool &has_bg)
{
  int f, b;
  sdlc_pair(PAIR_NUMBER(c), f, b);
  if(c & A_BOLD)
  {
    f |= 8;
  }
  if(c & (A_REVERSE | A_STANDOUT))
  {
    std::swap(f, b);
  }
  fg     = rgb(f);
  bg     = rgb(b);
  has_bg = b != 0;
}

// button badge, returns x after it
int badge(int x, int y, const std::string &name, int h)
{
  SDL_Color col{90, 90, 100, 255};
  if(name == "A") col = {60, 170, 70, 255};
  else if(name == "B") col = {200, 60, 60, 255};
  else if(name == "X") col = {60, 110, 210, 255};
  else if(name == "Y") col = {215, 180, 40, 255};
  int r = h / 2;
  if(name.size() == 1)
  {
    disc(x + r, y + r, r, col);
    int tw = text_width(name, h - 4);
    text(x + r - tw / 2, y + 2, name, {255, 255, 255, 255}, h - 4, true);
    return x + h;
  }
  int tw = text_width(name, h - 6);
  int w  = tw + h;
  disc(x + r, y + r, r, col);
  disc(x + w - r, y + r, r, col);
  fill(x + r, y, w - h, h, col);
  text(x + h / 2, y + 3, name, {255, 255, 255, 255}, h - 6, true);
  return x + w;
}

// ------------------------------------------------------------- tiles ----
const std::vector<layer> *find_key(const std::string &k)
{
  auto it = tile_keys.find(k);
  return it == tile_keys.end() ? nullptr : &it->second;
}

const std::vector<layer> *resolve(const std::string &key, const std::string &env)
{
  if(!env.empty())
  {
    if(auto *l = find_key(key + "@" + env))
    {
      return l;
    }
  }
  return find_key(key);
}

void draw_layers(const std::vector<layer> &ls, const std::string &env, int x, int y, int sz, int depth = 0)
{
  for(const layer &l : ls)
  {
    if(!l.ref.empty())
    {
      if(depth < 3)
      {
        if(auto *r = resolve(l.ref, env))
        {
          draw_layers(*r, env, x, y, sz, depth + 1);
        }
      }
      continue;
    }
    const sprite &s = sprites[l.sprite];
    int nf          = static_cast<int>(s.frames.size());
    int f           = nf > 1 ? static_cast<int>((anim_clock / (nf > 2 ? 180 : 500)) % nf) : 0;
    SDL_Rect dst{x, y, sz, sz};
    if(l.tinted)
    {
      SDL_SetTextureColorMod(tex_tiles, l.tr, l.tg, l.tb);
    }
    SDL_RenderCopy(renderer, tex_tiles, &s.frames[f], &dst);
    if(l.tinted)
    {
      SDL_SetTextureColorMod(tex_tiles, 255, 255, 255);
    }
  }
}

std::string pool_key(const std::string &k)
{
  // "pool:<group>:<name>" -> "<group>#n"
  auto p2 = k.find(':', 5);
  if(p2 == std::string::npos)
  {
    return k;
  }
  std::string g = k.substr(5, p2 - 5);
  int n         = variant_counts.count(g) ? variant_counts[g] : 1;
  uint32_t h    = 2166136261u;
  for(char c : k.substr(p2 + 1))
  {
    h = (h ^ static_cast<uint8_t>(c)) * 16777619u;
  }
  return g + "#" + std::to_string(h % std::max(1, n));
}

bool joins(const std::string &family, const std::string &k)
{
  if(family == "wall")
  {
    return k == "wall" || k == "open_door" || k == "closed_door" || k == "portcullis";
  }
  if(family == "c_road")
  {
    return k == "c_road" || k == "c_city" || k == "c_village" || k == "c_castle" || k == "c_temple" ||
           k == "c_pass";
  }
  if(family == "c_river")
  {
    return k == "c_river" || k == "c_chaos_sea";
  }
  return false;
}

std::string autotile_suffix(const std::string &family, bool u, bool d, bool l, bool r)
{
  if(family == "wall")
  {
    if(!u && !d && !l && !r) return "flat";
    if(!u && !d) return "left right";
    if(!l && !r) return "up down";
    std::string s;
    if(l) s += "left ";
    if(r) s += "right ";
    if(u) s += "up ";
    if(d) s += "down ";
    s.pop_back();
    return s;
  }
  // trails and rivers
  if(!u && !d) return "left right";
  if(!l && !r) return "up down";
  std::string s;
  if(u) s += "up ";
  if(d) s += "down ";
  if(l) s += "left ";
  if(r) s += "right ";
  s.pop_back();
  return s;
}

struct map_view
{
  bool active = false;
  SDL_Rect rect{};
  int tile    = 32;
  float cam_x = 0, cam_y = 0; // window cell at the viewport's top-left
  const WINDOW *w = nullptr;
};
map_view view;

// The game caps the map at 64 columns; on wide screens we push the sidebar to
// the right edge and give the tile view the space in between.
int side_shift(const sdl_screen &scr)
{
  if(!scr.map_win)
  {
    return 0;
  }
  return std::max(0, scr.cols - (scr.map_win->w + 42));
}

int cell_x(const sdl_screen &scr, int r, int c)
{
  const WINDOW *w = scr.map_win;
  if(view.active && w && r >= w->by && r < w->by + w->h && c >= w->bx + w->w)
  {
    return (c + side_shift(scr)) * cw;
  }
  return c * cw;
}

void draw_map(const sdl_screen &scr)
{
  view.active = false;
  const WINDOW *w = scr.map_win;
  if(!w)
  {
    return;
  }
  fe_map_state ms;
  omega_map_state(ms);
  if(!ms.valid)
  {
    return;
  }
  view.active = true;
  view.w      = w;
  view.rect   = {w->bx * cw, w->by * ch, (w->w + side_shift(scr)) * cw, w->h * ch};
  view.tile   = std::max(16, TILE_SIZES[tile_zoom] * ch / 20);
  int sz      = view.tile;

  // camera target: the targeting cursor, else the player
  float tx = static_cast<float>(ms.px - ms.hoff), ty = static_cast<float>(ms.py - ms.voff);
  ui_mode m = mode();
  if((m == UI_TARGET || m == UI_LOOK) && scr.cursor_win == w)
  {
    tx = static_cast<float>(scr.cursor_x - w->bx);
    ty = static_cast<float>(scr.cursor_y - w->by);
  }
  float vis_w = static_cast<float>(view.rect.w) / sz, vis_h = static_cast<float>(view.rect.h) / sz;
  auto cam    = [](float t, float vis, int extent) {
    if(vis >= extent)
    {
      return (extent - vis) / 2.0f;
    }
    return std::clamp(t + 0.5f - vis / 2.0f, 0.0f, extent - vis);
  };
  view.cam_x = cam(tx, vis_w, w->w);
  view.cam_y = cam(ty, vis_h, w->h);

  // facing from movement
  if(ms.px != last_px || ms.py != last_py)
  {
    if(last_px >= 0 && std::abs(ms.px - last_px) <= 1 && std::abs(ms.py - last_py) <= 1)
    {
      if(ms.px > last_px) facing = 'e';
      else if(ms.px < last_px) facing = 'w';
      else if(ms.py > last_py) facing = 's';
      else if(ms.py < last_py) facing = 'n';
    }
    last_px = ms.px;
    last_py = ms.py;
  }

  SDL_RenderSetClipRect(renderer, &view.rect);
  fill(view.rect.x, view.rect.y, view.rect.w, view.rect.h, {12, 10, 16, 255});

  int x0 = static_cast<int>(std::floor(view.cam_x)) - 1, y0 = static_cast<int>(std::floor(view.cam_y)) - 1;
  int x1 = static_cast<int>(std::ceil(view.cam_x + vis_w)) + 1, y1 = static_cast<int>(std::ceil(view.cam_y + vis_h)) + 1;
  int gw = x1 - x0 + 1, gh = y1 - y0 + 1;
  std::vector<std::vector<std::string>> grid(static_cast<size_t>(gw) * gh);
  auto at = [&](int x, int y) -> std::vector<std::string> & { return grid[(y - y0) * gw + (x - x0)]; };
  for(int y = y0; y <= y1; ++y)
  {
    for(int x = x0; x <= x1; ++x)
    {
      if(x < 0 || y < 0 || x >= w->w || y >= w->h)
      {
        continue;
      }
      chtype c = w->cells[y * w->w + x];
      if((c & A_CHARTEXT) == ' ')
      {
        continue;
      }
      omega_cell_keys(x + ms.hoff, y + ms.voff, c, at(x, y));
    }
  }
  auto base = [&](int x, int y) -> std::string {
    if(x < x0 || y < y0 || x > x1 || y > y1)
    {
      return {};
    }
    auto &v = at(x, y);
    return v.empty() ? std::string() : v.front();
  };

  for(int y = y0; y <= y1; ++y)
  {
    for(int x = x0; x <= x1; ++x)
    {
      if(x < 0 || y < 0 || x >= w->w || y >= w->h)
      {
        continue;
      }
      auto &keys = at(x, y);
      if(keys.empty())
      {
        continue;
      }
      int px = view.rect.x + static_cast<int>(std::lround((x - view.cam_x) * sz));
      int py = view.rect.y + static_cast<int>(std::lround((y - view.cam_y) * sz));
      chtype shown = w->cells[y * w->w + x];
      for(const std::string &k0 : keys)
      {
        std::string k = k0;
        if(k == "glyph")
        {
          SDL_Color fg, bg;
          bool hb;
          cell_colors(shown, fg, bg, hb);
          int gw2 = sz * font_w / font_h;
          glyph(px + (sz - gw2) / 2, py, gw2, sz, shown & A_CHARTEXT, fg, true);
          continue;
        }
        if(k.rfind("pool:", 0) == 0)
        {
          k = pool_key(k);
        }
        if(k == "player")
        {
          std::string pk = "player:" + player_set + ":" + std::string(1, facing);
          if(auto *l = find_key(pk))
          {
            draw_layers(*l, ms.env, px, py, sz);
            continue;
          }
        }
        if(k == "wall")
        {
          // solid rock deep inside the walls stays dark; only faces are drawn
          bool interior = true;
          for(int dy = -1; dy <= 1 && interior; ++dy)
          {
            for(int dx = -1; dx <= 1; ++dx)
            {
              std::string n = base(x + dx, y + dy);
              if(!(n.empty() || n == "wall"))
              {
                interior = false;
                break;
              }
            }
          }
          if(interior)
          {
            continue;
          }
          std::string suf = autotile_suffix(k, joins(k, base(x, y - 1)), joins(k, base(x, y + 1)),
                                            joins(k, base(x - 1, y)), joins(k, base(x + 1, y)));
          const std::vector<layer> *l = find_key(k + "@" + ms.env + "#" + suf);
          if(!l)
          {
            l = find_key(k + "#" + suf);
          }
          if(l)
          {
            draw_layers(*l, ms.env, px, py, sz);
            continue;
          }
        }
        if(auto *l = resolve(k, ms.env))
        {
          draw_layers(*l, ms.env, px, py, sz);
        }
        else
        {
          SDL_Color fg, bg;
          bool hb;
          cell_colors(shown, fg, bg, hb);
          int gw2 = sz * font_w / font_h;
          glyph(px + (sz - gw2) / 2, py, gw2, sz, shown & A_CHARTEXT, fg, true);
        }
      }
    }
  }

  // targeting cursor
  if(scr.cursor_visibility && scr.cursor_win == w)
  {
    int cx_ = scr.cursor_x - w->bx, cy_ = scr.cursor_y - w->by;
    int px  = view.rect.x + static_cast<int>(std::lround((cx_ - view.cam_x) * sz));
    int py  = view.rect.y + static_cast<int>(std::lround((cy_ - view.cam_y) * sz));
    uint8_t a = static_cast<uint8_t>(160 + 95 * std::sin(anim_clock / 150.0));
    outline(px, py, sz, sz, {255, 230, 80, a}, std::max(2, sz / 16));
  }
  // direction arrows around the player
  if(mode() == UI_DIRECTION)
  {
    int pcx = view.rect.x + static_cast<int>(std::lround((ms.px - ms.hoff - view.cam_x) * sz)) + sz / 2;
    int pcy = view.rect.y + static_cast<int>(std::lround((ms.py - ms.voff - view.cam_y) * sz)) + sz / 2;
    for(int i = 0; i < 8; ++i)
    {
      int ax = pcx + DX[i] * sz, ay = pcy + DY[i] * sz;
      bool sel = (i == cur_dir);
      disc(ax, ay, sz / 4, sel ? SDL_Color{255, 220, 60, 230} : SDL_Color{255, 255, 255, 110});
    }
  }
  SDL_RenderSetClipRect(renderer, nullptr);
}

// ------------------------------------------------------------ options ----
std::string row_text(const sdl_screen &scr, int r, std::vector<const WINDOW *> *src = nullptr)
{
  std::string s(scr.cols, ' ');
  if(src)
  {
    src->assign(scr.cols, nullptr);
  }
  for(int c = 0; c < scr.cols; ++c)
  {
    const sdl_cell &cell = scr.cells[r * scr.cols + c];
    if(view.active && cell.src == scr.map_win)
    {
      continue;
    }
    s[c] = static_cast<char>(cell.ch & A_CHARTEXT);
    if(src)
    {
      (*src)[c] = cell.src;
    }
  }
  return s;
}

void scan_options(const sdl_screen &scr)
{
  int old_key = (opt_sel >= 0 && opt_sel < static_cast<int>(options.size())) ? options[opt_sel].key : -1;
  options.clear();
  options_from_popup = false;
  ui_mode m          = mode();
  if(!(m == UI_PROMPT || m == UI_MENU))
  {
    return;
  }
  int msg_top = scr.lines - 6;
  // 1) menu lines like "a: Greet.", "  a - Weapon Hand: dagger", "1: threaten"
  for(int r = 0; r < msg_top && r < scr.lines; ++r)
  {
    std::string s = row_text(scr, r);
    size_t i      = 0;
    while(i < s.size() && i < 5 && s[i] == ' ')
    {
      ++i;
    }
    if(i + 2 >= s.size())
    {
      continue;
    }
    char k = s[i];
    if(!(std::isalnum(static_cast<unsigned char>(k)) || k == '$'))
    {
      continue;
    }
    size_t j = i + 1;
    if(s[j] == ' ' && j + 1 < s.size() && s[j + 1] == '-')
    {
      j += 2;
    }
    else if(s[j] == ':' || s[j] == ')')
    {
      j += 1;
    }
    else
    {
      continue;
    }
    if(j < s.size() && s[j] != ' ')
    {
      continue;
    }
    std::string label = s.substr(j);
    label.erase(0, label.find_first_not_of(' '));
    label.erase(label.find_last_not_of(' ') + 1);
    // the inventory menu puts the slot first; keep it readable
    options.push_back({static_cast<unsigned char>(k), label, r});
  }
  // 2) prompt choices on the last message line:
  //    "Select an item [abc,?]"  or  "Acquire which kind of item: !?][}{)/=%\&"
  if(options.empty())
  {
    for(int r = scr.lines - 1; r >= msg_top && r >= 0; --r)
    {
      std::string s = row_text(scr, r);
      s.erase(s.find_last_not_of(' ') + 1);
      if(s.empty())
      {
        continue;
      }
      std::string picks;
      auto colon = s.rfind(": ");
      if(m == UI_PROMPT && colon != std::string::npos)
      {
        std::string tail = s.substr(colon + 2);
        if(tail.size() >= 2 && tail.size() <= 30 && tail.find(' ') == std::string::npos)
        {
          picks = tail;
        }
      }
      if(picks.empty())
      {
        auto close = s.rfind(']');
        auto open  = close == std::string::npos ? std::string::npos : s.rfind('[', close);
        if(open != std::string::npos)
        {
          std::string inner = s.substr(open + 1, close - open - 1);
          inner             = inner.substr(0, inner.find(','));
          if(!inner.empty() && inner.size() <= 30 && inner.find(' ') == std::string::npos)
          {
            for(char c : inner)
            {
              if(std::isalnum(static_cast<unsigned char>(c)) || c == '$')
              {
                picks += c;
              }
            }
          }
        }
      }
      for(char c : picks)
      {
        options.push_back({static_cast<unsigned char>(c), std::string(1, c), -1});
      }
      options_from_popup = !options.empty();
      break;
    }
  }
  opt_sel = 0;
  for(size_t i = 0; i < options.size(); ++i)
  {
    if(options[i].key == old_key)
    {
      opt_sel = static_cast<int>(i);
    }
  }
}

void draw_options(const sdl_screen &scr)
{
  if(options.empty())
  {
    return;
  }
  if(!options_from_popup)
  {
    const option &o = options[std::clamp(opt_sel, 0, static_cast<int>(options.size()) - 1)];
    int y           = o.row * ch;
    uint8_t a       = static_cast<uint8_t>(70 + 30 * std::sin(anim_clock / 200.0));
    fill(0, y, std::min(scr.cols, 64) * cw, ch, {255, 220, 80, a});
    outline(0, y, std::min(scr.cols, 64) * cw, ch, {255, 220, 80, 220}, 2);
    return;
  }
  // popup of bracket options
  int n   = static_cast<int>(options.size());
  int bw  = cw * 6;
  int pw  = std::min(n, 13) * bw + ch;
  int rows = (n + 12) / 13;
  int ph  = rows * (ch + ch / 2) + ch;
  int px  = (win_w - pw) / 2;
  int py  = (scr.lines - 6) * ch - ph - ch / 2;
  fill(px, py, pw, ph, {20, 18, 30, 235});
  outline(px, py, pw, ph, {200, 190, 140, 255}, 2);
  for(int i = 0; i < n; ++i)
  {
    int bx = px + ch / 2 + (i % 13) * bw, by = py + ch / 2 + (i / 13) * (ch + ch / 2);
    bool sel = i == opt_sel;
    fill(bx + 2, by, bw - 4, ch + ch / 4, sel ? SDL_Color{255, 220, 80, 255} : SDL_Color{60, 56, 80, 255});
    std::string t = options[i].label;
    text(bx + (bw - text_width(t)) / 2, by + ch / 8, t, sel ? SDL_Color{0, 0, 0, 255} : SDL_Color{255, 255, 255, 255});
  }
}

// ------------------------------------------------------------- radial ----
struct radial_item
{
  int key;
  const char *label;
};
struct radial_page_def
{
  const char *name;
  std::array<radial_item, 8> items;
};
const radial_page_def DUNGEON_PAGES[] = {
  {"Actions", {{{'g', "Pick up"}, {'o', "Open"}, {'s', "Search"}, {'t', "Talk"},
               {'x', "Examine"}, {'c', "Close"}, {'z', "Bash"}, {'d', "Drop"}}}},
  {"Items", {{{'q', "Quaff"}, {'r', "Read"}, {'e', "Eat"}, {'a', "Zap wand"},
             {'A', "Activate"}, {'f', "Fire/throw"}, {'G', "Give"}, {9, "View pack"}}}},
  {"Travel", {{{'<', "Go up"}, {'>', "Go down"}, {'@', "Use spot"}, {'.', "Rest"},
              {',', "Nap"}, {'v', "Vault"}, {'T', "Tunnel"}, {'D', "Disarm"}}}},
  {"More", {{{'m', "Cast spell"}, {'p', "Pickpocket"}, {'E', "Dismount"}, {'F', "Tactics"},
            {'Z', "Bash item"}, {'M', "City move"}, {'C', "Call item"}, {'/', "What's that?"}}}},
  {"System", {{{'S', "Save"}, {'O', "Options"}, {16, "Messages"}, {'?', "Help"},
              {'Q', "Quit"}, {'R', "Rename"}, {'V', "Version"}, {18, "Redraw"}}}},
};
const radial_page_def COUNTRY_PAGES[] = {
  {"Countryside", {{{'>', "Enter site"}, {'s', "Search"}, {'H', "Hunt"}, {'x', "Examine"},
                   {'i', "Inventory"}, {'e', "Eat"}, {'d', "Drop"}, {'E', "Dismount"}}}},
  {"System", {{{'S', "Save"}, {'O', "Options"}, {16, "Messages"}, {'?', "Help"},
              {'Q', "Quit"}, {'R', "Rename"}, {'V', "Version"}, {'/', "Identify char"}}}},
};

bool in_country()
{
  fe_map_state ms;
  omega_map_state(ms);
  return ms.valid && ms.country;
}

const radial_page_def *pages(int &n)
{
  if(in_country())
  {
    n = 2;
    return COUNTRY_PAGES;
  }
  n = 5;
  return DUNGEON_PAGES;
}

void draw_radial()
{
  if(!radial_open)
  {
    return;
  }
  int n;
  const radial_page_def *pg = pages(n);
  radial_page_def const &p = pg[std::clamp(radial_page, 0, n - 1)];
  SDL_Rect area         = view.active ? view.rect : SDL_Rect{0, 0, win_w, win_h - hint_h};
  fill(area.x, area.y, area.w, area.h, {0, 0, 0, 120});
  int cx_ = area.x + area.w / 2, cy_ = area.y + area.h / 2;
  int R   = std::min(area.w, area.h) * 36 / 100;
  disc(cx_, cy_, R + ch * 2, {20, 18, 30, 220});
  disc(cx_, cy_, ch * 3, {50, 46, 70, 255});
  std::string title = p.name;
  text(cx_ - text_width(title) / 2, cy_ - ch, title, {255, 220, 120, 255});
  std::string pgs = std::to_string(radial_page + 1) + "/" + std::to_string(n);
  text(cx_ - text_width(pgs) / 2, cy_ + ch / 4, pgs, {170, 170, 190, 255});
  for(int i = 0; i < 8; ++i)
  {
    double ang = (i * 45 - 90) * M_PI / 180.0;
    int ix     = cx_ + static_cast<int>(std::cos(ang) * R);
    int iy     = cy_ + static_cast<int>(std::sin(ang) * R);
    bool sel   = i == radial_sel;
    disc(ix, iy, ch * 2 + (sel ? ch / 2 : 0), sel ? SDL_Color{255, 210, 70, 255} : SDL_Color{70, 64, 96, 255});
    std::string lab = p.items[i].label;
    SDL_Color tc    = sel ? SDL_Color{20, 16, 10, 255} : SDL_Color{240, 240, 250, 255};
    int ts          = ch;
    while(text_width(lab, ts) > ch * 5 && ts > 10)
    {
      --ts;
    }
    text(ix - text_width(lab, ts) / 2, iy - ts / 2, lab, tc, ts);
  }
}

// ---------------------------------------------------------------- osk ----
const char *OSK_ROWS[2][4] = {
  {"1234567890-=", "qwertyuiop[]", "asdfghjkl;'\\", "zxcvbnm,./`<"},
  {"!@#$%^&*()_+", "QWERTYUIOP{}", "ASDFGHJKL:\"|", "ZXCVBNM<>?~>"},
};
struct osk_special
{
  const char *label;
  int key; // 0 = shift
  int width;
};
const osk_special OSK_BOTTOM[] = {{"Shift", 0, 2}, {"Space", ' ', 4}, {"Del", KEY_BACKSPACE, 2},
                                  {"Esc", ESC, 2}, {"Enter", '\n', 2}};

int osk_key_at(int r, int c)
{
  if(r < 4)
  {
    return static_cast<unsigned char>(OSK_ROWS[osk_shift][r][c]);
  }
  int x = 0;
  for(auto &s : OSK_BOTTOM)
  {
    if(c < x + s.width)
    {
      return s.key;
    }
    x += s.width;
  }
  return '\n';
}

void osk_press(int key)
{
  if(key == 0)
  {
    osk_shift = !osk_shift;
    return;
  }
  push_key(key);
  if(osk_shift && key > 32 && key < 127)
  {
    osk_shift = false;
  }
  if(!osk_auto && key != ' ' && mode() != UI_TITLE)
  {
    osk_open = false; // single-key keyboard closes after use
  }
}

SDL_Rect osk_rect()
{
  int kw = std::max(cw * 4, ch * 2), kh = ch + ch / 2;
  int w = kw * 12 + ch, h = kh * 5 + ch + ch;
  return {(win_w - w) / 2, win_h - hint_h - h - ch / 2, w, h};
}

void draw_osk()
{
  if(!osk_open)
  {
    return;
  }
  SDL_Rect r = osk_rect();
  int kw = std::max(cw * 4, ch * 2), kh = ch + ch / 2;
  fill(r.x, r.y, r.w, r.h, {16, 14, 26, 240});
  outline(r.x, r.y, r.w, r.h, {200, 190, 140, 255}, 2);
  std::string title = mode() == UI_NUMBER ? "Enter a number" : (osk_auto ? "Type text" : "Send a key");
  text(r.x + ch / 2, r.y + ch / 4, title, {255, 220, 120, 255});
  int oy = r.y + ch + ch / 4;
  for(int row = 0; row < 5; ++row)
  {
    int x = r.x + ch / 2;
    if(row < 4)
    {
      for(int c = 0; c < 12; ++c)
      {
        bool sel = (row == osk_r && c == osk_c);
        fill(x + 2, oy + row * kh + 2, kw - 4, kh - 4, sel ? SDL_Color{255, 210, 70, 255} : SDL_Color{60, 56, 84, 255});
        std::string s(1, OSK_ROWS[osk_shift][row][c]);
        text(x + (kw - cw) / 2, oy + row * kh + (kh - ch) / 2, s, sel ? SDL_Color{0, 0, 0, 255} : SDL_Color{255, 255, 255, 255});
        x += kw;
      }
    }
    else
    {
      int c = 0;
      for(auto &s : OSK_BOTTOM)
      {
        bool sel = (row == osk_r && osk_c >= c && osk_c < c + s.width);
        bool on  = s.key == 0 && osk_shift;
        SDL_Color bg = sel ? SDL_Color{255, 210, 70, 255} : (on ? SDL_Color{110, 150, 90, 255} : SDL_Color{80, 74, 110, 255});
        fill(x + 2, oy + row * kh + 2, kw * s.width - 4, kh - 4, bg);
        text(x + (kw * s.width - text_width(s.label)) / 2, oy + row * kh + (kh - ch) / 2, s.label,
             sel ? SDL_Color{0, 0, 0, 255} : SDL_Color{255, 255, 255, 255});
        x += kw * s.width;
        c += s.width;
      }
    }
  }
}

// ---------------------------------------------------------- hint bar ----
std::vector<std::pair<std::string, std::string>> hints()
{
  std::vector<std::pair<std::string, std::string>> h;
  if(osk_open)
  {
    h = {{"A", "Type"}, {"B", osk_auto ? "Delete" : "Close"}, {"X", "Space"}, {"Y", "Shift"}, {"START", "Enter"}};
    h.push_back({"SELECT", osk_auto ? "Cancel" : "Close"});
    return h;
  }
  if(radial_open)
  {
    return {{"STICK", "Choose"}, {"A", "Do it"}, {"LB/RB", "Page"}, {"B", "Close"}};
  }
  switch(mode())
  {
    case UI_COMMAND:
    {
      std::string ctx = omega_context_label();
      h.push_back({"MOVE", "Walk"});
      h.push_back({"A", ctx});
      h.push_back({"X", "Inventory"});
      h.push_back({"Y", "Actions"});
      if(in_country())
      {
        h.push_back({"RT", "Search"});
        h.push_back({"LT", "Hunt"});
      }
      else
      {
        h.push_back({"LB", "Magic"});
        h.push_back({"LT", "Fire"});
        h.push_back({"RT", "Rest"});
        h.push_back({"RB+MOVE", "Run"});
      }
      h.push_back({"START", "System"});
      h.push_back({"SELECT", "Keys"});
      break;
    }
    case UI_DIRECTION:
      h = {{"MOVE", "Pick direction"}, {"B", "Cancel"}};
      break;
    case UI_TARGET:
      h = {{"MOVE", "Aim"}, {"A", "Target"}, {"B", "Cancel"}};
      break;
    case UI_LOOK:
      h = {{"MOVE", "Look around"}, {"A", "Done"}, {"B", "Done"}};
      break;
    case UI_YESNO:
      h = {{"A", "Yes"}, {"B", "No"}, {"X", "Quit"}};
      break;
    case UI_MORE:
      h = {{"A", "Continue"}};
      break;
    case UI_COUNT:
      h = {{"LEFT/RIGHT", "Amount"}, {"A", "Confirm"}};
      break;
    case UI_TEXT:
    case UI_NUMBER:
      h = {{"SELECT", "Keyboard"}};
      break;
    case UI_TITLE:
      h = {{"UP/DOWN", "Choose"}, {"A", "Start"}, {"X", "Edit name"}};
      break;
    case UI_MENU:
    case UI_PROMPT:
      if(!options.empty())
      {
        h = {{options_from_popup ? "LEFT/RIGHT" : "UP/DOWN", "Choose"}, {"A", "Select"}, {"B", "Back"}};
        if(mode() == UI_MENU)
        {
          h.push_back({"LB/RB", "Scroll"});
        }
      }
      else
      {
        h = {{"A", "OK / Enter"}, {"B", "Back"}, {"X", "Space"}, {"DPAD", "Arrows"}};
      }
      h.push_back({"SELECT", "Keys"});
      break;
  }
  return h;
}

void draw_hints()
{
  int y = win_h - hint_h;
  fill(0, y, win_w, hint_h, {24, 22, 34, 255});
  fill(0, y, win_w, 1, {90, 84, 120, 255});
  int bh = hint_h - hint_h / 4;
  int x  = ch / 2;
  int by = y + (hint_h - bh) / 2;
  for(auto &[b, label] : hints())
  {
    if(label.empty())
    {
      continue;
    }
    int nx = badge(x, by, b, bh);
    nx     = text(nx + ch / 3, by + (bh - ch) / 2, label, {225, 225, 235, 255});
    x      = nx + ch;
    if(x > win_w)
    {
      break;
    }
  }
}

// -------------------------------------------------------------- render ----
void render()
{
  const sdl_screen &scr = sdlc_screen();
  anim_clock            = SDL_GetTicks();
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  draw_map(scr);

  // menus drawn over the map: dim the whole tile view behind them
  bool dimmed = false;
  if(view.active)
  {
    const WINDOW *w = scr.map_win;
    int covered     = 0;
    for(int r = w->by; r < w->by + w->h && r < scr.lines; ++r)
    {
      for(int c = w->bx; c < w->bx + w->w && c < scr.cols; ++c)
      {
        const sdl_cell &cell = scr.cells[r * scr.cols + c];
        if(cell.src && cell.src != w && (cell.ch & A_CHARTEXT) != ' ')
        {
          ++covered;
        }
      }
    }
    if(covered > 8)
    {
      fill(view.rect.x, view.rect.y, view.rect.w, view.rect.h, {8, 8, 14, 200});
      dimmed = true;
    }
  }

  // text cells
  for(int r = 0; r < scr.lines; ++r)
  {
    for(int c = 0; c < scr.cols; ++c)
    {
      const sdl_cell &cell = scr.cells[r * scr.cols + c];
      int x = cell_x(scr, r, c), y = r * ch;
      if(view.active)
      {
        SDL_Point pt{x, y};
        bool inside = SDL_PointInRect(&pt, &view.rect);
        if(inside && (cell.src == scr.map_win || cell.src == nullptr))
        {
          continue;
        }
        if(inside && !dimmed)
        {
          fill(x, y, cw, ch, {8, 8, 14, 140}); // overlay panel (menus over the map)
        }
      }
      SDL_Color fg, bg;
      bool has_bg;
      cell_colors(cell.ch, fg, bg, has_bg);
      if(has_bg)
      {
        fill(x, y, cw, ch, bg);
      }
      glyph(x, y, cw, ch, cell.ch & A_CHARTEXT, fg);
    }
  }
  // text cursor
  if(scr.cursor_visibility && scr.cursor_win && scr.cursor_win != scr.map_win)
  {
    if((anim_clock / 400) % 2 == 0)
    {
      fill(cell_x(scr, scr.cursor_y, scr.cursor_x), scr.cursor_y * ch + ch - 3, cw, 3, {255, 255, 255, 220});
    }
  }
  draw_options(scr);
  draw_radial();
  draw_osk();
  draw_hints();
  if(SDL_GetTicks() < flash_until)
  {
    fill(0, 0, win_w, win_h, {255, 255, 255, 60});
  }
  SDL_RenderPresent(renderer);
  last_present = SDL_GetTicks();
  dirty        = false;
}

// --------------------------------------------------------------- input ----
void open_osk(bool automatic)
{
  osk_open  = true;
  osk_auto  = automatic;
  osk_shift = false;
  osk_r     = mode() == UI_NUMBER ? 0 : 1;
  osk_c     = 0;
}

void osk_move(int dx, int dy)
{
  if(dy)
  {
    osk_r = (osk_r + dy + 5) % 5;
  }
  if(dx)
  {
    if(osk_r == 4)
    {
      // jump between the wide bottom keys
      int starts[] = {0, 2, 6, 8, 10};
      int idx      = 0;
      for(int i = 0; i < 5; ++i)
      {
        if(osk_c >= starts[i])
        {
          idx = i;
        }
      }
      idx   = (idx + dx + 5) % 5;
      osk_c = starts[idx];
    }
    else
    {
      osk_c = (osk_c + dx + 12) % 12;
    }
  }
}

void option_move(int dy)
{
  if(options.empty())
  {
    return;
  }
  int n = static_cast<int>(options.size());
  int s = opt_sel + dy;
  if(s < 0 || s >= n)
  {
    if(mode() == UI_MENU)
    {
      push_key(dy < 0 ? KEY_UP : KEY_DOWN); // scroll the menu itself
      return;
    }
    s = (s + n) % n;
  }
  opt_sel = s;
}

void on_dir(int d)
{
  int dx = DX[d], dy = DY[d];
  dirty  = true;
  if(radial_open)
  {
    radial_sel = d;
    return;
  }
  if(osk_open)
  {
    osk_move(dx, dy);
    return;
  }
  switch(mode())
  {
    case UI_COMMAND:
      push_key(held[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] ? std::toupper(VI[d]) : VI[d]);
      break;
    case UI_DIRECTION:
    case UI_TARGET:
    case UI_LOOK:
      push_key(VI[d]);
      break;
    case UI_COUNT:
      push_key((dx > 0 || dy < 0) ? '>' : '<');
      break;
    default:
      if(!options.empty())
      {
        if(options_from_popup && dx != 0 && dy == 0)
        {
          option_move(dx);
        }
        else if(options_from_popup)
        {
          option_move(dy * 13);
        }
        else if(dy != 0)
        {
          option_move(dy);
        }
      }
      else if(dy < 0) push_key(KEY_UP);
      else if(dy > 0) push_key(KEY_DOWN);
      else if(dx < 0) push_key(KEY_LEFT);
      else if(dx > 0) push_key(KEY_RIGHT);
      break;
  }
}

void on_button(int b)
{
  dirty = true;
  if(b == SDL_CONTROLLER_BUTTON_BACK)
  {
    if(osk_open)
    {
      if(osk_auto)
      {
        push_key(ESC);
      }
      osk_open = false;
    }
    else
    {
      radial_open = false;
      open_osk(false);
    }
    return;
  }
  if(osk_open)
  {
    switch(b)
    {
      case SDL_CONTROLLER_BUTTON_A: osk_press(osk_key_at(osk_r, osk_c)); break;
      case SDL_CONTROLLER_BUTTON_B:
        if(osk_auto) push_key(KEY_BACKSPACE);
        else osk_open = false;
        break;
      case SDL_CONTROLLER_BUTTON_X: push_key(' '); break;
      case SDL_CONTROLLER_BUTTON_Y: osk_shift = !osk_shift; break;
      case SDL_CONTROLLER_BUTTON_START: push_key('\n'); if(!osk_auto) osk_open = false; break;
      default: break;
    }
    return;
  }
  if(radial_open)
  {
    int n;
    const radial_page_def *pg = pages(n);
    switch(b)
    {
      case SDL_CONTROLLER_BUTTON_A:
        if(radial_sel >= 0)
        {
          push_key(pg[std::clamp(radial_page, 0, n - 1)].items[radial_sel].key);
          radial_open = false;
        }
        break;
      case SDL_CONTROLLER_BUTTON_B:
      case SDL_CONTROLLER_BUTTON_Y:
        radial_open = false;
        break;
      case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: radial_page = (radial_page + n - 1) % n; break;
      case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: radial_page = (radial_page + 1) % n; break;
      case SDL_CONTROLLER_BUTTON_START: radial_page = n - 1; break;
      default: break;
    }
    return;
  }
  if(b == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
  {
    tile_zoom = (tile_zoom + 1) % static_cast<int>(TILE_SIZES.size());
    return;
  }
  bool country = in_country();
  switch(mode())
  {
    case UI_COMMAND:
      switch(b)
      {
        case SDL_CONTROLLER_BUTTON_A: push_key(omega_context_key()); break;
        case SDL_CONTROLLER_BUTTON_X: push_key('i'); break;
        case SDL_CONTROLLER_BUTTON_Y: radial_open = true; radial_page = 0; radial_sel = -1; break;
        case SDL_CONTROLLER_BUTTON_START:
        {
          int n;
          pages(n);
          radial_open = true;
          radial_page = n - 1;
          radial_sel  = -1;
          break;
        }
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: if(!country) push_key('m'); break;
        case VB_LT: push_key(country ? 'H' : 'f'); break;
        case VB_RT: push_key(country ? 's' : '.'); break;
        case SDL_CONTROLLER_BUTTON_LEFTSTICK: push_key(country ? 's' : 's'); break;
        default: break;
      }
      break;
    case UI_DIRECTION:
      if(b == SDL_CONTROLLER_BUTTON_B) push_key(ESC);
      break;
    case UI_TARGET:
      if(b == SDL_CONTROLLER_BUTTON_A) push_key('.');
      else if(b == SDL_CONTROLLER_BUTTON_B) push_key(ESC);
      else if(b == SDL_CONTROLLER_BUTTON_Y) push_key('?');
      break;
    case UI_LOOK:
      if(b == SDL_CONTROLLER_BUTTON_A || b == SDL_CONTROLLER_BUTTON_B) push_key(ESC);
      break;
    case UI_YESNO:
      if(b == SDL_CONTROLLER_BUTTON_A) push_key('y');
      else if(b == SDL_CONTROLLER_BUTTON_B) push_key('n');
      else if(b == SDL_CONTROLLER_BUTTON_X) push_key('q');
      break;
    case UI_MORE:
      if(b == SDL_CONTROLLER_BUTTON_A || b == SDL_CONTROLLER_BUTTON_B || b == SDL_CONTROLLER_BUTTON_X)
        push_key(' ');
      break;
    case UI_COUNT:
      if(b == SDL_CONTROLLER_BUTTON_A || b == SDL_CONTROLLER_BUTTON_B) push_key(ESC);
      break;
    case UI_TITLE:
      if(b == SDL_CONTROLLER_BUTTON_A || b == SDL_CONTROLLER_BUTTON_START) push_key('\n');
      else if(b == SDL_CONTROLLER_BUTTON_X) open_osk(false);
      break;
    case UI_TEXT:
    case UI_NUMBER:
      if(b == SDL_CONTROLLER_BUTTON_A) open_osk(true);
      else if(b == SDL_CONTROLLER_BUTTON_START) push_key('\n');
      break;
    case UI_MENU:
    case UI_PROMPT:
      switch(b)
      {
        case SDL_CONTROLLER_BUTTON_A:
          if(!options.empty()) push_key(options[std::clamp(opt_sel, 0, static_cast<int>(options.size()) - 1)].key);
          else push_key('\n');
          break;
        case SDL_CONTROLLER_BUTTON_B: push_key(ESC); break;
        case SDL_CONTROLLER_BUTTON_START: push_key('\n'); break;
        case SDL_CONTROLLER_BUTTON_X: push_key(' '); break;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: push_key(KEY_UP); break;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: push_key(mode() == UI_MENU ? ' ' : KEY_DOWN); break;
        default: break;
      }
      break;
  }
}

// direction from currently held dpad buttons / stick
int held_dir()
{
  int dx = 0, dy = 0;
  if(held[SDL_CONTROLLER_BUTTON_DPAD_LEFT]) dx -= 1;
  if(held[SDL_CONTROLLER_BUTTON_DPAD_RIGHT]) dx += 1;
  if(held[SDL_CONTROLLER_BUTTON_DPAD_UP]) dy -= 1;
  if(held[SDL_CONTROLLER_BUTTON_DPAD_DOWN]) dy += 1;
  if(dx || dy)
  {
    return dir_from(dx, dy);
  }
  float fx = stick_x / 32767.0f, fy = stick_y / 32767.0f;
  if(std::sqrt(fx * fx + fy * fy) < 0.55f)
  {
    return -1;
  }
  double ang = std::atan2(fx, -fy) * 180.0 / M_PI; // 0 = up, clockwise
  int sector = static_cast<int>(std::lround(ang / 45.0));
  return (sector % 8 + 8) % 8;
}

bool repeats()
{
  if(radial_open)
  {
    return false;
  }
  return true;
}

void update_dir()
{
  int d   = held_dir();
  Uint32 now = SDL_GetTicks();
  if(d != cur_dir)
  {
    cur_dir   = d;
    dir_fired = false;
    // short debounce so dpad diagonals (two buttons) register as one move
    bool needs_debounce = !radial_open && !osk_open &&
                          (mode() == UI_COMMAND || mode() == UI_DIRECTION || mode() == UI_TARGET);
    dir_fire_at = now + (needs_debounce ? 45 : 0);
    if(radial_open && d >= 0)
    {
      radial_sel = d;
      dirty      = true;
    }
  }
}

void tick_dir()
{
  if(cur_dir < 0)
  {
    return;
  }
  Uint32 now = SDL_GetTicks();
  if(now < dir_fire_at)
  {
    return;
  }
  if(!dir_fired)
  {
    on_dir(cur_dir);
    dir_fired   = true;
    dir_fire_at = now + 300;
  }
  else if(repeats())
  {
    on_dir(cur_dir);
    dir_fire_at = now + (mode() == UI_COMMAND ? 140 : 110);
  }
}

void push_mouse(int x, int y, mmask_t state)
{
  MEVENT e{};
  e.x      = x;
  e.y      = y;
  e.bstate = state;
  mouseq.push_back(e);
  push_key(KEY_MOUSE);
}

bool touch_osk(int x, int y)
{
  if(!osk_open)
  {
    return false;
  }
  SDL_Rect r = osk_rect();
  SDL_Point p{x, y};
  if(!SDL_PointInRect(&p, &r))
  {
    return false;
  }
  int kw = std::max(cw * 4, ch * 2), kh = ch + ch / 2;
  int oy = r.y + ch + ch / 4;
  int row = (y - oy) / kh, col = (x - r.x - ch / 2) / kw;
  if(row >= 0 && row < 5 && col >= 0 && col < 12)
  {
    osk_r = row;
    osk_c = col;
    osk_press(osk_key_at(row, col));
  }
  return true;
}

void on_click(int x, int y, bool down)
{
  dirty = true;
  if(down && touch_osk(x, y))
  {
    return;
  }
  if(osk_open)
  {
    return;
  }
  if(radial_open && down)
  {
    // tap a slice
    SDL_Rect area = view.active ? view.rect : SDL_Rect{0, 0, win_w, win_h - hint_h};
    int cx_ = area.x + area.w / 2, cy_ = area.y + area.h / 2;
    double ang = std::atan2(x - cx_, -(y - cy_)) * 180.0 / M_PI;
    int s      = (static_cast<int>(std::lround(ang / 45.0)) % 8 + 8) % 8;
    radial_sel = s;
    on_button(SDL_CONTROLLER_BUTTON_A);
    return;
  }
  const sdl_screen &scr = sdlc_screen();
  SDL_Point p{x, y};
  if(view.active && SDL_PointInRect(&p, &view.rect))
  {
    int wx = static_cast<int>(std::floor(view.cam_x + static_cast<float>(x - view.rect.x) / view.tile));
    int wy = static_cast<int>(std::floor(view.cam_y + static_cast<float>(y - view.rect.y) / view.tile));
    if(mode() == UI_COMMAND)
    {
      if(!down)
      {
        return;
      }
      // tap to step towards
      fe_map_state ms;
      omega_map_state(ms);
      int dx = (wx + ms.hoff > ms.px) - (wx + ms.hoff < ms.px);
      int dy = (wy + ms.voff > ms.py) - (wy + ms.voff < ms.py);
      int d  = dir_from(dx, dy);
      if(d >= 0)
      {
        push_key(VI[d]);
      }
      else
      {
        push_key(omega_context_key());
      }
      return;
    }
    push_mouse(view.w->bx + wx, view.w->by + wy, down ? BUTTON1_PRESSED : BUTTON1_RELEASED);
    return;
  }
  if(!down)
  {
    return;
  }
  // tap on a highlighted option line
  int row = y / ch;
  for(size_t i = 0; i < options.size(); ++i)
  {
    if(options[i].row == row)
    {
      opt_sel = static_cast<int>(i);
      push_key(options[i].key);
      return;
    }
  }
  if(y >= win_h - hint_h)
  {
    open_osk(false);
    return;
  }
  (void)scr;
}

void handle_key(const SDL_KeyboardEvent &k)
{
  SDL_Keycode s = k.keysym.sym;
  bool ctrl     = (k.keysym.mod & KMOD_CTRL) != 0;
  if(ctrl && s >= SDLK_a && s <= SDLK_z)
  {
    push_key(s - SDLK_a + 1);
    return;
  }
  switch(s)
  {
    case SDLK_ESCAPE:
      if(radial_open || osk_open)
      {
        radial_open = osk_open = false;
        dirty                  = true;
      }
      else
      {
        push_key(ESC);
      }
      break;
    case SDLK_RETURN:
    case SDLK_KP_ENTER: push_key('\n'); break;
    case SDLK_BACKSPACE: push_key(KEY_BACKSPACE); break;
    case SDLK_DELETE: push_key(KEY_DC); break;
    case SDLK_UP: push_key(KEY_UP); break;
    case SDLK_DOWN: push_key(KEY_DOWN); break;
    case SDLK_LEFT: push_key(KEY_LEFT); break;
    case SDLK_RIGHT: push_key(KEY_RIGHT); break;
    case SDLK_HOME: push_key(KEY_HOME); break;
    case SDLK_TAB: push_key('\t'); break;
    case SDLK_F11:
    {
      Uint32 f = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
      SDL_SetWindowFullscreen(window, f ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
      break;
    }
    case SDLK_EQUALS:
      if(ctrl)
      {
        tile_zoom = (tile_zoom + 1) % static_cast<int>(TILE_SIZES.size());
        dirty     = true;
      }
      break;
    default: break;
  }
}

void open_pad(int index)
{
  if(pad || !SDL_IsGameController(index))
  {
    return;
  }
  pad = SDL_GameControllerOpen(index);
}

void handle_resize()
{
  int lines, cols;
  compute_grid(lines, cols);
  const sdl_screen &scr = sdlc_screen();
  if(lines != scr.lines || cols != scr.cols)
  {
    sdlc_resized(lines, cols);
    push_key(KEY_RESIZE);
  }
  dirty = true;
}

void pump(int wait_ms)
{
  SDL_Event e;
  bool got = wait_ms > 0 ? SDL_WaitEventTimeout(&e, wait_ms) : SDL_PollEvent(&e);
  while(got)
  {
    switch(e.type)
    {
      case SDL_QUIT:
        omega_window_closed();
        break;
      case SDL_WINDOWEVENT:
        if(e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
          handle_resize();
        }
        dirty = true;
        break;
      case SDL_TEXTINPUT:
        if(!(SDL_GetModState() & KMOD_CTRL))
        {
          for(const char *p = e.text.text; *p; ++p)
          {
            if(static_cast<unsigned char>(*p) < 128)
            {
              push_key(static_cast<unsigned char>(*p));
            }
          }
        }
        break;
      case SDL_KEYDOWN:
        handle_key(e.key);
        break;
      case SDL_CONTROLLERDEVICEADDED:
        open_pad(e.cdevice.which);
        break;
      case SDL_CONTROLLERDEVICEREMOVED:
        if(pad && SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(pad)) == e.cdevice.which)
        {
          SDL_GameControllerClose(pad);
          pad = nullptr;
          held.fill(false);
        }
        break;
      case SDL_CONTROLLERBUTTONDOWN:
        held[e.cbutton.button] = true;
        if(e.cbutton.button >= SDL_CONTROLLER_BUTTON_DPAD_UP && e.cbutton.button <= SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
        {
          update_dir();
        }
        else
        {
          on_button(e.cbutton.button);
        }
        break;
      case SDL_CONTROLLERBUTTONUP:
        held[e.cbutton.button] = false;
        if(e.cbutton.button >= SDL_CONTROLLER_BUTTON_DPAD_UP && e.cbutton.button <= SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
        {
          update_dir();
        }
        break;
      case SDL_CONTROLLERAXISMOTION:
        switch(e.caxis.axis)
        {
          case SDL_CONTROLLER_AXIS_LEFTX: stick_x = e.caxis.value; update_dir(); break;
          case SDL_CONTROLLER_AXIS_LEFTY: stick_y = e.caxis.value; update_dir(); break;
          case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
          {
            bool on = e.caxis.value > 16000;
            if(on && !trig_l) on_button(VB_LT);
            trig_l     = on;
            held[VB_LT] = on;
            break;
          }
          case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
          {
            bool on = e.caxis.value > 16000;
            if(on && !trig_r) on_button(VB_RT);
            trig_r     = on;
            held[VB_RT] = on;
            break;
          }
          default: break;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        if(e.button.button == SDL_BUTTON_LEFT)
        {
          int x = e.button.x, y = e.button.y;
          // window coords -> renderer output coords (HiDPI)
          int ww, wh;
          SDL_GetWindowSize(window, &ww, &wh);
          if(ww > 0 && wh > 0)
          {
            x = x * win_w / ww;
            y = y * win_h / wh;
          }
          on_click(x, y, e.type == SDL_MOUSEBUTTONDOWN);
        }
        break;
      default:
        break;
    }
    got = SDL_PollEvent(&e);
  }
  tick_dir();
}
// ------------------------------------------------- scripted test input ----
// OMEGA_PAD_SCRIPT=<file>: one command per line, run whenever the game waits
// for input. Used for automated testing of the pad controls.
std::deque<std::string> script;
bool script_loaded = false;

void save_shot(const std::string &path)
{
  render();
  SDL_Surface *s = SDL_CreateRGBSurfaceWithFormat(0, win_w, win_h, 32, SDL_PIXELFORMAT_ARGB8888);
  SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_ARGB8888, s->pixels, s->pitch);
  SDL_SaveBMP(s, path.c_str());
  SDL_FreeSurface(s);
}

bool run_script_step()
{
  if(!script_loaded)
  {
    script_loaded = true;
    if(const char *f = SDL_getenv("OMEGA_PAD_SCRIPT"))
    {
      std::ifstream in(f);
      std::string l;
      while(std::getline(in, l))
      {
        if(!l.empty() && l[0] != '#')
        {
          script.push_back(l);
        }
      }
    }
  }
  if(script.empty())
  {
    return false;
  }
  std::string l = script.front();
  script.pop_front();
  std::string cmd = l.substr(0, l.find(' '));
  std::string arg = l.find(' ') == std::string::npos ? "" : l.substr(l.find(' ') + 1);
  static const std::unordered_map<std::string, int> buttons = {
    {"A", SDL_CONTROLLER_BUTTON_A}, {"B", SDL_CONTROLLER_BUTTON_B}, {"X", SDL_CONTROLLER_BUTTON_X},
    {"Y", SDL_CONTROLLER_BUTTON_Y}, {"LB", SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
    {"RB", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER}, {"LT", VB_LT}, {"RT", VB_RT},
    {"START", SDL_CONTROLLER_BUTTON_START}, {"SELECT", SDL_CONTROLLER_BUTTON_BACK},
    {"L3", SDL_CONTROLLER_BUTTON_LEFTSTICK}, {"R3", SDL_CONTROLLER_BUTTON_RIGHTSTICK}};
  static const std::unordered_map<std::string, int> dirs = {
    {"up", 0}, {"ur", 1}, {"right", 2}, {"dr", 3}, {"down", 4}, {"dl", 5}, {"left", 6}, {"ul", 7}};
  if(buttons.count(cmd))
  {
    on_button(buttons.at(cmd));
  }
  else if(dirs.count(cmd))
  {
    int n = arg.empty() ? 1 : std::atoi(arg.c_str());
    for(int i = 0; i < n; ++i)
    {
      on_dir(dirs.at(cmd));
    }
  }
  else if(cmd == "key")
  {
    push_key(arg == "ESC" ? ESC : (arg == "ENTER" ? '\n' : static_cast<unsigned char>(arg[0])));
  }
  else if(cmd == "ctrl")
  {
    push_key(std::tolower(static_cast<unsigned char>(arg[0])) - 'a' + 1);
  }
  else if(cmd == "text")
  {
    for(char c : arg) push_key(static_cast<unsigned char>(c));
  }
  else if(cmd == "tap")
  {
    int x = 0, y = 0;
    std::sscanf(arg.c_str(), "%d %d", &x, &y);
    on_click(x, y, true);
    on_click(x, y, false);
  }
  else if(cmd == "shot")
  {
    save_shot(arg);
  }
  else if(cmd == "dump")
  {
    // write the current mode + options for assertions
    std::ofstream o(arg, std::ios::app);
    o << "mode=" << mode() << " options=" << options.size() << " osk=" << osk_open << " radial=" << radial_open
      << " ctx=" << omega_context_label() << "\n";
  }
  else if(cmd == "env")
  {
    omega_debug_change_env(std::atoi(arg.c_str()));
  }
  else if(cmd == "quit")
  {
    fe_shutdown();
    std::exit(0);
  }
  return true;
}
} // namespace

// ============================================================ public API ==
void ui_push_mode(ui_mode m)
{
  modes.push_back(m);
  radial_open = false;
  if(m == UI_TEXT || m == UI_NUMBER)
  {
    if(pad || SDL_getenv("OMEGA_PAD_SCRIPT"))
    {
      open_osk(true);
    }
  }
  else if(osk_open && !osk_auto)
  {
    // keep a manually opened keyboard
  }
  opt_sel = 0;
  dirty   = true;
}

void ui_pop_mode()
{
  if(!modes.empty())
  {
    ui_mode m = modes.back();
    modes.pop_back();
    if((m == UI_TEXT || m == UI_NUMBER) && osk_auto)
    {
      osk_open = false;
      osk_auto = false;
    }
  }
  dirty = true;
}

bool fe_init(int &lines, int &cols)
{
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
  SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "1");
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0)
  {
    fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
    return false;
  }
  if(char *base = SDL_GetBasePath())
  {
    asset_dir = std::string(base) + "assets/";
    SDL_free(base);
  }
  if(const char *e = SDL_getenv("OMEGA_ASSETS"))
  {
    asset_dir = std::string(e) + "/";
  }
  Uint32 flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
  bool fullscreen = SDL_getenv("SteamDeck") || SDL_getenv("OMEGA_FULLSCREEN");
  if(fullscreen)
  {
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  }
  int ww = 1280, wh = 800;
  if(const char *e = SDL_getenv("OMEGA_WINDOW"))
  {
    std::sscanf(e, "%dx%d", &ww, &wh);
  }
  window = SDL_CreateWindow("Omega", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ww, wh, flags);
  if(!window)
  {
    fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
    return false;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(!renderer)
  {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  }
  if(!renderer)
  {
    fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
    return false;
  }
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  tex_tiles = load_rgba(asset_dir + "tiles.rgba");
  tex_font  = load_rgba(asset_dir + "font.rgba");
  if(!tex_tiles || !tex_font)
  {
    fprintf(stderr, "omega: set OMEGA_ASSETS to the folder holding tiles.rgba and font.rgba\n");
    return false;
  }
  SDL_SetTextureScaleMode(tex_font, SDL_ScaleModeLinear);
  SDL_SetTextureScaleMode(tex_tiles, SDL_ScaleModeNearest);
  tex_disc = make_disc();
  {
    std::ifstream f(asset_dir + "font.txt");
    f >> font_w >> font_h;
  }
  load_tile_index(asset_dir + "tiles.txt");
  if(const char *p = SDL_getenv("OMEGA_HERO"))
  {
    player_set = p;
  }

  SDL_GameControllerEventState(SDL_ENABLE);
  for(int i = 0; i < SDL_NumJoysticks(); ++i)
  {
    open_pad(i);
  }
  SDL_StartTextInput();
  compute_grid(lines, cols);
  return true;
}

void fe_shutdown()
{
  if(pad)
  {
    SDL_GameControllerClose(pad);
  }
  if(renderer)
  {
    SDL_DestroyRenderer(renderer);
  }
  if(window)
  {
    SDL_DestroyWindow(window);
  }
  SDL_Quit();
  renderer = nullptr;
  window   = nullptr;
}

void fe_mark_dirty()
{
  dirty = true;
}

void fe_present_if_due()
{
  if(!renderer)
  {
    return;
  }
  if(dirty && SDL_GetTicks() - last_present >= 33)
  {
    render();
  }
}

int fe_get_key(int timeout_ms)
{
  if(!renderer)
  {
    return ERR;
  }
  Uint32 start = SDL_GetTicks();
  bool rescanned = false;
  while(true)
  {
    if(!keyq.empty())
    {
      int k = keyq.front();
      keyq.pop_front();
      dirty = true;
      return k;
    }
    if(!rescanned)
    {
      scan_options(sdlc_screen());
      rescanned = true;
      if(run_script_step())
      {
        rescanned = false;
        continue;
      }
    }
    if(timeout_ms == 0)
    {
      pump(0);
      if(keyq.empty())
      {
        return ERR;
      }
      continue;
    }
    // animate while idle
    if(dirty || SDL_GetTicks() - last_present > 100)
    {
      render();
    }
    int wait = 16;
    if(timeout_ms > 0)
    {
      int left = static_cast<int>(timeout_ms - (SDL_GetTicks() - start));
      if(left <= 0)
      {
        return ERR;
      }
      wait = std::min(wait, left);
    }
    pump(wait);
  }
}

void fe_unget_key(int key)
{
  keyq.push_front(key);
}

void fe_flash()
{
  flash_until = SDL_GetTicks() + 80;
  dirty       = true;
}

void fe_delay(int ms)
{
  if(!renderer)
  {
    SDL_Delay(ms);
    return;
  }
  Uint32 end = SDL_GetTicks() + ms;
  render();
  while(SDL_GetTicks() < end)
  {
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
      if(e.type == SDL_QUIT)
      {
        omega_window_closed();
      }
    }
    Uint32 now = SDL_GetTicks();
    SDL_Delay(std::min<Uint32>(8, end > now ? end - now : 0));
  }
}

bool fe_pop_mouse(MEVENT &e)
{
  if(mouseq.empty())
  {
    return false;
  }
  e = mouseq.front();
  mouseq.pop_front();
  return true;
}

void fe_push_mouse_front(const MEVENT &e)
{
  mouseq.push_front(e);
}
