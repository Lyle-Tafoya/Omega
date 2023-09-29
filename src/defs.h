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

/* This file is the header file for all omega modules */
/* defs.h */

#ifndef OMEGA_DEFS_H_
#define OMEGA_DEFS_H_

#include <string>

/* omega will NOT function unless the implementor sets the appropriate
definitions in the following section. */

/*--------------------------USER DEFINITIONS--------------------------*/

/* OMEGALIB is where all the data files reside.
   Note the final / is necessary.
   msdos note: \ is the C string escape character, so you need \\ in the
   path given in OMEGALIB
   This might usually be "/usr/games/lib/omegalib/", for unix,
   or something like "c:\\games\\omega\\omegalib\\" for msdos */

#ifndef OMEGALIB
#  define OMEGALIB            "./lib/"
#endif

/* Comment the following line out if you want users to be able to override */
/* the OMEGALIB define, above, by setting the environment variable OMEGALIB */
/* (I recommend leaving this line uncommented, unless you're compiling */
/* for someone else and don't know where they'll be putting the omegalib */
/* directory, as is the case with compiling executables for home computers. */
/* It would be downright insecure to comment this line out in a multi-user */
/* environment, especially if you're going to run omega setuid.) */

//#define FIXED_OMEGALIB

// set WIZARD to maintainers's username
#ifndef WIZARD
#  define WIZARD            "wtanksle"
#endif

// If CATCH_SIGNALS is set to 1, will not dump core, nicer for players.
// dbx still intercepts the signals first, so it's ok for debugging
#ifndef CATCH_SIGNALS
#  define CATCH_SIGNALS       1
#endif

//---------------------------SYSTEM DEFINITIONS---------------------------

// Don't change anything from here on (unless you know what you're doing)
constexpr int VERSION           = 401;
const std::string VERSIONSTRING = "omega rebirth version 0.4.1";

constexpr int VACANT    = 0;
constexpr int ABORT     = -1;
constexpr int CASHVALUE = -2;

// moderately arbitrary but probably cannot be easily changed
constexpr int MAXWIDTH           = 64;
constexpr int MAXLENGTH          = 64;
constexpr int SMALLSCREENLENGTH  = 16;

// number of slots in inventory. Cannot be changed without work
constexpr int MAXITEMS           = 16;

// number of slots in pack. Should be <= 26
constexpr int MAXPACK            = 26;

// number of items in pawn shop. Should be <= 26
constexpr int PAWNITEMS          = 20;

// number of lines back strings are recalled
constexpr int STRING_BUFFER_SIZE = 25;

// Verbosity levels
enum verbosity_levels
{
  TERSE,
  MEDIUM,
  VERBOSE
};

// Arbitrary. Max of the levels in the dungeons
constexpr int MAXLEVELS     = 21;

// levels in each dungeon
constexpr int ASTRALLEVELS  = 5;
constexpr int SEWERLEVELS   = 18;
constexpr int CASTLELEVELS  = 16;
constexpr int CAVELEVELS    = 10;
constexpr int VOLCANOLEVELS = 20;

// Overall Game Progress Vector Bits
enum game_progress_bits : long
{
  SPOKE_TO_DRUID      = (1 << 0),
  COMPLETED_CAVES     = (1 << 1),
  COMPLETED_SEWERS    = (1 << 2),
  COMPLETED_CASTLE    = (1 << 3),
  COMPLETED_ASTRAL    = (1 << 4),
  COMPLETED_VOLCANO   = (1 << 5),
  KILLED_DRAGONLORD   = (1 << 6),
  KILLED_EATER        = (1 << 7),
  KILLED_LAWBRINGER   = (1 << 8),
  COMPLETED_CHALLENGE = (1 << 9),
  SOLD_CONDO          = (1 << 10),
  FAST_MOVE           = (1 << 11),
  SKIP_PLAYER         = (1 << 12),
  SKIP_MONSTERS       = (1 << 13),
  MOUNTED             = (1 << 14),
  SUPPRESS_PRINTING   = (1 << 15),
  LOST                = (1 << 16),
  ARENA_MODE          = (1 << 17),
  CHEATED             = (1 << 18),
  BANK_BROKEN         = (1 << 19),
  CLUB_MEMBER         = (1 << 20),
  PREPARED_VOID       = (1 << 21),
  DESTROYED_ORDER     = (1 << 22),
  GAVE_STARGEM        = (1 << 23),
  ATTACKED_ORACLE     = (1 << 24),
  UNDEAD_GUARDS       = (1 << 25)
};

// non-existant environments for the random number seeding routine
constexpr int E_RESTORE = -2;
constexpr int E_RANDOM = -1;

// general environment types
enum environment_id
{
  E_NEVER_NEVER_LAND,
  E_COUNTRYSIDE,
  E_CITY,
  E_VILLAGE,
  E_TACTICAL_MAP,
  E_SEWERS,
  E_CASTLE,
  E_CAVES,
  E_VOLCANO,
  E_ASTRAL,
  E_ARENA,
  E_HOVEL,
  E_MANSION,
  E_HOUSE,
  E_DLAIR,
  E_ABYSS,
  E_STARPEAK,
  E_MAGIC_ISLE,
  E_TEMPLE,
  E_CIRCLE,
  E_COURT,
};
constexpr int E_MAX = E_COURT;

// player game status
enum game_end_status
{
  DEAD = 1,
  QUIT,
  WIN,
  BIGWIN,
};

// kind of arbitrary
constexpr int MAXROOMS       = 48;
constexpr int MAXCONNECTIONS = 4;
constexpr int STRING_LEN     = 100;

// some random characters
constexpr int ESCAPE         = 27;
constexpr int DELETE         = 127;

// weapon types
enum weapon_type
{
  CUTTING = 1,
  THRUSTING,
  STRIKING,
  MISSILE,
};

// random aux constants
// aux field for private residences in city
enum building_status
{
  BURGLED  = 2,
  LOCKED   = 3,
  UNLOCKED = 0
};

// cannot use M command on site with this aux value
constexpr int NOCITYMOVE = 666;

/* bow and crossbow object aux fields */
enum bow_status
{
  UNLOADED,
  LOADED
};

// alignment used randomly throughout
enum alignment_id
{
  LAWFUL  = 1,
  CHAOTIC,
  NEUTRAL,
};


// ranks in guilds, etc
enum guild_id
{
  LEGION,
  ARENA,
  COLLEGE,
  THIEVES,
  ORDER,
  CIRCLE,
  NOBILITY,
  PRIESTHOOD,
  ADEPT
};
constexpr int NUMRANKS = 9;

enum legion_ranks
{
  LEGIONAIRE = 1,
  CENTURION,
  FORCE_LEADER,
  COLONEL,
  COMMANDANT
};

enum arena_ranks
{
  TRAINEE = 1,
  BESTIARIUS,
  RETIARIUS,
  GLADIATOR,
  CHAMPION
};

enum college_ranks
{
  NOVICE = 1,
  STUDENT,
  PRECEPTOR,
  MAGE,
  ARCHMAGE
};

enum thieves_ranks
{
  TMEMBER = 1,
  ATHIEF,
  THIEF,
  TMASTER,
  SHADOWLORD
};

enum order_ranks
{
  GALLANT = 1,
  GUARDIAN,
  CHEVALIER,
  PALADIN,
  JUSTICIAR
};

enum circle_ranks
{
  INITIATE = 1,
  ENCHANTER,
  SORCEROR,
  HIGHSORCEROR,
  PRIME
};

enum nobility_ranks
{
  COMMONER = 1,
  ESQUIRE,
  KNIGHT,
  LORD,
  DUKE
};

enum priesthood_ranks
{
  LAY = 1,
  ACOLYTE,
  PRIEST,
  SPRIEST,
  HIGHPRIEST
};

// different priesthoods
enum priesthood_id
{
  ODIN = 1,
  SET,
  ATHENA,
  HECATE,
  DRUID,
  DESTINY
};

// MONSTER STATUS/ABILITY BITS
enum monster_status_bits
{
  AWAKE       = (1 << 0),
  MOBILE      = (1 << 1),
  HOSTILE     = (1 << 2),
  WANDERING   = (1 << 4),
  HUNGRY      = (1 << 5),
  GREEDY      = (1 << 6),
  NEEDY       = (1 << 7),
  ONLYSWIM    = (1 << 8),
  FLYING      = (1 << 9),
  INTANGIBLE  = (1 << 10),
  M_INVISIBLE = (1 << 11),
  SWIMMING    = (1 << 12),
  POISONOUS   = (1 << 13),
  EDIBLE      = (1 << 14)
};

// PLAYER STATUS INDICES
enum status_id
{
  ACCURACY,
  BLINDED,
  SLOWED,
  DISPLACED,
  SLEPT,
  DISEASED,
  POISONED,
  HASTED,
  BREATHING,
  INVISIBLE,
  REGENERATING,
  VULNERABLE,
  BERSERK,
  IMMOBILE,
  ALERT,
  AFRAID,
  HERO,
  LEVITATING,
  ACCURATE,
  TRUESIGHT,
  SHADOWFORM,
  ILLUMINATION,
  DEFLECTION,
  PROTECTION, // PROTECTION is deviant -- indicates protective value, not duration
  RETURNING   // RETURNING is somewhat deviant--how many turns 'til RETURN spell goes off
};
constexpr int NUMSTATI = 25;

// player immunity indices
// also monster immunity bits (2^n)
// also damage types
enum immunity_id
{
  EVERYTHING = -1,
  UNSTOPPABLE,
  NORMAL_DAMAGE,
  FLAME,
  COLD,
  ELECTRICITY,
  POISON,
  ACID,
  FEAR,
  SLEEP,
  NEGENERGY,
  OTHER_MAGIC,
  THEFT,
  GAZE,
  INFECTION
};
constexpr int NUMIMMUNITIES = 14;

// location lstatus bits
enum lstatus_bits
{
  SEEN    = (1 << 0),
  LIT     = (1 << 1),
  SECRET  = (1 << 2),
  STOPS   = (1 << 3),
  CHANGED = (1 << 4)
};

// room string id for use in roomname()
constexpr int NUMROOMNAMES = 30;

#define RS_WALLSPACE        1
#define RS_CORRIDOR         2
#define RS_CAVERN           3
#define RS_GOBLINKING       4
#define RS_DRAGONLORD       5
#define RS_PONDS            6
#define RS_OCEAN            7
#define RS_WYRM             8
#define RS_ADEPT            9
#define RS_DESTINY          10
#define RS_ODIN             11
#define RS_SET              12
#define RS_ATHENA           13
#define RS_HECATE           14
#define RS_DRUID            15
#define RS_COUNTRYSIDE      16
#define RS_ARENA            17
#define RS_SEWER_DUCT       18
#define RS_DRAINED_SEWER    19
#define RS_DROWNED_SEWER    20
#define RS_KITCHEN          21
#define RS_BEDROOM          22
#define RS_BATHROOM         23
#define RS_DININGROOM       24
#define RS_SECRETPASSAGE    25
#define RS_CLOSET           26
#define RS_LOWERASTRAL      27
#define RS_EARTHPLANE       28
#define RS_WATERPLANE       29
#define RS_AIRPLANE         30
#define RS_FIREPLANE        31
#define RS_HIGHASTRAL       32
#define RS_VOLCANO          33
#define RS_STARPEAK         34
#define RS_MAGIC_ISLE       35
#define RS_CIRCLE           36
#define RS_ZORCH            37
#define RS_COURT            38

#define ROOMBASE            39 // normal room name indices start after the RS_ constants

#include <curses.h>

#define COL_FG_BLINK A_BLINK

#ifdef COLOR_PAIR

#  ifdef OMEGA_CLRGEN
#    define CLR(fg)      OMEGA_CLRGEN1 fg
#    define CLRS(fg, bg) OMEGA_CLRGEN2 fg bg
#  else
#    include "clrgen.h"
#    define CLR(fg)      CLR_##fg##_BLACK
#    define CLRS(fg, bg) CLR_##fg##_##bg
#  endif

#else

#  define CLR(fg)      0
#  define CLRS(fg, bg) 0

#endif

// objects, locations, and terrain; characters to draw
#define NULL_ITEM               '\0'
#define SPACE                   (' ' | CLR(WHITE))
#define WALL                    ('#' | CLR(GREY))
#define PORTCULLIS              ('7' | CLR(WHITE))
#define OPEN_DOOR               ('|' | CLR(BROWN))
#define CLOSED_DOOR             ('-' | CLR(BROWN))
#define WHIRLWIND               ('6' | CLR(LIGHT_BLUE))
#define ABYSS                   ('0' | CLRS(BLACK, BROWN))
#define VOID_CHAR               (' ' | CLR(WHITE))
#define LAVA                    ('`' | CLR(RED))
#define HEDGE                   ('\"' | CLR(GREEN))
#define WATER                   ('~' | CLR(BLUE))
#define FIRE                    (';' | CLR(LIGHT_RED))
#define TRAP                    ('^' | CLR(RED))
#define LIFT                    ('_' | CLR(BRIGHT_WHITE))
#define STAIRS_UP               ('<' | CLR(WHITE))
#define STAIRS_DOWN             ('>' | CLR(WHITE))
#define FLOOR                   ('.' | CLR(BROWN))
#define PLAYER                  ('@' | CLRS(BLACK, WHITE))
#define CORPSE                  ('+' | CLR(RED))
#define STATUE                  ('1' | CLR(GREY))
#define RUBBLE                  ('4' | CLR(GREY))
#define ALTAR                   ('8' | CLR(LIGHT_BLUE))
#define CASH                    ('$' | CLR(YELLOW))       // various kinds of money
#define PILE                    ('*' | CLR(BRIGHT_WHITE)) // several objects in one place
#define FOOD                    ('%' | CLR(BROWN))
#define WEAPON                  (')' | CLR(GREY))
#define MISSILEWEAPON           ('(' | CLR(BROWN))
#define SCROLL                  ('?' | CLR(YELLOW))
#define POTION                  ('!' | CLR(LIGHT_GREEN))
#define ARMOR                   (']' | CLR(GREY))
#define SHIELD                  ('[' | CLR(BROWN))
#define CLOAK                   ('}' | CLR(CYAN))
#define BOOTS                   ('{' | CLR(BROWN))
#define STICK                   ('/' | CLR(BROWN))

#define RING                    ('=' | CLR(YELLOW))
#define THING                   ('\\' | CLR(WHITE))
#define ARTIFACT                ('&' | CLR(YELLOW))

// TERRAIN TYPES
#define PLAINS                  ('-' | CLR(LIGHT_GREEN))
#define TUNDRA                  ('_' | CLR(GREY))
#define ROAD                    ('.' | CLR(BROWN))
#define MOUNTAINS               ('^' | CLR(GREY))
#define PASS                    ('v' | CLR(BROWN))
#define RIVER                   ('~' | CLR(BLUE))
#define CITY                    ('O' | CLR(WHITE))
#define VILLAGE                 ('o' | CLR(WHITE))
#define FOREST                  ('(' | CLR(LIGHT_GREEN))
#define JUNGLE                  (')' | CLR(GREEN))
#define SWAMP                   ('=' | CLR(GREEN))
#define VOLCANO                 ('!' | CLR(RED))
#define CASTLE                  ('%' | CLR(GREY))
#define TEMPLE                  ('X' | CLR(BROWN))
#define CAVES                   ('*' | CLRS(BLACK, BROWN))
#define DESERT                  ('\"' | CLR(YELLOW))
#define CHAOS_SEA               ('+' | CLR(LIGHT_PURPLE))
#define STARPEAK                ('|' | CLR(LIGHT_BLUE))
#define DRAGONLAIR              ('$' | CLR(BROWN))
#define MAGIC_ISLE              ('&' | CLR(PURPLE))

#define CHAIR                   ('5' | CLR(BROWN))
#define SAFE                    ('3' | CLR(GREY))
#define FURNITURE               ('2' | CLR(BROWN))
#define BED                     ('9' | CLR(CYAN))

// total number of player options
constexpr int NUMOPTIONS      = 11;

// number of options with true/false values
constexpr int NUMTFOPTIONS    = 9;

// The slot number of the two options not in Player.options
constexpr int VERBOSITY_LEVEL = 10;
constexpr int SEARCH_DURATION = 11;

/* Player.options bits */
enum options_bits
{
  BELLICOSE        = (1 << 0),
  JUMPMOVE         = (1 << 1),
  RUNSTOP          = (1 << 2),
  PICKUP           = (1 << 3),
  CONFIRM          = (1 << 4),
  PARANOID_CONFIRM = (1 << 7),
  SHOW_COLOUR      = (1 << 8)
};

/* This has to be changed whenever an item is added */
#define NUMSCROLLS              24
#define NUMPOTIONS              18
#define NUMFOODS                16
#define NUMTHINGS               26
#define NUMWEAPONS              41
#define NUMARMOR                17
#define NUMSHIELDS              8
#define NUMCLOAKS               7
#define NUMBOOTS                7
#define NUMRINGS                10
#define NUMSTICKS               17
#define NUMARTIFACTS            24

/* running sum of itemtypes, for indexing into Objects array */
#define THINGID                 0
#define FOODID                  NUMTHINGS
#define SCROLLID                (FOODID + NUMFOODS)
#define POTIONID                (SCROLLID + NUMSCROLLS)
#define WEAPONID                (POTIONID + NUMPOTIONS)
#define ARMORID                 (WEAPONID + NUMWEAPONS)
#define SHIELDID                (ARMORID + NUMARMOR)
#define CLOAKID                 (SHIELDID + NUMSHIELDS)
#define BOOTID                  (CLOAKID + NUMCLOAKS)
#define RINGID                  (BOOTID + NUMBOOTS)
#define STICKID                 (RINGID + NUMRINGS)
#define ARTIFACTID              (STICKID + NUMSTICKS)
#define CASHID                  (ARTIFACTID + NUMARTIFACTS)
/* Corpse's aux field is monster id */
#define CORPSEID                (CASHID + 1)

#define TOTALITEMS              (CORPSEID + 1)

/* describing unique items and monsters */
#define COMMON                  0
#define UNIQUE_UNMADE           1
#define UNIQUE_MADE             2
#define UNIQUE_TAKEN            3

/* general item function id's */
#define I_NO_OP                 0
#define I_NOTHING               1

/* note some of these functions are for other types of items too */

/* scroll functions */
#define I_BLESS                 101
#define I_ACQUIRE               102
#define I_ENCHANT               103
#define I_TELEPORT              104
#define I_WISH                  105
#define I_CLAIRVOYANCE          106
#define I_DISPLACE              107
#define I_ID                    108
#define I_FIREFLASH             109
#define I_SPELLS                110
#define I_JANE_T                111
#define I_ALERT                 112
#define I_FLUX                  113
#define I_CHARGE                114
#define I_WARP                  115
#define I_KNOWLEDGE             116
#define I_LAW                   117
#define I_HINT                  118
#define I_HERO                  119
#define I_TRUESIGHT             120
#define I_ILLUMINATE            121
#define I_DEFLECT               122

/* potion functions */
#define I_HEAL                  201
#define I_OBJDET                202
#define I_MONDET                203
#define I_SLEEP_SELF            204
#define I_RESTORE               205
#define I_NEUTRALIZE_POISON     206
#define I_SPEED                 207
#define I_AZOTH                 208
#define I_REGENERATE            210
#define I_INVISIBLE             211
#define I_BREATHING             212
#define I_FEAR_RESIST           213
#define I_AUGMENT               214
#define I_CHAOS                 215
#define I_ACCURACY              216
#define I_LEVITATION            217
#define I_CURE                  218

/* stick functions */
#define I_FIREBOLT              301
#define I_LBOLT                 302
#define I_MISSILE               303
#define I_SLEEP_OTHER           304
#define I_FIREBALL              305
#define I_LBALL                 306
#define I_SUMMON                307
#define I_HIDE                  308
#define I_DISRUPT               309
#define I_DISINTEGRATE          310
#define I_SNOWBALL              311
#define I_APPORT                312
#define I_DISPEL                313
#define I_POLYMORPH             314
#define I_FEAR                  315

/* food functions */
#define I_FOOD                  401
#define I_LEMBAS                402
#define I_STIM                  403
#define I_POW                   404
#define I_IMMUNE                405
#define I_POISON_FOOD           406
#define I_CORPSE                407
#define I_PEPPER_FOOD           408
#define I_CANNIBAL              409
#define I_INEDIBLE              410

/* boots functions */
#define I_PERM_SPEED            501
#define I_PERM_HERO             502
#define I_PERM_LEVITATE         503
#define I_PERM_AGILITY          504
#define I_BOOTS_JUMPING         505
#define I_BOOTS_7LEAGUE         506

/* cloak functions */
#define I_PERM_DISPLACE         601
#define I_PERM_NEGIMMUNE        602
#define I_PERM_INVISIBLE        603
#define I_PERM_ACCURACY         604
#define I_PERM_PROTECTION       605
#define I_PERM_TRUESIGHT        606

/* ring functions */
#define I_PERM_VISION           701
#define I_PERM_BURDEN           702
#define I_PERM_STRENGTH         703
#define I_PERM_GAZE_IMMUNE      704
#define I_PERM_FIRE_RESIST      705
#define I_PERM_POISON_RESIST    706
#define I_PERM_REGENERATE       707
#define I_PERM_KNOWLEDGE        708

/* armor functions */
#define I_PERM_ENERGY_RESIST    801
#define I_PERM_BREATHING        802
#define I_PERM_FEAR_RESIST      803
#define I_NORMAL_ARMOR          804

/* artifact functions */
#define I_ORBFIRE               901
#define I_ORBWATER              902
#define I_ORBEARTH              903
#define I_ORBAIR                904
#define I_ORBMASTERY            905
#define I_ORBDEAD               906
#define I_CRYSTAL               907
#define I_ANTIOCH               908
#define I_KOLWYNIA              909
#define I_DEATH                 910
#define I_ENCHANTMENT           911
#define I_HELM                  912
#define I_LIFE                  913
#define I_JUGGERNAUT            914
#define I_SYMBOL                915
#define I_STARGEM               916
#define I_SCEPTRE               917
#define I_PLANES                918

/* weapons functions */
#define I_NORMAL_WEAPON         1001
#define I_LIGHTSABRE            1002
#define I_DEMONBLADE            1003
#define I_MACE_DISRUPT          1004
#define I_TANGLE                1005
#define I_ARROW                 1006
#define I_BOLT                  1007
#define I_VORPAL                1008
#define I_DESECRATE             1009
#define I_FIRESTAR              1010
#define I_DEFEND                1011
#define I_VICTRIX               1012
#define I_EMPIRE                1013
#define I_SCYTHE                1014
#define I_ACIDWHIP              1015

/* thing functions */
#define I_PICK                  1101
#define I_KEY                   1102
#define I_SHOVEL                1103 /* unused */
#define I_EXCAVATOR             1104 /* unused */
#define I_PERM_ILLUMINATE       1105
#define I_TRAP                  1106
#define I_RAISE_PORTCULLIS      1107
#define I_TORCH_USE             1108
#define I_TORCH_UNEQUIP         1109

/* shield functions */
#define I_PERM_DEFLECT          1201
#define I_NORMAL_SHIELD         1202

/* monster function ids */
/* Its conceivable for a function of one type to be called when another
would usually occur. A monster's special function may be an extra move,
for example. */

#define M_NO_OP                 -1

/* talk functions */
#define M_TALK_STUPID           101
#define M_TALK_SILENT           102
#define M_TALK_HUNGRY           103
#define M_TALK_GREEDY           104
#define M_TALK_TITTER           105
#define M_TALK_MAN              106
#define M_TALK_ROBOT            107
#define M_TALK_EVIL             108
#define M_TALK_BURBLE           109
#define M_TALK_SLITHY           110
#define M_TALK_MIMSY            111
#define M_TALK_SEDUCTOR         112
#define M_TALK_MP               113
#define M_TALK_IM               114
#define M_TALK_GUARD            115
#define M_TALK_GHOST            116
#define M_TALK_HINT             117
#define M_TALK_BEG              118
#define M_TALK_EF               119
#define M_TALK_GF               120
#define M_TALK_MORGON           121
#define M_TALK_LB               122
#define M_TALK_DEMONLOVER       123
#define M_TALK_ASSASSIN         124
#define M_TALK_NINJA            125
#define M_TALK_THIEF            126
#define M_TALK_MERCHANT         127
#define M_TALK_HORSE            128
#define M_TALK_PARROT           129
#define M_TALK_ANIMAL           130
#define M_TALK_HYENA            131
#define M_TALK_SERVANT          132
#define M_TALK_SCREAM           133
#define M_TALK_DRUID            134
#define M_TALK_ARCHMAGE         135
#define M_TALK_PRIME            136

/* ability functions */
#define M_SP_SURPRISE           201
#define M_SP_MP                 202
#define M_SP_THIEF              203
#define M_SP_AGGRAVATE          204
#define M_SP_POISON_CLOUD       205
#define M_SP_HUGE               206
#define M_SP_SUMMON             207
#define M_SP_ILLUSION           208
#define M_SP_FLUTTER            209
#define M_SP_ESCAPE             210
#define M_SP_SPELL              211
#define M_SP_EXPLODE            212
#define M_SP_DEMON              213
#define M_SP_ACID_CLOUD         214
#define M_SP_WHIRL              215
#define M_SP_GHOST              216
#define M_SP_WHISTLEBLOWER      217
#define M_SP_EATER              218
#define M_SP_LAWBRINGER         219
#define M_SP_DRAGONLORD         220
#define M_SP_DE                 221
#define M_SP_DEMONLOVER         222
#define M_SP_SEDUCTOR           223
#define M_SP_MASTER             224
#define M_SP_WYRM               225
#define M_SP_BLACKOUT           226
#define M_SP_BOG                227
#define M_SP_MERCHANT           228
#define M_SP_WERE               229
#define M_SP_SERVANT            231
#define M_SP_AV                 232
#define M_SP_LW                 233
#define M_SP_SWARM              234
#define M_SP_ANGEL              235
#define M_SP_MB                 236
#define M_SP_MIRROR             237
#define M_SP_RAISE              238
#define M_SP_DEATH              239
#define M_SP_COURT              240
#define M_SP_LAIR               241
#define M_SP_PRIME              242

/* rangestrike functions */
#define M_STRIKE_MISSILE        301
#define M_STRIKE_FBOLT          302
#define M_STRIKE_LBALL          303
#define M_STRIKE_FBALL          304
#define M_STRIKE_BLIND          305
#define M_STRIKE_SNOWBALL       306
#define M_STRIKE_MASTER         307
#define M_STRIKE_SONIC          308

/* combat functions */
#define M_MELEE_NORMAL          401
#define M_MELEE_FIRE            402
#define M_MELEE_DRAGON          403
#define M_MELEE_MP              404
#define M_MELEE_ELEC            405
#define M_MELEE_POISON          406
#define M_MELEE_NG              407
#define M_MELEE_SUCCUBUS        408
#define M_MELEE_SPIRIT          409
#define M_MELEE_DISEASE         410
#define M_MELEE_SLEEP           411
#define M_MELEE_COLD            412
#define M_MELEE_MASTER          413
#define M_MELEE_GRAPPLE         414
#define M_MELEE_DEATH           415

/* movement functions */
#define M_MOVE_NORMAL           501
#define M_MOVE_FLUTTER          502
#define M_MOVE_TELEPORT         503
#define M_MOVE_FOLLOW           504
#define M_MOVE_RANDOM           505
#define M_MOVE_SMART            506
#define M_MOVE_SPIRIT           507
#define M_MOVE_SCAREDY          508
#define M_MOVE_CONFUSED         509
#define M_MOVE_ANIMAL           510
#define M_MOVE_LEASH            230

/* MLx -> index to Monsters starting for level x */
/* MLx -> number of monsters of level x or less */
/* NML_x -> number of monsters of level x */
/* NML-X must be changed whenever a monster is added */
/* This whole thing MUST be repaired.  Monster levels must
 * be represented elsewhere. */
#define ML0                     0
#define NML_0                   9
#define ML1                     (ML0 + NML_0) /* 9 */
#define NML_1                   22
#define ML2                     (ML1 + NML_1) /* 31 */
#define NML_2                   14
#define ML3                     (ML2 + NML_2) /* 45 */
#define NML_3                   15
#define ML4                     (ML3 + NML_3) /* 60 */
#define NML_4                   18
#define ML5                     (ML4 + NML_4) /* 78 */
#define NML_5                   14
#define ML6                     (ML5 + NML_5) /* 92 */
#define NML_6                   13
#define ML7                     (ML6 + NML_6) /* 105 */
#define NML_7                   15
#define ML8                     (ML7 + NML_7) /* 120 */
#define NML_8                   12
#define ML9                     (ML8 + NML_8) /* 132 */
#define NML_9                   8
#define ML10                    (ML9 + NML_9) /* 140 */
#define NML_10                  10

#define NUMMONSTERS             (ML10 + NML_10) /* 150 */

/* Some monster ID's : (Those that are explicitly named in code) */
/* Actually, there are still many magic constants floating around. */
/* Eventually I'll get around to making each monster's id a constant.... */
/* done, thanks to David Given. */
#define RANDOM                  -1
#define HORNET                  (ML0 + 0)
#define MEND_PRIEST             (ML0 + 1)
#define ITIN_MERCH              (ML0 + 2)
#define GUARD                   (ML0 + 3)
#define NPC                     (ML0 + 4)
#define SHEEP                   (ML0 + 5)
#define MERCHANT                (ML0 + 6)
#define ZERO_NPC                (ML0 + 7)
#define HISCORE_NPC             (ML0 + 8)
#define GRUNT                   (ML1 + 0)
#define TSETSE                  (ML1 + 1)
#define FNORD                   (ML1 + 2)
#define SEWER_RAT               (ML1 + 3)
#define AGGRAVATOR              (ML1 + 4)
#define BLIPPER                 (ML1 + 5)
#define GOBLIN                  (ML1 + 6)
#define PHANTASTICON            (ML1 + 7)
#define ROBOT                   (ML1 + 8)
#define GEEK                    (ML1 + 9)
#define BOROGROVE               (ML1 + 10)
#define QUAIL                   (ML1 + 11)
#define BADGER                  (ML1 + 12)
#define HAWK                    (ML1 + 13)
#define DEER                    (ML1 + 14)
#define CAMEL                   (ML1 + 15)
#define ANTEATER                (ML1 + 16)
#define BUNNY                   (ML1 + 17)
#define TROUT                   (ML1 + 18)
#define BASS                    (ML1 + 19)
#define PARROT                  (ML1 + 20)
#define HYENA                   (ML1 + 21)
#define APPR_NINJA              (ML2 + 0)
#define NIGHT_GAUNT             (ML2 + 1)
#define SNEAK_THIEF             (ML2 + 2)
#define EYE                     (ML2 + 3)
#define TOVE                    (ML2 + 4)
#define NASTY                   (ML2 + 5)
#define GHOST                   (ML2 + 6)
#define ENCHANTOR               (ML2 + 7) /* use 'OR' to avoid conflict with circle rank */
#define MURK                    (ML2 + 8)
#define GOBLIN_CHIEF            (ML2 + 9)
#define WOLF                    (ML2 + 10)
#define ANT                     (ML2 + 11)
#define ELEPHANT                (ML2 + 12)
#define HORSE                   (ML2 + 13)
#define SALAMANDER              (ML3 + 0)
#define CATOBLEPAS              (ML3 + 1)
#define L_FDEMON                (ML3 + 2)
#define ACID_CLOUD              (ML3 + 3)
#define PHANTOM                 (ML3 + 4)
#define GOBLIN_KING             (ML3 + 5)
#define PTERODACTYL             (ML3 + 6)
#define GOBLIN_SHAMAN           (ML3 + 7)
#define LION                    (ML3 + 8)
#define BRIGAND                 (ML3 + 9)
#define BEAR                    (ML3 + 10)
#define MAMBA                   (ML3 + 11)
#define MANOWAR                 (ML3 + 12)
#define WEREHUMAN               (ML3 + 13)
#define THOUGHTFORM             (ML3 + 14)
#define MANTICORE               (ML4 + 0)
#define TASMANIAN               (ML4 + 1)
#define AUTO_MINOR              (ML4 + 2)
#define DENEBIAN                (ML4 + 3)
#define JUBJUB                  (ML4 + 4)
#define HAUNT                   (ML4 + 5)
#define INCUBUS                 (ML4 + 6)
#define SATYR                   (ML4 + 7)
#define CROC                    (ML4 + 8)
#define TORPOR                  (ML4 + 9)
#define DOBERMAN                (ML4 + 10)
#define FUZZY                   (ML4 + 11)
#define SERV_LAW                (ML4 + 12)
#define SERV_CHAOS              (ML4 + 13)
#define SWARM                   (ML4 + 14)
#define BAN_SIDHE               (ML4 + 15)
#define GRUE                    (ML4 + 16)
#define GENIN                   (ML4 + 17)
#define DRAGONETTE              (ML5 + 0)
#define TESLA                   (ML5 + 1)
#define WYVERN                  (ML5 + 2)
#define CATEAGLE                (ML5 + 3)
#define FROST_DEMON             (ML5 + 4)
#define SPECTRE                 (ML5 + 5)
#define NECROMANCER             (ML5 + 6)
#define SHADOW                  (ML5 + 7)
#define BOGTHING                (ML5 + 8)
#define ASTRAL_VAMP             (ML5 + 9)
#define LAVA_WORM               (ML5 + 10)
#define MANABURST               (ML5 + 11)
#define OUTER_DEMON             (ML5 + 12)
#define MIRRORSHADE             (ML5 + 13)
#define FIRE_ELEM               (ML6 + 0)
#define AIR_ELEM                (ML6 + 1)
#define WATER_ELEM              (ML6 + 2)
#define EARTH_ELEM              (ML6 + 3)
#define BANDERSNATCH            (ML6 + 4)
#define LICHE                   (ML6 + 5)
#define TRITON                  (ML6 + 6)
#define MAST_THIEF              (ML6 + 7)
#define TRICER                  (ML6 + 8)
#define RAKSHASA                (ML6 + 9)
#define DEMON_SERP              (ML6 + 10)
#define ANGEL                   (ML6 + 11)
#define CHUNIN                  (ML6 + 12)
#define BEHEMOTH                (ML7 + 0)
#define NAZGUL                  (ML7 + 1)
#define UNICORN                 (ML7 + 2)
#define ROUS                    (ML7 + 3)
#define ILL_FIEND               (ML7 + 4)
#define GREAT_WYRM              (ML7 + 5)
#define FLAME_DEV               (ML7 + 6)
#define LURKER                  (ML7 + 7)
#define SANDMAN                 (ML7 + 8)
#define MIRRORMAST              (ML7 + 9)
#define ELDER_GRUE              (ML7 + 10)
#define LOATHLY                 (ML7 + 11)
#define ZOMBIE                  (ML7 + 12)
#define RICOCHET                (ML7 + 13)
#define INNER_DEMON             (ML7 + 14)
#define GOOD_FAIRY              (ML8 + 0)
#define BAD_FAIRY               (ML8 + 1)
#define AUTO_MAJOR              (ML8 + 2)
#define DRAGON                  (ML8 + 3)
#define JABBERWOCK              (ML8 + 4)
#define FDEMON_L                (ML8 + 5)
#define TIGERSHARK              (ML8 + 6)
#define JONIN                   (ML8 + 7)
#define SHADOW_SLAY             (ML8 + 8)
#define MIL_PRIEST              (ML8 + 9)
#define COMA                    (ML8 + 10)
#define HIGH_ANGEL              (ML8 + 11)
#define JOTUN                   (ML9 + 0)
#define INVIS_SLAY              (ML9 + 1)
#define KING_WYV                (ML9 + 2)
#define DEATHSTAR               (ML9 + 3)
#define THAUMATURGIST           (ML9 + 4)
#define VAMP_LORD               (ML9 + 5)
#define ARCHANGEL               (ML9 + 6)
#define DEMON_PRINCE            (ML9 + 7)
#define DEATH                   (ML10 + 0)
#define EATER                   (ML10 + 1)
#define LAWBRINGER              (ML10 + 2)
#define DRAGON_LORD             (ML10 + 3)
#define DEMON_EMP               (ML10 + 4)
#define LORD_EARTH              (ML10 + 5)
#define LORD_AIR                (ML10 + 6)
#define LORD_WATER              (ML10 + 7)
#define LORD_FIRE               (ML10 + 8)
#define ELEM_MASTER             (ML10 + 9)

/* location functions */
#define L_NO_OP                 0

/* random sites */
#define L_LIFT                  1
#define L_BALANCESTONE          2
#define L_FIRE                  3
#define L_WHIRLWIND             4
#define L_VOIDSTONE             5
#define L_WARNING               6
#define L_ARENA_EXIT            7
#define L_HOUSE_EXIT            8
#define L_SAFE                  9

/* city level shop and guild functions */
/* following are those in CitySiteList */
#define NUMCITYSITES            27
#define CITYSITEBASE            10
#define L_CHARITY               10
#define L_ARMORER               11
#define L_CLUB                  12
#define L_GYM                   13
#define L_THIEVES_GUILD         14
#define L_COLLEGE               15
#define L_HEALER                16
#define L_CASINO                17
#define L_TAVERN                18
#define L_MERC_GUILD            19
#define L_ALCHEMIST             20
#define L_SORCERORS             21
#define L_CASTLE                22
#define L_ARENA                 23
#define L_DPW                   24
#define L_LIBRARY               25
#define L_PAWN_SHOP             26
#define L_BANK                  27
#define L_CONDO                 28
#define L_ORACLE                29
#define L_ORDER                 30
#define L_DINER                 31
#define L_COMMANDANT            32
#define L_CRAP                  33
#define L_TEMPLE                34
#define L_COUNTRYSIDE           35
#define L_BROTHEL               36
/* end of city sites */

/* random sites */
#define L_JAIL                  37
#define L_TEMPLE_WARNING        38
#define L_LAWSTONE              39
#define L_CHAOSTONE             40

/* final abyss sites ignore levitation*/
#define L_EARTH_STATION         41
#define L_FIRE_STATION          42
#define L_WATER_STATION         43
#define L_AIR_STATION           44
#define L_VOID_STATION          45
#define L_VOID                  46
#define L_VOICE1                47
#define L_VOICE2                48
#define L_VOICE3                49

#define L_SACRIFICESTONE        50

/* circle hq sites */
#define L_TOME1                 51
#define L_TOME2                 52
#define L_ENTER_CIRCLE          53
#define L_CIRCLE_LIBRARY        54

/* other site functions */
#define L_DRUID                 55
#define L_ALTAR                 56

#define L_GARDEN                57
#define L_ADEPT                 58
#define L_SEWER                 59

#define L_OMEGA                 60
#define L_CARTOGRAPHER          61
#define L_STABLES               62
#define L_COMMONS               63
#define L_GRANARY               64
#define L_MAZE                  65
#define L_HOVEL                 66
#define L_HOUSE                 67
#define L_MANSION               68
#define L_OCCUPIED_HOUSE        69
#define L_TACTICAL_EXIT         70
#define L_VAULT                 71
#define L_CEMETARY              72
#define L_THRONE                73
#define L_ESCALATOR             74
#define L_ENTER_COURT           75

#define L_TRIFID                76
#define L_FINAL_ABYSS           77
#define L_RAISE_PORTCULLIS      78

#define L_MINDSTONE             79

/* above LEVITATION_AVOIDANCE, no effect if player is levitating */
#define LEVITATION_AVOIDANCE    80

/* random sites */
#define L_CHAOS                 81
#define L_WATER                 82
#define L_LAVA                  83
#define L_MAGIC_POOL            84
#define L_PORTCULLIS_TRAP       85
#define L_DROP_EVERY_PORTCULLIS 87
#define L_PORTCULLIS            88

/* traps */
#define NUMTRAPS                13
#define TRAP_BASE               89

/* traps */
#define L_TRAP_DART             89
#define L_TRAP_PIT              90
#define L_TRAP_DOOR             91
#define L_TRAP_SNARE            92
#define L_TRAP_BLADE            93
#define L_TRAP_FIRE             94
#define L_TRAP_TELEPORT         95
#define L_TRAP_DISINTEGRATE     96
#define L_TRAP_SLEEP_GAS        97
#define L_TRAP_ACID             98
#define L_TRAP_MANADRAIN        99
#define L_TRAP_ABYSS            100
#define L_TRAP_SIREN            101

/* more random sites */
#define L_STATUE_WAKE           102
#define L_STATUE_RANDOM         103

#define L_HEDGE                 104
#define L_RUBBLE                105

#define L_ABYSS                 106

// player possession slots
enum inventory_slots
{
  O_UP_IN_AIR,
  O_READY_HAND,
  O_WEAPON_HAND,
  O_LEFT_SHOULDER,
  O_RIGHT_SHOULDER,
  O_BELT1,
  O_BELT2,
  O_BELT3,
  O_SHIELD,
  O_ARMOR,
  O_BOOTS,
  O_CLOAK,
  O_RING1,
  O_RING2,
  O_RING3,
  O_RING4
};

/* typedefs needed by structs */

typedef chtype Symbol;

/* structure definitions */

struct room
{
  int lighted;
  int left, right, top, bottom;
  int rsi; /* index into roomname switch */
};

struct monster
{
  struct objectlist *possessions;
  unsigned char      attacked;
  int                aux1, aux2, x, y, click;
  int                id, hp, hit, ac, dmg, sense, wakeup, level, speed;
  unsigned char      sleep, treasure;
  long               xpv;
  int                corpseweight, corpsevalue, transformid, startthing;
  unsigned char      uniqueness;
  int                talkf, movef, meleef, strikef, specialf;
  long               status, immunity;
  Symbol             monchar;
  std::string        monstring;
  std::string        corpsestr;
  std::string        meleestr;
};

struct monsterlist
{
  monster     *m;
  monsterlist *next;
};

struct object
{
  int           id;
  int           weight;
  int           plus;
  int           charge;
  int           dmg;
  int           hit;
  int           aux;
  int           number;
  int           fragility;
  long          basevalue;
  unsigned char known;
  unsigned char used;
  int           blessing;
  unsigned char type;
  unsigned char uniqueness;
  int           on_use;
  int           on_equip;
  int           on_unequip;
  unsigned char level;
  Symbol        objchar;
  std::string   objstr;
  std::string   truename;
  std::string   cursestr;
};

struct player
{
  int            str, con, dex, agi, iq, pow, maxstr, maxcon, maxdex, maxagi, maxiq, maxpow;
  long           xp;
  int            level, hp, maxhp, hit, dmg, absorption, speed, click;
  int            defense, food, alignment;
  long           mana, maxmana;
  long           cash;
  int            patron, birthday;
  int            sx, sy; // sanctuary coordinates
  int            x, y;   // current player coordinates
  int            itemweight, maxweight;
  int            immunity[NUMIMMUNITIES];
  int            status[NUMSTATI];
  long           options;
  int            rank[NUMRANKS];
  long           guildxp[NUMRANKS];
  std::string    name;
  std::string    meleestr;
  object        *possessions[MAXITEMS];
  object        *pack[MAXPACK];
  int            packptr;
};

struct objectlist
{
  struct object     *thing;
  struct objectlist *next;
};

// terrain locations
struct terrain
{
  Symbol base_terrain_type;
  Symbol current_terrain_type;
  char   aux;
  char   status;
};

// dungeon locations
struct location
{
  char               p_locf;     // function executed when moved on
  unsigned char      lstatus;    // seen,stopsrun,lit,secret,
  char               roomnumber; // so room can be named
  Symbol             locchar;    // terrain type
  Symbol             showchar;   // char instantaneously drawn for site
  int                aux;        // signifies various things
  unsigned char      buildaux;   // used in constructing level
  struct objectlist *things;
  struct monster    *creature;
};

struct level
{
  char          depth; // which level is this
  struct level *next;  // pointer to next level in dungeon
#ifdef SAVE_LEVELS
  // Over 64K worth of data!
  struct location *site[MAXWIDTH]; // dungeon data
#else
  struct location site[MAXWIDTH][MAXLENGTH]; // dungeon data
#endif
  char                generated;    // has the level been made (visited) yet?
  char                numrooms;     // number of rooms on level
  char                tunnelled;    // amount of tunnelling done on this level
  monsterlist        *mlist;        // List of monsters on level
  int                 environment;  // where kind of level is this?
  int                 last_visited; // time player was last on this level
};

/* random typedef's */

typedef monsterlist mltype;
typedef mltype            *pml;

typedef struct monster montype;
typedef montype       *pmt;

typedef struct location loctype;
typedef loctype        *plc;

typedef struct level levtype;
typedef levtype     *plv;

typedef struct object objtype;
typedef objtype      *pob;

typedef struct objectlist oltype;
typedef oltype           *pol;

/* random  function declarations from system libraries */

#undef sign
/* These must be made to work for both longs and ints */
#define sign(n) (((n) < 0) ? -1 : (((n) > 0) ? 1 : 0))

/* WDT: This should be harmless under ANSI C, and will stop
 * some errors under bizarre platforms. */
constexpr long pow2(int n)
{
  return 1L << n;
}

inline bool loc_statusp(int x, int y, int status, const level &lvl)
{
  return ((lvl.site[x][y].lstatus & (status)) ? true : false);
}
inline void lset(int x, int y, int status, level &lvl)
{
  lvl.site[x][y].lstatus |= (status);
}
inline void lreset(int x, int y, int status, level &lvl)
{
  lvl.site[x][y].lstatus &= ~(status);
}

inline bool c_statusp(int x, int y, int status, const terrain (&country)[MAXWIDTH][MAXLENGTH])
{
  return ((country[x][y].status & (status)) ? true : false);
}
inline void c_set(int x, int y, int status, terrain (&country)[MAXWIDTH][MAXLENGTH])
{
  country[x][y].status |= (status);
}

inline bool m_statusp(const monster &m, int status)
{
  return ((m.status & status) ? true : false);
}
inline void m_status_set(monster &m, int status)
{
  m.status |= status;
}
inline void m_status_reset(monster &m, int status)
{
  m.status &= ~(status);
}
inline bool m_immunityp(const monster &m, int status)
{
  return ((m.immunity & pow2(status)) ? true : false);
}

inline bool gamestatusp(int flag, const long &status)
{
  return ((status & flag) ? true : false);
}
inline void setgamestatus(int flag, long &status)
{
  status |= flag;
}
inline void resetgamestatus(int flag, long &status)
{
  status &= ~(flag);
}

inline bool optionp(int option, const player &p)
{
  return ((p.options & option) ? true : false);
}
inline void optionset(int option, player &p)
{
  p.options |= option;
}
inline void optionreset(int option, player &p)
{
  p.options &= ~(option);
}

#endif
