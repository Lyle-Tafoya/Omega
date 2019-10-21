/* omega copyright (C) by Laurence Raphael Brothers, 1987 */
/* oguild.c */
/* L_ functions  */ 
/* These functions implement the guilds, priesthoods, etc. */
/* They are all l_ functions since they are basically activated*/
/* at some site or other. */

#include <strings.h>
#include "oglob.h"

/* from ospell */
extern char *spellid();


/* from oinv */
extern char *itemid();
extern int inpack();

/* from oetc */
extern void hint(),learnclericalspells(),nameprint();
extern char *slotstr();

/* from omon */
extern void m_status_set();

/* from osite */
extern void l_jail();

/* from oaux */
extern void conform_lost_object(),conform_unused_object(),p_death();
extern void p_damage(),p_poison(),calc_melee(),gain_experience(),tacmode();
extern void add_item_to_pack(),p_drown(),p_teleport(),p_win(),foodcheck();
extern int item_value(),true_item_value();

/* from oeffect (1 or 2)*/
extern void disintegrate(),identify(),level_drain(),augment(),cleanse();
extern void heal(),acquire(),enchant(),bless(),learnspell();

/* from outil */
extern int random_range(),hour();

/* from oscr */
extern void mprint(),menuclear(),menuprint(),xredraw(),printm();
extern void menu_item_print(),mnumprint(),dataprint();
extern char ynq(),mgetc(),*msgscanstring();

/* from oitem */
extern pob create_object();

/* from olev */
pmt m_create();

/* site movef functions */

void l_druid(),l_thieves_guild(),l_college(),l_merc_guild(),l_sorcerors();
void l_altar(),l_arena();

/* auxillary functions for above */
void make_hp(),hp_req_test(),hp_req_print();

void l_thieves_guild()
{
  int fee,i,done=FALSE,dues=1000;
  char action;
  pob lockpick;
  mprint("You have penetrated to the Lair of the Thieves' Guild.");
  if (! nighttime()) 
    mprint("There aren't any thieves around in the daytime.");
  else {
    while (! done) {
      menuclear();
      menuprint("a: Join the Thieves' Guild.\n");
      menuprint("b: Raise your Guild rank.\n");
      menuprint("c: Get an item identified.\n");
      menuprint("d: Fence an item.\n");
      menuprint("ESCAPE: Leave this Den of Iniquity.");
      action = mgetc();
      if (action == ESCAPE) done = TRUE;
      else if (action == 'a') {
	done = TRUE;
	if (Player.rank[THIEVES]> 0)
	  mprint("You are already a member!");
	else if (Player.alignment > 10) 
	  mprint("You are too lawful to be a thief!");
	else {
	  dues = (int) (dues * (1+(12-Player.dex)/9.0));
	  dues += Player.alignment*5;
	  dues = max(100,dues);
	  mprint("Dues are ");
	  mnumprint(dues);
	  mprint("Au");
	  mprint("Pay it? [yn]");
	  if (ynq() =='y') {
	    if (Player.cash < dues) {
	      mprint("You can't cheat the Thieves' Guild!");
	      mprint("... but the Thieves' Guild can cheat you....");
	      Player.cash = 0;
	    }
	    else {
	      printm("\nShadowlord");
	      mprint(Shadowlord);
	      mprint("enters your name into the roll of the Thieves' Guild."); 
	      mprint("As a special bonus, you get a free lockpick.");
	      mprint("You are taught the spell of Object Detection.");
	      Spells[S_OBJ_DET].known = TRUE; 
	      Thieflevel = Player.level;
	      lockpick = ((pob) calloc(1,sizeof(objtype)));
	      *lockpick = Objects[THINGID+2]; /* lock pick */
	      add_item_to_pack(lockpick);
	      Player.cash -= dues;
	      dataprint();
	      Player.rank[THIEVES]=TMEMBER;
	      Player.maxdex++;Player.dex++;Player.agi++;Player.maxagi++;
	    }
	  }
	}
      }
      else if (action == 'b') {
	if (Player.rank[THIEVES]==0)
	  mprint("You are not even a member!");
	else if (Player.rank[THIEVES]==SHADOWLORD) 
	  mprint("You can't get any higher than this!");
	else if (Player.rank[THIEVES]==TMASTER) {
	  if (Player.level-Thieflevel <= Shadowlordlevel)
	    mprint("You are not experienced enough to advance.");
	  if (! (i=inpack(THINGID+16,-1))+1)
	    mprint("You must bring back Morgon's Badge!");
	  else  {
	    mprint("The Badge is put in a place of honor in the Guild Hall.");
	    conform_lost_object(Player.possessions[i]);
	    mprint("You are now the Shadowlord of the Thieves' Guild!");
	    strcpy(Shadowlord,Player.name);
	    Shadowlordlevel = Player.level;
	    mprint("You now know the Spell of Shadowform.");
	    Spells[S_SHADOWFORM].known = TRUE;
	    Player.rank[THIEVES]=SHADOWLORD;
	    Player.maxagi += 2;
	    Player.maxdex += 2;
	    Player.agi += 2;
	    Player.dex += 2;
	  }
	}
	else if (Player.rank[THIEVES]==THIEF) {
	  if (Player.level-Thieflevel < 13)
	    mprint("You are not experienced enough to advance.");
	  else  {
	    mprint("You are now a Master Thief of the Guild!");
	    mprint("You now know the Spell of Apportation.");
	    mprint("To advance to the next level you must return with");
	    mprint("the badge of Morgon, the Justiciar (cursed be his name).");
	    mprint("If you don't meet him on a higher level, Morgon may");
	    mprint("be found as one of the Guardians on level 39.");
	    Spells[S_APPORT].known = TRUE;
	    Player.rank[THIEVES]=TMASTER;
	    Player.maxagi++;
	    Player.maxdex++;
	    Player.agi++;
	    Player.dex++;
	  }
	}
	else if (Player.rank[THIEVES]==ATHIEF) {	
	  if (Player.level-Thieflevel < 7)
	    mprint("You are not experienced enough to advance.");
	  else  {
	    mprint("You are now a ranking Thief of the Guild!");
	    mprint("You now know the Spell of Invisibility.");
	    Spells[S_INVISIBLE].known = TRUE;
	    Player.rank[THIEVES]=THIEF;
	    Player.agi++;
	    Player.maxagi++;
	  }
	}
	else if (Player.rank[THIEVES]==TMEMBER) {
	  if (Player.level-Thieflevel < 3)
	    mprint("You are not experienced enough to advance.");
	  else  {
	    mprint("You are now an Apprentice Thief!");
	    mprint("You now know the Spell of Levitation.");
	    Spells[S_LEVITATE].known = TRUE;
	    Player.rank[THIEVES]=ATHIEF;
	    Player.dex++;
	    Player.maxdex++;
	  }
	}
      }
      else if (action == 'c') {
	if (Player.rank[THIEVES]==0) {
	  printm("\nRTG, Inc, Appraisers. Identification Fee: 50Au.");
	  fee = 50;
	}
	else {
	  fee = 5;
	  printm("\nIt'll cost you 5Au, mac!");
	}
	mprint("Pay it? [yn]");
	if (ynq()=='y') {
	  if (Player.cash < fee)
	    mprint("Try again when you have the cash.");
	  else {
	    Player.cash -= fee;
	    dataprint();
	    identify(0);
	  }
	}
      }
      else if (action == 'd') {
	if (Player.rank[THIEVES]==0)
	  mprint("Fence? Who said anything about a fence?");
	else {
	  printm("\nFence which item?");
	  i = getitem(NULL);
	  if ((i==ABORT) || (Player.possessions[i-'a'] == NULL))
	    mprint("Huh, Is this some kind of set-up?");
	  else if (Player.possessions[i-'a']->known == 0)
	    mprint("You got to have it appraised, first!");
	  else {
	    mprint("I'll give you ");
	    mnumprint((int) (item_value(Player.possessions[i-'a'])/2));
	    mprint("Au for it. OK? [yn]");
	    if (ynq() == 'y') {
	      Player.cash += ((int) (item_value(Player.possessions[i-'a'])/2));
	      dataprint();
	      conform_lost_objects(1,Player.possessions[i-'a']);
	    }
	    else mprint("Hey, gimme a break, it was a fair price!");
	  }
	}
      }
    }
  }
  xredraw();
}

void l_college()
{
  char action;
  int i,j,done=FALSE,enrolled = FALSE;
  mprint("The Collegium Magii. Founded 16937, AOF.");
  if (nighttime())
    mprint("The Registration desk is closed at night....");
  else {
    while (! done) {
      menuclear();
      menuprint("a: Enroll in the College.\n");
      menuprint("b: Raise your College rank.\n");
      menuprint("c: Do spell research.\n");
      menuprint("ESCAPE: Leave these hallowed halls.\n");
      action = mgetc();
      if (action == ESCAPE) done = TRUE;
      else if (action == 'a') {
	if (Player.rank[COLLEGE] > 0)
	  mprint("You are already enrolled!");
	else if (Player.iq < 13) 
	  mprint("Your low IQ renders you incapable of being educated.");
	else {
	  if (Player.iq > 17) {
	    mprint("You are given a scholarship!");
	    enrolled=TRUE;
	  }
	  else {
	    mprint("Tuition is 1000Au");
	    mprint("Pay it? [yn]");
	    if (ynq() =='y') {
	      if (Player.cash < 1000)
		mprint("You don't have the funds!");
	      else {
		Player.cash -= 1000;
		enrolled = TRUE;
		dataprint();
	      }		
	    }
	  }
	  if (enrolled) {
	    mprint("Archmage");
	    mprint(Archmage);
	    mprint("greets you and congratulates you on your acceptance.");
	    mprint("You are now enrolled in the Collegium Magii!");
	    mprint("You are now a Novice.");
	    Collegelevel = Player.level;
	    Player.rank[COLLEGE] = INITIATE;
	    Player.maxiq += 1;
	    Player.iq += 1;
	    Player.maxpow += 1;
	    Player.pow += 1;
	  }
	}
      }
      else if (action == 'b') {
	if (Player.rank[COLLEGE] == 0)
	  mprint("You have not even been initiated, yet!");
	else if (Player.rank[COLLEGE]==ARCHMAGE) 
	  mprint("You are at the pinnacle of mastery in the Collegium.");
	else if (Player.rank[COLLEGE]==MAGE) {
	  if (Player.level-Collegelevel <= Archmagelevel)
	    mprint("You are not experienced enough to advance.");
	  else if (! (i=(inpack(CORPSEID,ML10+1)+1)))
	    mprint("You must return with the heart of the Eater of Magic!");
	  else  {
	    mprint("The Heart is sent to the labs for analysis.");
	    conform_lost_object(Player.possessions[i]);
	    mprint("You are now Archmage of the Collegium Magii!");
	    strcpy(Archmage,Player.name);
	    Archmagelevel = Player.level-Collegelevel;
	    Player.rank[COLLEGE] = ARCHMAGE;
	    Player.maxiq += 5;
	    Player.iq += 5;
	    Player.maxpow += 5;
	    Player.pow += 5;
	  }
	}
	else if (Player.rank[COLLEGE]==PRECEPTOR) {
	  if (Player.level-Collegelevel < 13)
	    mprint("You are not experienced enough to advance.");
	  else  {
	    mprint("You are now a Mage of the Collegium Magii!");
	    mprint("To become Archmage, you must return with the");
	    mprint("heart of the Eater of Magic");
	    mprint("If you don't meet it on a higher level, the Eater may");
	    mprint("be found as one of the Guardians on level 39.");
	    Player.rank[COLLEGE] = MAGE;
	    Player.maxiq += 2;
	    Player.iq += 2;
	    Player.maxpow += 2;
	    Player.pow += 2;
	  }
	}
	else if (Player.rank[COLLEGE]==STUDENT) {
	  if (Player.level-Collegelevel < 7)
	    mprint("You are not experienced enough to advance.");
	  else  {
	    mprint("You are now a Preceptor of the Collegium Magii!");
	    mprint("You are taught the basics of ritual magic.");
	    Spells[S_RITUAL].known = TRUE;
	    Player.rank[COLLEGE] = PRECEPTOR;
	  }
	  Player.maxiq += 1;
	  Player.iq += 1;
	  Player.maxpow += 1;
	  Player.pow += 1;
	}
	else if (Player.rank[COLLEGE]==NOVICE) {
	  if (Player.level-Collegelevel < 3)
	    mprint("You are not experienced enough to advance.");
	  else  {
	    mprint("You are now a Student at the Collegium Magii!");
	    mprint("You are taught the spell of identification.");
	    Spells[S_IDENTIFY].known = TRUE;
	    Player.rank[COLLEGE] = STUDENT;
	    Player.maxiq += 1;
	    Player.iq += 1;
	    Player.maxpow += 1;
	    Player.pow += 1;
	  }
	}
      }
      else if (action == 'c') {
	if (Player.rank[COLLEGE]==0) 
	  mprint("Only Students and Faculty may do spell research here.");
	else {
	  printm("\nLab fee: 2000 Au.");
	  mprint("Pay it? [yn]");
	  if (ynq()=='y') {
	    if (Player.cash < 2000)
	      mprint("Try again when you have the cash.");
	    else {
	      Player.cash -= 2000;
	      dataprint();
	      learnspell(0);
	    }
	  }
	}
      }
    }
  }
  xredraw();
}



void l_sorcerors()
{
  char action;
  int i,j,done=FALSE,fee=3000,total;
  mprint("The Circle of Sorcerors. May we help you?");
  while (! done) {
    menuclear();
    menuprint("a: Become an Initiate of the Circle.\n");
    menuprint("b: Raise your rank in the Circle.\n");
    menuprint("c: Restore mana points\n");
    menuprint("ESCAPE: Leave these Chambers of Power.\n");
    action = mgetc();
    if (action == ESCAPE) done = TRUE;
    else if (action == 'a') {
      if (Player.rank[CIRCLE] > 0)
	  mprint("You are already an initiate!");
      else if (Player.alignment > 0)
	mprint("You may not join -- you reek of Law!");
      else {
	fee += Player.alignment*100;
	fee = (int) (fee * (1+(12-Player.pow)/9.0));
	fee = max(100,fee);
	mprint("For you, there is an initiation fee of ");
	mnumprint(fee);
	mprint("Au");
	mprint("Pay it? [yn]");
	if (ynq() =='y') {
	  if (Player.cash < fee) 
	    mprint("Try again when you have the cash!");
	  else {
	    printm("\nPrime Sorceror");
	    mprint(Prime);
	    mprint("conducts your initiation into the circle of novices.");
	    mprint("You learn the Spell of Magic Missiles.");
	    Spells[S_MISSILE].known = TRUE;
	    Sorcerorlevel = Player.level;
	    Player.cash -= fee;
	    dataprint();
	    Player.rank[CIRCLE] = INITIATE;
	    Player.maxpow++;
	    Player.pow++;
	  }
	}
      }
    }
    else if (action == 'b') {
      if (Player.rank[CIRCLE] == 0)
	mprint("You have not even been initiated, yet!");
      else if (Player.alignment > -1) {
	mprint("Ahh! You have grown too lawful!!!");
	mprint("You are hereby blackballed from the Circle!");
	Player.rank[CIRCLE]=0;
	mprint("A pox upon thee!");
	if (! Player.immunity[INFECTION])
	  Player.status[DISEASED]+=100;
	mprint("And a curse on your possessions!");
	acquire(-1);
	enchant(-1);
	bless(-1);
	mprint("Die, false sorceror!");
	p_damage(25,UNSTOPPABLE,"a sorceror's curse");
      }
      else if (Player.rank[CIRCLE]==PRIME) 
	mprint("You are at the pinnacle of mastery in the Circle.");
      else if (Player.rank[CIRCLE]==HIGHSORCEROR) {
	if (Player.level-Sorcerorlevel <= Primelevel)
	  mprint("You are not experienced enough to advance.");
	else if (! (i=(inpack(CORPSEID,ML10+2)+1)))
	  mprint("You must return with the Crown of the LawBringer!");
	else  {
	  mprint("The Crown is ritually sacrificed to the Lords of Chaos.");
	  conform_lost_object(Player.possessions[i]);
	  mprint("You are now the Prime Sorceror of the Inner Circle!");
	  strcpy(Prime,Player.name);
	  Primelevel = Player.level-Sorcerorlevel;
	  mprint("You learn the Spell of Disintegration!");
	  Spells[S_DISINTEGRATE].known = TRUE;
	  Player.rank[CIRCLE] = PRIME;
	  Player.maxpow += 10;
	  Player.pow += 10;
	}
      }
      else if (Player.rank[CIRCLE]==SORCEROR) {
	if (Player.level-Sorcerorlevel < 13)
	  mprint("You are not experienced enough to advance.");
	else  {
	  mprint("You are now a High Sorceror of the Inner Circle!");
	  mprint("You learn the Spell of Disruption!");
	  mprint("To advance you must return with the LawBringer's Crown!");
	  mprint("If you don't meet him on a higher level, he may");
	  mprint("be found as one of the Guardians on level 39.");
	  Spells[S_DISRUPT].known = TRUE;
	  Player.rank[CIRCLE] = HIGHSORCEROR;
	  Player.maxpow += 5;
	  Player.pow += 5;
	}
      }
      else if (Player.rank[CIRCLE]==ENCHANTER) {
	if (Player.level-Sorcerorlevel < 7)
	  mprint("You are not experienced enough to advance.");
	else  {
	  mprint("You are now a member of the Circle of Sorcerors!");
	  mprint("You learn the Spell of Ball Lightning!");
	  Spells[S_LBALL].known = TRUE;
	  Player.rank[CIRCLE] = SORCEROR;
	  Player.maxpow += 2; 
	  Player.pow+=2;
	}
      }
      else if (Player.rank[CIRCLE]==INITIATE) {
	if (Player.level-Sorcerorlevel < 3)
	  mprint("You are not experienced enough to advance.");
	else  {
	  mprint("You are now a member of the Circle of Enchanters!");
	  mprint("You learn the Spell of Firebolts.");
	  Spells[S_FIREBOLT].known = TRUE;
	  Player.rank[CIRCLE] = ENCHANTER;
	  Player.maxpow++;
	  Player.pow++;
	}
      }
    }
    else if (action == 'c') {
      done = TRUE;
      fee = Player.level*100;
      if (Player.rank[CIRCLE]) fee = fee / 2;
      mprint("That will be:");
      mnumprint(fee);
      mprint("Au.");
      mprint("Pay it [yn]?");
      if (ynq()=='y') {
	if (Player.cash < fee) 
	  mprint("Begone, deadbeat, or face the wrath of the Circle!");
	else {
	  Player.cash -= fee;
	  total = calcmana();
	  while (Player.mana < total) {
	    Player.mana++;
	    dataprint();
	  }
	  mprint("Have a sorcerous day, now!");
	}
      }
      else mprint("Be seeing you!");
    }
  }
  xredraw();
}



void l_merc_guild()
{
  char action;
  int i,j,done=FALSE;
  pob newitem;

  mprint("Legion of Destiny, Mercenary Guild, Inc.");
  if (nighttime())
    mprint("The barracks are under curfew right now."); 
  else {
    while (! done) {
      menuclear();
      menuprint("a: Enlist in the Guild.\n");
      menuprint("b: Apply for a promotion.\n");
      menuprint("ESCAPE: Go on leave.\n");
      action = mgetc();
      if (action == ESCAPE) done = TRUE;
      else if (action == 'a') {
	done = TRUE;
	if (Player.rank[LEGION]>0)
	  mprint("You are already a recruit!");
	else if (Player.rank[ARENA]>0)
	  mprint("The legion don't need any Arena Jocks. Git!");
	else if (Player.con < 13) {
	  mprint("You are too fragile to join the legion.");
	}
	else if (Player.str < 13) {
	  mprint("Your strength is too low to pass the physical!");
	}
	else {
	  printm("\nCommandant");
	  mprint(Commandant);
	  mprint("shakes your hand.");
	  mprint("The Legion pays you a 100Au induction fee.");
	  mprint("You are also issued a shortsword and leather.");
	  mprint("You are now a Legionaire.");
	  newitem = ((pob) calloc(1,sizeof(objtype)));
	  *newitem = Objects[WEAPONID+1]; /* shortsword */
	  add_item_to_pack(newitem);
	  newitem = ((pob) calloc(1,sizeof(objtype)));
	  *newitem = Objects[ARMORID+1]; /* leather */
	  add_item_to_pack(newitem);
	  Player.cash += 100;
	  Player.rank[LEGION] = LEGIONAIRE;
	  Merclevel = Player.level;
	  Player.str++; Player.con++;
	  Player.maxstr++; Player.maxcon++;
	}
      }
      else if (action == 'b') {
	done = TRUE;
	if (Player.rank[LEGION]==0)
	  mprint("You have not even enlisted, yet!");
	else if (Player.rank[LEGION]==COMMANDANT)
	  mprint("You can't be promoted any further!");
	else if (Player.rank[LEGION]==COLONEL) {
	  if (Player.level-Merclevel <= Commandantlevel)
	    mprint("Your record does not yet permit promotion.");
	  else if (! (i=inpack(WEAPONID+37,-1)+1))
	    mprint("You must return with the Sword of the Dark Emperor!");
	  else  {
	    mprint("The Sword of Empire is melted down....");
	    conform_lost_object(Player.possessions[i]);
	    mprint("You have been promoted to Commandant of the Legion!");
	    strcpy(Commandant,Player.name);
	    Commandantlevel = Player.level-Merclevel;
	    mprint("You now know the Spell of Regeneration.");
	    Spells[S_REGENERATE].known = TRUE;
	    Player.rank[LEGION]=COMMANDANT;
	    Player.maxstr += 2;
	    Player.str += 2;
	    Player.maxcon += 2;
	    Player.con += 2;
	    mprint("You are awarded a vorpal sword, and mithril plate.");
	    newitem = ((pob) calloc(1,sizeof(objtype)));
	    *newitem = Objects[WEAPONID+31]; /* vorpal sword */
	    newitem->known = 2;
	    add_item_to_pack(newitem);
	    newitem = ((pob) calloc(1,sizeof(objtype)));
	    *newitem = Objects[ARMORID+11]; /* mithril plate */
	    newitem->known = 2;
	    mprint("You also receive a salary upgrade.");
	    add_item_to_pack(newitem);
	    Player.cash += 1000;
	  }
	}
	else if (Player.rank[LEGION]==FORCE_LEADER) {
	  if (Player.level-Merclevel < 13)
	    mprint("Your request is denied.");
	  else  {
	    mprint("You have been promoted to Legion Colonel!");
	    mprint("You now know the spell of Heroism.");
	    mprint("Your next promotion is contingent on the return of");
	    mprint("the Sword of the Dark Emperor.");
	    mprint("If you don't meet him on a higher level, he may");
	    mprint("be found as one of the Guardians on level 39.");
	    Spells[S_HERO].known = TRUE;
	    Player.rank[LEGION]=COLONEL;
	    Player.maxstr++;
	    Player.str++;
	    Player.maxcon++;
	    Player.con++;
	    mprint("You are awarded a magic greatsword, and plate armor.");
	    newitem = ((pob) calloc(1,sizeof(objtype)));
	    *newitem = Objects[WEAPONID+5]; /* greatsword */
	    newitem->plus = 5;
	    newitem->known = 2;
	    add_item_to_pack(newitem);
	    newitem = ((pob) calloc(1,sizeof(objtype)));
	    *newitem = Objects[ARMORID+8]; /* plate */
	    newitem->known = 2;
	    add_item_to_pack(newitem);
	    mprint("You also receive a salary upgrade.");
	    Player.cash += 500;
	  }
	}
	else if (Player.rank[LEGION] == CENTURION) {
	  if (Player.level-Merclevel < 7)
	    mprint("You are not admitted to OCS.");
	  else  {
	    mprint("You are now a Legion Force-Leader!");
	    mprint("You now know the Spell of Invulnerability.");
	    Spells[S_INVULNERABLE].known = TRUE;
	    Player.rank[LEGION]=FORCE_LEADER;
	    Player.maxstr++;
	    Player.str++;
	    mprint("You are awarded a bastard sword, and plate mail.");
	    newitem = ((pob) calloc(1,sizeof(objtype)));
	    *newitem = Objects[WEAPONID+3]; /* bastard sword */
	    newitem->known = 2;
	    add_item_to_pack(newitem);
	    newitem = ((pob) calloc(1,sizeof(objtype)));
	    *newitem = Objects[ARMORID+7]; /* plate mail*/
	    newitem->known = 2;
	    add_item_to_pack(newitem);
	    mprint("You also receive a salary upgrade.");
	    Player.cash += 250;
	  }
	}
	else if (Player.rank[LEGION]==LEGIONAIRE) {
	  if (Player.level-Merclevel < 3)
	    mprint("You do not get the promotion!");
	  else  {
	    mprint("You are now a Legion Centurion!");
	    mprint("You now know the spell of Monster Detection.");
	    Spells[S_MON_DET].known = TRUE;
	    Player.rank[LEGION] = CENTURION;
	    Player.maxcon++;
	    Player.con++;
	    mprint("You are awarded a broad sword, and chain mail.");
	    newitem = ((pob) calloc(1,sizeof(objtype)));
	    *newitem = Objects[WEAPONID+2]; /* broad sword */
	    newitem->known = 2;
	    add_item_to_pack(newitem);
	    newitem = ((pob) calloc(1,sizeof(objtype)));
	    *newitem = Objects[ARMORID+4]; /* plate mail*/
	    newitem->known = 2;
	    add_item_to_pack(newitem);
	    mprint("You also receive a salary upgrade.");
	    Player.cash += 100;
	  }
	}
      }
    }
  }
  xredraw();
}

void l_castle()
{
  int i,done=FALSE;
  char action;
  if (Player.level < 3) {
    mprint("You can't possibly enter the castle, you nobody!");
    mprint("Come back when you are famous.");
  }
  else {
    while (! done) {
      menuclear();
      menuprint("a: Ask for an audience with the Duke.\n");
      menuprint("ESCAPE: Leave this seat of nobility.\n");
      action = mgetc();
      if (action == ESCAPE) done = TRUE;
      else if (action == 'a') {
	if (Player.rank[NOBILITY]<DUKE) {
	  printm("\nHis Grace,");
	  mprint(Duke);
	  mprint("-- Duke of Rampart! <fanfare>");
	}
	if (Player.rank[NOBILITY]==0) {
	  mprint("Well, sirrah, wouldst embark on a quest? [yn]");
	  if (ynq() == 'y') {
	    mprint("Splendid. Bring me the head of the Goblin King.");
	    Player.rank[NOBILITY]=COMMONER;
	  }
	  else {
	    mprint("You scoundrel! Guards! Take this blackguard away!");
	    p_damage(25,UNSTOPPABLE,"castle guards for lese majeste");
	    l_jail();
	    done = TRUE;
	  }
	}
	else if (Player.rank[NOBILITY]==COMMONER) {
	  if ((i=inpack(CORPSEID,ML3+5))+1) {
	    mprint("Good job, sirrah! I promote you to the rank of esquire.");
	    conform_lost_object(Player.possessions[i]);
	    Player.rank[NOBILITY]=ESQUIRE;
	    gain_experience(100);
	    mprint("Now that you have proved yourself true, another quest!");
	    mprint("Bring me a suit of dragonscale armor.");
	  }
	  else mprint("Do not return until you achieve the quest, caitiff!");
	}
	else if (Player.rank[NOBILITY]==ESQUIRE) {
	  if ((i=inpack(ARMORID+12,-1))+1) { 
	    mprint("My thanks, squire. In return, I dub thee knight!");
	    conform_lost_object(Player.possessions[i]);
	    Player.rank[NOBILITY]=KNIGHT;
	    gain_experience(1000);
	    mprint("If thou wouldst please me further...");
	    mprint("Bring to me the sword: Defender!");
	    mprint("It is said to be in the possession of the Great Wyrm");
	  }
	  else mprint("Your quest is not yet complete, squire.");
	}
	else if (Player.rank[NOBILITY]==KNIGHT) {
	  if ((i=inpack(WEAPONID+34,-1))+1) {
	    mprint("Thanks, good sir knight.");
	    mprint("Here are letters patent to a peerage!");
	    conform_lost_object(Player.possessions[i]);
	    Player.rank[NOBILITY]=LORD;
	    gain_experience(10000);
	    mprint("If you would do me a final service...");
	    mprint("I require the Orb of Mastery. If you would be so kind...");
	    mprint("By the way, you might find the Orb in the possession");
	    mprint("Of the Elemental Master on Level 39");
	  }
	  else mprint("Greetings, sir. My sword? What, you don't have it?");
	}
	else if (Player.rank[NOBILITY]==LORD) {
	  if ((i=inpack(ARTIFACTID+0,-1))+1) {
	    mprint("My sincerest thanks, my lord.");
	    mprint("You have proved yourself a true paragon of chivalry");
	    mprint("I abdicate the Duchy in your favor....");
	    mprint("Oh, you can keep the Orb, by the way....");
	    Player.rank[NOBILITY]=DUKE;
	    gain_experience(10000);
	    strcpy(Duke,Player.name);
	  }
	  else mprint("I didn't really think you were up to the task....");
	}
	else if (Player.rank[NOBILITY]==DUKE)
	  mprint("Hey, you ARE the Duke of Rampart.");
      }
    }
  }
  xredraw();
}

void l_arena()
{
  char response;
  pob newitem;
  struct monster m;
  int prize,value;
  static int kills=0;

  if (Player.rank[ARENA] == -1)
    mprint("A gang of gladiators hurls you out by the scruff of your neck.");
  else {
    mprint("Rampart Coliseum");
    menuclear();
    menuprint("\na: Become a registered Gladiator");
    menuprint("\nb: Enter the Games");
    menuprint("\nESCAPE: leave this bloodstained pit");
    response = mgetc();
    if (response == 'a') {
      if (Player.rank[ARENA]>0)
	mprint("No more training until you kill something.");
      else if (Player.rank[LEGION]>0)
	mprint("We don't train no stinkin' mercs!");
      else if (Player.str < 16)
	mprint("Yer too weak to train!");
      else {
	mprint("Ok, yer now an Arena Trainee.");
	mprint("Here's a wooden sword, and a shield");
	newitem = ((pob) calloc(1,sizeof(objtype)));
	*newitem = Objects[WEAPONID+17]; /* club */
	add_item_to_pack(newitem);
	newitem = ((pob) calloc(1,sizeof(objtype)));
	*newitem = Objects[SHIELDID+2]; /* shield */
	add_item_to_pack(newitem);
	Player.rank[ARENA] = TRAINEE;
	mprint("You've got 5000Au credit at the Gym.");
	Gymcredit+=5000;
      }
    }
    else if (response == 'b') {
      printm("\nOK, we're arranging a match....");
      switch(Player.rank[ARENA]*2+random_range(2)) {
      case 0: 
	nameprint(); mprint("the pencil-necked geek");
	m = Monsters[ML1+9];
	value = 0;
	break;
      case 1:
	nameprint(); mprint("the hornet");
	m = Monsters[ML0+0];
	value = 0;
	break;
      case 2:
	nameprint(); mprint("the goblin");
	value = 2;
	m = Monsters[ML1+6];
	break;
      case 3:
	value = 1;
	nameprint(); mprint("the grunt");
	m = Monsters[ML1+0];
	break;
      case 4:
	value = 2;
	nameprint(); mprint("the ninja");
	m = Monsters[ML2+0];
	break;
      case 5:
	value = 1;
	nameprint(); mprint("the salamander");
	m = Monsters[ML3+0];
	break;
      case 6:
	value = 2;
	nameprint(); mprint("the lich");
	m = Monsters[ML6+5];
	break;
      case 7:
	value = 2;
	nameprint(); mprint("the behemoth");
	m = Monsters[ML7+0];
	break;
      case 8:
      case 9:
	value = 3;
	mprint(Champion); ("the arena champion");
	strcpy(m.monstring,Champion);
	m = Monsters[ML7+3];
	m.level = 20;
	m.hp = Championlevel*20;
	m.hit = Championlevel*2;
	m.ac = Championlevel*2;
	m.dmg = Championlevel;
	m.xpv = Championlevel*Championlevel;
	m.actions = min(10,Championlevel/3);
	m.tactics = MM_GOOD;
	break;
      default:
	mprint("You have a challenger:");
	do 
	  m = Monsters[random_range(ML9-ML0)+ML0];
	while ((m.uniqueness != COMMON) || (m.dmg == 0));
	morewait();
	nameprint();
	strcpy(Str1,"the ");
	strcat(Str1,m.monstring);
	mprint(Str1);
	break;
      }
      m.attacked = TRUE;
      mprint("You sure you want to fight? [yn]");
      if (ynq()=='y') {
	mprint("Then let the battle begin....");
	m.x = Player.x; m.y = Player.y;
	Arena = TRUE;
	tacmode(&m);
	Arena = FALSE;
	if (m.hp>0) {
	  mprint("You wimp! How could you disappoint our fans!");
	  kills = 0;
	  if (Player.rank[ARENA] < 5) {
	    mprint("For your cowardice you are penalized 2500Au Gym credit!");
	    Gymcredit-=2500;
	  }
	  else {
	    mprint("A disappointed crowd thrashes you within inches...");
	    mprint("You are kicked out of the gladiators' guild!");
	    Player.rank[ARENA] = -1;
	    Player.hp = -1;
	  }
	}
	else {
	  if (Player.rank[ARENA]==0)
	    mprint("So big deal, you won. Do you expect a prize?");
	  else {
	    if (m.level == 20) {
	      mprint("The crowd roars its approval!");
	      mprint("You are the new Arena Champion!");
	      Championlevel = Player.level;
	      strcpy(Champion,Player.name);
	    }
	    kills+=value;
	    mprint("Good fight!");
	    mprint("Your prize is:");
	    prize = m.level * m.level * m.level * 20;
	    mnumprint(prize);
	    mprint("Au");
	    Player.cash+=prize;
	    if ((kills > 3) && (Player.rank[ARENA] < 5)){
	      kills = 0;
	      mprint("You've been promoted to a stronger class!");
	      Player.rank[ARENA]++;
	      mprint("You are also entitled to additional training.");
	      Gymcredit+=value*2500;
	    }
	  }
	}
      }
      else {
	mprint("What kind of gladiator are you, anyhow? A gladiolus?");
	mprint("Oh, ha ha ha, that was a funny one. Oh, ha ha.");
      }
    }
    xredraw();
  }
}
void l_altar()
{
  int i,j,done=FALSE,deity = Dungeon[Dlevel][Player.x][Player.y].aux;
  char response;

  switch(deity) {
    default:
      mprint("This rude altar has no markings.");
      break;
    case ODIN:
      mprint("This granite altar is graven with a gallows.");
      break;
    case SET:
      mprint("This sandstone altar has a black hand drawn on it.");
      break;
    case HECATE:
      mprint("This silver altar is inlaid with a black crescent moon.");
      break;
    case ATHENA:
      mprint("This golden altar is inscribed with an owl");
      break;
    case DESTINY:
      mprint("This crystal altar is in the form of an omega.");
      break;
    case DRUID:
      mprint("This oaken altar is ornately engraved with leaves.");
      break;
  }
  menuclear();
  menuprint("a: Pray to the deity of the altar.\n");
  menuprint("ESCAPE: Leave this sacred ground.\n");
  while(! done) {
    response = mgetc();
    if (response == 'a') {
      done = TRUE;
      if (Player.patron != deity) {
	if (Player.patron) {
	  Player.pow--;
	  Player.maxpow--;
	  switch(Player.patron) {
	  case ODIN:
	    mprint("Odin notices your lack of faith!");
	    mprint("You are struck by a thunderbolt!");
	    p_damage(Player.level*5,UNSTOPPABLE,"Odin's wrath");
	    if (Player.hp > 0) {
	      mprint("The bolt warps your feeble frame....");
	      Player.maxcon = ((int)(Player.maxcon/2));
	      Player.con = min(Player.con,Player.maxcon);
	      Player.maxstr = ((int)(Player.maxstr/2));
	      Player.con = min(Player.str,Player.maxstr);
	      if (Player.primary != NULL) {
		mprint("Your weapon disintegrates....");
		conform_lost_object(Player.primary);
	      }
	      if (Player.armor != NULL) {
		mprint("Your armor disintegrates....");
		conform_lost_object(Player.armor);
	      }
	      if (Player.secondary != NULL) {
		mprint("Your shield disintegrates....");
		conform_lost_object(Player.secondary);
	      }
	      Player.patron = 0;
	    }
	    break;
  	  case SET:
	    mprint("Set notices your lack of faith!");
	    mprint("You are blasted by a shaft of black fire!");
	    p_damage(Player.level*5,UNSTOPPABLE,"Set's anger");
	    if (Player.hp > 0) {
	      mprint("You are wreathed in clouds of smoke.");
	      for(i=0;i<MAXITEMS;i++) 
		if ((Player.possessions[i] != NULL) &&
		    (Player.possessions[i]->blessing > -1))
		  conform_lost_object(Player.possessions[i]);
	      Player.patron = 0;
	      mprint("You feel Set's Black Hand on your heart....");
	      Player.con = Player.maxcon = ((int) (Player.maxcon / 4));
	    }
	    break;
	  case HECATE:
	    mprint("Hecate notices your lack of faith!");
	    mprint("You are zapped by dark moonbeams!");
	    p_damage(Player.level*5,UNSTOPPABLE,"Hecate's malice");
	    if (Player.hp > 0) {
	      mprint("The beams leach you of magical power!");
	      Player.maxpow = ((int) (Player.maxpow/5));
	      Player.pow = min(Player.pow,Player.maxpow);
	      for(i=0;i<NUMSPELLS;i++) 
		Spells[i].known = FALSE;
	      Player.patron = 0;
	    }
	    break;
  	  case ATHENA:
	    mprint("Athena notices your lack of faith!");
	    mprint("You are zorched by godsfire!");
	    if (Player.hp > 0) {
	      mprint("The fire burns away your worldly experience!");
	      Player.level = 0;
	      Player.xp = 0;
	      Player.maxhp = Player.hp = Player.con;
	      Player.patron = 0;
	      mprint("Your power is reduced by the blast!!!");
	      Player.pow = Player.maxpow = ((int)Player.maxpow/3);
	      Player.mana = min(Player.mana,calcmana());
	    }
	    break;
	  case DESTINY:
	    mprint("The Lords of Destiny ignore your lack of faith.");
	    break;
	  case DRUID:
	    mprint("Your treachery to the ArchDruid has been noted.");
	    if (random_range(2) == 1)
	      Player.alignment += 20;
	    else Player.alignment -= 20;
	    break;
	  }
	}
	else switch(deity) {
	default:
	  mprint("Some nameless god blesses you....");
	  Player.hp = Player.maxhp;
	  mprint("The altar crumbles to dust and blows away");
	  Dungeon[Dlevel][Player.x][Player.y].locchar = FLOOR;
	  Dungeon[Dlevel][Player.x][Player.y].p_locf = L_NO_OP;
	  break;
	case ODIN:
	  if (Player.alignment > 0) {
	    mprint("Odin hears your prayer!");
	    mprint(Priest[ODIN]);
	    mprint("personally blesses you.");
	    mprint("You are now a lay devotee of Odin.");
	    Clericlevel = Player.level;
	    Player.patron = ODIN;
	    Player.rank[PRIESTHOOD] = LAY;
	    learnclericalspells(ODIN,LAY);
	  }
	  else mprint("Odin ignores you.");
	    break;
	case SET:
	  if (Player.alignment < 0) {
	    mprint("Set hears your prayer!");
	    mprint(Priest[SET]);
	    mprint("personally blesses you.");
	    mprint("You are now a lay devotee of Set.");
	    Clericlevel = Player.level;
	    Player.patron = SET;
	    Player.rank[PRIESTHOOD] = LAY;
	    learnclericalspells(SET,LAY);
	  }
	  else mprint("Set ignores you.");
	  break;
	case ATHENA:
	  if (Player.alignment > 0) {
	    mprint("Athena hears your prayer!");
	    mprint(Priest[ATHENA]);
	    mprint("personally blesses you.");
	    mprint("You are now a lay devotee of Athena.");
	    Clericlevel = Player.level;
	    Player.patron = ATHENA;
	    Player.rank[PRIESTHOOD] = LAY;
	    learnclericalspells(ATHENA,LAY);
	  }
	  else mprint("Athena ignores you.");
	  break;
	case HECATE:
	  if (Player.alignment < 0) {
	    mprint("Hecate hears your prayer!");
	    mprint(Priest[HECATE]);
	    mprint("personally blesses you.");
	    mprint("You are now a lay devotee of Hecate.");
	    Clericlevel = Player.level;
	    Player.patron = HECATE;
	    Player.rank[PRIESTHOOD] = LAY;
	    learnclericalspells(HECATE,LAY);
	  }
	  else mprint("Hecate ignores you.");
	  break;
	case DRUID:
	  if (abs(Player.alignment) < 10) {
	    mprint(Priest[DRUID]);
	    mprint("personally blesses you.");
	    mprint("You are now a lay devotee of the Druids.");
	    Clericlevel = Player.level;
	    Player.patron = DRUID;
	    Player.rank[PRIESTHOOD] = LAY;
	    learnclericalspells(DRUID,LAY);
	  }
	  else {
	    mprint("You hear a voice....");
	    mprint("Only those who embody the Balance may become Druids.");
	  }
	  break;
	case DESTINY:
	  mprint("The Lords of Destiny could hardly care less.");
	  mprint("But you can consider yourself now to be a lay devotee.");
	  Clericlevel = Player.level;
	  Player.patron = DESTINY;
	  Player.rank[PRIESTHOOD] = LAY;
	  break;
	}
      }
      else {
	if ((((deity == ODIN) || (deity == ATHENA)) && 
	     (Player.alignment < 1)) ||
	    (((deity == SET) || (deity == HECATE)) && 
	     (Player.alignment > 1)) ||
	    ((deity == DRUID) && (abs(Player.alignment) > 10))) {
	  mprint("You have swerved from the One True Path!");
	  mprint("Your deity is greatly displeased...");
	  Player.xp -= Player.level*Player.level;
	  Player.xp = max(0,Player.xp);
	}
	else if (Player.rank[PRIESTHOOD]==HIGHPRIEST)
	  mprint("Your prayer gladdens your deity, but has no other effect.");
	else if (Player.rank[PRIESTHOOD]== SPRIEST) {
	  if (Player.level-Clericlevel <= Priestlevel[deity])
	    mprint("Your prayers yield no results.");
	  else hp_req_test(Clericlevel);
	}
	else if (Player.rank[PRIESTHOOD]==PRIEST) {
	  if (Player.level-Clericlevel < 13)
	    mprint("Your prayers yield no results.");
	  else  {
	    mprint("Your deity raises you to the post of Senior Priest");
	    hp_req_print();
	    Player.rank[PRIESTHOOD] = SPRIEST;
	  }
	}
	else if (Player.rank[PRIESTHOOD]==ACOLYTE) {
	  if (Player.level-Clericlevel < 7)
	    mprint("Your prayers yield no results.");
	  else  {
	    mprint("Your deity raises you to the post of Priest");
	    Player.rank[PRIESTHOOD] = PRIEST;
	    learnclericalspells(deity,PRIEST);
	  }
	}
	else if (Player.rank[PRIESTHOOD]==LAY) {
	  if (Player.level-Clericlevel < 3)
	    mprint("Your prayers yield no results.");
	  else  {
	    mprint("Your deity raises you to the post of Acolyte");
	    Player.rank[PRIESTHOOD] = ACOLYTE;
	    learnclericalspells(deity,ACOLYTE);
	  }
	}
      }
    }
    else if (response == ESCAPE) done = TRUE;
  }
  xredraw();
}


void hp_req_test(entrylevel)
int entrylevel;
{
  int i,j,k,l,ok=FALSE;
  switch  (Player.patron) {
  case ODIN:
    if ((i=inpack(ARTIFACTID+17,-1))+1)
      make_hp(i,entrylevel);
    else hp_req_print();
    break;
  case SET:
    if ((i=inpack(ARTIFACTID+14,-1))+1)
      make_hp(i,entrylevel);
    else hp_req_print();
    break;
  case ATHENA:
    if ((i=inpack(ARTIFACTID+15,-1))+1)
      make_hp(i,entrylevel);
    else hp_req_print();
    break;
  case HECATE:
    if ((i=inpack(ARTIFACTID+16,-1))+1)
      make_hp(i,entrylevel);
    else hp_req_print();
    break;
  case DRUID:
    if ((i=inpack(ARTIFACTID+14,-1))+1)
      make_hp(i,entrylevel);
    else if ((i=inpack(ARTIFACTID+15,-1))+1)
      make_hp(i,entrylevel);
    else if ((i=inpack(ARTIFACTID+16,-1))+1)
      make_hp(i,entrylevel);
    else if ((i=inpack(ARTIFACTID+17,-1))+1)
      make_hp(i,entrylevel);
    else hp_req_print();
    break;
  case DESTINY:
    if ((i=inpack(ARTIFACTID+19,-1))+1)
      make_hp(i,entrylevel);
    else hp_req_print();
    break;
  }
}


void hp_req_print()
{
  mprint("To advance further, you must obtain the Holy Symbol of");
  switch(Player.patron) {
  case ODIN:
    mprint(Priest[SET]);
    mprint("whose Temple of the Black Hand is somewhere on levels 34 to 38");
    break;
  case SET:
    mprint(Priest[ODIN]);
    mprint("whose Shrine of the Noose is somewhere on levels 34 to 38");
    break;
  case ATHENA:
    mprint(Priest[HECATE]);
    mprint("whose Church of the Far Side is somewhere on levels 34 to 38");
    break;
  case HECATE:
    mprint(Priest[ATHENA]);
    mprint("whose Parthenon is somewhere on levels 34 to 38");
    break;
  case DRUID:
    mprint("any of the aligned priests");
    mprint("whose temples are somewhere on levels 34 to 38");
    break;
  case DESTINY:
    mprint(Priest[DESTINY]);
    mprint("whose Hall of Fate is somewhere on levels 34 to 38");
    break;
  }
}

void make_hp(i,entrylevel)
int i,entrylevel;
{
  mprint("You notice a change in the symbol you carry....");
  switch(Player.patron) {
  case ODIN:
    *(Player.possessions[i]) = Objects[ARTIFACTID+14];
    break;
  case SET:
    *(Player.possessions[i]) = Objects[ARTIFACTID+15];
    break;
  case ATHENA:
    *(Player.possessions[i]) = Objects[ARTIFACTID+16];
    break;
  case HECATE:
    *(Player.possessions[i]) = Objects[ARTIFACTID+17];
    break;
  case DRUID:
    *(Player.possessions[i]) = Objects[ARTIFACTID+18];
    break;
  case DESTINY:
    *(Player.possessions[i]) = Objects[ARTIFACTID+19];
    break;
  }
  Player.possessions[i]->known = 2;
  Player.possessions[i]->charge = 17;
  mprint("Your deity raises you to the post of High Priest!");
  strcpy(Priest[Player.patron],Player.name);
  Priestlevel[Player.patron] = Player.level - entrylevel;
  Player.rank[PRIESTHOOD] = HIGHPRIEST;
  learnclericalspells(Player.patron,HIGHPRIEST);
}
