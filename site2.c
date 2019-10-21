/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* site2.c */
/* 2nd half of site functions and aux functions to them */

#include "glob.h"



void l_condo()
{
  pol ol,prev=NULL;
  int i,done=FALSE,over=FALSE,weeksleep=FALSE;
  char response;

  if (! gamestatusp(SOLD_CONDO)) {
    print1("Rampart Arms. Weekly Rentals and Purchases");
    print2("Which are you interested in [r,p, or ESCAPE] ");
    response = mgetc();
    if (response == 'p') {
      print2("Only 50,000Au. Buy it? [yn] ");
      if (ynq2()=='y') {
	if (Player.cash < 50000) 
	  print3("No mortgages, buddy.");
	else {
	  setgamestatus(SOLD_CONDO);
	  Player.cash-=50000;
	  dataprint();
	  print2("You are the proud owner of a luxurious condo penthouse.");
	  Condoitems = NULL;
	}
      }
    }
    else if (response == 'r') {
      print2("Weekly Rental, 1000Au. Pay for it? [yn] ");
      if (ynq2()=='y') {
	if (Player.cash < 1000)
	  print2("Hey, pay the rent or out you go....");
	else {
	  weeksleep = TRUE;
	  Player.cash -=1000;
	  dataprint();
	}
      }
    }
    else print2("Please keep us in mind for your housing needs.");
  }
  else {
    while (! done) {
      menuclear();
      menuprint("Home Sweet Home\n");
      menuprint("a: Leave items in your safe.\n");
      menuprint("b: Retrieve items.\n");
      menuprint("c: Take a week off to rest.\n");
      menuprint("d: Retire permanently.\n");
      menuprint("ESCAPE: Leave this place.\n");
      showmenu();
      response = (char) mcigetc();
      if (response == 'a') {
	i = getitem(NULL_ITEM);
	if (i != ABORT) {
	  if (Player.possessions[i]->blessing < 0) 
	    print2("The item just doesn't want to be stored away...");
	  else {
	    ol = ((pol) checkmalloc(sizeof(oltype)));
	    ol->thing = Player.possessions[i];
	    ol->next = Condoitems;
	    Condoitems = ol;
	    conform_unused_object(Player.possessions[i]);
	    Player.possessions[i] = NULL;
	  }
	}
      }
      else if (response == 'b') {
	ol = Condoitems;
	while ((ol != NULL) && (! over)) {
	  print1("Retrieve ");
	  nprint1(itemid(ol->thing));
	  nprint1(" [ynq] ");
	  response = (char) mcigetc();
	  if (response == 'y') {
	    gain_item(ol->thing);
	    if (ol == Condoitems)
	      Condoitems = Condoitems->next;
	    else if (prev != NULL) prev->next = ol->next;
	  }
	  else if (response == 'q')
	    over = TRUE;
	  prev = ol;
	  ol = ol->next;
	}
      }
      else if (response == 'c') {
	  weeksleep = TRUE;
	  print1("You take a week off to rest...");
	  morewait();
      }
      else if (response == 'd') {
	clearmsg();
	print1("You sure you want to retire, now? [yn] ");
	if (ynq1() == 'y') {
	  p_win();
	}
      }
      else if (response == ESCAPE) done = TRUE;
    }
    xredraw();
  }
  if (weeksleep) {
    clearmsg();
    print1("Taking a week off to rest...");
    morewait();
    toggle_item_use(TRUE);
    Player.hp = Player.maxhp;
    Player.str = Player.maxstr;
    Player.agi = Player.maxagi;
    Player.con = Player.maxcon;
    Player.dex = Player.maxdex;
    Player.iq = Player.maxiq;
    Player.pow = Player.maxpow;
    for (i=0;i<NUMSTATI;i++)
      if (Player.status[i]<1000) Player.status[i]=0;
    toggle_item_use(FALSE);
    Player.food = 36;
    print2("You're once again fit and ready to continue your adventure.");
    Time += 60*24*7;
    Date += 7;
    moon_check();
    timeprint();
  }
}




void gymtrain(maxstat,stat)
int *maxstat,*stat;
{
  if (Gymcredit + Player.cash < 2000)
    print2("You can't afford our training!");
  else {
    if (Gymcredit > 2000) Gymcredit -= 2000;
    else {
      Player.cash -= (2000-Gymcredit);
      Gymcredit = 0;
    }
    print2("Sweat. Sweat. ");
    if ((*maxstat < 30) && 
	((*maxstat < random_range(30)) || (random_range(3)==1))){
      nprint2("The training pays off!");
      (*maxstat)++;
      (*stat)++;
    }
    else {
      nprint2("You feel the healthy glow of a good workout.");
      if (*stat < *maxstat) {
	(*stat)++;
	print3("A feeling of rehabilitation washes through you.");
      }
    }
  }
  dataprint();
}

void healforpay()
{
  if (Player.cash < 50)
    print2("You can't afford to be healed!");
  else {
    Player.cash -= 50;
    Player.hp += 20+random_range(20);
    if (Player.hp > Player.maxhp) 
      Player.hp = Player.maxhp;
    print2("Another medical marvel....");
  }
  calc_melee();
}

void cureforpay()
{
  if (Player.cash < 250)
    print2("You can't afford to be cured!");
  else {
    Player.cash -= 250;
    Player.status[DISEASED] = 0;
    print2("Quarantine lifted....");
    showflags();
  }
}  



void pacify_guards()
{
  pml ml;

  for(ml=Level->mlist;ml!=NULL;ml=ml->next)
    if ((ml->m->id == GUARD) || /*guard*/
	((ml->m->id == HISCORE_NPC) && (ml->m->aux2 == 15))) {/* justiciar */
      m_status_reset(ml->m,HOSTILE);
      ml->m->specialf = M_NO_OP;
      if (ml->m->id == GUARD && ml->m->hp > 0 && ml->m->aux1 > 0) {
	if (Level->site[ml->m->x][ml->m->y].creature == ml->m)
	  Level->site[ml->m->x][ml->m->y].creature = NULL;
	ml->m->x = ml->m->aux1;
	ml->m->y = ml->m->aux2;
	Level->site[ml->m->x][ml->m->y].creature = ml->m;
      }
      else if (ml->m->id == HISCORE_NPC && ml->m->hp > 0 &&
	  Current_Environment == E_CITY) {
	if (Level->site[ml->m->x][ml->m->y].creature == ml->m)
	  Level->site[ml->m->x][ml->m->y].creature = NULL;
	ml->m->x = 40;
	ml->m->y = 62;
	Level->site[ml->m->x][ml->m->y].creature = ml->m;
      }
    }
    if (Current_Environment == E_CITY)
      Level->site[40][60].p_locf = L_ORDER; /* undoes action in alert_guards */
}

void send_to_jail()
{
  if (Player.rank[ORDER] > 0) {
    print1("A member of the Order of Paladins sent to jail!");
    print2("It cannot be!");
    morewait();
    print1("You are immediately expelled permanently from the Order!");
    print2("Your name is expunged from the records....");
    Player.rank[ORDER] = -1;
  }
  else if (gamestatusp(DESTROYED_ORDER)) 
    print1("The destruction of the Order of Paladins has negated the law!");
  else if ((Current_Environment != E_CITY) &&
	   (Last_Environment != E_CITY)) 
    print1("Fortunately, there is no jail around here, so you are freed!");
  else {
    pacify_guards();
    if (((Current_Environment == E_HOUSE) || 
	 (Current_Environment == E_MANSION) ||
	 (Current_Environment == E_HOVEL)) &&
	(Last_Environment == E_CITY))
    {
      setgamestatus(SUPPRESS_PRINTING);
      change_environment(E_CITY);
      resetgamestatus(SUPPRESS_PRINTING);
    }
    if (Current_Environment == E_CITY) {
      if (gamestatusp(UNDEAD_GUARDS)) {
	print1("You are taken to a weirdly deserted chamber where an undead");
	print2("Magistrate presides over a court of ghosts and haunts.");
	morewait();
	print1("'Mr. Foreman, what is the verdict?'");
	print2("'Guilty as charged, your lordship.'");
	morewait();
	clearmsg();
	print1("'Guilty...");
	morewait();
	nprint1("Guilty...");
	morewait();
	nprint1("Guilty...");
	morewait();
	nprint1("Guilty...'");
	clearmsg();
	print1("The members of the court close in around, fingers pointing.");
	print2("You feel insubstantial hands closing around your throat....");
	print3("You feel your life draining away!");
	while(Player.level > 0) {
	  Player.level--;
	  Player.xp /= 2;
	  Player.hp /= 2;
	  dataprint();
	}
	Player.maxhp = Player.maxcon;
	morewait();
	print1("You are finally released, a husk of your former self....");
	Player.x = 58;
	Player.y = 40;
	screencheck(58);
      }
      else if (Player.alignment + random_range(200) < 0) {
	print1("Luckily for you, a smooth-tongued advocate from the");
	print2("Rampart Chaotic Liberties Union gets you off!");
	Player.x = 58;
	Player.y = 40;
	screencheck(58);
      }
      else switch(Imprisonment++) {
      case 0:
	print1("The Magistrate sternly reprimands you.");
	print2("As a first-time offender, you are given probation.");
	Player.y = 58;
	Player.x = 40;
	screencheck(58);
	break;
      case 1:
	print1("The Magistrate expresses shame for your conduct.");
	print2("You are thrown in jail!");
	morewait();	
	repair_jail();
	Player.y = 54;
	Player.x = 37 + (2*random_range(4));
	screencheck(54);
	l_portcullis_trap();
	break;
      default:
	print1("The Magistrate renders summary judgement.");
	print2("You are sentenced to prison!");
	morewait();
	print1("The guards recognize you as a 'three-time-loser'"); 
	print2("...and beat you up a little to teach you a lesson.");
	p_damage(random_range(Imprisonment * 10),
		 UNSTOPPABLE,
		 "police brutality");
	morewait();
	repair_jail();
	Player.y = 54;
	Player.x = 37 + (2*random_range(4));
	screencheck(54);
	l_portcullis_trap();
      }
    }
  }
}






void l_adept()
{
  print1("You see a giant shimmering gate in the form of an omega.");
  if (! gamestatusp(ATTACKED_ORACLE)) {
    if (Player.str+Player.con+Player.iq+Player.pow < 100)
      print2("A familiar female voice says: I would not advise this now....");
    else print2("A familiar female voice says: Go for it!");
    morewait();
    clearmsg();
  }
  print2("Enter the mystic portal? [yn] ");
  if (ynq2()!='y') {
    if (Player.level > 100) {
      print1("The Lords of Destiny spurn your cowardice....");
      Player.xp = 0;
      Player.level = 0;
      Player.hp = Player.maxhp = Player.con;
      Player.mana = calcmana();
      print2("You suddenly feel very inexperienced.");
      dataprint();
    }	
  }
  else {
    clearmsg();
    print1("You pass through the portal.");
    morewait();
    drawomega();
    print1("Like wow man! Colors! ");
    if (Player.patron != DESTINY) {
      print2("Strange forces try to tear you apart!");
      p_damage(random_range(200),UNSTOPPABLE,"a vortex of chaos");
    }
    else print2("Some strange force shields you from a chaos vortex!");
    morewait();
    print1("Your head spins for a moment....");
    print2("and clears....");
    morewait();
    Player.hp = Player.maxhp;
    Player.mana = calcmana();
    change_environment(E_ABYSS);
  }
}




void l_trifid()
{
  int damage=0,stuck=TRUE;
  print1("The hedge comes alive with a surge of alien growth!");
  while (stuck) {
    dataprint();
    damage += Level->depth/2+1;
    print2("Razor-edged vines covered in suckers attach themselves to you.");
    morewait();
    if (find_and_remove_item(THINGID+6,-1)) {
      print1("Thinking fast, you toss salt water on the trifid...");
      print2("The trifid disintegrates with a frustrated sigh.");
      Level->site[Player.x][Player.y].locchar = FLOOR;
      Level->site[Player.x][Player.y].p_locf = L_NO_OP;
      lset(Player.x, Player.y, CHANGED);
      gain_experience(1000);
      stuck = FALSE;
    }
    else {
      p_damage(damage,UNSTOPPABLE,"a trifid");
      morewait();
      print1("You are entangled in tendrils...");
      menuclear();
      menuprint("a: Try to break free.\n");
      menuprint("b: Hang limp and hope the tendrils uncoil.\n");
      menuprint("c: Pray for assistance.\n");
      menuprint("d: Attempt to bargain with the hedge.\n");
      menuprint("e: Click your heels together and wish for escape.\n");
      menuprint("ANYTHING ELSE: writhe and scream hopelessly.\n");
      showmenu();
      switch(menugetc()) {
      case 'a': 
	if (Player.str > random_range(200)) {
	  print1("Amazing! You're now free.");
	  print2("The trifid writhes hungrily at you.");
	  stuck = FALSE;
	}
	else print1("Well, THAT didn't work.");
	break;
      case 'b': 
	print1("Well, at least you're facing your fate with dignity.");
	break;
      case 'c': 
	if ((Player.patron == DRUID) && 
	    (Player.rank[PRIESTHOOD] > random_range(5))) {
	    print1("A shaft of golden light bathes the alien plant");
	    print2("which grudginly lets you go....");
	    stuck = FALSE;
	  }
	else print1("You receive no divine aid as yet.");
	break;
      case 'd':
	print1("The hedge doesn't answer your entreaties.");
	break;
      case 'e':
	print1("You forgot your ruby slippers, stupid.");
	break;
      default:
	print1("The hedge enjoys your camp play-acting....");
	break;
      }
    }
  }
  xredraw();
}




void l_vault()
{
  print1("You come to a thick vault door with a complex time lock.");
  if ((hour()==23)) {
    print2("The door is open.");
    Level->site[12][56].locchar = FLOOR;
  }
  else {
    print2("The door is closed.");
    Level->site[12][56].locchar = WALL;
    morewait();
    clearmsg();
    print1("Try to crack it? [yn] ");
    if (ynq1()=='y') {
      if (random_range(100) < Player.rank[THIEVES]*Player.rank[THIEVES]) {
	print2("The lock clicks open!!!");
	gain_experience(5000);
	Level->site[12][56].locchar = FLOOR;
      }	
      else {
	print2("Uh, oh, set off the alarm.... The castle guard arrives....");
	morewait();
	if (Player.rank[NOBILITY] == DUKE) {
	  clearmsg();
	  print1("\"Ah, just testing us, your Grace?  I hope we're up to scratch.\"");
	  morewait();
	}
	else
	  send_to_jail();
      }
    }
    else print2("Good move.");
  }
}


void l_brothel()
{
  char response;
  print1("You come to a heavily reinforced inner door.");
  print2("A sign reads `The House of the Eclipse'");
  morewait();
  clearmsg();
  print1("Try to enter? [yn] ");
  if (ynq1()=='y') {
    menuclear();
    menuprint("a:knock on the door.\n");
    menuprint("b:try to pick the lock.\n");
    menuprint("c:bash down the door.\n");
    menuprint("ESCAPE: Leave this house of ill repute.\n");
    showmenu();
    do 
      response = menugetc();
    while ((response != 'a') &&
	   (response != 'b') &&
	   (response != 'c') &&
	   (response != ESCAPE));
    xredraw();
    if (response == 'a') {
      if (!nighttime())
	print2("There is no reponse.");
      else {
	print1("A window opens in the door.");
	print2("`500Au, buddy. For the night.' pay it? [yn] ");
	if (ynq2()=='y') {
	  if (Player.cash < 500) {
	    print1("`What, no roll?!'");
	    print2("The bouncer bounces you a little and lets you go.");
	    p_damage(25,UNSTOPPABLE,"da bouncer");
	  }
	  else {
	    Player.cash -= 500;
	    print1("You are ushered into an opulently appointed hall.");
	    print2("After an expensive dinner (takeout from Les Crapuleux)");
	    morewait();
	    if (Player.preference == 'n') {
	      switch(random_range(4)) {
	      case 0:
		print1("you spend the evening playing German Whist with");
		break;
	      case 1:
		print1("you spend the evening discussing philosophy with");
		break;
	      case 2:
		print1("you spend the evening playing chess against");
		break;
	      case 3:
		print1("you spend the evening telling your adventures to");
	      }
	      print2("various employees of the House of the Eclipse.");
	    }
	    else {
	      print1("you spend an enjoyable and educational evening with");
	      if (Player.preference == 'm' ||
		  (Player.preference == 'y' && random_range(2)))
		switch(random_range(4)) {
		case 0: 
		  print2("Skarn the Insatiable, a satyr.");
		  break;
		case 1: 
		  print2("Dryden the Defanged, an incubus.");
		  break;
		case 2: 
		  print2("Gorgar the Equipped, a centaur.");
		  break;
		case 3: 
		  print2("Hieronymus, the mendicant priest of Eros.");
		  break;
		}	
	      else switch(random_range(4)) {
	      case 0: 
		print2("Noreen the Nymph (omaniac)");
		break;
	      case 1: 
		print2("Angelface, a recanted succubus.");
		break;
	      case 2: 
		print2("Corporal Sue of the City Guard (moonlighting).");
		break;
	      case 3: 
		print2("Sheena the Queena the Jungle, a wereleopard.");
		break;
	      }	
	    }
	    morewait();
	    if (hour() > 12) Time += ((24-hour())+8) * 60;
	    else {
	      Time += ((9-hour())*60);
	      Date++;
	    }
	    Player.food = 40;
	    Player.status[DISEASED] = 0;
	    Player.status[POISONED] = 0;
	    Player.hp = Player.maxhp;
	    /* reduce temporary stat gains to max stat levels */
	    toggle_item_use(TRUE);
	    Player.str = min(Player.str,Player.maxstr);
	    Player.con = min(Player.con,Player.maxcon);
	    Player.agi = min(Player.agi,Player.maxagi);
	    Player.dex = min(Player.dex,Player.maxdex);
	    Player.iq = min(Player.iq,Player.maxiq);
	    Player.pow = min(Player.pow,Player.maxpow);
	    toggle_item_use(FALSE);
	    if (Player.preference == 'n')
	      Player.iq++; /* whatever :-) */
	    else
	      Player.con++;
	    gain_experience(100);
	    timeprint();
	    dataprint();
	    showflags();
	    morewait();
	    clearmsg();
	    if (Player.preference == 'n')
	      print1("You arise refreshed the next morning...");
	    else
	      print1("You arise, tired but happy, the next morning...");
	  }
	}
	else print2("What are you, some kinda prude?");
      }
    }
    else if (response == 'b') {
      if (nighttime()) {
	print1("As you fumble at the lock, the door opens....");
	print2("The bouncer tosses you into the street.");
      }
      else print1("The door appears to be bolted and barred from behind.");
    }
    else if (response == 'c') {
      if (nighttime()) {
	print1("As you charge toward the door it opens....");
	print2("Yaaaaah! Thud!");
	morewait();
	print1("You run past the startled bouncer into a wall.");
	p_damage(20,UNSTOPPABLE,"a move worthy of Clouseau");
	print2("The bouncer tosses you into the street.");
      }
      else {
	print1("Ouch! The door resists your efforts.");
	p_damage(1,UNSTOPPABLE,"a sturdy door");
	morewait();
	print1("You hear an irritated voice from inside:");
	print2("'Keep it down out there! Some of us are trying to sleep!'");
      }
    }
  }
}



#ifndef MSDOS_SUPPORTED_ANTIQUE
/* if signp is true, always print message, otherwise do so only sometimes */
void sign_print(x,y,signp)
int x,y,signp;
{
  if ((Level->site[x][y].p_locf >= CITYSITEBASE) &&
      (Level->site[x][y].p_locf < CITYSITEBASE+NUMCITYSITES))
    CitySiteList[Level->site[x][y].p_locf - CITYSITEBASE][0] = TRUE;
  switch(Level->site[x][y].p_locf) {
  case L_CHARITY:
    print1("You notice a sign: The Rampart Orphanage And Hospice For The Needy.");
    break;
  case L_MANSION:
    print1("You notice a sign:");
    print2("This edifice protected by DeathWatch Devices, Ltd.");
    morewait();
    break;
  case L_GRANARY:  
    print1("You notice a sign:");
    print2("Public Granary: Entrance Strictly Forbidden.");
    break;
  case L_PORTCULLIS:
    if (Level->site[x][y].locchar == FLOOR)
      print1("You see a groove in the floor and slots above you.");
    break;
  case L_STABLES:
    print1("You notice a sign:");
    print2("Village Stables");
    break;
  case L_COMMONS:
    print1("You notice a sign:");
    print2("Village Commons: No wolves allowed.");
    break;
  case L_MAZE:
    print1("You notice a sign:");
    print2("Hedge maze closed for trifid extermination.");
    break;
  case L_BANK:
    if (signp) {
      print1("You notice a sign:");
      print2("First Bank of Omega: Autoteller Carrel.");
    }
    break;
  case L_TEMPLE:
    print1("You see the ornate portico of the Rampart Pantheon");
    break;
  case L_ARMORER:
    if (signp) {
      print1("You notice a sign:");
      print2("Julie's Armor of Proof and Weapons of Quality");
    }
    break;
  case L_CLUB:
    if (signp) {
      print1("You notice a sign:");
      print2("Rampart Explorers' Club.");
    }
    break;
  case L_GYM:
    if (signp) {
      print1("You notice a sign:");
      print2("The Rampart Gymnasium, (affil. Rampart Coliseum).");
    }
    break;
  case L_HEALER:
    if (signp) {
      print1("You notice a sign:");
      print2("Rampart Healers. Member RMA.");
    }
    break;
  case L_CASINO:
    if (signp) {
      print1("You notice a sign:");
      print2("Rampart Mithril Nugget Casino.");
    }
    break;
  case L_SEWER:
    print1("A sewer entrance. You don't want to go down THERE, do you?");
    break;
  case L_COMMANDANT:
    if (signp) {
      print1("You notice a sign:");
      print2("Commandant Sonder's Rampart-fried Lyzzard partes. Open 24 hrs.");
    }
    break;
  case L_DINER:
    if (signp) {
      print1("You notice a sign:");
      print2("The Rampart Diner. All you can eat, 25Au.");
    }
    break;
  case L_CRAP:
    if (signp) {
      print1("You notice a sign:"); 
      print2("Les Crapeuleaux. (****)");
    }
    break;
  case L_TAVERN:
    if (signp) {
      print1("You notice a sign:"); 
      print2("The Centaur and Nymph -- J. Riley, prop.");
    }
    break;
  case L_ALCHEMIST:
    if (signp) {
      print1("You notice a sign:"); 
      print2("Ambrosias' Potions et cie.");
    }
    break;
  case L_DPW:
    if (signp) {
      print1("You notice a sign:"); 
      print2("Rampart Department of Public Works.");
    }
    break;
  case L_LIBRARY:
    if (signp) {
      print1("You notice a sign:"); 
      print2("Rampart Public Library.");
    }
    break;
  case L_CONDO:
    if (signp) {
      print1("You notice a sign:"); 
      if (gamestatusp(SOLD_CONDO)) 
	print2("Home Sweet Home");
      else print2("Luxury Condominium For Sale; Inquire Within");
    }
    break;
  case L_PAWN_SHOP:
    if (signp) {
      print1("You notice a sign:"); 
      print2("Knight's Pawn Shop.");
    }
    break;
  case L_CEMETARY:
    print1("You notice a sign:"); 
    print2("Rampart City Cemetary. Closed -- Full.");
    break;
  case L_GARDEN:
    print1("You notice a sign:"); 
    print2("Rampart Botanical Gardens---Do not deface statues.");
    break;
  case L_JAIL:
    print1("You notice a sign:"); 
    print2("Rampart City Gaol -- always room for more.");
    break;
  case L_ORACLE:
    print1("You notice a sign:");
    print2("The Oracle of the Cyan Flames");
    morewait();
    break;
  }
}
#endif
  


void l_countryside()
{
  if (optionp(CONFIRM)) {
    clearmsg();
    print1("Do you really want to return to the countryside? ");
    if (ynq1() != 'y')
      return;
  }
  change_environment(E_COUNTRYSIDE);
}



void l_oracle()
{
  char response;
  if (gamestatusp(ATTACKED_ORACLE) && (! gamestatusp(COMPLETED_ASTRAL))) {
    print1("You come before a blue crystal dais. You see a broken mirror.");
    print2("Look in the mirror? [yn] ");
    if (ynq2()=='y') {
      print1("A strange force rips you from your place....");
      Player.hp = 1;
      print2("You feel drained....");
      dataprint();
      print3("You find yourself in a weird flickery maze.");
      change_environment(E_ASTRAL);
    }
  }
  else {
    print1("You come before a blue crystal dais. There is a bell and a mirror.");
    print2("Ring the bell [b], look in the mirror [m], or leave [ESCAPE] ");
    do response = (char) mcigetc();
    while ((response != 'b') && (response != 'm') && (response != ESCAPE));
    if (response == 'b') {
      print1("The ringing note seems to last forever.");
      print2("You notice a robed figure in front of you....");
      morewait();
      print1("The oracle doffs her cowl. Her eyes glitter with blue fire!");
      print2("Attack her? [yn] ");
      if (ynq2() == 'y') {
	setgamestatus(ATTACKED_ORACLE);
	print1("The oracle deftly avoids your attack.");
	print2("She sneers at you and vanishes.");
      }
      else {
	print2("She stares at you...and speaks:");
	if (!gamestatusp(SPOKE_TO_DRUID)) {
	  print3("'The ArchDruid speaks wisdom in his forest shrine.'");
	}
	else if (!gamestatusp(COMPLETED_CAVES)) {
	  print3("'Thou mayest find aught of interest in the caves to the South.'");
	}
	else if (!gamestatusp(COMPLETED_SEWERS)) {
	  print3("'Turn thy attention to the abyssal depths of the city.'");
	}
	else if (!gamestatusp(COMPLETED_CASTLE)) {
	  print3("'Explorest thou the depths of the Castle of the ArchMage.'");
	}
	else if (!gamestatusp(COMPLETED_ASTRAL)) {
	  morewait();
	  print1("'Journey to the Astral Plane and meet the Gods' servants.'");
	  print2("The oracle holds out her hand. Do you take it? [yn] ");
	  if (ynq2()=='y') {
	    print1("'Beware: Only the Star Gem can escape the Astral Plane.'");
	    print2("A magic portal opens behind the oracle. She leads you");
	    morewait();
	    print1("through a sequence of special effects that would have");
	    print2("IL&M technicians cursing in awe and deposits you in an");
	    morewait();
	    clearmsg();
	    print1("odd looking room whose walls seem strangely insubstantial.");
	    gain_experience(5000);
	    change_environment(E_ASTRAL);
	  }
	  else print3("You detect the hint of a sneer from the oracle.");
	}
	else if (!gamestatusp(COMPLETED_VOLCANO)) {
	  print3("'The infernal maw may yield its secrets to thee now.'");
	}
	else if (!gamestatusp(COMPLETED_CHALLENGE)) {
	  print3("'The challenge of adepthood yet awaits thee.'");
	}
	else {
	  morewait();
	  print1("'My lord: Thou hast surpassed my tutelage forever.");
	  print2("Fare thee well.'");
	  print3("The oracle replaces her hood and seems to fade away....");
	}
      }
    }
    else if (response == 'm') {
      print1("You seem to see yourself. Odd....");
      knowledge(1);
    }
    else print2("You leave this immanent place.");
  }
}

void l_mansion()
{
  print1("Enter the mansion? [yn] ");
  if (ynq1()=='y')
    change_environment(E_MANSION);
}

void l_house()
{
  print1("Enter the house? [yn] ");
  if (ynq1()=='y')
    change_environment(E_HOUSE);
}

void l_hovel()
{
  print1("Enter the hovel? [yn] ");
  if (ynq1()=='y')
    change_environment(E_HOVEL);
}

void l_safe()
{
  char response;
  pob newitem;
  int attempt = 0;
  print1("You have discovered a safe!");
  print2("Pick the lock [p], Force the door [f], or ignore [ESCAPE]");
  do response = (char) mcigetc();
  while ((response != 'p') && (response != 'f') && (response != ESCAPE));
  if (response == 'p')
    attempt = (2*Player.dex + Player.rank[THIEVES]*10 - random_range(100))/10;
  else if (response == 'f')
    attempt = (Player.dmg - random_range(100))/10;
  if (attempt > 0) {
    Player.alignment -= 4;
    gain_experience(50);
    print2("The door springs open!");
    Level->site[Player.x][Player.y].locchar = FLOOR;
    Level->site[Player.x][Player.y].p_locf = L_NO_OP;
    lset(Player.x, Player.y, CHANGED);
    if (random_range(2) == 1) {
      print1("You find:");
      do {
	newitem = create_object(difficulty());
	print2(itemid(newitem));
	morewait();
	gain_item(newitem);
      } while (random_range(3) == 1);
    }
    else print2("The safe was empty (awwwww....)");
  }
  else {
    print3("Your attempt at burglary failed!");
    if (attempt == -1) {
      print1("A siren goes off! You see flashing red lights everywhere!");
      morewait();
      if (Last_Environment == E_CITY) {
	print2("The city guard shows up! They collar you in no time flat!");
	change_environment(E_CITY);
	morewait();
	send_to_jail();
      }
    }
    else if (attempt == -2) {
      print1("There is a sudden flash!");
      p_damage(random_range(25),FLAME,"a safe");
      print2("The safe has self-destructed.");
      Level->site[Player.x][Player.y].locchar = RUBBLE;
      Level->site[Player.x][Player.y].p_locf = L_RUBBLE;
      lset(Player.x, Player.y, CHANGED);
    }
    else if (attempt == -3) {
      print1("The safe jolts you with electricity!");
      lball(Player.x,Player.y,Player.x,Player.y,30);
    }
    else if (attempt < -3) {
      print1("You are hit by an acid spray!");
      if (Player.possessions[O_CLOAK] != NULL) {
	print2("Your cloak is destroyed!");
	conform_lost_object(Player.possessions[O_CLOAK]);
	p_damage(10,ACID,"a safe");
      }
      else if (Player.possessions[O_ARMOR] != NULL) {
	print2("Your armor corrodes!");
	Player.possessions[O_ARMOR]->dmg-=3;
	Player.possessions[O_ARMOR]->hit-=3;
	Player.possessions[O_ARMOR]->aux-=3;
	p_damage(10,ACID,"a safe");
      }
      else {
	print2("The acid hits your bare flesh!");
	p_damage(random_range(100),ACID,"a safe");
      }
    }
  }
}


void l_cartographer()
{
  int i,j,x,y;
  print1("Ye Olde Mappe Shoppe.");
  print2("Map of the local area: 500Au. Buy it? [yn] ");
  if (ynq2()=='y') {
    if (Player.cash < 500) 
      print3("Cursed be cheapskates! May you never find an aid station....");
    else {
      print3("You now have the local area mapped.");
      Player.cash -= 500;
      dataprint();
      switch(Villagenum) {
      case 1: x = 56; y = 5; break;
      default: case 2: x = 35; y = 11; break;
      case 3: x = 10; y = 40; break;
      case 4: x = 7; y = 6; break;
      case 5: x = 40; y = 43; break;
      case 6: x = 20; y = 41; break;
      }	
      for(i=x-15;i<=x+15;i++)
	for(j=y-15;j<=y+15;j++)
	  if ((i>=0)&&(i<64)&&(j>=0)&&(j<64)) {
	    if (Country[i][j].current_terrain_type !=
	      Country[i][j].base_terrain_type) {
	      c_set(i, j, CHANGED);
	      Country[i][j].current_terrain_type =
		Country[i][j].base_terrain_type;
	    }
	    c_set(i, j, SEEN);
	  }
    }
  }
  else print3("Don't blame me if you get lost....");
}
	    
void l_charity()
{
  long donation;
  print2("'Greetings, friend. Do you wish to make a donation?' [yn] ");
  if (ynq2()!='y') 
    print3("'Pinchpurse!'");
  else {
    clearmsg();
    print1("How much can you give? ");
    donation = parsenum();
    if (donation < 1)
      print2("'Go stick your head in a pig.'");
    else if (donation > Player.cash)
      print2("'I'm afraid you're charity is bigger than your purse!'");
    else if (donation < max(100,Player.level*Player.level*100)) {
      print2("'Oh, can't you do better than that?'");
      print3("'Well, I guess we'll take it....'");
      if (Player.alignment < 10) Player.alignment++;
      Player.cash -= donation;
    }
    else {
      print2("'Oh thank you kindly, friend, and bless you!'");
      Player.cash -= donation;
      Player.alignment += 5;
    }
  }
  dataprint();
} 

