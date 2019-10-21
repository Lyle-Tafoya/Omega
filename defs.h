/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* This file is the header file for all omega modules */
/* defs.h */

/* omega will NOT function unless the implementor sets the appropriate
definitions in the following section. */

/*--------------------------USER DEFINITIONS--------------------------*/

/* Implementor should uncomment the following if his system uses
string.h instead of strings.h (try man string) */

#define STRING

/* Implementor should uncomment the following if random and srandom 
   are not available  (try man random) */

/* #define NORANDOM */

/* Implementor should uncomment the following if omega appears to
redraw the screen excessively. */

#define EXCESSIVE_REDRAW

/* The following definition is recommended. Remove it only if you have
huge amounts of disk space and are annoyed at waiting a few more seconds
on save and restore. */

#define COMPRESS_SAVE_FILES

/* If your system has gzip, I recommend using it instead of compress */
/* (try just typing 'gzip' at the shell prompt) */

#define USE_GZIP

/* If your system doesn't have the usleep call, uncomment this line */
/* (try man usleep) */

/* #define NO_USLEEP */

/* OMEGALIB is where all the data files reside. 
   Note the final / is necessary.
   msdos note: \ is the C string escape character, so you need \\ in the
   path given in OMEGALIB
   This might usually be "/usr/games/lib/omegalib/", for unix,
   or something like "c:\\games\\omega\\omegalib\\" for msdos */

#define OMEGALIB "./lib/"

/* Comment the following line out if you want users to be able to override */
/* the OMEGALIB define, above, by setting the environment variable OMEGALIB */
/* (I recommend leaving this line uncommented, unless you're compiling */
/* for someone else and don't know where they'll be putting the omegalib */
/* directory, as is the case with compiling executables for home computers. */
/* It would be downright insecure to comment this line out in a multi-user */
/* environment, especially if you're going to run omega setuid.) */

/*#define FIXED_OMEGALIB*/

/* set WIZARD to maintainers's username */

#define WIZARD "wtanksle"

/* If CATCH_SIGNALS is set to 1, will not dump core, nicer for players. */
/* dbx still intercepts the signals first, so it's ok for debugging */

#define CATCH_SIGNALS 1


/*---------------------------SYSTEM DEFINITIONS---------------------------*/

/* Don't change anything from here on (unless you know what you're doing) */
#define VERSION 90
#define VERSIONSTRING "omega version 0.90"

#ifndef AMIGA
#ifndef MSDOS
#ifndef BSD
#ifndef SYSV
    ERROR! - One of these should be set - edit the makefile appropriately
#endif
#endif
#endif
#endif

#if defined(MSDOS_SUPPORTED_ANTIQUE)
#define SAVE_LEVELS
#endif

#ifdef COMPRESS_SAVE_FILES
# ifdef USE_GZIP
# define COMPRESSOR "gzip"
# define UNCOMPRESSOR "gunzip"
# define COMPRESS_EXT "gz"
# define EXT_LENGTH 2
# else
# define COMPRESSOR "compress"
# define UNCOMPRESSOR "uncompress"
# define COMPRESS_EXT "Z"
# define EXT_LENGTH 1
# endif
#endif

#define VACANT 0
#define ABORT -1
#define CASHVALUE -2

/* moderately arbitrary but probably cannot be easily changed */
#define MAXWIDTH 64
#define MAXLENGTH 64
#define SMALLSCREENLENGTH 16

/* number of slots in inventory. Cannot be changed without work */
#define MAXITEMS 16

/* number of slots in pack. Should be <= 26. */
#define MAXPACK 26

/* number of items in pawn shop. Should be <= 26 */
#define PAWNITEMS 20 /* DG -- the more the merrier.  WDT -- I agree. */

/* number of lines back strings are recalled */
#define STRING_BUFFER_SIZE 25

/* number of rerolls allowed +1 */  /* added by dagibbs (DG) */
#define REROLLS 31

/* Verbosity levels */
#define TERSE 0
#define MEDIUM 1
#define VERBOSE 2 

/* Arbitrary. Max of the levels in the dungeons */
#define MAXLEVELS 21

/* levels in each dungeon */
#define ASTRALLEVELS 5
#define SEWERLEVELS 18
#define CASTLELEVELS 16
#define CAVELEVELS 10
#define VOLCANOLEVELS 20

/* Overall Game Progress Vector Bits */
/* Long had BETTER have at least 32 bits.... */
#define SPOKE_TO_DRUID		0x1
#define COMPLETED_CAVES		0x2
#define COMPLETED_SEWERS	0x4
#define COMPLETED_CASTLE	0x8
#define COMPLETED_ASTRAL	0x10
#define COMPLETED_VOLCANO	0x20
#define KILLED_DRAGONLORD	0x40
#define KILLED_EATER		0x80
#define KILLED_LAWBRINGER	0x100
#define COMPLETED_CHALLENGE	0x200
#define SOLD_CONDO		0x400
#define FAST_MOVE		0x800
#define SKIP_PLAYER		0x1000
#define SKIP_MONSTERS		0x2000
#define MOUNTED			0x4000
#define SUPPRESS_PRINTING	0x8000
#define LOST			0x10000
#define ARENA_MODE		0x20000
#define CHEATED			0x40000
#define BANK_BROKEN		0x80000
#define CLUB_MEMBER		0x100000
#define PREPARED_VOID		0x200000
#define DESTROYED_ORDER		0x400000
#define GAVE_STARGEM		0x800000
#define ATTACKED_ORACLE		0x1000000
#define UNDEAD_GUARDS		0x2000000

/* non-existant environments for the random number seeding routine */
 /* added 12/30/98 (DG) */
#define E_RESTORE -2
#define E_RANDOM -1
/* general environment types */
#define E_NEVER_NEVER_LAND 0
#define E_COUNTRYSIDE 1
#define E_CITY 2
#define E_VILLAGE 3
#define E_TACTICAL_MAP 4
#define E_SEWERS 5
#define E_CASTLE 6
#define E_CAVES 7
#define E_VOLCANO 8
#define E_ASTRAL 9
#define E_ARENA 10
#define E_HOVEL 11
#define E_MANSION 12
#define E_HOUSE 13
#define E_DLAIR 14
#define E_ABYSS 15
#define E_STARPEAK 16
#define E_MAGIC_ISLE 17
#define E_TEMPLE 18
#define E_CIRCLE 19
#define E_COURT 20
#define E_MAX E_COURT

/* player game status */
#define DEAD 1
#define QUIT 2
#define WIN 3
#define BIGWIN 4

/* kind of arbitrary */
#define MAXROOMS 48
#define MAXCONNECTIONS 4 
#define STRING_LEN 100

/* some random characters */
#define ESCAPE 27
#define RETURN '\n'
#define LINEFEED '\r'
#define BACKSPACE '\b'
#define DELETE 127


/* tac mode action definitions */
/* have to remember to find where these are used, mostly unused, now! */
#define DISENGAGE 10
#define BLOCK 20
#define CUT 30
#define THRUST 40
#define MAGIC 50
#define LUNGE 60
#define RIPOSTE 70
#define WIELD 80
#define PICK_UP 90

/* as in attack low, block high, etc. */
/* These values may be added to the ones above to get things like
   block high, cut low, etc. CLEVER is only used by some monsters
   to cheat with.... */
#define LOW 1
#define CENTER 2
#define HIGH 3
#define CLEVER 4


/* weapon types */
#define CUTTING 1
#define THRUSTING 2
#define STRIKING 3
#define MISSILE 4

/* random aux constants */
/* aux field for private residences in city */
#define BURGLED 2
#define LOCKED 3
#define UNLOCKED 0

/* cannot use M command on site with this aux value */
#define NOCITYMOVE 666

/* bow and crossbow object aux fields */
#define LOADED 1
#define UNLOADED 0

/* alignment used randomly throughout*/
#define LAWFUL 1
#define CHAOTIC 2
#define NEUTRAL 3

/* spells */
#define NUMSPELLS 41

#define S_MON_DET 0
#define S_OBJ_DET 1
#define S_MISSILE 2
#define S_FIREBOLT 3
#define S_TELEPORT 4
#define S_LBALL 5
#define S_SLEEP 6
#define S_DISRUPT 7
#define S_DISINTEGRATE 8
#define S_POLYMORPH 9
#define S_HEAL 10
#define S_DISPEL 11
#define S_IDENTIFY 12
#define S_BREATHE 13
#define S_INVISIBLE 14
#define S_WARP 15
#define S_ENCHANT 16
#define S_BLESS 17
#define S_RESTORE 18
#define S_CURE 19
#define S_TRUESIGHT 20
#define S_HELLFIRE 21
#define S_KNOWLEDGE 22
#define S_HERO 23
#define S_RETURN 24
#define S_DESECRATE 25
#define S_HASTE 26
#define S_SUMMON 27
#define S_SANCTUARY 28
#define S_ACCURACY 29
#define S_RITUAL 30
#define S_FEAR 31
#define S_APPORT 32
#define S_SHADOWFORM 33
#define S_ALERT 34
#define S_REGENERATE 35
#define S_SANCTIFY 36
#define S_CLAIRVOYANCE 37
#define S_DRAIN 38
#define S_LEVITATE 39
#define S_WISH 40

/* ranks in guilds, etc */
#define NUMRANKS 9

#define LEGION 0
#define ARENA 1
#define COLLEGE 2
#define THIEVES 3
#define ORDER 4
#define CIRCLE 5
#define NOBILITY 6
#define PRIESTHOOD 7
#define ADEPT 8

#define LEGIONAIRE 1
#define CENTURION 2
#define FORCE_LEADER 3
#define COLONEL 4
#define COMMANDANT 5

#define TRAINEE 1
#define BESTIARIUS 2
#define RETIARIUS 3
#define GLADIATOR 4
#define CHAMPION 5

#define NOVICE 1
#define STUDENT 2
#define PRECEPTOR 3
#define MAGE 4
#define ARCHMAGE 5

#define TMEMBER 1
#define ATHIEF 2
#define THIEF 3
#define TMASTER 4
#define SHADOWLORD 5

#define GALLANT 1
#define GUARDIAN 2
#define CHEVALIER 3
#define PALADIN 4
#define JUSTICIAR 5

#define INITIATE 1
#define ENCHANTER 2
#define SORCEROR 3
#define HIGHSORCEROR 4
#define PRIME 5

#define COMMONER 1
#define ESQUIRE 2
#define KNIGHT 3
#define LORD 4
#define DUKE 5

#define LAY 1
#define ACOLYTE 2
#define PRIEST 3
#define SPRIEST 4
#define HIGHPRIEST 5

/* different priesthoods */
#define ODIN 1
#define SET 2
#define ATHENA 3
#define HECATE 4
#define DRUID 5
#define DESTINY 6



/* MONSTER STATUS/ABILITY BITS */
#define AWAKE 1
#define MOBILE 2
#define HOSTILE 4
#define WANDERING 16
#define HUNGRY 32
#define GREEDY 64
#define NEEDY  128
#define ONLYSWIM 256
#define FLYING 512
#define INTANGIBLE 1024
#define M_INVISIBLE 2048
#define SWIMMING 4096
#define POISONOUS 8192
#define EDIBLE 16384


/* PLAYER STATUS INDICES */
#define NUMSTATI 25

#define ACCURACY 0
#define BLINDED 1
#define SLOWED 2
#define DISPLACED 3
#define SLEPT 4
#define DISEASED 5
#define POISONED 6
#define HASTED 7
#define BREATHING 8
#define INVISIBLE 9
#define REGENERATING 10
#define VULNERABLE 11
#define BERSERK 12
#define IMMOBILE 13
#define ALERT 14
#define AFRAID 15
#define HERO 16
#define LEVITATING 17
#define ACCURATE 18
#define TRUESIGHT 19
#define SHADOWFORM 20
#define ILLUMINATION 21
#define DEFLECTION 22
#define PROTECTION 23
/* PROTECTION is deviant -- indicates protective value, not duration */
#define RETURNING 24
/* RETURNING is somewhat deviant--how many turns 'til RETURN spell goes off */

/* player immunity indices */
/* also monster immunity bits (2^n) */
/* also damage types */
#define NUMIMMUNITIES 14

#define UNSTOPPABLE 0 
#define NORMAL_DAMAGE 1
#define FLAME 2
#define COLD 3
#define ELECTRICITY 4
#define POISON 5
#define ACID 6
#define FEAR 7
#define SLEEP 8
#define NEGENERGY 9
#define OTHER_MAGIC 10
#define THEFT 11
#define GAZE 12
#define INFECTION 13
#define EVERYTHING -1


/* location lstatus bits */
#define SEEN 1
#define LIT 2
#define SECRET 4
#define STOPS 8
#define CHANGED 16


/* room string id */
/* for use in roomname() */
#define NUMROOMNAMES 30

/* normal room name indices start after the RS_ constants */
#define ROOMBASE 39
#define RS_COURT 38
#define RS_ZORCH 37
#define RS_CIRCLE 36
#define RS_MAGIC_ISLE 35
#define RS_STARPEAK 34
#define RS_VOLCANO 33
#define RS_HIGHASTRAL 32
#define RS_FIREPLANE 31
#define RS_AIRPLANE 30
#define RS_WATERPLANE 29
#define RS_EARTHPLANE 28
#define RS_LOWERASTRAL 27
#define RS_CLOSET 26
#define RS_SECRETPASSAGE 25
#define RS_DININGROOM 24
#define RS_BATHROOM 23
#define RS_BEDROOM 22
#define RS_KITCHEN 21
#define RS_DROWNED_SEWER 20
#define RS_DRAINED_SEWER 19
#define RS_SEWER_DUCT 18
#define RS_ARENA 17
#define RS_COUNTRYSIDE 16
#define RS_DRUID 15
#define RS_HECATE 14
#define RS_ATHENA 13
#define RS_SET 12
#define RS_ODIN 11
#define RS_DESTINY 10
#define RS_ADEPT 9
#define RS_WYRM 8
#define RS_OCEAN 7 
#define RS_PONDS 6
#define RS_DRAGONLORD 5
#define RS_GOBLINKING 4
#define RS_CAVERN 3
#define RS_CORRIDOR 2
#define RS_WALLSPACE 1

#if defined(MSDOS_SUPPORTED_ANTIQUE) || defined(AMIGA)
#define CLR(fg)		COL_##fg
#define CLRS(fg,bg)	COL_##fg|COL_BG_##bg
#endif

#ifdef MSDOS_SUPPORTED_ANTIQUE

#define COL_BLACK 0x0000
#define COL_BLUE 0x0100
#define COL_GREEN 0x0200
#define COL_CYAN 0x0300
#define COL_RED 0x0400
#define COL_PURPLE 0x0500
#define COL_BROWN 0x0600
#define COL_WHITE 0x0700
#define COL_GREY 0x0800
#define COL_LIGHT_BLUE 0x0900
#define COL_LIGHT_GREEN 0x0a00
#define COL_LIGHT_CYAN 0x0b00
#define COL_LIGHT_RED 0x0c00
#define COL_LIGHT_PURPLE 0x0d00
#define COL_YELLOW 0x0e00
#define COL_BRIGHT_WHITE 0x0f00
#define COL_BG_BLACK 0x0000
#define COL_BG_BLUE 0x1000
#define COL_BG_GREEN 0x2000
#define COL_BG_CYAN 0x3000
#define COL_BG_RED 0x4000
#define COL_BG_PURPLE 0x5000
#define COL_BG_BROWN 0x6000
#define COL_BG_WHITE 0x7000
#define COL_FG_BLINK 0x8000

#else
#ifdef AMIGA

#include <curses210.h>

/* unfortunately, this curses package only implements 8 colours... */

#define COL_WHITE 0x0100
#define COL_BLACK COL_WHITE
	    /* this assumes that all things with black fg have white bg */
#define COL_BROWN 0x0200
#define COL_YELLOW 0x0300
#define COL_GREY 0x0400
#define COL_GREEN 0x0500
#define COL_BLUE 0x0600
#define COL_RED 0x0700
#define COL_CYAN 0x0500		/* = green */
#define COL_PURPLE 0x0700	/* = red */
#define COL_LIGHT_BLUE 0x0600	/* = blue */
#define COL_LIGHT_GREEN 0x0500	/* = green */
#define COL_LIGHT_CYAN 0x0500	/* = green */
#define COL_LIGHT_RED 0x0700	/* = red */
#define COL_LIGHT_PURPLE 0x0100	/* = white */
#define COL_BRIGHT_WHITE 0x0100	/* = white */
#define COL_BG_BLACK 0x0000
#define COL_BG_WHITE (A_REVERSE<<8)
#define COL_BG_GREEN (A_REVERSE<<8)
#define COL_BG_CYAN (A_REVERSE<<8)
#define COL_BG_RED (A_REVERSE<<8)
#define COL_BG_PURPLE (A_REVERSE<<8)
#define COL_BG_BROWN (A_REVERSE<<8)
#define COL_BG_BLUE (A_REVERSE<<8)
#define COL_FG_BLINK 0x0000	/* not implemented :( */
/* WDT: thank goodness for that lack of implementation. */

#else
#include <curses.h>

#define COL_FG_BLINK A_BLINK

#ifdef COLOR_PAIR

# ifdef OMEGA_CLRGEN
#  define CLR(fg)	OMEGA_CLRGEN1 fg
#  define CLRS(fg, bg)	OMEGA_CLRGEN2 fg bg
# else
#  include "clrgen.h"
#  define CLR(fg)	CLR_##fg##_BLACK
#  define CLRS(fg, bg)	CLR_##fg##_##bg
# endif

#else

#define CLR(fg)		0
#define CLRS(fg,bg)	0

#endif
#endif
#endif

/* objects, locations, and terrain; characters to draw */
#define NULL_ITEM '\0'
#define SPACE (' ' | CLR(WHITE))
#define WALL ('#' | CLR(GREY))
#define PORTCULLIS ('7' | CLR(WHITE))
#define OPEN_DOOR ('|' | CLR(BROWN))
#define CLOSED_DOOR ('-' | CLR(BROWN))
#define WHIRLWIND ('6' | CLR(LIGHT_BLUE))
#define ABYSS ('0' | CLRS(BLACK,BROWN))
#define VOID_CHAR (' ' | CLR(WHITE))
#define LAVA ('`' | CLR(RED))
#define HEDGE ('\"' | CLR(GREEN))
#define WATER ('~' | CLR(BLUE))
#define FIRE (';' | CLR(LIGHT_RED))
#define TRAP ('^' | CLR(RED))
#define LIFT ('_' | CLR(BRIGHT_WHITE))
#define STAIRS_UP ('<' | CLR(WHITE))
#define STAIRS_DOWN ('>' | CLR(WHITE))
#define FLOOR ('.' | CLR(BROWN))
#define PLAYER ('@' | CLR(WHITE))
#define CORPSE ('+' | CLR(RED))
#define STATUE ('1' | CLR(GREY))
#define RUBBLE ('4' | CLR(GREY))
#define ALTAR ('8' | CLR(LIGHT_BLUE))
#define CASH ('$' | CLR(YELLOW))	/* various kinds of money */
#define PILE ('*' | CLR(BRIGHT_WHITE))	/* several objects in one place */
#define FOOD ('%' | CLR(BROWN))
#define WEAPON (')' | CLR(GREY))
#define MISSILEWEAPON ('(' | CLR(BROWN))
#define SCROLL ('?' | CLR(YELLOW))
#define POTION ('!' | CLR(LIGHT_GREEN))
#define ARMOR (']' | CLR(GREY))
#define SHIELD ('[' | CLR(BROWN))
#define CLOAK ('}' | CLR(CYAN))
#define BOOTS ('{' | CLR(BROWN))
#define STICK ('/' | CLR(BROWN))

#define RING ('=' | CLR(YELLOW))
#define THING ('\\' | CLR(WHITE))
#define ARTIFACT ('&' | CLR(YELLOW))

/* TERRAIN TYPES */
#define PLAINS ('-' | CLR(LIGHT_GREEN))
#define TUNDRA ('_' | CLR(GREY))
#define ROAD ('.' | CLR(BROWN))
#define MOUNTAINS ('^' | CLR(GREY))
#define PASS ('v' | CLR(BROWN))
#define RIVER ('~' | CLR(BLUE))
#define CITY ('O' | CLR(WHITE))
#define VILLAGE ('o' | CLR(WHITE))
#define FOREST ('(' | CLR(LIGHT_GREEN))
#define JUNGLE (')' | CLR(GREEN))
#define SWAMP ('=' | CLR(GREEN))
#define VOLCANO ('!' | CLR(RED))
#define CASTLE ('%' | CLR(GREY))
#define TEMPLE ('X' | CLR(BROWN))
#define CAVES ('*' | CLRS(BLACK,BROWN))
#define DESERT ('\"' | CLR(YELLOW))
#define CHAOS_SEA ('+' | CLR(LIGHT_PURPLE))
#define STARPEAK ('|' | CLR(LIGHT_BLUE))
#define DRAGONLAIR ('$' | CLR(BROWN))
#define MAGIC_ISLE ('&' | CLR(PURPLE))

#define CHAIR ('5' | CLR(BROWN))
#define SAFE ('3' | CLR(GREY))
#define FURNITURE ('2' | CLR(BROWN))
#define BED ('9' | CLR(CYAN))

/* wow, all characters used! */


/* total number of player options */
#define NUMOPTIONS 11

/* number of options with TRUE/FALSE values */
#define NUMTFOPTIONS 9

/* The slot number of the two options not in Player.options */
#define VERBOSITY_LEVEL 10
#define SEARCH_DURATION 11

/* Player.options bits */
#define BELLICOSE 1 
#define JUMPMOVE 2
#define RUNSTOP 4
#define PICKUP 8
#define CONFIRM 16
#define TOPINV 32
#define PACKADD 64
#define COMPRESS_OPTION 128
#define SHOW_COLOUR 256

/* This has to be changed whenever an item is added */
#define NUMSCROLLS 24
#define NUMPOTIONS 18
#define NUMFOODS 16
#define NUMTHINGS 26
#define NUMWEAPONS 41
#define NUMARMOR 17
#define NUMSHIELDS 8
#define NUMCLOAKS 7
#define NUMBOOTS 7
#define NUMRINGS 10
#define NUMSTICKS 17
#define NUMARTIFACTS 24

/* running sum of itemtypes, for indexing into Objects array */
#define THINGID 0 
#define FOODID NUMTHINGS
#define SCROLLID (FOODID + NUMFOODS)
#define POTIONID (SCROLLID + NUMSCROLLS)
#define WEAPONID (POTIONID + NUMPOTIONS)
#define ARMORID (WEAPONID + NUMWEAPONS)
#define SHIELDID (ARMORID + NUMARMOR)
#define CLOAKID (SHIELDID + NUMSHIELDS)
#define BOOTID (CLOAKID + NUMCLOAKS)
#define RINGID (BOOTID + NUMBOOTS)
#define STICKID (RINGID + NUMRINGS)
#define ARTIFACTID (STICKID + NUMSTICKS)
#define CASHID (ARTIFACTID+NUMARTIFACTS)
/* Corpse's aux field is monster id */
#define CORPSEID (CASHID+1)

#define TOTALITEMS (CORPSEID+1) 

/* describing unique items and monsters */
#define COMMON 0
#define UNIQUE_UNMADE 1
#define UNIQUE_MADE 2
#define UNIQUE_TAKEN 3

/* general item function id's */
#define I_NO_OP 0
#define I_NOTHING 1

  /* note some of these functions are for other types of items too */

    /* scroll functions */
#define I_BLESS 101
#define I_ACQUIRE 102
#define I_ENCHANT 103
#define I_TELEPORT 104
#define I_WISH 105
#define I_CLAIRVOYANCE 106
#define I_DISPLACE 107
#define I_ID 108
#define I_FIREFLASH 109
#define I_SPELLS 110
#define I_JANE_T 111
#define I_ALERT 112
#define I_FLUX 113
#define I_CHARGE 114
#define I_WARP 115
#define I_KNOWLEDGE 116
#define I_LAW 117
#define I_HINT 118
#define I_HERO 119
#define I_TRUESIGHT 120
#define I_ILLUMINATE 121
#define I_DEFLECT 122

    /* potion functions */
#define I_HEAL 201
#define I_OBJDET 202
#define I_MONDET 203
#define I_SLEEP_SELF 204
#define I_RESTORE 205
#define I_NEUTRALIZE_POISON 206
#define I_SPEED 207
#define I_AZOTH 208
#define I_REGENERATE 210
#define I_INVISIBLE 211
#define I_BREATHING 212
#define I_FEAR_RESIST 213
#define I_AUGMENT 214
#define I_CHAOS 215
#define I_ACCURACY 216
#define I_LEVITATION 217
#define I_CURE 218

    /* stick functions */
#define I_FIREBOLT 301
#define I_LBOLT 302
#define I_MISSILE 303
#define I_SLEEP_OTHER 304
#define I_FIREBALL 305
#define I_LBALL 306
#define I_SUMMON 307
#define I_HIDE 308
#define I_DISRUPT 309
#define I_DISINTEGRATE 310
#define I_SNOWBALL 311
#define I_APPORT 312
#define I_DISPEL 313
#define I_POLYMORPH 314
#define I_FEAR 315

    /* food functions */
#define I_FOOD 401
#define I_LEMBAS 402
#define I_STIM 403
#define I_POW 404
#define I_IMMUNE 405
#define I_POISON_FOOD 406
#define I_CORPSE 407
#define I_PEPPER_FOOD 408
#define I_CANNIBAL 409
#define I_INEDIBLE 410

    /* boots functions */
#define I_PERM_SPEED 501
#define I_PERM_HERO 502
#define I_PERM_LEVITATE 503
#define I_PERM_AGILITY 504
#define I_BOOTS_JUMPING 505
#define I_BOOTS_7LEAGUE 506

    /* cloak functions */
#define I_PERM_DISPLACE 601
#define I_PERM_NEGIMMUNE 602
#define I_PERM_INVISIBLE 603
#define I_PERM_ACCURACY 604
#define I_PERM_PROTECTION 605
#define I_PERM_TRUESIGHT 606

    /* ring functions */
#define I_PERM_VISION 701
#define I_PERM_BURDEN 702
#define I_PERM_STRENGTH 703
#define I_PERM_GAZE_IMMUNE 704
#define I_PERM_FIRE_RESIST 705
#define I_PERM_POISON_RESIST 706
#define I_PERM_REGENERATE 707
#define I_PERM_KNOWLEDGE 708

    /* armor functions */
#define I_PERM_ENERGY_RESIST 801
#define I_PERM_BREATHING 802
#define I_PERM_FEAR_RESIST 803
#define I_NORMAL_ARMOR 804

/* artifact functions */
#define I_ORBFIRE 901
#define I_ORBWATER 902
#define I_ORBEARTH 903
#define I_ORBAIR 904
#define I_ORBMASTERY 905
#define I_ORBDEAD 906
#define I_CRYSTAL 907
#define I_ANTIOCH 908
#define I_KOLWYNIA 909
#define I_DEATH 910
#define I_ENCHANTMENT 911
#define I_HELM 912
#define I_LIFE 913
#define I_JUGGERNAUT 914
#define I_SYMBOL 915
#define I_STARGEM 916
#define I_SCEPTRE 917
#define I_PLANES 918

/* weapons functions */
#define I_NORMAL_WEAPON 1001
#define I_LIGHTSABRE 1002
#define I_DEMONBLADE 1003
#define I_MACE_DISRUPT 1004
#define I_TANGLE 1005
#define I_ARROW 1006
#define I_BOLT 1007
#define I_VORPAL 1008
#define I_DESECRATE 1009
#define I_FIRESTAR 1010
#define I_DEFEND 1011
#define I_VICTRIX 1012
#define I_EMPIRE 1013
#define I_SCYTHE 1014
#define I_ACIDWHIP 1015

/* thing functions */
#define I_PICK 1101
#define I_KEY 1102
#define I_SHOVEL 1103 /* unused */
#define I_EXCAVATOR 1104 /* unused */
#define I_PERM_ILLUMINATE 1105
#define I_TRAP 1106
#define I_RAISE_PORTCULLIS 1107

/* shield functions */
#define I_PERM_DEFLECT 1201
#define I_NORMAL_SHIELD 1202

/* monster function ids */
/* Its conceivable for a function of one type to be called when another
would usually occur. A monster's special function may be an extra move,
for example. */

#define M_NO_OP -1

/* talk functions */
#define M_TALK_STUPID 101
#define M_TALK_SILENT 102
#define M_TALK_HUNGRY 103
#define M_TALK_GREEDY 104
#define M_TALK_TITTER 105
#define M_TALK_MAN 106
#define M_TALK_ROBOT 107
#define M_TALK_EVIL 108
#define M_TALK_BURBLE 109
#define M_TALK_SLITHY 110
#define M_TALK_MIMSY 111
#define M_TALK_SEDUCTOR 112
#define M_TALK_MP 113
#define M_TALK_IM 114
#define M_TALK_GUARD 115
#define M_TALK_GHOST 116
#define M_TALK_HINT 117
#define M_TALK_BEG 118
#define M_TALK_EF 119
#define M_TALK_GF 120
#define M_TALK_MORGON 121
#define M_TALK_LB 122
#define M_TALK_DEMONLOVER 123
#define M_TALK_ASSASSIN 124
#define M_TALK_NINJA 125
#define M_TALK_THIEF 126
#define M_TALK_MERCHANT 127
#define M_TALK_HORSE 128
#define M_TALK_PARROT 129
#define M_TALK_ANIMAL 130
#define M_TALK_HYENA 131
#define M_TALK_SERVANT 132
#define M_TALK_SCREAM 133
#define M_TALK_DRUID 134
#define M_TALK_ARCHMAGE 135
#define M_TALK_PRIME 136

/* ability functions */
#define M_SP_SURPRISE 201
#define M_SP_MP 202
#define M_SP_THIEF 203
#define M_SP_AGGRAVATE 204
#define M_SP_POISON_CLOUD 205
#define M_SP_HUGE 206
#define M_SP_SUMMON 207
#define M_SP_ILLUSION 208
#define M_SP_FLUTTER 209
#define M_SP_ESCAPE 210
#define M_SP_SPELL 211
#define M_SP_EXPLODE 212
#define M_SP_DEMON 213
#define M_SP_ACID_CLOUD 214
#define M_SP_WHIRL 215
#define M_SP_GHOST 216
#define M_SP_WHISTLEBLOWER 217
#define M_SP_EATER 218
#define M_SP_LAWBRINGER 219
#define M_SP_DRAGONLORD 220
#define M_SP_DE 221
#define M_SP_DEMONLOVER 222
#define M_SP_SEDUCTOR 223
#define M_SP_MASTER 224
#define M_SP_WYRM 225
#define M_SP_BLACKOUT 226
#define M_SP_BOG 227
#define M_SP_MERCHANT 228
#define M_SP_WERE 229
#define M_SP_SERVANT 231
#define M_SP_AV 232
#define M_SP_LW 233
#define M_SP_SWARM 234
#define M_SP_ANGEL 235
#define M_SP_MB 236
#define M_SP_MIRROR 237
#define M_SP_RAISE 238
#define M_SP_DEATH 239
#define M_SP_COURT 240
#define M_SP_LAIR 241
#define M_SP_PRIME 242

/* rangestrike functions */
#define M_STRIKE_MISSILE 301
#define M_STRIKE_FBOLT 302
#define M_STRIKE_LBALL 303
#define M_STRIKE_FBALL 304
#define M_STRIKE_BLIND 305
#define M_STRIKE_SNOWBALL 306
#define M_STRIKE_MASTER 307
#define M_STRIKE_SONIC 308

/* combat functions */
#define M_MELEE_NORMAL 401
#define M_MELEE_FIRE 402
#define M_MELEE_DRAGON 403
#define M_MELEE_MP 404
#define M_MELEE_ELEC 405
#define M_MELEE_POISON 406
#define M_MELEE_NG 407
#define M_MELEE_SUCCUBUS 408
#define M_MELEE_SPIRIT 409
#define M_MELEE_DISEASE 410
#define M_MELEE_SLEEP 411
#define M_MELEE_COLD 412
#define M_MELEE_MASTER 413
#define M_MELEE_GRAPPLE 414
#define M_MELEE_DEATH 415

/* movement functions */
#define M_MOVE_NORMAL 501
#define M_MOVE_FLUTTER 502
#define M_MOVE_TELEPORT 503
#define M_MOVE_FOLLOW 504
#define M_MOVE_RANDOM 505
#define M_MOVE_SMART 506
#define M_MOVE_SPIRIT 507
#define M_MOVE_SCAREDY 508
#define M_MOVE_CONFUSED 509
#define M_MOVE_ANIMAL 510
#define M_MOVE_LEASH 230

/* MLx -> index to Monsters starting for level x */
/* MLx -> number of monsters of level x or less */
/* NML_x -> number of monsters of level x */
/* NML-X must be changed whenever a monster is added */
/* This whole thing MUST be repaired.  Monster levels must
 * be represented elsewhere. */
#define ML0 0
#define NML_0 9
#define ML1 (ML0 + NML_0) /* 9 */
#define NML_1 22
#define ML2 (ML1 + NML_1) /* 31 */
#define NML_2 14
#define ML3 (ML2 + NML_2) /* 45 */
#define NML_3 15
#define ML4 (ML3 + NML_3) /* 60 */
#define NML_4 18
#define ML5 (ML4 + NML_4) /* 78 */
#define NML_5 14
#define ML6 (ML5 + NML_5) /* 92 */
#define NML_6 13
#define ML7 (ML6 + NML_6) /* 105 */
#define NML_7 15
#define ML8 (ML7 + NML_7) /* 120 */
#define NML_8 12
#define ML9 (ML8 + NML_8) /* 132 */
#define NML_9 8
#define ML10 (ML9 + NML_9) /* 140 */
#define NML_10 10

#define NUMMONSTERS (ML10 + NML_10)  /* 150 */

/* Some monster ID's : (Those that are explicitly named in code) */
/* Actually, there are still many magic constants floating around. */
/* Eventually I'll get around to making each monster's id a constant.... */
/* done, thanks to David Given. */
#define RANDOM -1
#define HORNET (ML0+0)
#define MEND_PRIEST (ML0+1)
#define ITIN_MERCH (ML0+2)
#define GUARD (ML0+3)
#define NPC (ML0+4)
#define SHEEP (ML0+5)
#define MERCHANT (ML0+6)
#define ZERO_NPC (ML0+7)
#define HISCORE_NPC (ML0+8)
#define GRUNT (ML1+0)
#define TSETSE (ML1+1)
#define FNORD (ML1+2)
#define SEWER_RAT (ML1+3)
#define AGGRAVATOR (ML1+4)
#define BLIPPER (ML1+5)
#define GOBLIN (ML1+6)
#define PHANTASTICON (ML1+7)
#define ROBOT (ML1+8)
#define GEEK (ML1+9)
#define BOROGROVE (ML1+10)
#define QUAIL (ML1+11)
#define BADGER (ML1+12)
#define HAWK (ML1+13)
#define DEER (ML1+14)
#define CAMEL (ML1+15)
#define ANTEATER (ML1+16)
#define BUNNY (ML1+17)
#define TROUT (ML1+18)
#define BASS (ML1+19)
#define PARROT (ML1+20)
#define HYENA (ML1+21)
#define APPR_NINJA (ML2+0)
#define NIGHT_GAUNT (ML2+1)
#define SNEAK_THIEF (ML2+2)
#define EYE (ML2+3)
#define TOVE (ML2+4)
#define NASTY (ML2+5)
#define GHOST (ML2+6)
#define ENCHANTOR (ML2+7)  /* use 'OR' to avoid conflict with circle rank */
#define MURK (ML2+8)
#define GOBLIN_CHIEF (ML2+9)
#define WOLF (ML2+10)
#define ANT (ML2+11)
#define ELEPHANT (ML2+12)
#define HORSE (ML2+13)
#define SALAMANDER (ML3+0)
#define CATOBLEPAS (ML3+1)
#define L_FDEMON (ML3+2)
#define ACID_CLOUD (ML3+3)
#define PHANTOM (ML3+4)
#define GOBLIN_KING (ML3+5)
#define PTERODACTYL (ML3+6)
#define GOBLIN_SHAMAN (ML3+7)
#define LION (ML3+8)
#define BRIGAND (ML3+9)
#define BEAR (ML3+10)
#define MAMBA (ML3+11)
#define MANOWAR (ML3+12)
#define WEREHUMAN (ML3+13)
#define THOUGHTFORM (ML3+14)
#define MANTICORE (ML4+0)
#define TASMANIAN (ML4+1)
#define AUTO_MINOR (ML4+2)
#define DENEBIAN (ML4+3)
#define JUBJUB (ML4+4)
#define HAUNT (ML4+5)
#define INCUBUS (ML4+6)
#define SATYR (ML4+7)
#define CROC (ML4+8)
#define TORPOR (ML4+9)
#define DOBERMAN (ML4+10)
#define FUZZY (ML4+11)
#define SERV_LAW (ML4+12)
#define SERV_CHAOS (ML4+13)
#define SWARM (ML4+14)
#define BAN_SIDHE (ML4+15)
#define GRUE (ML4+16)
#define GENIN (ML4+17)
#define DRAGONETTE (ML5+0)
#define TESLA (ML5+1)
#define WYVERN (ML5+2)
#define CATEAGLE (ML5+3)
#define FROST_DEMON (ML5+4)
#define SPECTRE (ML5+5)
#define NECROMANCER (ML5+6)
#define SHADOW (ML5+7)
#define BOGTHING (ML5+8)
#define ASTRAL_VAMP (ML5+9)
#define LAVA_WORM (ML5+10)
#define MANABURST (ML5+11)
#define OUTER_DEMON (ML5+12)
#define MIRRORSHADE (ML5+13)
#define FIRE_ELEM (ML6+0)
#define AIR_ELEM (ML6+1)
#define WATER_ELEM (ML6+2)
#define EARTH_ELEM (ML6+3)
#define BANDERSNATCH (ML6+4)
#define LICHE (ML6+5)
#define TRITON (ML6+6)
#define MAST_THIEF (ML6+7)
#define TRICER (ML6+8)
#define RAKSHASA (ML6+9)
#define DEMON_SERP (ML6+10)
#define ANGEL (ML6+11)
#define CHUNIN (ML6+12)
#define BEHEMOTH (ML7+0)
#define NAZGUL (ML7+1)
#define UNICORN (ML7+2)
#define ROUS (ML7+3)
#define ILL_FIEND (ML7+4)
#define GREAT_WYRM (ML7+5)
#define FLAME_DEV (ML7+6)
#define LURKER (ML7+7)
#define SANDMAN (ML7+8)
#define MIRRORMAST (ML7+9)
#define ELDER_GRUE (ML7+10)
#define LOATHLY (ML7+11)
#define ZOMBIE (ML7+12)
#define RICOCHET (ML7+13)
#define INNER_DEMON (ML7+14)
#define GOOD_FAIRY (ML8+0)
#define BAD_FAIRY (ML8+1)
#define AUTO_MAJOR (ML8+2)
#define DRAGON (ML8+3)
#define JABBERWOCK (ML8+4)
#define FDEMON_L (ML8+5)
#define TIGERSHARK (ML8+6)
#define JONIN (ML8+7)
#define SHADOW_SLAY (ML8+8)
#define MIL_PRIEST (ML8+9)
#define COMA (ML8+10)
#define HIGH_ANGEL (ML8+11)
#define JOTUN (ML9+0)
#define INVIS_SLAY (ML9+1)
#define KING_WYV (ML9+2)
#define DEATHSTAR (ML9+3)
#define THAUMATURGIST (ML9+4)
#define VAMP_LORD (ML9+5)
#define ARCHANGEL (ML9+6)
#define DEMON_PRINCE (ML9+7)
#define DEATH (ML10+0)
#define EATER (ML10+1)
#define LAWBRINGER (ML10+2)
#define DRAGON_LORD (ML10+3)
#define DEMON_EMP (ML10+4)
#define LORD_EARTH (ML10+5)
#define LORD_AIR (ML10+6)
#define LORD_WATER (ML10+7)
#define LORD_FIRE (ML10+8)
#define ELEM_MASTER (ML10+9)

/* location functions */
#define L_NO_OP 0

/* random sites */
#define L_LIFT 1
#define L_BALANCESTONE 2
#define L_FIRE 3
#define L_WHIRLWIND 4
#define L_VOIDSTONE 5
#define L_WARNING 6
#define L_ARENA_EXIT 7
#define L_HOUSE_EXIT 8
#define L_SAFE 9

/* city level shop and guild functions */
/* following are those in CitySiteList */
#define NUMCITYSITES 27
#define CITYSITEBASE 10
#define L_CHARITY 10
#define L_ARMORER 11
#define L_CLUB 12
#define L_GYM 13
#define L_THIEVES_GUILD 14
#define L_COLLEGE 15
#define L_HEALER 16
#define L_CASINO 17
#define L_TAVERN 18
#define L_MERC_GUILD 19
#define L_ALCHEMIST 20
#define L_SORCERORS 21
#define L_CASTLE 22
#define L_ARENA 23
#define L_DPW 24
#define L_LIBRARY 25
#define L_PAWN_SHOP 26
#define L_BANK 27
#define L_CONDO 28
#define L_ORACLE 29
#define L_ORDER 30
#define L_DINER 31
#define L_COMMANDANT 32
#define L_CRAP 33
#define L_TEMPLE 34
#define L_COUNTRYSIDE 35
#define L_BROTHEL 36
/* end of city sites */

/* random sites */
#define L_JAIL 37
#define L_TEMPLE_WARNING 38
#define L_LAWSTONE 39
#define L_CHAOSTONE 40

/* final abyss sites ignore levitation*/
#define L_EARTH_STATION 41
#define L_FIRE_STATION 42
#define L_WATER_STATION 43
#define L_AIR_STATION 44
#define L_VOID_STATION 45
#define L_VOID 46
#define L_VOICE1 47
#define L_VOICE2 48
#define L_VOICE3 49

#define L_SACRIFICESTONE 50

/* circle hq sites */
#define L_TOME1 51
#define L_TOME2 52
#define L_ENTER_CIRCLE 53
#define L_CIRCLE_LIBRARY 54

/* other site functions */
#define L_DRUID 55
#define L_ALTAR 56

#define L_GARDEN 57
#define L_ADEPT 58
#define L_SEWER 59

#define L_OMEGA 60
#define L_CARTOGRAPHER 61
#define L_STABLES 62
#define L_COMMONS 63
#define L_GRANARY 64
#define L_MAZE 65
#define L_HOVEL 66
#define L_HOUSE 67
#define L_MANSION 68
#define L_OCCUPIED_HOUSE 69
#define L_TACTICAL_EXIT 70
#define L_VAULT 71
#define L_CEMETARY 72
#define L_THRONE 73
#define L_ESCALATOR 74
#define L_ENTER_COURT 75

#define L_TRIFID 76
#define L_FINAL_ABYSS 77
#define L_RAISE_PORTCULLIS 78

#define L_MINDSTONE 79

/* above LEVITATION_AVOIDANCE, no effect if player is levitating */
#define LEVITATION_AVOIDANCE 80

/* random sites */
#define L_CHAOS 81
#define L_WATER 82
#define L_LAVA 83
#define L_MAGIC_POOL 84
#define L_PORTCULLIS_TRAP 85
#define L_DROP_EVERY_PORTCULLIS 87
#define L_PORTCULLIS 88


/* traps */
#define NUMTRAPS 13
#define TRAP_BASE 89

/* traps */
#define L_TRAP_DART 89
#define L_TRAP_PIT 90
#define L_TRAP_DOOR 91
#define L_TRAP_SNARE 92
#define L_TRAP_BLADE 93
#define L_TRAP_FIRE 94
#define L_TRAP_TELEPORT 95
#define L_TRAP_DISINTEGRATE 96
#define L_TRAP_SLEEP_GAS 97
#define L_TRAP_ACID 98
#define L_TRAP_MANADRAIN 99
#define L_TRAP_ABYSS 100
#define L_TRAP_SIREN 101

/* more random sites */
#define L_STATUE_WAKE 102
#define L_STATUE_RANDOM 103

#define L_HEDGE 104
#define L_RUBBLE 105

#define L_ABYSS 106

/* player possession slots */
/* slot 0 should not be filled when out of inventory_control() */

#define O_UP_IN_AIR 0
#define O_READY_HAND 1
#define O_WEAPON_HAND 2
#define O_LEFT_SHOULDER 3
#define O_RIGHT_SHOULDER 4
#define O_BELT1 5
#define O_BELT2 6
#define O_BELT3 7
#define O_SHIELD 8 
#define O_ARMOR 9
#define O_BOOTS 10
#define O_CLOAK 11
#define O_RING1 12
#define O_RING2 13
#define O_RING3 14
#define O_RING4 15

/* typedefs needed by structs */

#if defined(MSDOS_SUPPORTED_ANTIQUE) || defined(AMIGA)
typedef short Symbol;
#else
typedef int Symbol;
#endif


/* structure definitions */

struct room {
  int lighted; 
  int left,right,top,bottom;
  int rsi; /* index into roomname switch */
};



struct spell {
  char known;
  char id;
  char powerdrain;
} ;


struct monster {
  struct objectlist *possessions;
  unsigned char attacked;
  int aux1,aux2,x,y,click;
  int id,hp,hit,ac,dmg,sense,wakeup,level,speed;
  unsigned char sleep,treasure;
  long xpv;
  int corpseweight,corpsevalue,transformid,startthing;
  unsigned char uniqueness;
  int talkf,movef,meleef,strikef,specialf;
  long status,immunity;
  Symbol monchar;
  char *monstring,*corpsestr,*meleestr;
};


struct monsterlist {
  struct monster *m;
  struct monsterlist *next;
};




struct player {
  int str,con,dex,agi,iq,pow,maxstr,maxcon,maxdex,maxagi,maxiq,maxpow;
  long xp;
  int level,hp,maxhp,hit,dmg,absorption,speed,click;
  int defense,food,alignment;
  long mana,maxmana;
  long cash;
  int patron,birthday;
  char preference;
  int sx,sy; /* sanctuary coordinates */
  int x,y; /* current player coordinates */
  int itemweight,maxweight;
  int immunity[NUMIMMUNITIES];
  int status[NUMSTATI];
  long options;
  int rank[NUMRANKS];
  long guildxp[NUMRANKS];
  char name[64];
  char meleestr[64];
  struct object *possessions[MAXITEMS];
  struct object *pack[MAXPACK];
  int packptr;
};

struct object {
  int id,weight,plus,charge,dmg,hit,aux,number,fragility;
  long basevalue;
  unsigned char known,used;
  int blessing;
  unsigned char type,uniqueness;
  int usef;
  unsigned char level;
  Symbol objchar;
  char *objstr,*truename,*cursestr;
};



struct objectlist {
  struct object *thing;
  struct objectlist *next;
};



/* terrain locations */
struct terrain {
  Symbol base_terrain_type;
  Symbol current_terrain_type;
  char aux;
  char status;
};

/* dungeon locations */
struct location {
  char p_locf; /* function executed when moved on */
  unsigned char lstatus; /* seen,stopsrun,lit,secret, */
  char roomnumber; /* so room can be named */
  Symbol locchar; /* terrain type */
  Symbol showchar; /*char instantaneously drawn for site */
  int aux; /* signifies various things */
  unsigned char buildaux; /* used in constructing level */
  struct objectlist *things; 
  struct monster *creature;
 };


struct level {
  char depth; /* which level is this */
  struct level *next; /* pointer to next level in dungeon */
#ifndef SAVE_LEVELS
  struct location site[MAXWIDTH][MAXLENGTH]; /* dungeon data */
#else
  /* Over 64K worth of data! */
  struct location *site[MAXWIDTH]; /* dungeon data */
#endif
  char generated; /* has the level been made (visited) yet? */
  char numrooms; /* number of rooms on level */
  char tunnelled; /* amount of tunnelling done on this level */
  struct monsterlist *mlist; /* List of monsters on level */
  int environment; /* where kind of level is this? */
  int last_visited; /* time player was last on this level */
};

 
/* random typedef's */

typedef struct monsterlist mltype;
typedef mltype *pml;

typedef struct monster montype;
typedef montype *pmt;

typedef struct location loctype;
typedef loctype *plc;

typedef struct level levtype;
typedef levtype *plv;

typedef struct object objtype;
typedef objtype *pob;

typedef struct objectlist oltype;
typedef oltype *pol;

/* random  function declarations from system libraries */

#include <stdlib.h>

/* The assert macro (for ANSI/ISO C).  Hopefully this will always work! */
#include <assert.h>

#ifdef MSDOS
#include <time.h>
#define getlogin() "pcuser"
#endif

#undef sign
#undef max
#undef min
#undef abs
/* These must be made to work for both longs and ints */
#define sign(n) (((n) < 0) ? -1 : (((n) > 0) ? 1 : 0))
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define abs(n) (((n) < 0) ? (-(n)) : (n))

#ifdef NORANDOM
#define RANDFUNCTION rand
#define SRANDFUNCTION srand
#endif

#ifndef NORANDOM
#define RANDFUNCTION random
#define SRANDFUNCTION srandom
#endif


/* WDT: This should be harmless under ANSI C, and will stop
 * some errors under bizarre platforms. */
#define pow2(n) (1L << (n))

/* these bit operations were functions, but are faster as macros... */

#define loc_statusp(x,y,stat) ((Level->site[x][y].lstatus&(stat))?1:0)
#define lset(x,y,stat) (Level->site[x][y].lstatus |= (stat))
#define lreset(x,y,stat) (Level->site[x][y].lstatus &= ~(stat))

#define c_statusp(x,y,stat) ((Country[x][y].status&(stat))?1:0)
#define c_set(x,y,stat) (Country[x][y].status |= (stat))
#define c_reset(x,y,stat) (Country[x][y].status &= ~(stat))

#define m_statusp(m,s) (((m)->status&(s))?1:0)
#define m_status_set(m,s) ((m)->status |= (s))
#define m_status_reset(m,s) ((m)->status &= ~(s))
#define m_immunityp(m,s) (((m)->immunity&pow2(s))?1:0)

#define gamestatusp(flag) ((GameStatus&(flag))?1:0)
#define setgamestatus(flag) (GameStatus |= (flag))
#define resetgamestatus(flag) (GameStatus &= ~(flag))

#define optionp(o) ((Player.options&(o))?1:0)
#define optionset(o) (Player.options |= (o))
#define optionreset(o) (Player.options &= ~(o))

/* systemV for some reason uses string.h instead of strings.h */
/* Also, random and srandom are unlikely to be found on system V... */

#ifdef STRING
#include <string.h>
#endif

#ifndef STRING
#include <strings.h>
#endif

#include <stdio.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifdef SAVE_LEVELS
plv msdos_changelevel();
#endif
