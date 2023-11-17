/*
Omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989
Modifications copyright (C) by Lyle Tafoya, 2019, 2021-2023

This file is part of Omega.

Omega is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Omega is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Omega. If not, see <https://www.gnu.org/licenses/>.
*/

/* scr.c */
/* functions that use curses routines directly */
/* plus a few file i/o stuff */
/* also some in file.c */

#include "scr.h"

#include "defs.h"
#include "file.h"
#include "glob.h"
#include "interactive_menu.hpp"
#include "scrolling_buffer.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <curses.h>
#include <format>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

extern bool terminal_size_too_small;
extern bool IsMenu;

int Lasty, Lastx;

#define CHARATTR(c) ((c) & ~0xff)

WINDOW *Showline[MAXITEMS];

WINDOW *message_window, *menu_window;
WINDOW *room_name_window;
WINDOW *name_window, *location_window, *time_window, *level_window;
WINDOW *health_label_window, *health_window, *health_meter_window;
WINDOW *mana_label_window, *mana_window, *mana_meter_window;
WINDOW *strength_label_window, *strength_window;
WINDOW *dexterity_label_window, *dexterity_window;
WINDOW *constitution_label_window, *constitution_window;
WINDOW *agility_label_window, *agility_window;
WINDOW *intelligence_label_window, *intelligence_window;
WINDOW *power_label_window, *power_window;
WINDOW *hitroll_label_window, *hitroll_window;
WINDOW *dmgroll_label_window, *dmgroll_window;
WINDOW *defense_label_window, *defense_window;
WINDOW *absorption_label_window, *absorption_window;
WINDOW *speed_label_window, *speed_window;
WINDOW *experience_label_window, *experience_window;
WINDOW *carry_label_window, *carry_window;
WINDOW *gold_label_window, *gold_window;
WINDOW *hunger_window, *poison_window, *disease_window, *footing_window;
WINDOW *shown_entities_window;

scrolling_buffer message_buffer;
interactive_menu *menu;

int message_window_length = 6;

void enable_attr(WINDOW *window, attr_t attrs)
{
  if(optionp(SHOW_COLOUR, Player))
  {
    wattrset(window, attrs);
  }
}

void color_waddch(WINDOW *window, const chtype ch)
{
  if(optionp(SHOW_COLOUR, Player))
  {
    waddch(window, ch);
  }
  else
  {
    waddch(window, ch & 0xff);
  }
}

void color_mvwaddch(WINDOW *window, int y, int x, const chtype ch)
{
  if(optionp(SHOW_COLOUR, Player))
  {
    mvwaddch(window, y, x, ch);
  }
  else
  {
    mvwaddch(window, y, x, ch & 0xff);
  }
}

void omega_wcolor_set(WINDOW *window, short pair)
{
  if(optionp(SHOW_COLOUR, Player))
  {
    wcolor_set(window, pair, nullptr);
  }
}

void color_waddstr(WINDOW *window, const std::string &s)
{
  std::string::size_type length = s.length();
  for(std::string::size_type i = 0; i < length; ++i)
  {
    if(s[i] == '|' && i < length - 1)
    {
      switch(s[++i])
      {
        case 'l':
          omega_wcolor_set(window, COLOR_BLACK);
          break;
        case 'L':
          omega_wcolor_set(window, COLOR_BLACK + 8);
          break;
        case 'r':
          omega_wcolor_set(window, COLOR_RED);
          break;
        case 'R':
          omega_wcolor_set(window, COLOR_RED + 8);
          break;
        case 'g':
          omega_wcolor_set(window, COLOR_GREEN);
          break;
        case 'G':
          omega_wcolor_set(window, COLOR_GREEN + 8);
          break;
        case 'y':
          omega_wcolor_set(window, COLOR_YELLOW);
          break;
        case 'Y':
          omega_wcolor_set(window, COLOR_YELLOW + 8);
          break;
        case 'b':
          omega_wcolor_set(window, COLOR_BLUE);
          break;
        case 'B':
          omega_wcolor_set(window, COLOR_BLUE + 8);
          break;
        case 'm':
          omega_wcolor_set(window, COLOR_MAGENTA);
          break;
        case 'M':
          omega_wcolor_set(window, COLOR_MAGENTA + 8);
          break;
        case 'c':
          omega_wcolor_set(window, COLOR_CYAN);
          break;
        case 'C':
          omega_wcolor_set(window, COLOR_CYAN + 8);
          break;
        case 'w':
          omega_wcolor_set(window, COLOR_WHITE);
          break;
        case 'W':
          omega_wcolor_set(window, COLOR_WHITE + 8);
          break;
        case '!':
          wattr_on(window, WA_REVERSE, nullptr);
          break;
        case '_':
          wattr_on(window, WA_UNDERLINE, nullptr);
          break;
        case '0':
          wattr_on(window, WA_NORMAL, nullptr);
          break;
        case '|':
          waddch(window, '|');
          break;
      }
    }
    else
    {
      waddch(window, s[i]);
    }
  }
}

void color_mvwaddstr(WINDOW *window, int y, int x, const std::string &s)
{
  wmove(window, y, x);
  color_waddstr(window, s);
}

struct entity_info
{
  std::string name;
  chtype character;
  int count;
  long int value;
};

bool operator<(const entity_info &a, const entity_info &b)
{
  return a.value < b.value;
}

std::unordered_map<std::string, entity_info> shown_mobs;
std::unordered_map<std::string, entity_info> shown_items;
std::array entity_maps{&shown_mobs, &shown_items};
int max_shown_entities;

long int perceived_item_value(object &o)
{
  if(o.known == 2)
  {
    return true_item_value(&o);
  }
  else if(o.known == 1 || Objects[o.id].known)
  {
    return Objects[o.id].basevalue;
  }
  else
  {
    // These values are the computed averages for each type as of February 15 2022
    // TODO calculate these values on startup
    switch(o.objchar)
    {
      case THING:
        return 332;
      case FOOD:
        return 68;
      case SCROLL:
        return 742;
      case POTION:
        return 147;
      case WEAPON:
        return 1131;
      case MISSILEWEAPON:
        return 171;
      case ARMOR:
        return 1402;
      case SHIELD:
        return 438;
      case CLOAK:
        return 716;
      case BOOTS:
        return 808;
      case RING:
        return 241;
      case STICK:
        return 518;
      case ARTIFACT:
        return 3629;
      case CASH:
        return o.basevalue;
      default:
        return 0;
    }
  }
}

void print_shown_entities()
{
  werase(shown_entities_window);
  int entity_count = 0;
  for(auto *entity_map : entity_maps)
  {
    std::priority_queue<entity_info> ordered_entities;
    for(auto &it : *entity_map)
    {
      ordered_entities.push(it.second);
    }
    for(; !ordered_entities.empty() && entity_count < max_shown_entities; ++entity_count)
    {
      wmove(shown_entities_window, entity_count, 0);
      const entity_info &e = ordered_entities.top();
      for(int i = 0; i < e.count; ++i)
      {
        color_waddch(shown_entities_window, e.character);
      }
      wstandend(shown_entities_window);
      wprintw(shown_entities_window, " %s", e.name.c_str());
      ordered_entities.pop();
    }
    if(entity_count == max_shown_entities)
    {
      break;
    }
  }
  wnoutrefresh(shown_entities_window);
}

void print_messages()
{
  werase(message_window);
  const std::deque<std::string> &message_history = message_buffer.get_message_history();
  size_t size = std::min(message_history.size(), static_cast<size_t>(message_window_length));
  uint16_t i  = 0;
  for(auto message = message_history.rbegin(); message != message_history.rend() && i < size;
      ++message, ++i)
  {
    mvwaddstr(message_window, static_cast<int>(size - 1 - i), 0, message->c_str());
  }
  if(size > 0)
  {
    wmove(message_window, static_cast<int>(size - 1), static_cast<int>(message_history.back().size()));
  }
  wnoutrefresh(message_window);
}

void expand_message_window()
{
  message_window_length = LINES;
  wresize(message_window, LINES, COLS);
  mvwin(message_window, 0, 0);
  print_messages();
}

void shrink_message_window()
{
  message_window_length = 6;
  wresize(message_window, 6, COLS);
  mvwin(message_window, ScreenLength, 0);
  print_messages();
}

void clear_message_window()
{
  message_buffer.clear();
  werase(message_window);
}

void calculate_offsets(int x, int y)
{
  ScreenOffset     = std::max(0, std::min(y - ScreenLength / 2, LENGTH - ScreenLength));
  HorizontalOffset = std::max(0, std::min(x - ScreenWidth / 2, WIDTH - ScreenWidth));
}

void show_screen()
{
  werase(level_window);
  shown_items.clear();
  int left   = std::max(0, HorizontalOffset);
  int top    = std::max(0, ScreenOffset);
  int bottom = std::min(LENGTH, ScreenOffset + ScreenLength);
  int right  = std::min(WIDTH, HorizontalOffset + ScreenWidth);

  if(Current_Environment != E_COUNTRYSIDE)
  {
    for(int y = top; y < bottom; ++y)
    {
      wmove(level_window, screenmod(y), 0);
      for(int x = left; x < right; ++x)
      {
        chtype c     = loc_statusp(x, y, SEEN, *Level) ? getspot(x, y, false) : SPACE;
        bool is_pile = c == PILE;
        if(is_pile)
        {
          c = Level->site[x][y].things->thing->objchar | A_STANDOUT;
        }
        color_waddch(level_window, c);

        if((!Level->site[x][y].things || (Level->site[x][y].things->thing->objchar != c && !is_pile)) ||
           (Player.x == x && Player.y == y && (!Player.status[INVISIBLE] || Player.status[TRUESIGHT])) ||
           (Level->site[x][y].creature &&
            (!m_statusp(*Level->site[x][y].creature, M_INVISIBLE) || Player.status[TRUESIGHT])))
        {
          continue;
        }
        object *o = Level->site[x][y].things->thing;
        if(shown_items.find(o->objstr) == shown_items.end())
        {
          std::string obj_name  = itemid(o);
          shown_items[obj_name] = {obj_name, c, 1, perceived_item_value(*o)};
        }
        else
        {
          ++shown_items[o->objstr].count;
        }
      }
    }
  }
  else
  {
    shown_mobs.clear();
    for(int y = top; y < bottom; ++y)
    {
      wmove(level_window, screenmod(y), 0);
      for(int x = left; x < right; ++x)
      {
        chtype c = c_statusp(x, y, SEEN, Country) ? Country[x][y].current_terrain_type : SPACE;
        color_waddch(level_window, c);
      }
    }
  }
  wnoutrefresh(level_window);
  print_shown_entities();
}

int mgetc()
{
  int cursor_visibility = curs_set(1);
  print_messages();
  doupdate();
  int player_input = wgetch(message_window);
  curs_set(cursor_visibility);
  return player_input;
}

/* case insensitive mgetc -- sends uppercase to lowercase */
int mcigetc()
{
  print_messages();
  doupdate();
  int c = wgetch(message_window);
  if(c >= static_cast<int>('A') && c <= static_cast<int>('Z'))
  {
    return c + static_cast<int>('a' - 'A');
  }
  else
  {
    return c;
  }
}

char menugetc()
{
  print_messages();
  doupdate();
  return wgetch(menu_window);
}

int ynq()
{
  int cursor_visibility = curs_set(1);
  print_messages();
  int p;
  do
  {
    doupdate();
    p = wgetch(message_window);
  } while(p != 'n' && p != 'y' && p != 'q' && p != ESCAPE && p != EOF && p != ' ');
  switch(p)
  {
    case 'y':
      message_buffer.append("yes.", false);
      break;
    case ' ':
      p = 'n';
      [[fallthrough]];
    case 'n':
      message_buffer.append("no.", false);
      break;
    case ESCAPE:
      p = 'q';
      [[fallthrough]];
    case 'q':
      message_buffer.append("quit.", false);
      break;
    default:
      assert(p == EOF);
  }
  curs_set(cursor_visibility);
  print_messages();
  return p;
}

void erase_level()
{
  werase(level_window);
  wnoutrefresh(level_window);
}

void queue_message(const std::string &message, bool force_break)
{
  if(!gamestatusp(SUPPRESS_PRINTING, GameStatus))
  {
    message_buffer.receive(message, force_break);
  }
}

void append_message(const std::string &message, bool force_break)
{
  if(!gamestatusp(SUPPRESS_PRINTING, GameStatus))
  {
    message_buffer.append(message, true, force_break);
  }
}

void replace_last_message(const std::string &message)
{
  if(!gamestatusp(SUPPRESS_PRINTING, GameStatus))
  {
    message_buffer.replace_last(message);
  }
}

void omega_title()
{
  showmotd();
  clear();
  touchwin(stdscr);
  wnoutrefresh(stdscr);
}

/* blanks out ith line of menu_window or level_window */
void hide_line(int i)
{
  werase(Showline[i]);
  touchwin(Showline[i]);
  wnoutrefresh(Showline[i]);
}

void calculate_screen_size()
{
  if(LINES < 24 || COLS < 80)
  {
    queue_message("Minimum Screen Size: 24 Lines by 80 Columns.");
    terminal_size_too_small = true;
  }
  else
  {
    terminal_size_too_small = false;
    ScreenLength            = std::min(MAXLENGTH, LINES - 6);
    ScreenWidth             = std::min(MAXWIDTH, COLS - 42);
    max_shown_entities      = LINES - 18;
  }
}

void initialize_colors()
{
  if(!can_change_color())
  {
    return;
  }
  init_color(COLOR_BLACK, 0, 0, 0);
  init_color(COLOR_RED, 698, 94, 94);
  init_color(COLOR_GREEN, 94, 698, 94);
  init_color(COLOR_YELLOW, 698, 408, 94);
  init_color(COLOR_BLUE, 94, 94, 698);
  init_color(COLOR_MAGENTA, 698, 94, 698);
  init_color(COLOR_CYAN, 94, 698, 698);
  init_color(COLOR_WHITE, 698, 698, 698);
  init_color(COLOR_BLACK + 8, 408, 408, 408);
  init_color(COLOR_RED + 8, 1000, 329, 329);
  init_color(COLOR_GREEN + 8, 329, 1000, 329);
  init_color(COLOR_YELLOW + 8, 1000, 1000, 329);
  init_color(COLOR_BLUE + 8, 329, 329, 1000);
  init_color(COLOR_MAGENTA + 8, 1000, 329, 1000);
  init_color(COLOR_CYAN + 8, 329, 1000, 1000);
  init_color(COLOR_WHITE + 8, 1000, 1000, 1000);
}

void initialize_windows()
{
  calculate_screen_size();

  for(int i = 0; i < MAXITEMS; ++i)
  {
    Showline[i] = newwin(1, (COLS - 42 < 64 ? COLS : 64), i, 0);
    werase(Showline[i]);
  }
  uint16_t menu_width = (COLS - 42 < 64 ? COLS : 64);
  menu_window         = newwin(ScreenLength, menu_width, 0, 0);
  keypad(menu_window, true);
  menu = new interactive_menu(menu_window, menu_width, ScreenLength);

  keypad(stdscr, true);
  message_window = newwin(6, COLS, ScreenLength, 0);
  keypad(message_window, true);
  level_window = newwin(ScreenLength, ScreenWidth, 0, 0);
  keypad(level_window, true);
  shown_entities_window     = newwin(max_shown_entities, 41, 13, ScreenWidth + 1);
  name_window               = newwin(1, 33, 0, ScreenWidth + 1);
  time_window               = newwin(1, 8, 0, ScreenWidth + 34);
  health_label_window       = newwin(1, 3, 1, ScreenWidth + 1);
  health_window             = newwin(1, 11, 1, ScreenWidth + 6);
  health_meter_window       = newwin(1, 24, 1, ScreenWidth + 18);
  mana_label_window         = newwin(1, 3, 2, ScreenWidth + 1);
  mana_window               = newwin(1, 11, 2, ScreenWidth + 6);
  mana_meter_window         = newwin(1, 24, 2, ScreenWidth + 18);
  strength_label_window     = newwin(1, 4, 3, ScreenWidth + 1);
  strength_window           = newwin(1, 7, 3, ScreenWidth + 6);
  dexterity_label_window    = newwin(1, 4, 4, ScreenWidth + 1);
  dexterity_window          = newwin(1, 7, 4, ScreenWidth + 6);
  constitution_label_window = newwin(1, 4, 5, ScreenWidth + 1);
  constitution_window       = newwin(1, 7, 5, ScreenWidth + 6);
  agility_label_window      = newwin(1, 4, 6, ScreenWidth + 1);
  agility_window            = newwin(1, 7, 6, ScreenWidth + 6);
  intelligence_label_window = newwin(1, 4, 7, ScreenWidth + 1);
  intelligence_window       = newwin(1, 7, 7, ScreenWidth + 6);
  power_label_window        = newwin(1, 4, 8, ScreenWidth + 1);
  power_window              = newwin(1, 7, 8, ScreenWidth + 6);
  gold_label_window         = newwin(1, 3, 9, ScreenWidth + 1);
  gold_window               = newwin(1, 6, 9, ScreenWidth + 6);
  hitroll_label_window      = newwin(1, 4, 3, ScreenWidth + 18);
  hitroll_window            = newwin(1, 3, 3, ScreenWidth + 25);
  dmgroll_label_window      = newwin(1, 4, 4, ScreenWidth + 18);
  dmgroll_window            = newwin(1, 3, 4, ScreenWidth + 25);
  defense_label_window      = newwin(1, 4, 5, ScreenWidth + 18);
  defense_window            = newwin(1, 3, 5, ScreenWidth + 25);
  absorption_label_window   = newwin(1, 4, 6, ScreenWidth + 18);
  absorption_window         = newwin(1, 3, 6, ScreenWidth + 25);
  speed_label_window        = newwin(1, 4, 7, ScreenWidth + 18);
  speed_window              = newwin(1, 4, 7, ScreenWidth + 25);
  experience_label_window   = newwin(1, 6, 8, ScreenWidth + 18);
  experience_window         = newwin(1, 9, 8, ScreenWidth + 25);
  carry_label_window        = newwin(1, 6, 9, ScreenWidth + 18);
  carry_window              = newwin(1, 11, 9, ScreenWidth + 25);
  hunger_window             = newwin(1, 8, 3, ScreenWidth + 32);
  poison_window             = newwin(1, 8, 4, ScreenWidth + 32);
  disease_window            = newwin(1, 8, 5, ScreenWidth + 32);
  footing_window            = newwin(1, 10, 6, ScreenWidth + 32);
  location_window           = newwin(1, 41, 11, ScreenWidth + 1);
  room_name_window          = newwin(1, 41, 12, ScreenWidth + 1);

  clear();
  touchwin(stdscr);
}

void resize_screen()
{
  calculate_screen_size();

  for(int i = 0; i < MAXITEMS; ++i)
  {
    wresize(Showline[i], 1, (COLS - 42 < 64 ? COLS : 64));
  }

  wresize(level_window, ScreenLength, ScreenWidth);
  wresize(shown_entities_window, max_shown_entities, 41);
  wresize(message_window, 6, COLS);
  menu->resize((COLS - 42 < 64 ? COLS : 64), ScreenLength);

  mvwin(message_window, ScreenLength, 0);
  mvwin(shown_entities_window, 13, ScreenWidth + 1);
  mvwin(name_window, 0, ScreenWidth + 1);
  mvwin(time_window, 0, ScreenWidth + 34);
  mvwin(health_label_window, 1, ScreenWidth + 1);
  mvwin(health_window, 1, ScreenWidth + 6);
  mvwin(health_meter_window, 1, ScreenWidth + 18);
  mvwin(mana_label_window, 2, ScreenWidth + 1);
  mvwin(mana_window, 2, ScreenWidth + 6);
  mvwin(mana_meter_window, 2, ScreenWidth + 18);
  mvwin(strength_label_window, 3, ScreenWidth + 1);
  mvwin(strength_window, 3, ScreenWidth + 6);
  mvwin(dexterity_label_window, 4, ScreenWidth + 1);
  mvwin(dexterity_window, 4, ScreenWidth + 6);
  mvwin(constitution_label_window, 5, ScreenWidth + 1);
  mvwin(constitution_window, 5, ScreenWidth + 6);
  mvwin(agility_label_window, 6, ScreenWidth + 1);
  mvwin(agility_window, 6, ScreenWidth + 6);
  mvwin(intelligence_label_window, 7, ScreenWidth + 1);
  mvwin(intelligence_window, 7, ScreenWidth + 6);
  mvwin(power_label_window, 8, ScreenWidth + 1);
  mvwin(power_window, 8, ScreenWidth + 6);
  mvwin(gold_label_window, 9, ScreenWidth + 1);
  mvwin(gold_window, 9, ScreenWidth + 6);
  mvwin(hitroll_label_window, 3, ScreenWidth + 18);
  mvwin(hitroll_window, 3, ScreenWidth + 25);
  mvwin(dmgroll_label_window, 4, ScreenWidth + 18);
  mvwin(dmgroll_window, 4, ScreenWidth + 25);
  mvwin(defense_label_window, 5, ScreenWidth + 18);
  mvwin(defense_window, 5, ScreenWidth + 25);
  mvwin(absorption_label_window, 6, ScreenWidth + 18);
  mvwin(absorption_window, 6, ScreenWidth + 25);
  mvwin(speed_label_window, 7, ScreenWidth + 18);
  mvwin(speed_window, 7, ScreenWidth + 25);
  mvwin(experience_label_window, 8, ScreenWidth + 18);
  mvwin(experience_window, 8, ScreenWidth + 25);
  mvwin(carry_label_window, 9, ScreenWidth + 18);
  mvwin(carry_window, 9, ScreenWidth + 25);
  mvwin(hunger_window, 3, ScreenWidth + 32);
  mvwin(poison_window, 4, ScreenWidth + 32);
  mvwin(disease_window, 5, ScreenWidth + 32);
  mvwin(footing_window, 6, ScreenWidth + 32);
  mvwin(location_window, 11, ScreenWidth + 1);
  mvwin(room_name_window, 12, ScreenWidth + 1);
  xredraw();
}

int get_player_input(WINDOW *window)
{
  print_messages();
  while(true)
  {
    int x, y;
    getyx(window, y, x);
    doupdate();
    int player_input = mvwgetch(window, y, x);
    if(player_input == KEY_RESIZE)
    {
      resize_screen();
      print_messages();
      if(IsMenu)
      {
        menu->print();
      }
      else
      {
        screencheck(Lastx, Lasty);
        omshowcursor(Lastx, Lasty);
      }
    }
    else if(!terminal_size_too_small)
    {
      return player_input;
    }
  }
}

int get_message_input()
{
  return get_player_input(message_window);
}

int get_level_input()
{
  return get_player_input(level_window);
}

void more_wait()
{
  if(gamestatusp(SUPPRESS_PRINTING, GameStatus))
  {
    return;
  }
  append_message("--MORE--", true);
  int player_input;
  do
  {
    player_input = get_message_input();
  } while(player_input != KEY_ENTER && player_input != '\n' && player_input != ' ');
  message_buffer.pop_back();
}

void mouse_enable()
{
  mousemask(ALL_MOUSE_EVENTS, nullptr);
  mouseinterval(0);
}

void mouse_disable()
{
  mousemask(0, nullptr);
}

bool pop_mouse_event(mouse_event &event, MEVENT &mevent)
{
  if(getmouse(&mevent) == OK)
  {
    for(int button_num = 0; button_num < num_mouse_buttons; ++button_num)
    {
      for(int event_type_num = 0; event_type_num < num_mouse_event_types; ++event_type_num)
      {
        if(mevent.bstate & mouse_button_events[button_num][event_type_num])
        {
          event.type   = static_cast<mouse_event_t>(event_type_num);
          event.button = button_num + 1;
          event.x      = mevent.x;
          event.y      = mevent.y;
          return true;
        }
      }
    }
  }
  return false;
}

int get_mouse_event(WINDOW *window, mouse_event &event)
{
  MEVENT mevent;
  if(!pop_mouse_event(event, mevent))
  {
    return 0;
  }
  if(event.type == MOUSE_BUTTON_PRESSED)
  {
    wtimeout(window, 150);
    int input;
    while(event.type != MOUSE_BUTTON_TRIPLE_CLICKED && (input = wgetch(window)) != ERR)
    {
      if(input == KEY_MOUSE)
      {
        mouse_event rel;
        pop_mouse_event(rel, mevent);
        if(rel.type == MOUSE_BUTTON_RELEASED && rel.button == event.button)
        {
          if(event.type == MOUSE_BUTTON_PRESSED)
          {
            event.type = MOUSE_BUTTON_CLICKED;
          }
          else if(event.type == MOUSE_BUTTON_CLICKED)
          {
            event.type = MOUSE_BUTTON_DOUBLE_CLICKED;
          }
          else if(event.type == MOUSE_BUTTON_DOUBLE_CLICKED)
          {
            event.type = MOUSE_BUTTON_TRIPLE_CLICKED;
          }
        }
        else if(rel.type != MOUSE_BUTTON_PRESSED || rel.button != event.button)
        {
          ungetmouse(&mevent);
          break;
        }
      }
      else
      {
        ungetch(input);
        break;
      }
    }
    wtimeout(window, -1);
  }
  return mouse_button_events[event.button - 1][event.type];
}

int get_mouse_event(mouse_event &event)
{
  return get_mouse_event(level_window, event);
}

// initialize, screen, windows
void initgraf()
{
  initscr();
  noecho();
  crmode();
  start_color();
  clrgen_init();
  initialize_colors();
  initialize_windows();
  mouse_enable();
  curs_set(0);
#ifndef PDCURSES
  set_escdelay(0);
#endif
}

int lastx = -1, lasty = -1;

void drawplayer()
{
  if(Current_Environment == E_COUNTRYSIDE)
  {
    if(inbounds(lastx, lasty) && !offscreen(lastx, lasty))
    {
      chtype c = Country[lastx][lasty].current_terrain_type;
      color_mvwaddch(level_window, screenmod(lasty), screenmod_horizontal(lastx), c);
    }
    color_mvwaddch(level_window, screenmod(Player.y), screenmod_horizontal(Player.x), PLAYER);
  }
  else
  {
    if(inbounds(lastx, lasty) && !offscreen(lastx, lasty))
    {
      plotspot(lastx, lasty, !Player.status[BLINDED]);
    }
    if(!Player.status[INVISIBLE] || Player.status[TRUESIGHT])
    {
      color_mvwaddch(level_window, screenmod(Player.y), screenmod_horizontal(Player.x), PLAYER);
    }
    else
    {
      chtype c = getspot(Player.x, Player.y, false);
      color_mvwaddch(level_window, screenmod(Player.y), screenmod_horizontal(Player.x), c | A_REVERSE);
    }
  }
  lastx = Player.x;
  lasty = Player.y;
}

void setlastxy(int new_x, int new_y) /* used when changing environments */
{
  lastx = new_x;
  lasty = new_y;
}

bool litroom(int x, int y)
{
  if(Level->site[x][y].roomnumber < ROOMBASE)
  {
    return false;
  }
  else
  {
    return loc_statusp(x, y, LIT, *Level) || Player.status[ILLUMINATION];
  }
}

void drawvision(int x, int y)
{
  static int oldx = -1, oldy = -1;
  int i, j;

  if(Current_Environment != E_COUNTRYSIDE)
  {
    if(Player.status[BLINDED])
    {
      drawspot(oldx, oldy);
      drawspot(x, y);
      drawplayer();
    }
    else
    {
      if(Player.status[ILLUMINATION] > 0)
      {
        for(i = -2; i < 3; i++)
        {
          for(j = -2; j < 3; j++)
          {
            if(inbounds(x + i, y + j))
            {
              if(view_los_p(x + i, y + j, Player.x, Player.y))
              {
                dodrawspot(x + i, y + j);
              }
            }
          }
        }
      }
      else
      {
        for(i = -1; i < 2; i++)
        {
          for(j = -1; j < 2; j++)
          {
            if(inbounds(x + i, y + j))
            {
              dodrawspot(x + i, y + j);
            }
          }
        }
      }
      drawplayer();
      drawmonsters(false); /* erase all monsters */
      drawmonsters(true);  /* draw those now visible */
    }
    if((!gamestatusp(FAST_MOVE, GameStatus)) || (!optionp(JUMPMOVE, Player)))
    {
      wnoutrefresh(level_window);
    }
    oldx = x;
    oldy = y;
  }
  else
  {
    for(i = -1; i < 2; ++i)
    {
      for(j = -1; j < 2; ++j)
      {
        if(!inbounds(x + i, y + j) || c_statusp(x + i, y + j, SEEN, Country))
        {
          continue;
        }
        c_set(x + i, y + j, SEEN, Country);
        if(!offscreen(x + i, y + j))
        {
          chtype c = Country[x + i][y + j].current_terrain_type;
          color_mvwaddch(level_window, screenmod(y + j), screenmod_horizontal(x + i), c);
        }
      }
    }
    drawplayer();
    wnoutrefresh(level_window);
  }
}

void omshowcursor(int x, int y)
{
  if(!offscreen(x, y))
  {
    wmove(level_window, screenmod(y), screenmod_horizontal(x));
    wnoutrefresh(level_window);
  }
}

void levelrefresh()
{
  wnoutrefresh(level_window);
}

/* draws a particular spot under if in line-of-sight */
void drawspot(int x, int y)
{
  if(inbounds(x, y))
  {
    chtype c = getspot(x, y, false);
    if(c != Level->site[x][y].showchar)
    {
      if(view_los_p(Player.x, Player.y, x, y))
      {
        lset(x, y, SEEN, *Level);
        Level->site[x][y].showchar = c;
        putspot(x, y, c);
      }
    }
  }
}

/* draws a particular spot regardless of line-of-sight */
void dodrawspot(int x, int y)
{
  if(inbounds(x, y))
  {
    chtype c = getspot(x, y, false);
    if(c != Level->site[x][y].showchar)
    {
      lset(x, y, SEEN, *Level);
      Level->site[x][y].showchar = c;
      putspot(x, y, c);
    }
  }
}

/* write a blank to a spot if it is floor */
void blankoutspot(int i, int j)
{
  if(inbounds(i, j))
  {
    lreset(i, j, LIT, *Level);
    lset(i, j, CHANGED, *Level);
    if(Level->site[i][j].locchar == FLOOR)
    {
      Level->site[i][j].showchar = SPACE;
      putspot(i, j, SPACE);
    }
  }
}

/* blank out a spot regardless */
void blotspot(int i, int j)
{
  if(inbounds(i, j))
  {
    lreset(i, j, SEEN, *Level);
    Level->site[i][j].showchar = SPACE;
    if(!offscreen(i, j))
    {
      color_mvwaddch(level_window, screenmod(j), screenmod_horizontal(i), SPACE);
    }
  }
}

/* for displaying activity specifically at some point */
void plotspot(int x, int y, int showmonster)
{
  if(loc_statusp(x, y, SEEN, *Level))
  {
    putspot(x, y, getspot(x, y, showmonster));
  }
  else
  {
    putspot(x, y, SPACE);
  }
}

/* Puts c at x,y on screen. No fuss, no bother. */
void putspot(int x, int y, chtype c)
{
  if(!offscreen(x, y))
  {
    color_mvwaddch(level_window, screenmod(y), screenmod_horizontal(x), c);
  }
}

/* regardless of line of sight, etc, draw a monster */
void plotmon(monster *m)
{
  if(!offscreen(m->x, m->y))
  {
    color_mvwaddch(level_window, screenmod(m->y), screenmod_horizontal(m->x), m->monchar);
  }
}

/* if display, displays monsters, otherwise erases them */
void drawmonsters(int display)
{
  shown_mobs.clear();
  int left   = std::max(0, HorizontalOffset);
  int top    = std::max(0, ScreenOffset);
  int bottom = std::min(LENGTH, ScreenOffset + ScreenLength);
  int right  = std::min(WIDTH, HorizontalOffset + ScreenWidth);
  for(monster *m : Level->mlist)
  {
    if(m->hp > 0)
    {
      if(display)
      {
        if(view_los_p(Player.x, Player.y, m->x, m->y))
        {
          if(Player.status[TRUESIGHT] || (!m_statusp(*m, M_INVISIBLE)))
          {
            if(!optionp(SHOW_COLOUR, Player) && (m->level > 5) && ((m->monchar & 0xff) != '@') &&
               ((m->monchar & 0xff) != '|'))
            {
              wstandout(level_window);
            }
            putspot(m->x, m->y, m->monchar);
            if(!optionp(SHOW_COLOUR, Player))
            {
              wstandend(level_window);
            }

            if(m->x < left || m->x > right || m->y < top || m->y > bottom)
            {
              continue;
            }
            if(shown_mobs.find(m->monstring) == shown_mobs.end())
            {
              shown_mobs[m->monstring] = {m->monstring, m->monchar, 1, m->level};
            }
            else
            {
              ++shown_mobs[m->monstring].count;
            }
          }
        }
      }
      else
      {
        erase_monster(m);
      }
    }
  }
  print_shown_entities();
}

/* replace monster with what would be displayed if monster weren't there */
void erase_monster(monster *m)
{
  if(loc_statusp(m->x, m->y, SEEN, *Level))
  {
    putspot(m->x, m->y, getspot(m->x, m->y, false));
  }
  else
  {
    blotspot(m->x, m->y);
  }
}

/* find apt char to display at some location */
chtype getspot(int x, int y, int showmonster)
{
  if(loc_statusp(x, y, SECRET, *Level))
  {
    return (WALL);
  }
  else
  {
    switch(Level->site[x][y].locchar)
    {
      case WATER:
        if(!Level->site[x][y].creature)
        {
          return (WATER);
        }
        else if(m_statusp(*Level->site[x][y].creature, SWIMMING))
        {
          return (WATER);
        }
        else if(showmonster)
        {
          return (Level->site[x][y].creature->monchar);
        }
        else
        {
          return (WATER);
        }
      /* these sites never show anything but their location char's */
      case CLOSED_DOOR:
      case LAVA:
      case FIRE:
      case ABYSS:
        return (Level->site[x][y].locchar);
      /* rubble and hedge don't show items on their location */
      case RUBBLE:
      case HEDGE:
        if(showmonster && Level->site[x][y].creature)
        {
          if((m_statusp(*Level->site[x][y].creature, M_INVISIBLE)) && (!Player.status[TRUESIGHT]))
          {
            return (getspot(x, y, false));
          }
          else
          {
            return (Level->site[x][y].creature->monchar);
          }
        }
        else
        {
          return (Level->site[x][y].locchar);
        }
      /* everywhere else, first try to show monster, next show items, next show
       location char */
      default:
        if(showmonster && Level->site[x][y].creature)
        {
          if((m_statusp(*Level->site[x][y].creature, M_INVISIBLE)) && (!Player.status[TRUESIGHT]))
          {
            return (getspot(x, y, false));
          }
          else
          {
            return (Level->site[x][y].creature->monchar);
          }
        }
        else if(Level->site[x][y].things)
        {
          if(Level->site[x][y].things->next)
          {
            return PILE;
          }
          else
          {
            return Level->site[x][y].things->thing->objchar;
          }
        }
        else
        {
          return Level->site[x][y].locchar;
        }
    }
  }
}

void commanderror()
{
  std::string message = std::to_string(Cmd) + " : unknown command";
  message_buffer.receive(message);
}

void timeprint()
{
  werase(time_window);
  enable_attr(time_window, A_BOLD);
  wprintw(time_window, "%2d:%d", showhour(), showminute());
  if(showminute() == 0)
  {
    waddch(time_window, '0');
  }
  wprintw(time_window, hour() > 11 ? " PM" : " AM");
  wnoutrefresh(time_window);
}

void print_name()
{
  werase(name_window);
  enable_attr(name_window, A_BOLD);
  waddstr(name_window, std::format("{}, {}", Player.name, levelname(Player.level)).c_str());
  wnoutrefresh(name_window);
}

void print_combat_stats()
{
  werase(hitroll_label_window);
  enable_attr(hitroll_label_window, CLR(GREY) | A_BOLD);
  waddstr(hitroll_label_window, "Hit:");
  wnoutrefresh(hitroll_label_window);

  werase(hitroll_window);
  enable_attr(hitroll_window, CLR(WHITE) | A_BOLD);
  wprintw(hitroll_window, "%d", Player.hit);
  wnoutrefresh(hitroll_window);

  werase(dmgroll_label_window);
  enable_attr(dmgroll_label_window, CLR(GREY));
  waddstr(dmgroll_label_window, "Dmg:");
  wnoutrefresh(dmgroll_label_window);

  werase(dmgroll_window);
  enable_attr(dmgroll_window, A_BOLD);
  wprintw(dmgroll_window, "%d", Player.dmg);
  wnoutrefresh(dmgroll_window);

  werase(defense_label_window);
  enable_attr(defense_label_window, CLR(GREY));
  waddstr(defense_label_window, "Def:");
  wnoutrefresh(defense_label_window);

  werase(defense_window);
  enable_attr(defense_window, A_BOLD);
  wprintw(defense_window, "%d", Player.defense);
  wnoutrefresh(defense_window);

  werase(absorption_label_window);
  enable_attr(absorption_label_window, CLR(GREY));
  waddstr(absorption_label_window, "Arm:");
  wnoutrefresh(absorption_label_window);

  werase(absorption_window);
  enable_attr(absorption_window, A_BOLD);
  wprintw(absorption_window, "%d", Player.absorption);
  wnoutrefresh(absorption_window);

  werase(speed_label_window);
  enable_attr(speed_label_window, CLR(GREY));
  waddstr(speed_label_window, "Spd:");
  wnoutrefresh(speed_label_window);

  werase(speed_window);
  enable_attr(speed_window, A_BOLD);
  wprintw(speed_window, "%d.%d", 5 / Player.speed, 500 / Player.speed % 100);
  wnoutrefresh(speed_window);
}

void print_health()
{
  int hp_meter_length = std::min(
    24, Player.maxhp == 0 ? 0 : static_cast<int>(static_cast<float>(Player.hp) / Player.maxhp * 24)
  );
  std::string hp_meter_filled(hp_meter_length, '=');
  std::string hp_meter_empty(24 - hp_meter_length, '-');

  werase(health_label_window);
  enable_attr(health_label_window, CLR(GREY));
  waddstr(health_label_window, "HP:");
  wnoutrefresh(health_label_window);

  werase(health_window);
  enable_attr(health_window, A_BOLD);
  wprintw(health_window, "%d", Player.hp);
  color_waddch(health_window, '/' | CLR(GREY));
  enable_attr(health_window, A_BOLD);
  wprintw(health_window, "%d", Player.maxhp);
  wnoutrefresh(health_window);

  werase(health_meter_window);
  enable_attr(health_meter_window, CLR(GREEN) | A_BOLD);
  waddstr(health_meter_window, hp_meter_filled.c_str());
  enable_attr(health_meter_window, CLR(GREY));
  waddstr(health_meter_window, hp_meter_empty.c_str());
  wnoutrefresh(health_meter_window);
}

void print_mana()
{
  int mana_meter_length = std::min(
    24, Player.maxmana == 0 ? 0 : static_cast<int>(static_cast<float>(Player.mana) / Player.maxmana * 24)
  );
  std::string mana_meter_filled(mana_meter_length, '=');
  std::string mana_meter_empty(24 - mana_meter_length, '-');

  werase(mana_label_window);
  enable_attr(mana_label_window, CLR(GREY));
  waddstr(mana_label_window, "MP:");
  wnoutrefresh(mana_label_window);

  werase(mana_window);
  enable_attr(mana_window, A_BOLD);
  wprintw(mana_window, "%ld", Player.mana);
  color_waddch(mana_window, '/' | CLR(GREY));
  enable_attr(mana_window, A_BOLD);
  wprintw(mana_window, "%ld", Player.maxmana);
  wnoutrefresh(mana_window);

  werase(mana_meter_window);
  enable_attr(mana_meter_window, CLR(BLUE) | A_BOLD);
  waddstr(mana_meter_window, mana_meter_filled.c_str());
  enable_attr(mana_meter_window, CLR(GREY));
  waddstr(mana_meter_window, mana_meter_empty.c_str());
  wnoutrefresh(mana_meter_window);
}

void print_vitals()
{
  print_health();
  print_mana();
}

void print_strength()
{
  werase(strength_label_window);
  enable_attr(strength_label_window, CLR(GREY));
  waddstr(strength_label_window, "Str:");
  wnoutrefresh(strength_label_window);

  werase(strength_window);
  enable_attr(strength_window, A_BOLD);
  wprintw(strength_window, "%d", Player.str);
  color_waddch(strength_window, '/' | CLR(GREY));
  enable_attr(strength_window, A_BOLD);
  wprintw(strength_window, "%d", Player.maxstr);
  wnoutrefresh(strength_window);
}

void print_dexterity()
{
  werase(dexterity_label_window);
  enable_attr(dexterity_label_window, CLR(GREY));
  waddstr(dexterity_label_window, "Dex:");
  wnoutrefresh(dexterity_label_window);

  werase(dexterity_window);
  enable_attr(dexterity_window, A_BOLD);
  wprintw(dexterity_window, "%d", Player.dex);
  color_waddch(dexterity_window, '/' | CLR(GREY));
  enable_attr(dexterity_window, A_BOLD);
  wprintw(dexterity_window, "%d", Player.maxdex);
  wnoutrefresh(dexterity_window);
}

void print_constitution()
{
  werase(constitution_label_window);
  enable_attr(constitution_label_window, CLR(GREY));
  waddstr(constitution_label_window, "Con:");
  wnoutrefresh(constitution_label_window);

  werase(constitution_window);
  enable_attr(constitution_window, A_BOLD);
  wprintw(constitution_window, "%d", Player.con);
  color_waddch(constitution_window, '/' | CLR(GREY));
  enable_attr(constitution_window, A_BOLD);
  wprintw(constitution_window, "%d", Player.maxcon);
  wnoutrefresh(constitution_window);
}

void print_agility()
{
  werase(agility_label_window);
  enable_attr(agility_label_window, CLR(GREY));
  waddstr(agility_label_window, "Agi:");
  wnoutrefresh(agility_label_window);

  werase(agility_window);
  enable_attr(agility_window, A_BOLD);
  wprintw(agility_window, "%d", Player.agi);
  color_waddch(agility_window, '/' | CLR(GREY));
  enable_attr(agility_window, A_BOLD);
  wprintw(agility_window, "%d", Player.maxagi);
  wnoutrefresh(agility_window);
}

void print_intelligence()
{
  werase(intelligence_label_window);
  enable_attr(intelligence_label_window, CLR(GREY));
  waddstr(intelligence_label_window, "Int:");
  wnoutrefresh(intelligence_label_window);

  werase(intelligence_window);
  enable_attr(intelligence_window, A_BOLD);
  wprintw(intelligence_window, "%d", Player.iq);
  color_waddch(intelligence_window, '/' | CLR(GREY));
  enable_attr(intelligence_window, A_BOLD);
  wprintw(intelligence_window, "%d", Player.maxiq);
  wnoutrefresh(intelligence_window);
}

void print_power()
{
  werase(power_label_window);
  enable_attr(power_label_window, CLR(GREY));
  waddstr(power_label_window, "Pow:");
  wnoutrefresh(power_label_window);

  werase(power_window);
  enable_attr(power_window, A_BOLD);
  wprintw(power_window, "%d", Player.pow);
  color_waddch(power_window, '/' | CLR(GREY));
  enable_attr(power_window, A_BOLD);
  wprintw(power_window, "%d", Player.maxpow);
  wnoutrefresh(power_window);
}

void print_attributes()
{
  print_strength();
  print_dexterity();
  print_constitution();
  print_agility();
  print_intelligence();
  print_power();
}

void print_gold()
{
  werase(gold_label_window);
  enable_attr(gold_label_window, CLR(GREY));
  waddstr(gold_label_window, "Au:");
  wnoutrefresh(gold_label_window);

  werase(gold_window);
  enable_attr(gold_window, A_BOLD);
  wprintw(gold_window, "%ld", Player.cash);
  wnoutrefresh(gold_window);
}

void print_level()
{
  werase(experience_label_window);
  enable_attr(experience_label_window, CLR(GREY));
  waddstr(experience_label_window, "Level:");
  wnoutrefresh(experience_label_window);

  werase(experience_window);
  enable_attr(experience_window, A_BOLD);
  wprintw(experience_window, "%d", Player.level);
  color_waddch(experience_window, '/' | CLR(GREY));
  enable_attr(experience_window, A_BOLD);
  wprintw(experience_window, "%ld", Player.xp);
  wnoutrefresh(experience_window);
}

void print_weight()
{
  werase(carry_label_window);
  enable_attr(carry_label_window, CLR(GREY));
  waddstr(carry_label_window, "Carry:");
  wnoutrefresh(carry_label_window);

  werase(carry_window);
  enable_attr(carry_window, A_BOLD);
  wprintw(carry_window, "%d", Player.itemweight);
  color_waddch(carry_window, '/' | CLR(GREY));
  enable_attr(carry_window, A_BOLD);
  wprintw(carry_window, "%d", Player.maxweight);
  wnoutrefresh(carry_window);
}

void dataprint()
{
  print_name();
  print_vitals();
  print_attributes();
  print_gold();
  print_level();
  print_weight();
}

/* redraw each permanent window */
void xredraw()
{
  touchwin(stdscr);
  touchwin(message_window);
  touchwin(level_window);
  touchwin(name_window);
  touchwin(time_window);
  touchwin(health_label_window);
  touchwin(health_window);
  touchwin(health_meter_window);
  touchwin(mana_label_window);
  touchwin(mana_window);
  touchwin(mana_meter_window);
  touchwin(strength_label_window);
  touchwin(strength_window);
  touchwin(dexterity_label_window);
  touchwin(dexterity_window);
  touchwin(constitution_label_window);
  touchwin(constitution_window);
  touchwin(agility_label_window);
  touchwin(agility_window);
  touchwin(intelligence_label_window);
  touchwin(intelligence_window);
  touchwin(power_label_window);
  touchwin(power_window);
  touchwin(hitroll_label_window);
  touchwin(hitroll_window);
  touchwin(dmgroll_label_window);
  touchwin(dmgroll_window);
  touchwin(defense_label_window);
  touchwin(defense_window);
  touchwin(absorption_label_window);
  touchwin(absorption_window);
  touchwin(speed_label_window);
  touchwin(speed_window);
  touchwin(hunger_window);
  touchwin(poison_window);
  touchwin(disease_window);
  touchwin(footing_window);
  touchwin(carry_label_window);
  touchwin(carry_window);
  touchwin(gold_label_window);
  touchwin(gold_window);
  touchwin(experience_label_window);
  touchwin(experience_window);
  touchwin(location_window);
  touchwin(shown_entities_window);

  wnoutrefresh(stdscr);
  wnoutrefresh(message_window);
  wnoutrefresh(level_window);
  wnoutrefresh(name_window);
  wnoutrefresh(time_window);
  wnoutrefresh(health_label_window);
  wnoutrefresh(health_window);
  wnoutrefresh(health_meter_window);
  wnoutrefresh(mana_label_window);
  wnoutrefresh(mana_window);
  wnoutrefresh(mana_meter_window);
  wnoutrefresh(strength_label_window);
  wnoutrefresh(strength_window);
  wnoutrefresh(dexterity_label_window);
  wnoutrefresh(dexterity_window);
  wnoutrefresh(constitution_label_window);
  wnoutrefresh(constitution_window);
  wnoutrefresh(agility_label_window);
  wnoutrefresh(agility_window);
  wnoutrefresh(intelligence_label_window);
  wnoutrefresh(intelligence_window);
  wnoutrefresh(power_label_window);
  wnoutrefresh(power_window);
  wnoutrefresh(hitroll_label_window);
  wnoutrefresh(hitroll_window);
  wnoutrefresh(dmgroll_label_window);
  wnoutrefresh(dmgroll_window);
  wnoutrefresh(defense_label_window);
  wnoutrefresh(defense_window);
  wnoutrefresh(absorption_label_window);
  wnoutrefresh(absorption_window);
  wnoutrefresh(speed_label_window);
  wnoutrefresh(speed_window);
  wnoutrefresh(hunger_window);
  wnoutrefresh(poison_window);
  wnoutrefresh(disease_window);
  wnoutrefresh(footing_window);
  wnoutrefresh(carry_label_window);
  wnoutrefresh(carry_window);
  wnoutrefresh(gold_label_window);
  wnoutrefresh(gold_window);
  wnoutrefresh(experience_label_window);
  wnoutrefresh(experience_window);
  wnoutrefresh(location_window);
  wnoutrefresh(shown_entities_window);
}

void menuaddch(char c)
{
  waddch(menu_window, c);
  wnoutrefresh(menu_window);
}

bool stillonblock()
{
  bool display = true;
  int c;
  do
  {
    werase(message_window);
    if(display)
    {
      wprintw(message_window, "<<<STAY?>>>");
    }
    else
    {
      wprintw(message_window, ">>>STAY?<<<");
    }
    display = !display;
    doupdate();
    c = wgetch(message_window);
  } while((c != ' ') && (c != ESCAPE) && (c != EOF));
  werase(message_window);
  wnoutrefresh(message_window);
  return (c == ' ');
}

void menuclear()
{
  werase(menu_window);
  touchwin(menu_window);
  wnoutrefresh(menu_window);
}

void menuprint(const std::string &s)
{
  if(getcury(menu_window) >= ScreenLength - 2)
  {
    wnoutrefresh(menu_window);
    werase(menu_window);
    touchwin(menu_window);
  }
  wprintw(menu_window, "%s", s.c_str());
}

void showmenu()
{
  wnoutrefresh(menu_window);
}

void endgraf()
{
  clear();
  touchwin(stdscr);
  refresh();
  endwin();
}

void plotchar(chtype pyx, int x, int y)
{
  if(!offscreen(x, y))
  {
    color_mvwaddch(level_window, screenmod(y), screenmod_horizontal(x), pyx);
    wnoutrefresh(level_window);
  }
}

void draw_explosion(chtype pyx, int x, int y)
{
  int i, j;

  for(j = 0; j < 3; j++)
  {
    for(i = 0; i < 9; i++)
    {
      plotchar(pyx, x + Dirs[0][i], y + Dirs[1][i]);
    }
    doupdate();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    for(i = 0; i < 9; i++)
    {
      plotchar(SPACE, x + Dirs[0][i], y + Dirs[1][i]);
    }
    doupdate();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
  }
  for(i = 0; i < 9; i++)
  {
    plotspot(x + Dirs[0][i], y + Dirs[1][i], true);
  }
  wnoutrefresh(level_window);
}

std::string msgscanstring()
{
  int cursor_visibility = curs_set(1);
  std::string input_str;
  int player_input = mgetc();
  while(player_input != '\n')
  {
    if(player_input == KEY_BACKSPACE || player_input == '\b' || player_input == KEY_DC || player_input == DELETE)
    {
      if(!input_str.empty())
      {
        input_str.pop_back();
        dobackspace();
      }
    }
    else if(player_input >= 32 && player_input <= 126)
    {
      input_str.push_back(static_cast<char>(player_input));
      message_buffer.append(std::string(1, player_input), false);
      print_messages();
    }
    player_input = mgetc();
  }
  curs_set(cursor_visibility);
  return input_str;
}

void locprint(const std::string &s)
{
  werase(location_window);
  enable_attr(location_window, A_BOLD);
  waddstr(location_window, s.c_str());
  wnoutrefresh(location_window);
}

void room_name_print(const std::string &room_name)
{
  werase(room_name_window);
  enable_attr(room_name_window, A_BOLD);
  waddstr(room_name_window, room_name.c_str());
  wnoutrefresh(room_name_window);
}

// draw everything whether visible or not
void drawscreen()
{
  int i, j;
  if(Current_Environment == E_COUNTRYSIDE)
  {
    for(i = 0; i < WIDTH; i++)
    {
      for(j = 0; j < LENGTH; j++)
      {
        c_set(i, j, SEEN, Country);
      }
    }
  }
  else
  {
    for(i = 0; i < WIDTH; i++)
    {
      for(j = 0; j < LENGTH; j++)
      {
        lset(i, j, SEEN, *Level);
      }
    }
  }
  if(Current_Environment == E_CITY)
  {
    for(i = 0; i < NUMCITYSITES; i++)
    {
      CitySiteList[i][0] = 1;
    }
  }
  show_screen();
}

/*selects a number up to range */

int getnumber(int range)
{
  int done = false, value = 1;
  int atom;

  if(range == 1)
  {
    return (1);
  }
  else
  {
    std::string message = "How many? Change with < or >, ESCAPE to select: " + std::to_string(value);
    append_message(message, true);
    while(!done)
    {
      message = "How many? Change with < or >, ESCAPE to select: " + std::to_string(value);
      message_buffer.replace_last(message);
      do
      {
        atom = mcigetc();
      } while((atom != '<') && (atom != '>') && (atom != ESCAPE));
      if((atom == '>') && (value < range))
      {
        value++;
      }
      else if((atom == '<') && (value > 1))
      {
        value--;
      }
      else if(atom == ESCAPE)
      {
        done = true;
      }
    }
  }
  return (value);
}

/* reads a positive number up to 999999 */
long parsenum()
{
  int number[8];
  int place = -1;
  int i, mult = 1;
  long num = 0;

  int player_input = ' ';
  while(player_input != ESCAPE && player_input != '\n')
  {
    player_input = mgetc();
    if(player_input == KEY_BACKSPACE || player_input == '\b' || player_input == KEY_DC || player_input == DELETE)
    {
      if(place > -1)
      {
        number[place] = 0;
        --place;
        dobackspace();
      }
    }
    else if(player_input <= '9' && player_input >= '0' && place < 7)
    {
      ++place;
      number[place] = player_input;
      message_buffer.append(std::string(1, player_input), false);
      print_messages();
    }
  }
  if(player_input == ESCAPE)
  {
    return ABORT;
  }
  else
  {
    for(i = place; i >= 0; --i)
    {
      num += mult * (number[i] - '0');
      mult *= 10;
    }
    return num;
  }
}

void maddch(char c)
{
  waddch(message_window, c);
  wnoutrefresh(message_window);
}

void display_death(const std::string &source)
{
  clear();
  touchwin(stdscr);
  addstr(std::format("\n\n\n\nRequiescat In Pace, {} ({} points)\n", Player.name, calc_points()).c_str()
  );
  std::string killed_by{std::format("Killed by {}", source)};
  addstr(killed_by.c_str());
  addstr(".\n\n\n\n\nHit 'c' to continue.");
  while(wgetch(stdscr) != 'c')
  {
    ;
  }
  clear();
  touchwin(stdscr);
  refresh();
  extendlog(killed_by, DEAD);
}

void display_win()
{
  clear();
  touchwin(stdscr);
  std::string win_message;
  if(Player.rank[ADEPT])
  {
    addstr(std::format("\n\n\n\n{} is a total master of omega with {} points!", Player.name, FixedPoints)
             .c_str());
    printw(" is a total master of omega with %ld points!", FixedPoints);
    win_message = "A total master of omega";
  }
  else
  {
    win_message = "retired a winner";
    addstr(
      std::format("\n\n\n\n{} triumphed in omega with {} points!", Player.name, calc_points()).c_str()
    );
  }
  addstr("\n\n\n\n\nHit 'c' to continue.");
  while(getch() != 'c')
  {
    ;
  }
  clear();
  touchwin(stdscr);
  refresh();
  if(Player.rank[ADEPT])
  {
    extendlog(win_message, BIGWIN);
  }
  else
  {
    extendlog(win_message, WIN);
  }
}

void display_quit()
{
  clear();
  touchwin(stdscr);
  std::string quit_message = "A quitter.";
  addstr(
    std::format("\n\n\n\n{} wimped out with {} points!\n\n\n\n\n", Player.name, calc_points()).c_str()
  );
  addstr("Hit 'c' to continue.");
  while(getch() != 'c')
  {
    ;
  }
  clear();
  touchwin(stdscr);
  refresh();
  extendlog(quit_message, QUIT);
}

void display_bigwin()
{
  clear();
  touchwin(stdscr);
  std::string win_message = "retired, an Adept of Omega.";
  addstr(
    std::format("\n\n\n\n{} {} with {} points!\n\n\n\n\n", Player.name, win_message, FixedPoints).c_str()
  );
  addstr("Hit 'c' to continue.");
  while(getch() != 'c')
  {
    ;
  }
  clear();
  touchwin(stdscr);
  refresh();
  extendlog(win_message, BIGWIN);
}

void dobackspace()
{
  std::string last_message = message_buffer.get_message_history().back();
  last_message.pop_back();
  message_buffer.replace_last(last_message);
  print_messages();
}

void print_hunger_status()
{
  werase(hunger_window);

  if(Player.food < 0)
  {
    wprintw(hunger_window, "Starving");
  }
  else if(Player.food <= 3)
  {
    wprintw(hunger_window, "Weak");
  }
  else if(Player.food <= 10)
  {
    wprintw(hunger_window, "Ravenous");
  }
  else if(Player.food <= 20)
  {
    wprintw(hunger_window, "Hungry");
  }
  else if(Player.food <= 30)
  {
    wprintw(hunger_window, "Peckish");
  }
  else if(Player.food <= 36)
  {
    wprintw(hunger_window, "Content");
  }
  else if(Player.food <= 44)
  {
    wprintw(hunger_window, "Satiated");
  }
  else
  {
    wprintw(hunger_window, "Bloated");
  }

  wnoutrefresh(hunger_window);
}

void print_poison_status()
{
  werase(poison_window);

  if(Player.status[POISONED] > 0)
  {
    wprintw(poison_window, "Poisoned");
  }
  else
  {
    wprintw(poison_window, "Vigorous");
  }

  wnoutrefresh(poison_window);
}

void print_disease_status()
{
  werase(disease_window);

  if(Player.status[DISEASED] > 0)
  {
    wprintw(disease_window, "Diseased");
  }
  else
  {
    wprintw(disease_window, "Healthy");
  }

  wnoutrefresh(disease_window);
}

void print_footing_status()
{
  werase(footing_window);

  if(gamestatusp(MOUNTED, GameStatus))
  {
    wprintw(footing_window, "Mounted");
  }
  else if(Player.status[LEVITATING])
  {
    wprintw(footing_window, "Levitating");
  }
  else
  {
    wprintw(footing_window, "Afoot");
  }

  wnoutrefresh(footing_window);
}

void showflags()
{
  print_hunger_status();
  print_poison_status();
  print_disease_status();
  print_footing_status();
  doupdate();
}

void drawomega()
{
  clear();
  touchwin(stdscr);
  for(int i = 0; i < 7; ++i)
  {
    move(1, 1);
    enable_attr(stdscr, CHARATTR(CLR(LIGHT_BLUE)));
    printw("\n\n\n");
    printw("\n                                    *****");
    printw("\n                               ******   ******");
    printw("\n                             ***             ***");
    printw("\n                           ***                 ***");
    printw("\n                          **                     **");
    printw("\n                         ***                     ***");
    printw("\n                         **                       **");
    printw("\n                         **                       **");
    printw("\n                         ***                     ***");
    printw("\n                          ***                   ***");
    printw("\n                            **                 **");
    printw("\n                       *   ***                ***   *");
    printw("\n                        ****                    ****");
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    move(1, 1);
    enable_attr(stdscr, CHARATTR(CLR(CYAN)));
    printw("\n\n\n");
    printw("\n                                    +++++");
    printw("\n                               ++++++   ++++++");
    printw("\n                             +++             +++");
    printw("\n                           +++                 +++");
    printw("\n                          ++                     ++");
    printw("\n                         +++                     +++");
    printw("\n                         ++                       ++");
    printw("\n                         ++                       ++");
    printw("\n                         +++                     +++");
    printw("\n                          +++                   +++");
    printw("\n                            ++                 ++");
    printw("\n                       +   +++                +++   +");
    printw("\n                        ++++                    ++++");
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    move(1, 1);
    enable_attr(stdscr, CHARATTR(CLR(BLUE)));
    printw("\n\n\n");
    printw("\n                                    .....");
    printw("\n                               ......   ......");
    printw("\n                             ...             ...");
    printw("\n                           ...                 ...");
    printw("\n                          ..                     ..");
    printw("\n                         ...                     ...");
    printw("\n                         ..                       ..");
    printw("\n                         ..                       ..");
    printw("\n                         ...                     ...");
    printw("\n                          ...                   ...");
    printw("\n                            ..                 ..");
    printw("\n                       .   ...                ...   .");
    printw("\n                        ....                    ....");
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
  wattrset(stdscr, CHARATTR(CLR(WHITE)));
}

/* y is an absolute coordinate */
/* ScreenOffset is the upper left hand corner of the current screen
   in absolute coordinates */

void screencheck(int x, int y)
{
  Lastx = x;
  Lasty = y;
  calculate_offsets(x, y);
  show_screen();
  if(Current_Environment != E_COUNTRYSIDE)
  {
    drawmonsters(true);
  }
  if(!offscreen(Player.x, Player.y))
  {
    drawplayer();
  }
}

void spreadroomlight(int x, int y, int roomno)
{
  if(inbounds(x, y) && !loc_statusp(x, y, LIT, *Level) && Level->site[x][y].roomnumber == roomno)
  {
    lightspot(x, y);
    spreadroomlight(x + 1, y, roomno);
    spreadroomlight(x, y + 1, roomno);
    spreadroomlight(x - 1, y, roomno);
    spreadroomlight(x, y - 1, roomno);
  }
}

/* illuminate one spot at x y */
void lightspot(int x, int y)
{
  lset(x, y, LIT, *Level);
  lset(x, y, SEEN, *Level);
  lset(x, y, CHANGED, *Level);
  chtype c                   = getspot(x, y, false);
  Level->site[x][y].showchar = c;
  putspot(x, y, c);
}

void spreadroomdark(int x, int y, int roomno)
{
  if(inbounds(x, y))
  {
    if(loc_statusp(x, y, LIT, *Level) && (Level->site[x][y].roomnumber == roomno))
    {
      blankoutspot(x, y);
      spreadroomdark(x + 1, y, roomno);
      spreadroomdark(x, y + 1, roomno);
      spreadroomdark(x - 1, y, roomno);
      spreadroomdark(x, y - 1, roomno);
    }
  }
}

void display_pack()
{
  if(Player.packptr < 1)
  {
    queue_message("Pack is empty.");
  }
  else
  {
    std::vector<std::string> lines;
    lines.emplace_back("Items in Pack:");
    for(int i = 0; i < Player.packptr; i++)
    {
      lines.emplace_back(std::format("  {}: {}", static_cast<char>('a' + i), itemid(Player.pack[i])));
    }
    menu->load(lines);
    menu->get_player_input();
  }
}

object *find_first_pack_obj(uint8_t slot)
{
  setgamestatus(SUPPRESS_PRINTING, GameStatus);
  for(uint8_t i = 0; i < MAXPACK; ++i)
  {
    if(slottable(Player.pack[i], slot))
    {
      resetgamestatus(SUPPRESS_PRINTING, GameStatus);
      return Player.pack[i];
    }
  }
  resetgamestatus(SUPPRESS_PRINTING, GameStatus);
  return nullptr;
}

constexpr std::array SLOT_NAMES{
  "Up in Air     ", "Ready Hand    ", "Weapon Hand   ", "Left Shoulder ",
  "Right Shoulder", "Belt          ", "Belt          ", "Belt          ",
  "Shield        ", "Armor         ", "Boots         ", "Cloak         ",
  "Finger        ", "Finger        ", "Finger        ", "Finger        ",
};

void print_inventory_menu(chtype item_type)
{
  std::vector<std::string> lines;
  std::string line;
  for(uint8_t i = 1; i < SLOT_NAMES.size(); ++i)
  {
    object *item = Player.possessions[i];

    bool wildcard   = (item_type == NULL_ITEM || item_type == CASH);
    bool selectable = false;
    if((item && (item->objchar == item_type || wildcard)) || (!item && wildcard && find_first_pack_obj(i)))
    {
      selectable = true;
      line       = std::format("|Y{} |y-|w ", index_to_key(i));
    }
    else
    {
      line = "    ";
    }
    if(selectable)
    {
      line += std::format("|y{}:", SLOT_NAMES[i]);
    }
    else
    {
      line += std::format("|L{}:", SLOT_NAMES[i]);
    }
    if(item)
    {
      line += std::format("|w {}", itemid(item));
    }
    else
    {
      line += std::format("{} -|w", selectable ? "|w" : "|L");
    }
    lines.emplace_back(line);
  }
  menu->load(lines);
  menu->print();
}

int move_slot(int oldslot, int newslot, int maxslot)
{
  if((newslot >= 0) && (newslot < maxslot))
  {
    mvwaddstr(Showline[oldslot], 0, 0, "--");
    wnoutrefresh(Showline[oldslot]);
    wstandout(Showline[newslot]);
    mvwaddstr(Showline[newslot], 0, 0, ">>");
    wstandend(Showline[newslot]);
    wnoutrefresh(Showline[newslot]);
    return (newslot);
  }
  else
  {
    return (oldslot);
  }
}

void colour_on() {}

void colour_off()
{
  wattrset(level_window, CHARATTR(CLR(WHITE)));
}

void display_option_slot(int slot)
{
  hide_line(slot);
  werase(Showline[slot]);
  switch(slot)
  {
    case 1:
      wprintw(Showline[slot], "-- Option BELLICOSE [TF]: ");
      wprintw(Showline[slot], optionp(BELLICOSE, Player) ? "(now T) " : "(now F) ");
      break;
    case 2:
      wprintw(Showline[slot], "-- Option JUMPMOVE [TF]: ");
      wprintw(Showline[slot], optionp(JUMPMOVE, Player) ? "(now T) " : "(now F) ");
      break;
    case 3:
      wprintw(Showline[slot], "-- Option RUNSTOP [TF]: ");
      wprintw(Showline[slot], optionp(RUNSTOP, Player) ? "(now T) " : "(now F) ");
      break;
    case 4:
      wprintw(Showline[slot], "-- Option PICKUP [TF]: ");
      wprintw(Showline[slot], optionp(PICKUP, Player) ? "(now T) " : "(now F) ");
      break;
    case 5:
      wprintw(Showline[slot], "-- Option CONFIRM [TF]: ");
      wprintw(Showline[slot], optionp(CONFIRM, Player) ? "(now T) " : "(now F) ");
      break;
    case 6:
      wprintw(Showline[slot], "-- Option PARANOID_CONFIRM [TF]: ");
      wprintw(Showline[slot], optionp(PARANOID_CONFIRM, Player) ? "(now T) " : "(now F) ");
      break;
    case 7:
      wprintw(Showline[slot], "-- Option COLOUR [TF]: ");
      wprintw(Showline[slot], optionp(SHOW_COLOUR, Player) ? "(now T) " : "(now F) ");
      break;
    case 8:
      wprintw(Showline[slot], "-- Option MOUSE_ENABLED [TF]: ");
      wprintw(Showline[slot], optionp(MOUSE_ENABLED, Player) ? "(now T) " : "(now F} ");
      break;
    case VERBOSITY_LEVEL:
      wprintw(Showline[slot], "-- Option VERBOSITY [(T)erse,(M)edium,(V)erbose]: (now ");
      if(Verbosity == VERBOSE)
      {
        wprintw(Showline[slot], "Verbose)");
      }
      else if(Verbosity == MEDIUM)
      {
        wprintw(Showline[slot], "Medium)");
      }
      else
      {
        wprintw(Showline[slot], "Terse)");
      }
      break;
    case SEARCH_DURATION:
      wprintw(Showline[slot], "-- Option SEARCHNUM [0>x>10]: (now %d)", Searchnum);
      break;
  }
  wnoutrefresh(Showline[slot]);
}

void display_options()
{
  menuclear();
  hide_line(0);
  for(int i = 1; i <= NUMOPTIONS; ++i)
  {
    display_option_slot(i);
  }
}

/* nya ha ha ha ha haaaa.... */
void deathprint()
{
  mgetc();
  waddch(message_window, 'D');
  mgetc();
  waddch(message_window, 'e');
  mgetc();
  waddch(message_window, 'a');
  mgetc();
  waddch(message_window, 't');
  mgetc();
  waddch(message_window, 'h');
  mgetc();
}

int bufferpos = 0;

void bufferprint()
{
  int i = bufferpos - 1, c, finished = 0;
  wprintw(message_window, "^p for previous message, ^n for next, anything else to quit.");
  do
  {
    if(i >= STRING_BUFFER_SIZE)
    {
      i = 0;
    }
    if(i < 0)
    {
      i = STRING_BUFFER_SIZE - 1;
    }
    werase(message_window);
    waddstr(message_window, Stringbuffer[i].c_str());
    c = mgetc();
    if(c == 16)
    { /* ^p */
      i--;
    }
    else if(c == 14)
    { /* ^n */
      i++;
    }
    else
    {
      finished = 1;
    }
  } while(!finished);
  omshowcursor(Player.x, Player.y);
}

void clear_screen()
{
  clear();
  touchwin(stdscr);
  wnoutrefresh(stdscr);
}

void showscores()
{
  read_scores();
  clear();
  addstr(std::format(
           "High Score: {}, by {} ({})\n"
           "{}\n"
           "\n"
           "Lord of Chaos: {} ({})\n"
           "Lord of Law: {} ({})\n"
           "\n"
           "Duke of Rampart:              {} ({})\n"
           "Justiciar:                    {} ({})\n"
           "Commandant:                   {} ({})\n"
           "Champion:                     {} ({})\n"
           "Archmage:                     {} ({})\n"
           "Prime Sorceror:               {} ({})\n"
           "Shadowlord:                   {} ({})\n"
           "\n"
           "High Preists:\n"
           " of Odin:                     {} ({})\n"
           " of Set:                      {} ({})\n"
           " of Athena:                   {} ({})\n"
           " of Hecate:                   {} ({})\n"
           " of the Lords of Destiny:     {} ({})\n"
           "The ArchDruid:                {} ({})\n"
           "Tholian Grandmaster:          {} ({})\n"
           "\n"
           "Hit any key to continue.",
           Hiscore, Hiscorer, levelname(Hilevel), Hidescrip, Chaoslord, levelname(Chaoslordlevel),
           Lawlord, levelname(Lawlordlevel), Duke, levelname(Dukelevel), Justiciar,
           levelname(Justiciarlevel), Commandant, levelname(Commandantlevel), Champion,
           levelname(Championlevel), Archmage, levelname(Archmagelevel), Prime, levelname(Primelevel),
           Shadowlord, levelname(Shadowlordlevel), Priest[ODIN], levelname(Priestlevel[ODIN]),
           Priest[SET], levelname(Priestlevel[SET]), Priest[ATHENA], levelname(Priestlevel[ATHENA]),
           Priest[HECATE], levelname(Priestlevel[HECATE]), Priest[DESTINY],
           levelname(Priestlevel[DESTINY]), Priest[DRUID], levelname(Priestlevel[DRUID]), Grandmaster,
           levelname(Grandmasterlevel)
  )
           .c_str());
  getch();
  clear_screen();
}
