/* omega copyright (C) by Laurence Raphael Brothers, 1987 */
/* omove.c */
/* general functions for player moving -- traps, etc. also p_movefunction */

#include <strings.h>
#include "oglob.h"

/* from oaux */
extern void gain_experience(),p_death(),p_damage(),p_drown(),damage_item();

/* from oeffect 1 or 2 */
extern void p_poison(),p_teleport(),sleep_player();

/* from oinv */
extern void conform_lost_object();

/* from oeffect (1 or 2)*/
extern void disintegrate(),identify(),level_drain(),augment(),cleanse();
extern void heal(),acquire(),enchant(),bless(),dispel();

/* from ocom */
extern void levelchange();

/* from outil */
extern int random_range();

/* from oscr */
extern void mprint(),menuclear(),menuprint(),xredraw(),printm();
extern void menu_item_print(),mnumprint(),dataprint(),morewait();
extern char ynq(),mgetc(),*msgscanstr();


/* from osite.c */
extern void l_no_op(),l_druid(),l_temple(),l_armorer(),l_club(),l_gym();
extern void l_thieves_guild(),l_college(),l_healer(),l_statue_wake();
extern void l_sewer(),l_tavern(),l_merc_guild(),l_alchemist(),l_sorcerors();
extern void l_altar(),l_castle(),l_casino(),l_chaosstorm(),l_vault();
extern void l_arena(),l_citygate(),l_dpw(),l_library(),l_pawn_shop();
extern void l_condo(),l_cemetary(),l_residence(),l_garden(),l_maze();
extern void l_jail(),l_garden(),l_trifid(),l_portcullis(),l_portcullis_trap();
extern void l_warning(),l_adept(),l_final_abyss(),l_challenge(),l_guardian();
extern void l_brothel();

void p_movefunction();
void l_water(),l_hedge(),l_lava(),l_lift(),l_magic_pool(),l_whirlwind();
void l_abyss();
void l_trap_dart(),l_trap_pit(),l_trap_door(),l_trap_snare();
void l_trap_blade(),l_trap_fire(),l_trap_disintegrate(),l_trap_teleport();
void l_trap_acid(),l_trap_manadrain(),l_trap_sleepgas(),l_trap_abyss();

/* various traps */

void l_trap_dart()
{
  Dungeon[Dlevel][Player.x][Player.y].locchar = TRAP;
  if (random_range(100) < Player.absorption) 
    mprint("A dart plinks off your armor");
  else {
    mprint("You were hit by a dart!");
    p_damage(Dlevel,NORMAL_DAMAGE,"a dart trap");
    if (random_range(3)==1) {
      mprint("It was poisoned!");
      p_poison(Dlevel);
    }
  }
}

void l_trap_pit()
{
  Dungeon[Dlevel][Player.x][Player.y].locchar = TRAP;
  if (Player.itemweight < ((int) (Player.maxweight / 2)))
    mprint("You nimbly dodge a pit trap.");
  else {
    mprint("You fell into a pit!");
    if (random_range(3) == 1) {
      mprint("And were impaled by spikes!");
      p_damage(Dlevel*5,NORMAL_DAMAGE,"a spiked pit");
    }
    else p_damage(Dlevel*2,NORMAL_DAMAGE,"a pit");
    Player.status[IMMOBILE] += 3;
  }
}

void l_trap_door()
{
  Dungeon[Dlevel][Player.x][Player.y].locchar = TRAP;
  if (random_range(100) < Player.agi)
    mprint("You leap over a trap door.");
  else {
    mprint("You fell through a trap door!");
    p_damage(Dlevel*3,NORMAL_DAMAGE,"a trap door");
    levelchange(Dlevel+1,Dungeon[Dlevel][Player.x][Player.y].locchar);
  }
}

void l_trap_snare()
{
  Dungeon[Dlevel][Player.x][Player.y].locchar = TRAP;
  if (random_range(100) < Player.agi)
    mprint("You agilely avoid a snare.");
  else {
    mprint("You were caught in a snare!");
    Player.status[IMMOBILE]+=Dlevel+10;
  }
}

void l_trap_blade()
{
  Dungeon[Dlevel][Player.x][Player.y].locchar = TRAP;
  if (random_range(30) < Player.agi+Player.level)
    mprint("You duck under a scything blade!");
  else {
    mprint("A heavy blade scythes across the room and hits you!");
    p_damage(random_range(Dlevel*3)+Dlevel-Player.absorption,
	     NORMAL_DAMAGE,
	     "a blade trap");
  }
}

void l_trap_fire()
{
  if (random_range(50) < Player.agi+Player.level)
    mprint("You dodge a pillar of fire!");
  else {
    mprint("You were blasted by a fire trap!");
    p_damage(random_range((Dlevel+1)*5),FLAME,"a fire trap");
  }
  Dungeon[Dlevel][Player.x][Player.y].locchar = FIRE;
  Dungeon[Dlevel][Player.x][Player.y].p_locf = L_FIRE;
}

void l_trap_teleport()
{
  Dungeon[Dlevel][Player.x][Player.y].locchar = TRAP;
  mprint("You experience a momentary disorientation....");
  if (random_range(10000) > Dlevel*Dlevel) p_teleport(0);
  else p_teleport(-1);
}

void l_trap_disintegrate()
{
  Dungeon[Dlevel][Player.x][Player.y].locchar = TRAP;
  mprint("Oh, no! A disintegration trap!");
  disintegrate(Player.x,Player.y);
}

void l_trap_sleepgas()
{
  Dungeon[Dlevel][Player.x][Player.y].locchar = TRAP;
  mprint("Hsssssssss....");
  morewait();
  mprint("You detect a strange odor....");
  sleep_player((Dlevel/5)+1);
}


void l_trap_acid()
{
  int i,k,itemdamage;
  Dungeon[Dlevel][Player.x][Player.y].locchar = TRAP;
  if (Player.agi+Player.level < random_range(100)) {
    mprint("You are drenched by a spray of acid!");
    p_damage(random_range(Dlevel*5),ACID,"an acid trap");
    if (! p_immune(ACID)) {
      mprint("The acid seeps into your pack...");
      morewait();
      itemdamage = random_range(5);
      for(i=k=0;((i<MAXITEMS) && (k<itemdamage));i++)
	if (Player.possessions[i]!=NULL) {
	  k++;
	  damage_item(Player.possessions[i]);
	}
    }
  }
  else mprint("You somehow dodge a shower of hydroflouric acid!");
}

void l_trap_abyss()
{
  int i,k,itemdamge;
  Dungeon[Dlevel][Player.x][Player.y].locchar = ABYSS;
  if (Player.dex + Player.level < random_range(100)) {
    mprint("You stumble over a concealed entrance to the abyss!");
    l_abyss();
  }
  else mprint("You gingerly step around a concealed entrance to the abyss!");
}

void l_trap_manadrain()
{
  Dungeon[Dlevel][Player.x][Player.y].locchar = TRAP;
  if (Player.mana==0) {
    if (random_range(10)==3) {
      mprint("That's strange.... You feel repowered!");
      mprint("The manadrain trap overloads -- positive feedback....");
      Dungeon[Dlevel][Player.x][Player.y].locchar = FLOOR;
      Dungeon[Dlevel][Player.x][Player.y].p_locf = L_NO_OP;
      Player.mana = calcmana();
    }
    else mprint("You feel strangely unaffected by the manadrain trap.");
  }
  else {
    Dungeon[Dlevel][Player.x][Player.y].locchar = TRAP;
    mprint("A weird rainbow light seems to play over you....");
    mprint("You feel drained.");
    while (Player.mana > 0) {
      Player.mana--;
      dataprint();
    }
    dispel(-1);
  }
}


/* various miscellaneous location functions */
void l_water()
{
  if ((Player.armor != NULL)) {
    mprint("Your heavy armor drags you under the water!");
    p_drown();
  }
  else if (Player.itemweight > ((int) (Player.maxweight / 2))) {
    mprint("The weight of your pack drags you under water!");
    p_drown();
  }
  else switch(random_range(24)) {
    case 0:mprint("Splish. Splash!"); break;
    case 1:mprint("I want my ducky!"); break;
    case 2:mprint("So finally you take a bath!"); break;
    case 3:mprint("You must be very thirsty!"); break;
  }
}


void l_hedge()
{
  mprint("You struggle in the brambly hedge... ");
  switch(random_range(6)) {
    case 0: mprint("You are stabbed by thorns!");
            p_damage(random_range(6),NORMAL_DAMAGE,"a hedge");
	    mprint("The thorns were poisonous!");
            p_poison(random_range(12));
	    break;
    case 1: mprint("You are stabbed by thorns!");
            p_damage(random_range(12),NORMAL_DAMAGE,"a hedge");
	    break;
    case 2: mprint("You seem to have gotten stuck in the hedge.");
            Player.status[IMMOBILE]+=random_range(10);
	    break;
    case 3: if (Player.cloak != NULL) {
              mprint("Your cloak was torn on the brambles!");
	      conform_lost_objects(1,Player.cloak);
	    }
            else mprint("Yikes! These thorns are scratchy!");
            break;
    default: mprint("You make your way through unscathed."); break;
  }
}



void l_lava()
{
  mprint("Very clever -- walking into a pool of lava...");
  if (strcmp(Player.name,"Saltheart Foamfollower")==0) {
    mprint("Strangely enough, you don't seem terribly affected.");
    p_damage(1,UNSTOPPABLE,"slow death in a pool of lava");
  }
  else {
    p_damage(random_range(75),FLAME,"incineration in a pool of lava");
    if (Player.hp> 0) p_drown();
    Player.status[IMMOBILE]+=2;
  }
}


void l_whirlwind()
{
  mprint("You've just entered a strange electrical disturbance!");
  p_damage(random_range(Dlevel*5),ELECTRICITY,"magical lightning");
  if (random_range(100) > Player.str*2 + Player.agi*2) {
    mprint("A strange tornado-like wind picks you up and wafts you away!");
    p_teleport(0);
  }
  if (! p_immune(ELECTRICITY)) {
    Player.status[IMMOBILE]+=2;
    mprint("You are stunned by the discharges!");
  }
}

void l_fire()
{
  mprint("You boldly stride through the curtain of fire...");
  p_damage(random_range(100),FLAME,"self-immolation");
}

void l_abyss()
{
  int i;
  mprint("You enter the infinite abyss!");
  if (random_range(100)==13) {
    mprint("As you fall you see before you what seems like");
    mprint("an infinite congerie of iridescent bubbles.");
    mprint("You have met Yog Sothoth!!!");
    if (Player.alignment > -10) 
      p_death("the eater of souls");
    else {
      mprint("The All-In-One must have taken pity on you.");
      mprint("A transdimensional portal appears...");
      levelchange(Dlevel+1,FLOOR);
      gain_experience(2000);
      Player.alignment -= 50;
    }
  }
  else if (Dlevel > 30) {
    mprint("You fall forever, tumbling through an eerie void.");
    while(Player.hp>0) {
      Player.food--;
      foodcheck();
    }
  }
  else {
    i = 1;
    mprint("You fall...");
    while(random_range(4)!=2) {
      i++;
      mprint("...and fall...");
      morewait();
    }
    p_damage(i*5,NORMAL_DAMAGE,"fall through the abyss");
    mprint("Finally,you emerge through an interdimensional interstice...");
    if (Dlevel+i>NUMLEVELS-1) {
      mprint("Unfortunately you emerged deep underground.");
      mprint("You ened your existence with a muted popping sound...");
      p_death("warping into magma from the infinite abyss");
    }
    else {
      mprint("You built up some velocity during your fall, though....");
      levelchange(Dlevel+i,Dungeon[Dlevel][Player.x][Player.y].locchar);
      gain_experience(i*i);
    }
  }
}



void l_lift()
{
  char response;
  int levelnum;
  Dungeon[Dlevel][Player.x][Player.y].locchar = FLOOR;
  Dungeon[Dlevel][Player.x][Player.y].p_locf = L_NO_OP;
  mprint("You walk onto a shimmering disk....");
  mprint("A glow surrounds you.");
  mprint("You feel weightless.... You feel ghostly....");
  mprint("Go up, down, or neither [u,d,ESCAPE]");
  do response = mgetc();
  while ((response != 'u') && 
	 (response != 'd') &&
	 (response != ESCAPE));
  if (response != ESCAPE) {
    mprint("How many levels?");
    levelnum = parsenum();
    if (levelnum > 6) {
      mprint("The lift gives out partway...");
      levelnum = 6;
    }
    mprint("You rematerialize.....");
    levelchange((response=='d' ? Dlevel+levelnum : Dlevel - levelnum),FLOOR);
  }
}


void l_magic_pool()
{
  int possibilities=random_range(100);
  mprint("This pool seems to be enchanted....");
  if (possibilities == 0) {
    mprint("Oh no! You encounter the DREADED AQUAE MORTIS...");
    if (random_range(100) < Player.level) {
      mprint("The DREADED AQUAE MORTIS throttles you within inches....");
      mprint("but for some reason chooses to let you escape.");
      gain_experience(500);
      Player.hp = 1;
    }
    else p_death("the DREADED AQUAE MORTIS!");
  }
  else if (possibilities < 25)
    augment(0);
  else if (possibilities < 30)
    augment(1);
  else if (possibilities < 60)
    augment(-1);
  else if (possibilities < 65)
    cleanse(1);
  else if (possibilities < 80) {
    if (Player.primary != NULL) {
      mprint("You drop your weapon in the pool! It's gone forever!");
      conform_lost_objects(1,Player.primary);
    }
    else mprint("You feel fortunate.");
  }
  else if (possibilities < 90) {
    if (Player.primary != NULL) {
      mprint("Your weapon leaves the pool with a new edge....");
      Player.primary->plus += random_range(3)+1;
      calc_melee();
    }
    else mprint("You feel unfortunate.");
  }
  else if (possibilities < 95) {
    Player.hp = max(Player.hp+10,Player.maxhp);
    mprint("You feel healthier after the dip...");
  }
  else if (possibilities < 99) {
    mprint("Oooh, a tainted pool...");
    p_poison(10);
  }
  else {
    mprint("Wow! A pool of azoth!");
    heal(10);
    cleanse(1);
    Player.mana = calcmana()*3;
    Player.str = (Player.maxstr++)*3;
  }
  mprint("The pool seems to have dried up.");
  Dungeon[Dlevel][Player.x][Player.y].locchar = TRAP;
  Dungeon[Dlevel][Player.x][Player.y].p_locf = L_TRAP_PIT;
}
    












void p_movefunction(movef)
int movef;
{
  /* loc functs above traps should be activated whether levitating or not */
  if ((! Player.status[LEVITATING]) || 
      (movef > TRAP_BASE+NUMTRAPS)) 
    switch(movef) {

    /* miscellaneous */
    case L_NO_OP:l_no_op(); break;
    case L_HEDGE:l_hedge(); break;
    case L_WATER:l_water(); break;
    case L_LIFT:l_lift(); break;
    case L_LAVA:l_lava(); break;
    case L_FIRE:l_fire(); break;
    case L_MAGIC_POOL:l_magic_pool(); break;
    case L_WHIRLWIND:l_whirlwind(); break;
    case L_ABYSS:l_abyss(); break;
      
    case L_FINAL_ABYSS:l_final_abyss(); break;
    case L_ADEPT:l_adept(); break;
    case L_PORTCULLIS:l_portcullis(); break;
    case L_PORTCULLIS_TRAP:l_portcullis_trap(); break;
    case L_WARNING:l_warning(); break;
    case L_TRIFID:l_trifid(); break;
    case L_CHALLENGE:l_challenge(); break;
    case L_GUARDIAN:l_guardian(); break;

    case L_TRAP_DART:l_trap_dart(); break;
    case L_TRAP_PIT:l_trap_pit(); break;
    case L_TRAP_DOOR:l_trap_door(); break;
    case L_TRAP_SNARE:l_trap_snare(); break;
    case L_TRAP_BLADE:l_trap_blade(); break;
    case L_TRAP_FIRE:l_trap_fire(); break;
    case L_TRAP_TELEPORT:l_trap_teleport(); break;
    case L_TRAP_DISINTEGRATE:l_trap_disintegrate(); break;
    case L_TRAP_SLEEP_GAS:l_trap_sleepgas(); break;
    case L_TRAP_MANADRAIN:l_trap_manadrain(); break;
    case L_TRAP_ACID:l_trap_acid(); break;
    case L_TRAP_ABYSS:l_trap_abyss(); break;

   /*door functions */
    case L_BANK:l_bank(); break;
    case L_RESIDENCE:l_residence();break;
    case L_DRUID:l_druid(); break;
    case L_TEMPLE:l_temple(); break;
    case L_ARMORER:l_armorer(); break;
    case L_CLUB:l_club(); break;
    case L_GYM:l_gym(); break;
    case L_THIEVES_GUILD:l_thieves_guild(); break;
    case L_COLLEGE:l_college(); break;
    case L_HEALER:l_healer(); break;
    case L_STATUE_WAKE:l_statue_wake(); break;
    case L_CASINO:l_casino(); break;
    case L_SEWER:l_sewer(); break;
    case L_TAVERN:l_tavern(); break;
    case L_MERC_GUILD:l_merc_guild(); break;
    case L_ALCHEMIST:l_alchemist(); break;
    case L_SORCERORS:l_sorcerors(); break;
    case L_CASTLE:l_castle(); break;
    case L_JAIL:l_jail(); break;
    case L_GARDEN:l_garden(); break;
    case L_ARENA:l_arena(); break;
    case L_CITYGATE:l_citygate(); break;
    case L_CHAOSSTORM:l_chaosstorm();break;
    case L_VAULT:l_vault(); break;
    case L_DPW:l_dpw(); break;
    case L_LIBRARY:l_library(); break;
    case L_PAWN_SHOP:l_pawn_shop(); break;
    case L_CEMETARY:l_cemetary(); break;
    case L_CONDO:l_condo(); break;
    case L_MAZE: l_maze(); break;
    case L_ALTAR:l_altar(); break;
    case L_BROTHEL:l_brothel(); break;
  }
}



void l_no_op()
{
}


