/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* move.c */
/* general functions for player moving */

#include "glob.h"



/* various miscellaneous location functions */
void l_water()
{
  if (! gamestatusp(MOUNTED))  {
    if ((Player.possessions[O_ARMOR] != NULL)) {
      print1("Your heavy armor drags you under the water!");
      morewait();
      p_drown();
      print2("You reach the surface again.");
    }
    else if (Player.itemweight > ((int) (Player.maxweight / 2))) {
      print1("The weight of your burden drags you under water!");
      morewait();
      p_drown();
      print2("You reach the surface again.");
    }
    else switch(random_range(32)) {
    case 0:print1("Splish. Splash!"); break;
    case 1:print1("I want my ducky!"); break;
    case 2:print1("So finally you take a bath!"); break;
    case 3:print1("You must be very thirsty!"); break;
    }
  }
  else switch(random_range(32)) {
    case 0:print1("Your horse frolics playfully in the water."); break;
    case 1:print1("Your horse quenches its thirst."); break;
    case 2:print1("Your steed begins to swim...."); break;
    case 3:print1("Your mount thrashes about in the water."); break;
    }
}



void l_chaos()
{
  if (gamestatusp(MOUNTED)) {
    print1("Your steed tries to swim in the raw Chaos, but seems to");
    print2("be having some difficulties...");
    morewait();
    print1("probably because it's just turned into a chaffinch.");
    morewait();
    resetgamestatus(MOUNTED);
  }
  if (! onewithchaos)
    print1("You are immersed in raw Chaos....");
  if (Player.rank[ADEPT]) {
    if (! onewithchaos)
    {
      onewithchaos = 1;
      print2("You achieve oneness of Chaos....");
    }
    Player.mana = max(Player.mana,calcmana());
    Player.hp = max(Player.hp, Player.maxhp);
  }
  else if (Player.rank[PRIESTHOOD] && (! saved)) {
    print2("A mysterious force protects you from the Chaos!");
    print3("Wow.... You feel a bit smug.");
    gain_experience(500);
    saved = TRUE;
  }
  else {
    print2("Uh oh....");
    if (saved) nprint2("Nothing mysterious happens this time....");
    morewait();
    print1("Congratulations! You've achieved maximal entropy!");
    Player.alignment -= 50;
    gain_experience(1000);
    p_death("immersion in raw Chaos");
  }
}




void l_hedge()
{
  if (Player.patron == DRUID) print1("You move through the hedges freely.");
  else {
    print1("You struggle in the brambly hedge... ");
    switch(random_range(6)) {
    case 0: 
      print2("You are stabbed by thorns!");
      p_damage(random_range(6),NORMAL_DAMAGE,"a hedge");
      print3("The thorns were poisonous!");
      p_poison(random_range(12));
      break;
    case 1: 
      print2("You are stabbed by thorns!");
      p_damage(random_range(12),NORMAL_DAMAGE,"a hedge");
      break;
    case 2: 
      print2("You seem to have gotten stuck in the hedge.");
      Player.status[IMMOBILE]+=random_range(5)+1;
      break;
    case 3: 
      if (Player.possessions[O_CLOAK] != NULL) {
	print2("Your cloak was torn on the brambles!");
	dispose_lost_objects(1,Player.possessions[O_CLOAK]);
      }
      else print2("Ouch! These thorns are scratchy!");
      break;
    default: print2("You make your way through unscathed."); break;
    }
  }
}



void l_lava()
{
  print1("Very clever -- walking into a pool of lava...");
  if (gamestatusp(MOUNTED)) {
    print2("Your horse is incinerated... You fall in too!");
    resetgamestatus(MOUNTED);
  }
  morewait();
  if (strcmp(Player.name,"Saltheart Foamfollower")==0) {
    print1("Strangely enough, you don't seem terribly affected.");
    p_damage(1,UNSTOPPABLE,"slow death in a pool of lava");
  }
  else {
    p_damage(random_range(75),FLAME,"incineration in a pool of lava");
    if (Player.hp> 0) p_drown();
    Player.status[IMMOBILE]+=2;
  }
}



void l_fire()
{
  print1("You boldly stride through the curtain of fire...");
  if (gamestatusp(MOUNTED)) {
    print2("Your horse is fried and so are you...");
    resetgamestatus(MOUNTED);
  }
  p_damage(random_range(100),FLAME,"self-immolation");
}

void l_abyss()
{
  int i;
  if (Current_Environment != Current_Dungeon) {
    print1("You fall through a dimensional portal!");
    morewait();
    strategic_teleport(-1);
  }
  else {
    print1("You enter the infinite abyss!");
    morewait();
    if (random_range(100)==13) {
      print1("As you fall you see before you what seems like");
      print2("an infinite congerie of iridescent bubbles.");
      print3("You have met Yog Sothoth!!!");
      morewait();
      clearmsg();
      if (Player.alignment > -10) 
	p_death("the Eater of Souls");
      else {
	print1("The All-In-One must have taken pity on you.");
	print2("A transdimensional portal appears...");
	morewait();
	change_level(Level->depth,Level->depth+1,FALSE);
	gain_experience(2000);
	Player.alignment -= 50;
      }
    }
    else {
      i = 0;
      print1("You fall...");
      while(random_range(3)!=2) {
	if (i%6 == 0)
	    print2("and fall... ");
	else
	    nprint2("and fall... ");
	i++;
	morewait();
      }
      i++;
      print1("Finally,you emerge through an interdimensional interstice...");
      morewait();
      if (Level->depth+i>MaxDungeonLevels) {
	print2("You emerge high above the ground!!!!");
	print3("Yaaaaaaaah........");
	morewait();
	change_environment(E_COUNTRYSIDE);
	do {
	  Player.x = random_range(WIDTH);
	  Player.y = random_range(LENGTH);
	} while(Country[Player.x][Player.y].base_terrain_type == CHAOS_SEA);
	p_damage(i*50,NORMAL_DAMAGE,"a fall from a great height");
      }
      else {
	print2("You built up some velocity during your fall, though....");
	morewait();
	p_damage(i*5,NORMAL_DAMAGE,"a fall through the abyss");
	change_level(Level->depth,Level->depth+i,FALSE);
	gain_experience(i*i*50);
      }
    }
  }
}


void l_lift()
{
  char response;
  int levelnum;
  int distance;
  int too_far = 0;

  Level->site[Player.x][Player.y].locchar = FLOOR;
  Level->site[Player.x][Player.y].p_locf = L_NO_OP;
  lset(Player.x, Player.y, CHANGED);
  print1("You walk onto a shimmering disk....");
  print2("The disk vanishes, and a glow surrounds you.");
  print3("You feel weightless.... You feel ghostly....");
  morewait();
  clearmsg();
  print1("Go up, down, or neither [u,d,ESCAPE] ");
  do response = (char) mcigetc();
  while ((response != 'u') && 
	 (response != 'd') &&
	 (response != ESCAPE));
  if (response != ESCAPE) {
    print1("How many levels?");
    levelnum = (int) parsenum();
    if (levelnum > 6) {
      too_far = 1;
      levelnum = 6;
    }
    if (response == 'u' && Level->depth - levelnum < 1) {
      distance = levelnum - Level->depth;
      change_environment(E_COUNTRYSIDE); /* "you return to the countryside." */
      if (distance > 0) {
	nprint1("..");
	print2("...and keep going up!  You hang in mid air...");
	morewait();
	print3("\"What goes up...\"");
	morewait();
	print3("Yaaaaaaaah........");
	p_damage(distance*10,NORMAL_DAMAGE,"a fall from a great height");
      }
      return;
    }
    else if (response == 'd' && Level->depth + levelnum > MaxDungeonLevels) {
      too_far = 1;
      levelnum = MaxDungeonLevels - Level->depth;
    }
    if (levelnum == 0) {
      print1("Nothing happens.");
      return;
    }
    if (too_far) {
      print1("The lift gives out partway...");
      print2("You rematerialize.....");
    }
    else
      print1("You rematerialize.....");
    change_level(Level->depth,
		(response=='d' ? 
		 Level->depth+levelnum : 
		 Level->depth-levelnum),
		FALSE);
    roomcheck();
  }
}


void l_magic_pool()
{
  int possibilities=random_range(100);
  print1("This pool seems to be enchanted....");
  if (gamestatusp(MOUNTED)) {
    if (random_range(2)) {
      print2("Your horse is polymorphed into a fig newton.");
      resetgamestatus(MOUNTED);
    }
    else print2("Whatever it was, your horse enjoyed it....");
  }
  else  if (possibilities == 0) {
    print1("Oh no! You encounter the DREADED AQUAE MORTIS...");
    if (random_range(1000) < Player.level*Player.level*Player.level) {
      print2("The DREADED AQUAE MORTIS throttles you within inches....");
      print3("but for some reason chooses to let you escape.");
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
    if (Player.possessions[O_WEAPON_HAND] != NULL) {
      print1("You drop your weapon in the pool! It's gone forever!");
      dispose_lost_objects(1,Player.possessions[O_WEAPON_HAND]);
    }
    else print1("You feel fortunate.");
  }
  else if (possibilities < 90) {
    if (Player.possessions[O_WEAPON_HAND] != NULL) {
      print1("Your weapon leaves the pool with a new edge....");
      Player.possessions[O_WEAPON_HAND]->plus += random_range(10)+1;
      calc_melee();
    }
    else print1("You feel unfortunate.");
  }
  else if (possibilities < 95) {
    Player.hp += 10;
    print1("You feel healthier after the dip...");
  }
  else if (possibilities < 99) {
    print1("Oooh, a tainted pool...");
    p_poison(10);
  }
  else if (possibilities == 99) {
    print1("Wow! A pool of azoth!");
    heal(10);
    cleanse(1);
    Player.mana = calcmana()*3;
    Player.str = (Player.maxstr++)*3;
  }
  print2("The pool seems to have dried up.");
  Level->site[Player.x][Player.y].locchar = TRAP;
  Level->site[Player.x][Player.y].p_locf = L_TRAP_PIT;
  lset(Player.x, Player.y, CHANGED);
}
    












void l_no_op()
{
}


void l_tactical_exit()
{
  if (optionp(CONFIRM)) {
    clearmsg();
    print1("Do you really want to leave this place? ");
    if (ynq1() != 'y')
      return;
  }
  /* Free up monsters and items, and the level, if not SAVE_LEVELS */
  free_level(Level);
  Level = NULL;
  if ((Current_Environment == E_TEMPLE) || 
      (Current_Environment == E_TACTICAL_MAP) )
    change_environment(E_COUNTRYSIDE);
  else change_environment(Last_Environment);
}


void l_rubble()
{
  int screwup = random_range(100) - (Player.agi + Player.level); 
  print1("You climb over the unstable pile of rubble....");
  if (screwup < 0) print2("No problem!");
  else {
    print2("You tumble and fall in a small avalanche of debris!");
    print3("You're trapped in the pile!");
    Player.status[IMMOBILE]+=2;
    p_damage(screwup/5,UNSTOPPABLE,"rubble and debris");
    morewait();
  }
}





/* Drops all portcullises in 5 moves */
void l_portcullis_trap()
{
  int i,j,slam=FALSE;
  
  print3("Click.");
  morewait();
  for (i=max(Player.x-5,0);i<min(Player.x+6,WIDTH);i++)
    for(j=max(Player.y-5,0);j<min(Player.y+6,LENGTH);j++) {
      if ((Level->site[i][j].p_locf == L_PORTCULLIS) &&
	  (Level->site[i][j].locchar != PORTCULLIS)) {
	Level->site[i][j].locchar = PORTCULLIS;
	lset(i, j, CHANGED);
	putspot(i,j,PORTCULLIS);
	if ((i==Player.x)&&(j==Player.y)) {
	  print3("Smash! You've been hit by a falling portcullis!");
	  morewait();
	  p_damage(random_range(1000),NORMAL_DAMAGE,"a portcullis");
	}
	slam = TRUE;
      }
    }
  if (slam) print3("You hear heavy walls slamming down!");
}

/* drops every portcullis on level, then kills itself and all similar traps. */
void l_drop_every_portcullis()
{
  int i,j,slam=FALSE;
  
  print3("Click.");
  morewait();
  for (i=0;i<WIDTH;i++)
    for(j=0;j<LENGTH;j++) {
      if (Level->site[i][j].p_locf == L_DROP_EVERY_PORTCULLIS) {
	Level->site[i][j].p_locf = L_NO_OP;
	lset(i, j, CHANGED);
      }
      else if ((Level->site[i][j].p_locf == L_PORTCULLIS) &&
	  (Level->site[i][j].locchar != PORTCULLIS)) {
	Level->site[i][j].locchar = PORTCULLIS;
	lset(i, j, CHANGED);
	putspot(i,j,PORTCULLIS);
	if ((i==Player.x)&&(j==Player.y)) {
	  print3("Smash! You've been hit by a falling portcullis!");
	  morewait();
	  p_damage(random_range(1000),NORMAL_DAMAGE,"a portcullis");
	}
	slam = TRUE;
      }
    }
  if (slam) print3("You hear heavy walls slamming down!");
}



void l_raise_portcullis()
{
  int i,j,open=FALSE;
  for (i=0;i<WIDTH;i++)
    for(j=0;j<LENGTH;j++) {
      if (Level->site[i][j].locchar == PORTCULLIS) {
	Level->site[i][j].locchar = FLOOR;
	lset(i, j, CHANGED);
	putspot(i,j,FLOOR);
	open = TRUE;
      }
    }
  if (open) print1("You hear the sound of steel on stone!");
}


void l_arena_exit()
{
  resetgamestatus(ARENA_MODE);
#ifndef MSDOS_SUPPORTED_ANTIQUE
  free_level(Level);
#endif
  Level = NULL;
  change_environment(E_CITY);
}


void l_house_exit()
{
  if (optionp(CONFIRM)) {
    clearmsg();
    print1("Do you really want to leave this abode? ");
    if (ynq1() != 'y')
      return;
  }
#ifndef MSDOS_SUPPORTED_ANTIQUE
  free_level(Level);
#endif
  Level = NULL;
  change_environment(Last_Environment);
}


void l_void()
{
  clearmsg();
  print1("Geronimo!");
  morewait();
  clearmsg();
  print1("You leap into the void.");
  if (Level->mlist) {
    print2("Death peers over the edge and gazes quizzically at you....");
    morewait();
    print3("'Bye-bye,' he says... 'We'll meet again.'");
  }
  morewait();
  while(Player.hp>0) {
    Time+=60;
    hourly_check();
    usleep(250000);
  }
}


void l_fire_station()
{
  print1("The flames leap up, and the heat is incredible.");
  if (Player.immunity[FLAME]) {
    print2("You feel the terrible heat despite your immunity to fire!");
    morewait();
  }
  print2("Enter the flames? [yn] ");
  if (ynq2()=='y') {
    if (Player.hp == 1) p_death("total incineration");
    else Player.hp = 1;
    dataprint();
    print1("You feel like you are being incinerated! Jump back? [yn] ");
    if (ynq1()=='y')
      print2("Phew! That was close!");
    else {
      Player.pow -= (15+random_range(15));
      if (Player.pow > 0) {
	print2("That's odd, the flame seems to have cooled down now....");
	print3("A flicker of fire seems to dance above the void nearby.");
	Level->site[Player.x][Player.y].locchar = FLOOR;
	Level->site[Player.x][Player.y].p_locf = L_NO_OP;
	stationcheck();
      }
      else {
	print2("The flames seem to have leached away all your mana!");
	p_death("the Essence of Fire");
      }
    }
  }
  else print2("You flinch away from the all-consuming fire.");
}




void l_water_station()
{
  print1("The fluid seems murky and unknowably deep.");
  print2("It bubbles and hisses threateningly.");
  morewait();
  if (Player.status[BREATHING]) {
    print1("You don't feel sanguine about trying to breathe that stuff!");
    morewait();
  }
  if (Player.immunity[ACID]) {
    print2("The vapor burns despite your immunity to acid!");
    morewait();
  }
  print1("Enter the fluid? [yn] ");
  if (ynq1()=='y') {
    if (Player.hp == 1) p_death("drowning in acid (ick, what a way to go)");
    else Player.hp = 1;
    dataprint();
    print2("You choke....");
    morewait();
    nprint2("Your lungs burn....");
    morewait();
    print2("Your body begins to disintegrate.... Leave the pool? [yn] ");
    if (ynq2()=='y')
      print2("Phew! That was close!");
    else {
      clearmsg();
      Player.con -= (15+random_range(15));
      if (Player.con > 0) {
	print1("That's odd, the fluid seems to have been neutralized....");
	print2("A moist miasma wafts above the void nearby.");
	Level->site[Player.x][Player.y].locchar = FLOOR;
	Level->site[Player.x][Player.y].p_locf = L_NO_OP;
	stationcheck();
      }
      else {
	print2("The bubbling fluid has destroyed your constitution!");
	p_death("the Essence of Water");
      }
    }
    
  }
  else print2("You step back from the pool of acid.");
}




void l_air_station()
{
  print1("The whirlwind spins wildly and crackles with lightning.");
  if (Player.immunity[ELECTRICITY])
    print2("You feel static cling despite your immunity to electricity!");
  morewait();
  print1("Enter the storm? [yn] ");
  if (ynq1()=='y') {
    if (Player.hp == 1) p_death("being torn apart and then electrocuted");
    else Player.hp = 1;
    dataprint();
    print1("You are buffeted and burnt by the storm....");
    print2("You begin to lose consciousness.... Leave the storm? [yn] ");
    if (ynq1()=='y')
      print2("Phew! That was close!");
    else {
      Player.iq -= (random_range(15)+15);
      if (Player.iq > 0) {
	print1("That's odd, the storm subsides....");
	print2("A gust of wind brushes past the void nearby.");
	Level->site[Player.x][Player.y].locchar = FLOOR;
	Level->site[Player.x][Player.y].p_locf = L_NO_OP;
	stationcheck();
      }
      else {
	print2("The swirling storm has destroyed your intelligence!");
	p_death("the Essence of Air");
      }
    }
  }
  else print2("You step back from the ominous whirlwind.");
}




void l_earth_station()
{
  pob o;
  print1("The tendrilled mass reaches out for you from the muddy ooze.");
  if (find_item(&o,THINGID+6,-1))
    print2("A splash of salt water does nothing to dissuade the vines.");
  morewait();
  print1("Enter the overgrown mire? [yn] ");
  if (ynq1()=='y') {
    if (Player.hp == 1) p_death("being eaten alive");
    else Player.hp = 1;
    dataprint();
    print1("You are being dragged into the muck. Suckers bite you....");
    print2("You're about to be entangled.... Leave the mud? [yn] ");
    if (ynq2()=='y')
      print2("Phew! That was close!");
    else {
      Player.str -= (15+random_range(15));
      if (Player.str > 0) {
	print1("That's odd, the vine withdraws....");
	print2("A spatter of dirt sprays into the void nearby.");
	Level->site[Player.x][Player.y].locchar = FLOOR;
	Level->site[Player.x][Player.y].p_locf = L_NO_OP;
	stationcheck();
      }
      else {
	print2("The tendril has destroyed your strength!");
	p_death("the Essence of Earth");
      }
    }
  }
  else print2("You step back from the ominous vegetation.");
}

void stationcheck() 
{
  int stationsleft=FALSE;
  int i,j;
  morewait();
  clearmsg();
  print1("You feel regenerated.");
  Player.hp = Player.maxhp;
  dataprint();
  for(i=0;i<WIDTH;i++)
    for(j=0;j<LENGTH;j++)
      if ((Level->site[i][j].locchar == WATER) ||
	  (Level->site[i][j].locchar == HEDGE) ||
	  (Level->site[i][j].locchar == WHIRLWIND) ||
	  (Level->site[i][j].locchar == FIRE))
	stationsleft=TRUE;
  if (! stationsleft) {
    print1("There is a noise like a wild horse's neigh.");
    print2("You spin around, and don't see anyone around at all");
    print3("except for a spurred black cloaked figure carrying a scythe.");
    morewait();clearmsg();
    print1("Death coughs apologetically. He seems a little embarrassed.");
    print2("A voice peals out:");
    print3("'An Adept must be able to conquer Death himself....");
    make_site_monster(32,4,DEATH);
  }
}


/* To survive the void, the other four stations must be visited first,
   to activate the void, then something (Death's scythe, possibly) 
   must be thrown in to satiate the void, then all other items must
   be dropped, then the void must be entered. */

void l_void_station()
{
  int i,something=FALSE;
  print1("You are at the brink of an endless void. Enter it? [yn] ");
  if (ynq()=='y') {
    if (Level->mlist == NULL) {
      print2("You fall forever. Eventually you die of starvation.");
      morewait();
      while(Player.hp>0) {
	Time+=60;
	hourly_check();
	usleep(250000);
      }
    }
    else {
      print1("You enter the void.");
      print2("You feel a sudden surge of power from five directions.");
      morewait();
      something = (Player.packptr > 0);
      if (! something)
	for(i=0;((i<MAXITEMS)&&(!something));i++)
	  if (Player.possessions[i] != NULL)
	    something = TRUE;
      if (something) {
	print1("The flow of power is disrupted by something!");
	print2("The power is unbalanced! You lose control!");
	morewait();
	print1("Each of your cells explodes with a little scream of pain.");
	print2("Your disrupted essence merges with the megaflow.");
	p_death("the Power of the Void");
      }
      else if (! gamestatusp(PREPARED_VOID)){
	print1("The hungry void swallows you whole!");
	print2("Your being dissipates with a pathetic little sigh....");
	p_death("the Emptyness of the Void");
      }
      else {
	print1("The flow of power rages through your body,");
	print2("but you manage to master the surge!");
	print3("You feel adept....");
	morewait();clearmsg();
	print1("With a thought, you soar up through the void to the");
	print2("place from whence you came.");
	print3("As the platform of the Challenge dwindles beneath you");
	morewait();
	clearmsg();
	print1("You see Death raise his scythe to you in a salute.");
	Player.rank[ADEPT] = 1;
	setgamestatus(COMPLETED_CHALLENGE);
	FixedPoints = calc_points();
	/* set so change_environment puts player in correct temple! */
	Player.x = 49;
	Player.y = 59;
	print2("You find yourself back in the Temple of Destiny.");
	morewait();
	change_environment(E_TEMPLE);
      }
    }
  }
  else print2("You back away from the edge....");
}


void l_voice1()
{
  print1("A mysterious voice says: The Hunger of the Void must be satiated.");
  Level->site[Player.x][Player.y].p_locf = L_NO_OP;
}

void l_voice2()
{
  print1("A strange voice recites: Enter the Void as you entered the World.");
  Level->site[Player.x][Player.y].p_locf = L_NO_OP;
}

void l_voice3()
{
  print1("An eerie voice resounds: The Void is the fifth Elemental Station.");
  Level->site[Player.x][Player.y].p_locf = L_NO_OP;
}


void l_whirlwind()
{
  print1("Buffeting winds swirl you up!");
  p_damage(random_range(difficulty()*10),NORMAL_DAMAGE,"a magic whirlwind");
  if (random_range(2)) {
    print2("You are jolted by lightning!");
    p_damage(random_range(difficulty()*10),ELECTRICITY,"a magic whirlwind");
  }
  morewait();
  if (random_range(2)) {
    print1("The whirlwind carries you off....");
    if (random_range(20)==17)
      print2("'I don't think we're in Kansas anymore, toto.'");
    p_teleport(0);
  }
}


void l_enter_circle()
{
  print1("You see a translucent stairway before you, leading down.");
  print2("Take it? [yn] ");
  if (ynq()=='y')
    change_environment(E_CIRCLE);
}

void l_circle_library()
{
  print1("You see before you the arcane library of the Circle of Sorcerors.");
}

void l_tome1()
{
  menuclear();
  menuprint("\nYou discover in a dusty tome some interesting information....");
  menuprint("\nThe Star Gem holds a vast amount of mana, usable");
  menuprint("\nfor either Law or Chaos. It is magically linked to Star Peak");
  menuprint("\nand can either be activated or destroyed there. If destroyed,");
  menuprint("\nits power will be used for Chaos, if activated, for Law.");
  menuprint("\n\nIt is said the LawBringer has waited for an eternity");
  menuprint("\nat Star Peak for someone to bring him the gem.");
  menuprint("\nIt is also rumored that while anyone might destroy the gem,");
  menuprint("\nreleasing chaotic energy, only the LawBringer can release");
  menuprint("\nthe lawful potential of the gem.");
  showmenu();
  morewait();
  xredraw();
}


void l_tome2()
{
  menuclear();
  menuprint("\nYou discover in some ancient notes that the Star Gem can be");
  menuprint("\nused for transportation, but also read a caution that it must");
  menuprint("\nbe allowed to recharge a long time between uses.");
  menuprint("\nA marginal note says 'if only it could be reset to go somewhere");
  menuprint("\nbesides Star Peak, the gem might be useful....'");
  showmenu();
  morewait();
  xredraw();
}


void l_temple_warning()
{
  print1("A stern voice thunders in the air around you:");
  print2("'No unbelievers may enter these sacred precincts;");
  print3("those who defile this shrine will be destroyed!");
}

void l_throne()
{
  pob o;
  int i;
  print1("You have come upon a huge ornately appointed throne!");
  print2("Sit in it? [yn] ");
  if (ynq1()=='y') {
    if (! find_item(&o,ARTIFACTID+22,-1)) {
      print1("The throne emits an eerie violet-black radiance.");
      print2("You find, to your horror, that you cannot get up!");
      print3("You feel an abstract sucking sensation...");
      for(i=0;i<NUMSPELLS;i++) Spells[i].known = FALSE;
      Player.pow = 3;
      Player.mana = 0;
      Player.hp = 1;
      dispel(-1);
      morewait();clearmsg();
      print1("The radiance finally ceases. You can get up now.");
    }
    else {
      if (HiMagicUse == Date) 
	print3("You hear the sound of a magic kazoo played by an asthmatic.");
      else {
	HiMagicUse = Date;
	print1("Following some strange impulse, you raise the Sceptre....");
	print2("You hear a magical fanfare, repeated three times.");
	switch(HiMagic++) {
	case 0: 
	  print3("Strength.");
	  Player.str+=5;
	  Player.maxstr+=5;
	  break;
	case 1: 
	  print3("Constitution.");  
	  Player.con+=5;
	  Player.maxcon+=5;
	  break;
	case 2: 
	  print3("Dexterity.");
	  Player.dex+=5;
	  Player.maxdex+=5;
	  break;
	case 3: 
	  print3("Agility.");  
	  Player.agi+=5;
	  Player.maxagi+=5;
	  break;
	case 4: 
	  print3("Intelligence.");
	  Player.iq+=5;
	  Player.maxiq+=5;
	  break;
	case 5: 
	  print3("Power.");  
	  Player.pow+=5;
	  Player.maxpow+=5;
	  break;
	default:
	  if (Spells[S_WISH].known) {
	    print1("A mysterious voice mutters peevishly....");
	    print2("So what do you want now? A medal?");
	  }
	  else {
	    print1("Mystic runes appear in the air before you:");
	    print2("They appear to describe some high-powered spell.");
	    morewait();
	    print1("You hear a distant voice....");
	    print2("'You may now tread the path of High Magic.'");
	    Spells[S_WISH].known = TRUE;
	  }
	  break;
	case 17:
	  print1("Weird flickering lights play over the throne.");
	  print2("You hear a strange droning sound, as of a magical");
	  morewait();
	  print1("artifact stressed by excessive use....");
	  print2("With an odd tinkling sound the throne shatters!");
	  Level->site[Player.x][Player.y].locchar = RUBBLE;
	  Level->site[Player.x][Player.y].p_locf = L_RUBBLE;
	  lset(Player.x, Player.y, CHANGED);
	  if (find_and_remove_item(ARTIFACTID+22,-1)) {
	    morewait();
	    print1("Your sceptre reverberates with the noise, and");
	    print2("it too explodes in a spray of shards.");
	  }
	  break;
	}
	calc_melee();
	dataprint();
      }
    }
  }
}


void l_escalator()
{
  print1("You have found an extremely long stairway going straight up.");
  print2("The stairs are grilled steel and the bannister is rubber.");
  morewait();
  print1("Take the stairway? [yn] ");
  if (ynq1()=='y') {
    print1("The stairs suddenly start moving with a grind of gears!");
    print2("You are wafted to the surface....");
    change_environment(E_COUNTRYSIDE);
  }
}

void l_enter_court()
{
  print1("You have found a magical portal! Enter it? [yn] ");
  if (ynq1()=='y') {
    if (! gamestatusp(COMPLETED_CASTLE)) {
      if (! gamestatusp(ATTACKED_ORACLE)) {
	print2("A dulcet voice says: 'Jolly good show!'");
	morewait();
      }
      setgamestatus(COMPLETED_CASTLE);
    }
    change_environment(E_COURT);
  }
}

void l_chaostone()
{
  print1("This is a menhir carved of black marble with veins of gold.");
  print2("It emanates an aura of raw chaos, which is not terribly");
  morewait();
  print1("surprising, considering its location.");
  if (Player.alignment < 0) 
    print2("You feel an almost unbearable attraction to the stone.");
  else print2("You find it extremely difficult to approach the stone.");
  morewait();
  clearmsg();
  print1("Touch it? [yn] ");
  if (ynq1()=='y') {
    print1("A sudden flux of energy surrounds you!");
    morewait();
    if (stonecheck(-1)) {
      print2("You feel stronger!");
      Player.maxstr = min(Player.maxstr+10,max(30,Player.maxstr));
      dataprint();
    }
  }
  else print1("You step back from the ominous dolmech.");
}


void l_balancestone()
{
  print1("This is a massive granite slab teetering dangerously on a corner.");
  print2("You feel a sense of balance as you regard it.");
  morewait();
  clearmsg();
  print1("Touch it? [yn] ");
  if (ynq1()=='y') {
    print1("A vortex of mana spins about you!");
    if (abs(Player.alignment) > random_range(50)) {
      print2("The cyclone whirls you off to a strange place!");
      morewait();
      change_environment(E_COUNTRYSIDE);
      do {
	Player.x = random_range(WIDTH);
	Player.y = random_range(LENGTH);
      } while (Country[Player.x][Player.y].current_terrain_type == CHAOS_SEA);
      screencheck(Player.y);
      drawvision(Player.x,Player.y);
    }
    else {
      print2("You are being drained of experience! Step back? [yn] ");
      if (ynq2()=='y') {
	clearmsg();
	print1("The vortex calms down, dimishes, and then disappears.");
      }
      else {
	Player.xp -= Player.xp/4;
	dataprint();
	print2("The vortex vanishes. Suddenly, there is a clap of thunder!");
	morewait();
	Player.alignment = 0;
	strategic_teleport(1);
      }
    }
  }
  else print1("You step back from the unlikely boulder.");
}


void l_lawstone()
{
  print1("This is a stele carved of blueish-green feldspar.");
  print2("You feel an aura of serenity rising from it, and your gaze");
  morewait();
  print1("is attracted to the bulk of Star Peak to the North-East.");
  if (Player.alignment > 0) 
    print2("You feel a subtle attraction to the stone.");
  else print2("You find the stone extremely distasteful to contemplate.");
  morewait();
  clearmsg();
  print1("Touch it? [yn] ");
  if (ynq()=='y') {
    print1("A matrix of power flows about you!");
    morewait();
    if (stonecheck(1)) {
      print2("You feel more vigorous!");
      Player.maxcon = min(Player.maxcon+10,max(Player.maxcon,30));
      dataprint();
    }
  }
  else print1("You step back from the strange obelisk.");
}


void l_voidstone()
{
  int i;
  print1("This is a grey and uninteresting stone.");
  print2("A feeling of nihility emanates from it.");
  morewait();
  clearmsg();
  print1("Touch it? [yn] ");
  if (ynq()=='y') {
    print1("You feel negated.");
    morewait();
    Player.mana = 0;
    toggle_item_use(TRUE);
    for(i=0;i<NUMSTATI;i++)
      Player.status[i] = 0;
    for(i=0;i<MAXITEMS;i++) 
      if (Player.possessions[i] != NULL) {
	Player.possessions[i]->blessing = 0;
	Player.possessions[i]->plus = 0;
	Player.possessions[i]->usef = I_NOTHING;
      }
    toggle_item_use(FALSE);
    calc_melee();
  }
  else print1("You back away from the strange rock.");
}

void l_sacrificestone()
{
  int sacrifice=1;
  int oldmaxhp = Player.maxhp;
  print1("You have come on a weathered basaltic block.");
  print2("On the top surface is an indentation in human shape.");
  morewait();
  print1("You see old rust colored stains in the grain of the stone.");
  print2("You sense something awakening. Touch the block? [yn] ");
  if (ynq2() == 'y') {
    print1("You sense great pain emanating from the ancient altar.");
    print2("Climb on to the block? [yn] ");
    if (ynq2() == 'y') {
      print1("You are stuck fast to the block!");
      print2("You feel your life-force being sucked away!");
      morewait();
      print1("Hit ESCAPE to try and get up at any moment, SPACE to remain.");
      do {
	switch(random_range(4)) {
	  case 0: print2("You feel weaker."); break;
	  case 1: print2("You feel your life fading."); break;
	  case 2: print2("Your energy is vanishing."); break;
	  case 3: print2("You are being drained."); break;
	  }
	Player.hp -= sacrifice;
	Player.maxhp -= sacrifice/2;
	sacrifice *= 2;
	dataprint();
	if ((Player.hp < 1) || (Player.maxhp < 1))
	  p_death("self-sacrifice");
      } while (stillonblock());
      print1("You manage to wrench yourself off the ancient altar!");
      print2("You leave some skin behind, though....");
      morewait();
      if ((oldmaxhp > 10) && (Player.maxhp < 3 * oldmaxhp/4)) {
	print1("A strange red glow arises from the altar.");
	print2("The glow surrounds you.... You sense gratitude.");
	Player.pow += sacrifice;
	Player.maxpow += sacrifice/10;
	dataprint();
      }
      else {
	print1("You a have a sense of rejection.");
	print2("A roil of fetid vapor smokes up from the altar.");
	gain_experience(sacrifice);
      }
    }
    else {
      print1("You sense an emotion of pique all around you.");
      print2("You retreat from the strange stone.");
    }
  }
  else {
    print1("You decide discretion to be the better part of valour.");
    print2("The stone seems to subside sleepily.");
  }
}

void l_mindstone()
{
  print1("You approach a giant crystal of some opaline material.");
  print2("Flashes of irridescent light glint from the object.");
  morewait();
  print1("You feel your attention being drawn by the intricate crystal.");
  print2("Look away from the interesting phenomenon? [yn] ");
  if (ynq2()=='n') {
    print1("Your gaze focuses deeply on the gem....");
    print2("The crystal seems to open up and surround you!");
    morewait();
    if (stonecheck(0)) {
      print1("Your mind has been enhanced by the experience!");
      Player.maxiq = min(Player.maxiq+10,max(Player.maxiq,30));
      dataprint();
    }
  }
  else {
    print1("You manage to wrench your gaze from the odd jewel.");
    print2("The light flashes from the crystal diminish in frequency.");
  }
}
