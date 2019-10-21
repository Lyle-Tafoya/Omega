/* omega copyright (c) 1987 by Laurence Raphael Brothers */
/* This file is the header file for all omega modules */
/* odefs.h */

/* Implementor should set the following five definitions: */

/* OMEGALIB is where all the data files reside. 
   note the final / is necessary 
   this might usually be "/usr/games/lib/omegalib/" */

#define OMEGALIB "/u1/brothers/omega/lib/"

/* set WIZARD to implementor's username */
#define WIZARD "brothers"

/* If set to 0, this will allow players to make copies of saved games */
/* If set to 1, only the implementor will be allowed to restore
   copied games */
/*The implementor can now enable all currently saved games by making
   omega.saves a null file */
/* If omega.saves is absent, all files will permanently be enabled for
   restore, as well */
#define DESTRUCTIVE_RESTORE 1

/* If CATCH_SIGNALS is set to 1, will not dump core, nicer for players. */
/* dbx still instercepts the signals first, so its ok for debugging */
#define CATCH_SIGNALS 1

/* Don't change anything from here on (unless you know what you're doing) */
#define VERSION 60
#define VERSIONSTRING "omega version 0.60 (alpha)"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define VACANT 0
#define NULL 0
#define ABORT -1
#define CASHVALUE -2

/* These next eight should never be changed -- some places have hardwired
constants based on these values. I'll fix this eventually when I make
omega compatible with windowing systems like NeWS and sunview */ 

#define WIDTH 64
#define LENGTH 64
#define TOPEDGE 0
#define EDGE1 16
#define EDGE2 32
#define EDGE3 48
#define BOTTOMEDGE 63
#define NUMLEVELS 40


/* player game status */
#define DEAD 1
#define QUIT 2
#define WIN 3
#define BIGWIN 4

/* somewhat arbitrary -- MAXITEMS is so a..z can index items */
#define MAXITEMS 26

/* kind of arbitrary */
#define MAXROOMS 48
#define MAXCONNECTIONS 4 

/* some random characters */
#define ESCAPE 27
#define RETURN 13
#define LINEFEED 10
#define BACKSPACE 8
#define DELETE 127


/* tac mode action definitions */
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
#define LOCKED 1
#define UNLOCKED 0

/* bow and crossbow object aux fields */
#define LOADED 1
#define UNLOADED 0

/* alignment used randomly throughout*/
#define LAWFUL 1
#define CHAOTIC 2
#define NEUTRAL 3

/* spells */
#define NUMSPELLS 40

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
#define S_INVULNERABLE 31
#define S_APPORT 32
#define S_SHADOWFORM 33
#define S_ALERT 34
#define S_REGENERATE 35
#define S_SANCTIFY 36
#define S_CLAIRVOYANCE 37
#define S_DRAIN 38
#define S_LEVITATE 39



/* ranks in guilds, etc */
#define NUMRANKS 8

#define LEGION 0
#define ARENA 1
#define COLLEGE 2
#define THIEVES 3
#define CIRCLE 4
#define NOBILITY 5
#define PRIESTHOOD 6
#define ADEPT 7

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
#define COWARDLY 256
#define FLYING 512
#define SWIMMING 1024
#define INTANGIBLE 2048
#define M_INVISIBLE 4096

/* Monster tactical melee tendencies */
#define MM_FORCED 1
#define MM_BERSERK 2
#define MM_TIMID 4
#define MM_POOR 8
#define MM_AVERAGE 16
#define MM_GOOD 32

/* PLAYER STATUS INDICES */
#define NUMSTATI 24

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
#define PROTECTION 22
/* PROTECTION is deviant -- indicates protective value, not duration */
#define RETURNING 23
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

/* room string id */
/* for use in roomname() */
#define NUMROOMNAMES 30

/* normal room name indices start after the RS_ constants */
#define ROOMBASE 16

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

/* objects and terrain; characters to draw */
#define SPACE ' '
#define WALL '#'
#define OPEN_DOOR '|'
#define CLOSED_DOOR '-'
#define ABYSS '0'
#define WHIRLWIND '6'
#define WHIRLWIND2 '9'
#define LAVA '`'
#define HEDGE '\"'
#define WATER '~' 
#define FIRE ';'
#define TRAP '^'
#define LIFT '_'
#define UP '<'
#define DOWN '>'
#define FLOOR '.'
#define PLAYER '@'
#define CORPSE '+'
#define STATUE '1'
#define ALTAR '8'
#define CASH '$'      /* various kinds of money */
#define PILE '*'      /* several objects in one place */
#define FOOD '%'
#define WEAPON ')'
#define MISSILEWEAPON '('
#define SCROLL '?'
#define POTION '!'
#define ARMOR ']'
#define SHIELD '['
#define CLOAK '}'
#define BOOTS '{'
#define STICK '/'
#define RING '='
#define THING '\\'
#define ARTIFACT '&'
#define PORTAL 'O'
/* unused : 2 3 4 5 7 */


/* player option bits */
#define BELLICOSE 1 
#define JUMPMOVE 2
#define RUNSTOP 4
#define PICKUP 8
#define ASKME 16
#define CONFIRM 32

/* This has to be changed whenever an item is added */
#define NUMSCROLLS 23
#define NUMPOTIONS 18
#define NUMFOODS 15
#define NUMTHINGS 26
#define NUMWEAPONS 38
#define NUMARMOR 17
#define NUMSHIELDS 8
#define NUMCLOAKS 7
#define NUMBOOTS 6
#define NUMRINGS 10
#define NUMSTICKS 16
#define NUMARTIFACTS 21

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

/* item function id's */
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

    /* food functions */
#define I_FOOD 401
#define I_LEMBAS 402
#define I_STIM 403
#define I_POW 404
#define I_IMMUNE 405
#define I_POISON_FOOD 406
#define I_CORPSE 407
#define I_PEPPER_FOOD 408

    /* boots functions */
#define I_PERM_SPEED 501
#define I_PERM_HERO 502
#define I_PERM_LEVITATE 503
#define I_PERM_AGILITY 504
#define I_BOOTS_JUMPING 505

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

/* weapons functions */
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
#define I_DARK_EMP_SWORD 1013

/* thing functions */
#define I_PICK 1101
#define I_KEY 1102
#define I_SHOVEL 1103
#define I_EXCAVATOR 1104
#define I_PERM_ILLUMINATE 1105
#define I_TRAP 1106

/* monster functions */
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

/* special ability functions */
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
#define M_SP_MORGON 217
#define M_SP_EATER 218
#define M_SP_LAWBRINGER 219
#define M_SP_DRAGONLORD 220
#define M_SP_DE 221
#define M_SP_DEMONLOVER 222
#define M_SP_SEDUCTOR 223
#define M_SP_MASTER 224
#define M_SP_WYRM 225
#define M_SP_BLACKOUT 226

/* rangestrike functions */
#define M_STRIKE_MISSILE 301
#define M_STRIKE_FBOLT 302
#define M_STRIKE_LBALL 303
#define M_STRIKE_FBALL 304
#define M_STRIKE_BLIND 305
#define M_STRIKE_SNOWBALL 306
#define M_STRIKE_MASTER 307

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

/* MLx -> index to Monsters starting for level x */
/* MLx -> number of monsters of level x or less */
/* NML_x -> number of monsters of level x */
/* NML-X must be changed whenever a monster is added */
#define ML0 0
#define NML_0 5
#define ML1 (ML0 + NML_0)
#define NML_1 11
#define ML2 (ML1 + NML_1)
#define NML_2 9
#define ML3 (ML2 + NML_2)
#define NML_3 7
#define ML4 (ML3 + NML_3)
#define NML_4 10
#define ML5 (ML4 + NML_4)
#define NML_5 8
#define ML6 (ML5 + NML_5)
#define NML_6 8
#define ML7 (ML6 + NML_6)
#define NML_7 6
#define ML8 (ML7 + NML_7)
#define NML_8 11
#define ML9 (ML8 + NML_8)
#define NML_9 7
#define ML10 (ML9 + NML_9)
#define NML_10 10

#define NUMMONSTERS (ML10 + NML_10)




/* location functions */
#define L_NO_OP 0

/* random terrain types */
#define L_LIFT 1
#define L_HEDGE 2
#define L_WATER 3
#define L_LAVA 4
#define L_MAGIC_POOL 5
#define L_FIRE 6
#define L_WHIRLWIND 7
#define L_ABYSS 8


#define L_TRIFID 11
#define L_FINAL_ABYSS 12
#define L_WARNING 13
#define L_PORTCULLIS 14
#define L_PORTCULLIS_TRAP 15

/* traps */
#define NUMTRAPS 12
#define TRAP_BASE 21

/* traps */
#define L_TRAP_DART 21
#define L_TRAP_PIT 22
#define L_TRAP_DOOR 23
#define L_TRAP_SNARE 24
#define L_TRAP_BLADE 25
#define L_TRAP_FIRE 26
#define L_TRAP_TELEPORT 27
#define L_TRAP_DISINTEGRATE 28
#define L_TRAP_SLEEP_GAS 29
#define L_TRAP_ACID 30
#define L_TRAP_MANADRAIN 31
#define L_TRAP_ABYSS 32

/* shop and guild functions (mostly for city level) */
#define L_DRUID 41
#define L_RESIDENCE 42
#define L_TEMPLE 43
#define L_ARMORER 44
#define L_CLUB 45
#define L_GYM 46
#define L_THIEVES_GUILD 47
#define L_COLLEGE 48
#define L_HEALER 49
#define L_STATUE_WAKE 50
#define L_CASINO 51
#define L_SEWER 52
#define L_TAVERN 53
#define L_MERC_GUILD 54
#define L_ALCHEMIST 55
#define L_SORCERORS 56
#define L_CASTLE 57
#define L_JAIL 58
#define L_GARDEN 59
#define L_ARENA 60
#define L_MAZE 61
#define L_DPW 62
#define L_LIBRARY 63
#define L_PAWN_SHOP 64
#define L_BANK 65
#define L_CONDO 66
#define L_ALTAR 67
#define L_ADEPT 68
#define L_ALL_STATUES 69
#define L_OMEGA 70
#define L_CITYGATE 71
#define L_CHAOSSTORM 72
#define L_VAULT 73
#define L_CEMETARY 74
#define L_CHALLENGE 75
#define L_GUARDIAN 76
#define L_BROTHEL 77

/* structure definitions */

struct room {
  int lighted; 
  int left,right,top,bottom;
  int numconnections;
  int connections[MAXCONNECTIONS];
  int joined[MAXCONNECTIONS];
  int rsi; /* index into roomname switch */
};


struct level_data {
  int generated; /* has the level been made (visited) yet? */
  int numrooms; /* number of rooms on level */
  int tunnelled; /* amount of tunnelling done on this level */
  struct room rooms[MAXROOMS]; /* info about rooms */
};

struct spell {
  int known;
  int powerdrain;
  int id;
  void (*castf)();
} ;


struct monster {
  int id,hp,hit,ac,dmg,sense,wakeup,level;
  int speed,click;
  int status,immunity,tactics; /* bvs */
  int sleep; /* percent chance starts out asleep */
  int treasure; /* percent chance starts with treasure */
  int x,y;
  int xpv;
  int corpseweight,corpsevalue,transformid;
  int attacked,actions,hitloc;
  int uniqueness; /* common, unique and unmade, unique and made */
  char monchar;
  char monstring[64],corpsestr[64];
  struct objectlist *possessions;
  int talkf;
  int movef;
  int meleef;
  int strikef;
  int specialf;
};


struct monsterlist {
  struct monster *m;
  struct monsterlist *next;
};




struct player {
  int str,con,dex,agi,iq,pow,maxstr,maxcon,maxdex,maxagi,maxiq,maxpow;
  int xp,level,hp,maxhp,hit,dmg,absorption,speed,click;
  int defense,vision,food,alignment,mana,maxmana;
  int cash,patron;
  char preference;
  int sx,sy,slevel; /* sanctuary coordinates */
  int x,y; /* current player coordinates */
  int itemweight,maxweight;
  int numrings;
  int immunity[NUMIMMUNITIES];
  int status[NUMSTATI];
  int options;
  int rank[NUMRANKS];
  struct object *possessions[MAXITEMS];
  char name[64];
  /* need one of these for types of objects "in use" except rings */
  struct object *primary; /* primary hand -- weapon or shield */
  struct object *secondary; /* secondary hand -- ditto */
  struct object *armor;
  struct object *cloak;
  struct object *boots;
};



struct object {
  int id,weight,plus,charge,dmg,hit,aux,number,fragility;
  int basevalue, known, used, blessing,type;
  int uniqueness; /* 0 common, 1 unique, ungenerated, 2 u, g */
  int usef; /* effect when used */
  int level; /* 0..10 power and frequency of item */
  char objchar;
  char objstr[64],truename[64],cursestr[64];
};



struct objectlist {
  struct object *thing;
  struct objectlist *next;
};


struct location {
  char p_locf; /* function executed when moved on */
  char stopsrun; /* stops a fastmove */
  char seen; /* has spot ever been viewed by player */
  char secret; 
  char roomnumber;
  char locchar; /* terrain type */
  char showchar; /*char instantaneously drawn for site */
  int aux; /* signifies various things */
  struct objectlist *things; 
  struct monster *creature;
 };



 
/* random typedef's */

typedef struct monsterlist mltype;
typedef struct monster montype;

typedef mltype *pml;
typedef montype *pmt;


typedef struct object objtype;
typedef struct objectlist oltype;

typedef objtype *pob;
typedef oltype *pol;

/* random  function declarations */
char *malloc(),*calloc();
int free();
