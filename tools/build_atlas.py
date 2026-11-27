#!/usr/bin/env python3
"""Build the Omega tile atlas + bitmap font from DawnLike sprites.

Usage: build_atlas.py <DawnLikeAtlas/renamed dir> <out assets dir> [font.ttf]

Outputs:
  tiles.png / tiles.txt  - packed 16x16 sprites and the key -> layers table
  font.png  / font.txt   - monospace bitmap font, ASCII 32..126

tiles.txt format (one entry per line, '#' comments):
  sprite <name> <x> <y> [<x> <y> ...]          frames of a sprite in tiles.png
  key <key> <layer>[ + <layer> ...]            a layer is <sprite name>[@RRGGBB]
  variants <group> <key> <key> ...             appearance pools (potions, ...)

DawnLike is by DragonDePlatino with the DawnBringer palette, CC-BY 4.0.
Sprite names come from tommyettinger/DawnLikeAtlas (same licence).
"""
import math
import os
import re
import sys
from PIL import Image, ImageDraw, ImageFont

SRC = sys.argv[1]
OUT = sys.argv[2]
FONT = sys.argv[3] if len(sys.argv) > 3 else "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"

# ---------------------------------------------------------------- monsters --
# index == Monsters[] id in src/minit.h
MONSTERS = [
    "killer bee", "priest", "shopkeeper", "guard", "ordinary human",              # 0
    "white sheep", "shopkeeper", "ordinary human", "champion", "soldier",         # 5
    "tsetse fly", "gremlin", "sewer rat", "shrieker", "yellow light",             # 10
    "goblin", "homunculus", "glass golem", "tourist", "kiwi",                     # 15
    "hen", "woodchuck", "nighthawk", "deer", "camel",                             # 20
    "hog", "rabbit", "catfish", "red snapper", "green parrot",                    # 25
    "jackal", "ninja", "winged gargoyle", "thief", "floating eye",                # 30
    "iguana", "invisible creature", "ghost", "wizard", "violet fungus",           # 35
    "hobgoblin", "wolf", "giant ant", "mumak", "brown horse",                     # 40
    "salamander", "titanothere", "ice devil", "toxic cloud", "shade",             # 45
    "goblin prince", "condor", "orc shaman", "lion", "bandit",                    # 50
    "owlbear", "cobra", "jellyfish", "werewolf", "wisp",                          # 55
    "leocrotta", "lynx", "stone golem", "green slime", "peacock",                 # 60
    "wraith", "succubus", "satyr", "crocodile", "baluchitherium",                 # 65
    "hell hound", "dust vortex", "aligned priest", "minion of huhetotl", "locust", # 70
    "spirit", "lurker above", "ninja", "baby firedrake", "shocking sphere",       # 75
    "wyvern", "griffon", "ice devil", "barrow wight", "vampire mage",             # 80
    "shade", "lurker below", "vampire", "purple worm", "energy vortex",           # 85
    "lesser devil", "shade", "fire elemental", "air elemental", "water elemental",# 90
    "earth elemental", "panther", "lich", "marid", "ordinary human",              # 95
    "titanothere", "weretiger", "serpent", "angel", "ninja",                      # 100
    "ettin", "nazgul", "white unicorn", "rodent of unusual size", "pit fiend",    # 105
    "kingwyrm", "horned devil", "cthulhu", "sandestin", "master mind flayer",     # 110
    "eye tyrant", "python", "giant zombie", "freezing sphere", "nalfeshnee",      # 115
    "forest fairy", "mountain fairy", "iron golem", "firedrake", "jabberwock",    # 120
    "frost giant", "tiger shark", "ninja master", "slayer", "high priest",        # 125
    "hezrou", "angel", "storm giant", "invisible creature", "searwyrm",           # 130
    "flaming sphere", "archmage", "vampire lord", "weeping archangel", "asmodeus",# 135
    "death", "juiblex", "archon", "dreadwyrm", "demogorgon",                      # 140
    "xorn", "djinn", "kraken", "surtur", "titan",                                 # 145
]
assert len(MONSTERS) == 150, len(MONSTERS)

# ----------------------------------------------------------------- objects --
# index == Objects[] id in src/iinit.cpp. None => use the category's appearance pool.
TRAP_PART = "misshapen talisman"
OBJECTS = (
    ["camera", "dull clear gem", "lockpicks", "gleaming green gem", "gold foil",
     "stack of green bills", "pot", "key", "lamp"]
    + ["strange object"] * 7
    + ["balance talisman"]
    + [TRAP_PART] * 9
    # food 26..41
    + ["food ration", "lembas wafer", "tin", "tin", "tin", "tin", "lump of royal jelly",
       "c ration", "tomato", "pancake", "k ration", "buns", "corn", "fortune cookie",
       "meatball", "bag"]
    # scrolls 42..65, potions 66..83 -> appearance pools
    + [None] * 24 + [None] * 18
    # weapons 84..109
    + ["dagger", "short sword", "broadsword", "long sword", "katana", "two handed sword",
       "silver saber", "stiletto", "elven broadsword", "scimitar", "axe", "battle axe",
       "angled poleaxe", "mace", "war hammer", "aklys", "flail", "club", "quarterstaff",
       "spear", "hilted polearm", "trident", "bullwhip", "silver saber", "runesword", "mace"]
    # missile weapons 110..114
    + ["longbow", "crossbow", "arrow", "crossbow bolt", "rope"]
    # artifact weapons 115..124
    + ["elven broadsword", "tsurugi", "shuriken", "long sword", "scimitar",
       "elven short sword", "scythe", "club", "scythe", "bullwhip"]
    # armour 125..141
    + ["rain jacket", "lacquered armor", "brass armor", "chain shirt", "mail",
       "scale armor", "bronze armor", "full plate", "iron armor", "breastplate",
       "mirror plate", "sphinx armor", "firedrake armor", "glendrake armor",
       "stormwyrm armor", "darkwyrm armor", "dreadwyrm armor"]
    # shields 142..149
    + ["small shield", "orcish shield", "dwarvish shield", "elven shield",
       "large shield", "white handed shield", "polished silver shield", "polished silver shield"]
    # cloaks 150..156, boots 157..163, rings 164..173, sticks 174..190 -> pools
    + [None] * 7 + [None] * 7 + [None] * 10 + [None] * 17
    # artifacts 191..214
    + ["gleaming violet gem", "gleaming red gem", "gleaming blue gem", "gleaming brown gem",
       "gleaming white gem", "dull gray gem", "gleaming clear gem", "frag grenade",
       "amulet of yendor", "key", "black potion", "glowing wand", "helmet", "radiant potion",
       "jade pendant", "ruby pendant", "aquamarine pendant", "amethyst pendant",
       "garnet pendant", "sapphire pendant", "mounted statue", "gleaming yellow gem",
       "jeweled wand", "jasper amulet"]
    # cash, corpse
    + ["pile of gold coins", "corpse"]
)

# appearance pools: unidentified items pick a sprite by hashing their visible name
def pool(pred):
    return sorted(n for n in NAMES if pred(n))

# -------------------------------------------------------------- terrain ---
# keys produced by the C++ side (src/sdl_hooks.cpp). Layers are joined with '+'.
GRASS = "day grass floor c"
TERRAIN = {
    # dungeon / town
    "floor": "night stone floor c",
    "floor@city": "day stone floor c",
    "floor@village": "day dirt floor c",
    "floor@arena": "day dirt floor c",
    "floor@castle": "dusk tile floor c",
    "floor@mansion": "day tile floor c",
    "floor@house": "day tile floor c",
    "floor@hovel": "dusk dirt floor c",
    "floor@caves": "night dirt floor c",
    "floor@sewers": "night brick floor c",
    "floor@volcano": "night dirt floor c",
    "floor@astral": "night tile floor c",
    "floor@abyss": "night tile floor c",
    "floor@dlair": "night dirt floor c",
    "floor@temple": "day tile floor c",
    "open_door": "@floor + open wooden door front",
    "closed_door": "@floor + closed wooden door front",
    "portcullis": "@floor + closed iron portcullis front",
    "whirlwind": "@floor + little tornado",
    "abyss": "hole tile",
    "lava": "lava tile",
    "hedge": GRASS + " + green scrub",
    "water": "deep water tile",
    "fire": "@floor + little flame",
    "trap": "@floor + hazard tile",
    "lift": "level teleporter tile",
    "stairs_up": "@floor + large stairs up",
    "stairs_down": "@floor + large stairs down",
    "rubble": "@floor + gray rocks",
    "statue": "@floor + statue",
    "altar": "@floor + altar",
    "chair": "@floor + wooden chair right",
    "safe": "@floor + closed safe",
    "furniture": "@floor + wooden table",
    "bed": "@floor + bed a",
    # countryside
    "c_plains": GRASS + " + sparse green grass",
    "c_tundra": "morning tile floor c + sparse gray pebbles",
    "c_road": "day dirt floor c + sparse brown pebbles",
    "c_mountains": GRASS + " + brown peak alone",
    "c_pass": "day dirt floor c + sparse brown rocks",
    "c_river": "deep water tile",
    "c_city": GRASS + " + city",
    "c_village": GRASS + " + town",
    "c_forest": GRASS + " + light pine dense",
    "c_jungle": GRASS + " + palm dense",
    "c_swamp": "dusk grass floor c + sparse green scrub",
    "c_volcano": "day dirt floor c + red volcano alone",
    "c_castle": GRASS + " + castle",
    "c_temple": GRASS + " + palace",
    "c_caves": GRASS + " + burrow",
    "c_desert": "morning dirt floor c + sparse dry scrub",
    "c_chaos_sea": "deep water tile@c070ff",
    "c_starpeak": GRASS + " + blue peak alone",
    "c_dragonlair": "day dirt floor c + dragon skull",
    "c_magic_isle": GRASS + " + gold portal",
    # misc
    "player": "warrior s",
    "pile": "closed chest",
}
# autotiled sets: the renderer appends a suffix chosen from the neighbours
WALL_SUFFIXES = ["center", "flat", "left down", "left right", "left right down",
                 "left right up", "left right up down", "left up", "left up down",
                 "right down", "right up", "right up down", "up down"]
TRAIL_SUFFIXES = ["down left", "down left right", "down right", "left right", "up down",
                  "up down left", "up down left right", "up down right", "up left",
                  "up left right", "up right"]
AUTOTILE = {
    # key prefix -> (sprite prefix, suffixes, base layer)
    "wall": ("bright brick wall ", WALL_SUFFIXES, None),
    "wall@caves": ("bright rock wall ", WALL_SUFFIXES, None),
    "wall@volcano": ("bright heat wall ", WALL_SUFFIXES, None),
    "wall@abyss": ("bright infernal wall ", WALL_SUFFIXES, None),
    "wall@astral": ("bright blue wall ", WALL_SUFFIXES, None),
    "wall@castle": ("bright fort wall ", WALL_SUFFIXES, None),
    "wall@sewers": ("dark brick wall ", WALL_SUFFIXES, None),
    "wall@city": ("bright fort wall ", WALL_SUFFIXES, None),
}
PLAYER_SETS = ["warrior", "rogue", "mage", "paladin"]

# ------------------------------------------------------------------ build --
files = os.listdir(SRC)
frames = {}
for f in files:
    m = re.match(r"(.+?)(?:_(\d+))?\.png$", f)
    if not m:
        continue
    frames.setdefault(m.group(1), []).append((int(m.group(2) or 0), f))
NAMES = set(frames)

missing = []
used = set()


def need(name):
    if name not in NAMES:
        missing.append(name)
    else:
        used.add(name)


def parse_layers(spec):
    out = []
    for part in spec.split("+"):
        part = part.strip()
        out.append(part)
        if part.startswith("@"):
            continue  # reference to another key
        need(part.split("@")[0])
    return out


keys = {}
for i, n in enumerate(MONSTERS):
    keys[f"m{i}"] = parse_layers(n)
for i, n in enumerate(OBJECTS):
    if n:
        keys[f"o{i}"] = parse_layers(n)
for k, v in TERRAIN.items():
    keys[k] = parse_layers(v)
for k, (prefix, sufs, base) in AUTOTILE.items():
    for s in sufs:
        layers = ([base] if base else []) + [prefix + s]
        keys[f"{k}#{s}"] = parse_layers(" + ".join(layers))
for p in PLAYER_SETS:
    for d in "nsew":
        keys[f"player:{p}:{d}"] = parse_layers(f"{p} {d}")

pools = {
    "potion": pool(lambda n: n.endswith(" potion") and n not in ("black potion", "radiant potion")),
    "scroll": pool(lambda n: n.endswith(" scroll") and n != "blank scroll"),
    "ring": pool(lambda n: n.endswith(" ring") and not n.startswith(("unique", "meat", "spoiled", "engagement"))),
    "stick": pool(lambda n: n.endswith(" wand") and not n.startswith(("meat", "spoiled", "glowing", "jeweled"))),
    "cloak": ["desert cloak", "forest cloak", "hill cloak", "moldy cloak"],
    "boots": ["elven boots", "exquisite boots", "leather boots", "mountaineer boots", "quiet boots",
              "buckled shoes", "lead shoes", "tailored shoes", "bog waders"],
}
for g, names in pools.items():
    for i, n in enumerate(names):
        keys[f"{g}#{i}"] = parse_layers(n)

if missing:
    print("MISSING sprites:", sorted(set(missing)))
    sys.exit(1)

# pack every frame of every used sprite into a 16px grid
entries = []
for name in sorted(used):
    fr = sorted(frames[name])
    entries.append((name, [f for _, f in fr]))
total = sum(len(f) for _, f in entries)
cols = 64
rows = math.ceil(total / cols)
atlas = Image.new("RGBA", (cols * 16, rows * 16), (0, 0, 0, 0))
lines = ["# generated by tools/build_atlas.py - DawnLike (CC-BY 4.0) by DragonDePlatino & DawnBringer"]
idx = 0
for name, fs in entries:
    coords = []
    for f in fs:
        im = Image.open(os.path.join(SRC, f)).convert("RGBA")
        if im.size != (16, 16):
            im = im.resize((16, 16), Image.NEAREST)
        x, y = (idx % cols) * 16, (idx // cols) * 16
        atlas.paste(im, (x, y))
        coords += [x, y]
        idx += 1
    lines.append("sprite\t" + name + "\t" + " ".join(map(str, coords)))
for k, layers in keys.items():
    lines.append("key\t" + k + "\t" + "\t".join(layers))
for g, names in pools.items():
    lines.append(f"variants\t{g}\t{len(names)}")
os.makedirs(OUT, exist_ok=True)
atlas.save(os.path.join(OUT, "tiles.png"))
with open(os.path.join(OUT, "tiles.txt"), "w") as fh:
    fh.write("\n".join(lines) + "\n")
print(f"tiles: {len(used)} sprites, {total} frames, {len(keys)} keys -> {cols * 16}x{rows * 16}")

# ------------------------------------------------------------------ font ---
# White glyphs on transparent, fixed cells. Rendered big so it scales down cleanly.
size = 32
font = ImageFont.truetype(FONT, size)
bold = ImageFont.truetype(FONT.replace("Mono.ttf", "Mono-Bold.ttf"), size) if os.path.exists(
    FONT.replace("Mono.ttf", "Mono-Bold.ttf")) else font
asc, desc = font.getmetrics()
cw = round(font.getlength("M"))
ch = asc + desc
img = Image.new("RGBA", (cw * 95, ch * 2), (0, 0, 0, 0))
d = ImageDraw.Draw(img)
for row, f in enumerate((font, bold)):
    for i in range(95):
        d.text((i * cw, row * ch), chr(32 + i), font=f, fill=(255, 255, 255, 255))
img.save(os.path.join(OUT, "font.png"))
with open(os.path.join(OUT, "font.txt"), "w") as fh:
    fh.write(f"{cw} {ch}\n")
print(f"font: {cw}x{ch} cells")

# ------------------------------------------------------------ raw copies ---
# The game loads these so it only needs SDL2 at runtime (no PNG decoder).
import struct
for stem in ("tiles", "font"):
    im = Image.open(os.path.join(OUT, stem + ".png")).convert("RGBA")
    with open(os.path.join(OUT, stem + ".rgba"), "wb") as fh:
        fh.write(struct.pack("<II", im.width, im.height))
        fh.write(im.tobytes())
print("wrote raw RGBA copies")
