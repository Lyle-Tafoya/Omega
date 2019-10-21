/* omega copyright (C) by Laurence Raphael Brothers, 1987 */
/* osite.c */
/* L_ functions  */ 

#include <strings.h>
#include "oglob.h"

/* from ofile */
extern void theologyfile(),cityguidefile();

/* from ospell */
extern char *spellid();

/* from ocom */
extern void levelchange();

/* from oinv */
extern char *itemid();
extern int inpack(),get_money();
extern void conform_lost_objects();
/* from odepths.c */
extern void final_duel();

/* from oetc */
extern void hint(),learnclericalspells(),nameprint();
extern char *slotstr();

/* from omon */
extern int corpse_value();
extern void transform_corpse(),m_status_set();

/* from oaux */
extern void conform_lost_object(),conform_unused_object(),p_death();
extern void p_damage(),p_poison(),calc_melee(),gain_experience();
extern void add_item_to_pack(),p_drown(),p_teleport(),p_win(),foodcheck();
extern void toggle_item_use();
extern int item_value(),true_item_value();

/* from oguild */
extern void l_altar();


/* from oeffect (1 or 2)*/
extern void disintegrate(),identify(),level_drain(),augment(),cleanse();
extern void heal(),acquire(),enchant(),bless(),learnspell(),dispel();

/* from outil */
extern int random_range(),hour();

/* from oscr */
extern void mprint(),menuclear(),menuprint(),xredraw(),printm(),showflags();
extern void menu_item_print(),mnumprint(),dataprint(),show_screen();
extern void morewait(),drawvision(),drawomega();
extern char ynq(),mgetc(),*msgscanstring(),mcigetc(),menugetc();
extern void screencheck();

/* from oitem */
extern pob create_object();

/* from olev */
pmt m_create();



/* site p_locf functions */

void l_temple(),l_armorer(),l_club(),l_gym();
void l_healer(),l_statue_wake(),l_casino(),l_bank();
void l_sewer(),l_tavern(),l_alchemist(),l_vault();
void l_citygate(),l_dpw(),l_library(),l_pawn_shop(),l_chaosstorm();
void l_condo(),l_cemetary(),l_residence(),l_garden();
void l_jail(),l_adept(),l_trifid(),l_final_abyss(),l_warning();
void l_portcullis(),l_portcullis_trap(),l_guardian(),l_challenge();

/* auxiliary functions */
void healforpay(),cureforpay(),gymtrain(),buyfromstock(),wake_statue();
void send_to_jail();




/* the bank; can be broken into (!) */
void l_bank()
{
  int done=FALSE,valid=FALSE,amount;
  char response;
  char passwd[64];

  if (Bank_Broken)
    mprint("You see a damaged autoteller.");
  else {
    mprint("First Omegan Bank -- Autoteller Working.");
    while (! done) {
      printm("\nCurrent Balance: ");
      mnumprint(Balance);
      mprint(" Au. ");
      menuclear();
      menuprint("Enter command > ");
      response = menugetc();
      if (response == '?') {
	menuclear();
	menuprint("?: This List.\n");
	if (strcmp(Password,"")==0)
	  menuprint("O: Open an account.\n");
	else {
	  menuprint("P: Enter password.\n");
	  menuprint("D: Deposit.\n");
	  menuprint("W: Withdraw\n");
	}
	menuprint("X: eXit\n");
	morewait();
      }
      else if ((response == 'P') && (strcmp(Password,"") != 0)) {
	menuclear();
	menuprint("Enter password above: \n");
	printm("\nPassword: ");
	strcpy(passwd,msgscanstring());
	valid = (strcmp(passwd,Password)==0);
	if (! valid) {
	  done = TRUE;
	  menuclear();
	  menuprint("Alert! Alert! Invalid Password!\n");
	  menuprint("The police are being summoned!\n");
	  menuprint("Please wait for the police to arrive....\n\n");
	  menuprint("----Hit space bar to continue----\n");
	  response = menugetc();
	  if (response == SPACE) {
	    xredraw();
	    mprint("Ah ha! Trying to rob the bank, eh?");
	    mprint("Take him away, boys!");
	    send_to_jail();
	  }
	  else {
	    menuclear();
	    sleep(3);
	    menuprint("^@^@^@^@^@00AD1203BC0F0000FFFFFFFFFFFF\n");
	    menuprint("Interrupt in _get_space. Illegal Character.\n");
	    sleep(3);
	    menuprint("Aborting _police_alert.....\n");
	    menuprint("Attempting reboot.....\n");
	    sleep(3);
	    menuprint("Warning: Illegal shmop at _count_cash.\n");
	    menuprint("Warning: Command Buffer NOT CLEARED\n");
	    sleep(3);
	    menuprint("Reboot Complete. Execution Continuing.\n");
	    menuprint("Withdrawing: 4294967297 Au.\n");
	    menuprint("Warning: Arithmetic Overflow in _withdraw\n");
	    sleep(3);
	    menuprint("Yo mama. Core dumped.\n");
	    sleep(3);
	    xredraw();
	    mprint("The cash machine begins to spew gold pieces!");
	    mprint("You pick up your entire balance and then some!");
	    Player.cash += Balance + random_range(3000);
	    Balance = 0;
	    Bank_Broken = TRUE;
	  }
	}
	else menuprint("Password accepted. Working.\n");
      }
      else if ((response == 'D') && valid) {
	menuclear();
	menuprint("Enter amount to deposit above.\n");
	printm("\nAmount: ");
	amount = get_money(Player.cash);
	if (amount < 1) menuprint("Transaction aborted.\n");
	else {
	  menuprint("Transaction accomplished.\n");
	  Balance += amount;
	  Player.cash -= amount;
	}
      }
      else if ((response == 'W') && valid) {
	menuclear();
	menuprint("Enter amount to withdraw above.\n");
	printm("\nAmount: ");
	amount = get_money(Balance);
	if (amount < 1) menuprint("Transaction aborted.\n");
	else if (amount > Balance) 
	  menuprint("Withdrawal too large -- aborted.\n");
	else {
	  menuprint("Transaction accomplished.\n");
	  Balance -= amount;
	  Player.cash += amount;
	}
      }
      else if (response == 'X') {
	menuclear();
	menuprint("\n\nBye!\n\n");
	done = TRUE;
      }
      else if ((response == 'O') && (strcmp(Password,"")==0)) {
	menuclear();
	menuprint("Opening new account.\n");
	menuprint("Please enter password above.\n");
	printm("\nPassword: ");
	strcpy(Password,msgscanstring());
	if (strcmp(Password,"")==0) {
	  menuprint("Illegal to use null password -- aborted.\n");
	  done = TRUE;
	}
	else {
	  menuprint("Password validated; account saved.\n");
	  valid = TRUE;
	}
      }
      else menuprint(" Illegal command.\n");
      dataprint();
      morewait();
    }
  }
  xredraw();
}



/* The druid's altar in the botanical garden */
void l_druid()
{
  char response;
  mprint("You have entered the Inner Sanctum of the ArchDruid.");
  if (! Sawdruid) {
    mprint("The Archdruid congratulates you on reaching his sanctum.");
    mprint("You feel competent.");
    gain_experience(250);
    Sawdruid = TRUE;
  }
  menuclear();
  menuprint("\na: Continue to the altar or leave.");
  menuprint("\nb: Request a cleansing rite.");
  response = mgetc();
  if (response == 'a') {
    mprint("You may proceed");
    l_altar();
  }
  else if (response == 'b') {
    if (abs(Player.alignment > 3))
      mprint("You are not in tune with the Cosmic Balance!");
    else {
      mprint("The ArchDruid conducts a sacred rite of cleansing....");
      cleanse(0);
      mprint("The ArchDruid sends you on your way with a teleport spell.");
      p_teleport(1);
    }
  }
  else {
    mprint("You must not come this way without a serious purpose. Begone!");
    p_teleport(0);
  }
  xredraw();
}


void l_temple()
{
  mprint("You pass the ornate portico of the Rampart Pantheon.");
}

void l_armorer()
{
  int done = FALSE;
  char action;

  mprint("You have entered Julie's: 'Armorer Par Excellence'");
  if (hour() == 12) 
    mprint("Unfortunately, this is her lunch hour -- try again later.");
  else if (nighttime()) 
    mprint("It seems that Julie keeps regular business hours.");
  else while (! done) {
    printm("\nJulie's: Buy Armor, Weapons, or Leave [a,w,ESCAPE] ");
    action = mgetc();
    if (action == ESCAPE) 
      done = TRUE;
    else if (action == 'a') 
      buyfromstock(ARMORID,10);
    else if (action == 'w') 
      buyfromstock(WEAPONID,23);
  }
  xredraw();
}


void buyfromstock(base,numitems)
int base,numitems;  
{
  int i,j;
  char item;
  pob newitem;

  printm("\nPurchase which item? [ESCAPE to quit] ");
  menuclear();
  for(i=0;i<numitems;i++) 
    menu_item_print(i,&(Objects[base+i]));
  item = ' ';
  while ((item != ESCAPE) &&
	 ((item < 'a') || (item >= 'a'+numitems)))
    item = mgetc();
  if (item != ESCAPE) {
    i = item - 'a';
    newitem = ((pob) calloc(1,sizeof(objtype)));
    *newitem = Objects[base+i];
    newitem->known = 2;
    printm("\nI can let you have it for ");
    mnumprint(2*true_item_value(newitem));
    printm(" in gold.");
    mprint(" Buy it? [ynq] ");
    if (ynq() == 'y') {
      if (Player.cash < 2*true_item_value(newitem)) {
	mprint("Why not try again some time you have the cash?");
	free((char *) newitem);
      }
      else {
	Player.cash -= 2*true_item_value(newitem);
	dataprint();
	add_item_to_pack(newitem);
      }
    }
    else free((char *)newitem);
  }
}


void l_club()
{
  static int hinthour = 0;
  char response;

  mprint("Rampart Explorers' Club.");
  if (! Clubmember) {
    if (Player.level < 2) mprint("Only reknowned adventurers need apply.");
    else {
      mprint("Dues are 100Au. Pay it? [yn] ");
      if (ynq()=='y') {
	if (Player.cash < 100)
	  mprint("Beat it, or we'll blackball you!");
	else {
	  mprint("Welcome to the club!");
	  mprint("You are taught the spell of Return");
	  Spells[S_RETURN].known = TRUE;
	  Player.cash -= 100;
	  Clubmember = TRUE;
	}
      }
      else mprint("OK, but you're missing out on our benefits....");
    }
  }
  else {
    mprint("Shop at the club store or listen for rumors [sl] ");
    do response = mgetc(); 
    while ((response != 's') && (response != 'l') && (response != ESCAPE));
    if (response == 'l') {
      if (hinthour == hour()) mprint("You don't hear anything useful.");
      else {
	mprint("You overhear a conversation....");
	hint();
	hinthour = hour();
      }
    }
    else if (response == 's') {
      buyfromstock(THINGID+7,2);
      xredraw();
    }
    else if (response == ESCAPE)
      mprint("Be seeing you, old chap!");
  }
}

void l_gym()
{
  int done=TRUE;
  mprint("The Rampart Gymnasium, (affil. Rampart Coliseum).");
  if (Gymcredit<0) {
    mprint("You must pay us: ");
    mnumprint(abs(Gymcredit));
    mprint("before you may train here again.");
    mprint("Pay it? [yn]");
    if (ynq()=='y') {
      if (Player.cash < abs(Gymcredit))
	mprint("Sorry, you don't have it!");
      else {
	Player.cash += Gymcredit;
	Gymcredit = 0;
	dataprint();
      }
    }
    else mprint("See you around, deadbeat.");
  }
  do {
    if (Gymcredit>0) {
      mprint("You currently have: ");
      mnumprint(Gymcredit);
      mprint("Au credit here.");
    }
    if (Gymcredit > -1) {
      done = FALSE;
      menuclear();
      menuprint("Train for 2000 crowns");
      menuprint("\n\nChoose:");
      menuprint("\na: work out in the weight room");
      menuprint("\nb: use our gymnastics equipment");
      menuprint("\nc: take our new anaerobics course");
      menuprint("\nd: enroll in dance lessons.");
      menuprint("\nESCAPE: Leave this place.");
      switch(mgetc()) {
      case 'a': 
	gymtrain(&(Player.maxstr),&(Player.str));
	break;
      case 'b': 
	gymtrain(&(Player.maxdex),&(Player.dex));
	break;
      case 'c': 
	gymtrain(&(Player.maxcon),&(Player.con));
	break;
      case 'd':
	gymtrain(&(Player.maxagi),&(Player.agi));
	break;
      case ESCAPE:
	mprint("Well, it's your body you're depriving!"); 
	done = TRUE;
	break;
      }
    }
  } while (! done);
  xredraw();
  calc_melee();
}



void l_healer()
{
  mprint("Rampart Healers. Member RMA.");
  menuclear();
  menuprint("Choose:");
  menuprint("\na: Heal injuries (50 crowns)");
  menuprint("\nb: Cure disease (250 crowns)");
  menuprint("\nESCAPE: Leave this place.");
  switch(mgetc()) {
    case 'a': healforpay(); break;
    case 'b': cureforpay(); break;
    default: mprint("OK, but suppose you have Acute Satyriasis?"); break;
  }	      
  xredraw();
}

void l_statue_wake()
{
  int i;
  int x=Player.x,y=Player.y;
  for(i=0;i<9;i++)
    wake_statue(x+Dirs[0][i],y+Dirs[1][i],TRUE);
}

void wake_statue(x,y,first)
int x,y,first;
{
  int i;
  pml tml;
  if (Dungeon[Dlevel][x][y].locchar == STATUE) {
    if (! first) mprint("Another statue awakens!");
    else mprint("The statue springs to life!");
    Dungeon[Dlevel][x][y].locchar = FLOOR;
    tml = ((pml) calloc(1,sizeof(mltype)));
    tml->m =
      (Dungeon[Dlevel][x][y].creature = m_create(x,y,0,Dlevel+3));
    tml->next = Mlist[Dlevel];
    Mlist[Dlevel] = tml;
    for(i=0;i<8;i++) wake_statue(x+Dirs[0][i],y+Dirs[1][i],FALSE);
  }
}


void l_casino()
{
  int i,done = FALSE,a,b,c,match;
  static int winnings = 0;
  char response;
  mprint("Rampart Mithril Nugget Casino.");
  if (random_range(10)==1)
    mprint("Closed due to Grand Jury investigation.");
  else {
    menuclear();
    menuprint("a: Drop 100Au in the slots.\n");
    menuprint("b: Risk 1000Au  at roulette.\n");
    menuprint("ESCAPE: Leave this green baize hall.\n");
    while (! done) {
      response = mgetc();
      if (response == 'a') {
	if (Player.cash < 100) mprint("No credit, jerk.");
	else {
	  Player.cash -= 100;
	  dataprint();
	  for(i=0;i<20;i++) {
	    if (i==19) sleep(1);
	    a = random_range(10);
	    b = random_range(10);
	    c = random_range(10);
	    printm("\n");
	    printm(slotstr(a));
	    printm(slotstr(b));
	    printm(slotstr(c));
	  }
	  if (winnings > 0) do {
	    a = random_range(10);
	    b = random_range(10);
	    c = random_range(10);
	  } while ((a==b) || (a == c) || (b == c));
	  else {
	    a = random_range(10);
	    b = random_range(10);
	    c = random_range(10);
	  }
	  printm("\n");
	  printm(slotstr(a));
	  printm(slotstr(b));
	  printm(slotstr(c));
	  if ((a==b) && (a==c)) {
	    mprint("Jackpot Winner!");
	    winnings += (a+2)*(b+2)*(c+2)*5;
	    Player.cash += (a+2)*(b+2)*(c+2)*5;
	    dataprint();
	  }
	  else if (a==b) {
	    mprint("Winner!");
	    Player.cash += (a+2)*(b+2)*5;
	    dataprint();
	    winnings += (a+2)*(b+2)*5;
	  }
	  else if (a==c) {
	    mprint("Winner!");
	    Player.cash += (a+2)*(c+2)*5;
	    dataprint();
	    winnings += (a+2)*(c+2)*5;
	  }
	  else if (c==b) {
	    mprint("Winner!");
	    Player.cash += (c+2)*(b+2)*5;
	    dataprint();
	    winnings += (c+2)*(b+2)*5;
	  }
	  else {
	    mprint("Loser!");
	    winnings -= 100;
	  }
	}
      }
      else if (response == 'b') {
	if (Player.cash < 1000) mprint("No credit, jerk.");
	else {
	  Player.cash -= 1000;
	  dataprint();
	  mprint("Red or Black? [rb]");
	  do response = mgetc();
	  while ((response != 'r') && (response != 'b'));
	  match = (response == 'r' ? 0 : 1);
	  for(i=0;i<20;i++) {
	    if (i==19) sleep(1);
	    a = random_range(37);
	    b = a % 2;
	    printm("\n");
	    if (a == 0) printm(" 0 ");
	    else if (a==1) printm(" 0 - 0 ");
	    else {
	      printm(( b == 0) ? "Red ": "Black ");
	      mnumprint(a-1);
	    }
	  }
	  if (winnings > 0) do {
	    a = random_range(37);
	    b = a % 2;
	  } while (b == match);
	  else {
	    a = random_range(37);
	    b = a % 2;
	  }
	  printm("\n");
	  if (a == 0) printm(" 0 ");
	  else if (a==1) printm(" 0 - 0 ");
	  else {
	    printm((b == 0) ? "Red ": "Black ");
	    mnumprint(a-1);
	  }
	  if ((a > 1) && (b == match)){
	    mprint(" Winner!");
	    winnings += 1000;
	    Player.cash += 2000;
	    dataprint();
	  }
	  else {
	    mprint(" Loser!");
	    winnings -= 1000;
	    dataprint();
	  }
	}
      }
      else if (response == ESCAPE) done = TRUE;
    }
    xredraw();
  }
}



void l_sewer()
{
  mprint("A sewer entrance. You don't want to go down THERE, do you?");
}

void l_tavern()
{
  static int type;
  static int hinthour;
  char response;
  pob food;
  if ((Dungeon[Dlevel][Player.x][Player.y].aux < 1) ||
      (Dungeon[Dlevel][Player.x][Player.y].aux > 4)) {
    Dungeon[Dlevel][Player.x][Player.y].aux = (type % 4) + 1;
    type++;
  }
  switch(Dungeon[Dlevel][Player.x][Player.y].aux) {
  case 1:
    mprint("Commandant Sonder's Rampart-fried Lyzzard partes. Open 24 hrs.");
    mprint("Buy a bucket! Only 5 Au. Make a purchase? [yn]");
    if (ynq()=='y') {
      if (Player.cash < 5) 
	mprint("No handouts here, mac!");
      else {
	Player.cash -= 5;
	food = ((pob) calloc(1,sizeof(objtype)));
	*food = Objects[FOODID+0]; /* food ration */
	add_item_to_pack(food);
	mprint("There you go, mac! One Lyzzard Bucket, coming up.");
      }
    }
    else mprint("Don't blame the Commandant if you starve!");
    break;
  case 2:
    mprint("The Rampart Diner. All you can eat, 25Au.");
    mprint("Place an order? [yn]");
    if (ynq()=='y') {
      if (Player.cash < 25)
	mprint("TANSTAAFL! Now git!");
      else {
	Player.cash -= 25;
	dataprint();
	Player.food = 24;
	foodcheck();
      }
    }
    break;
  case 3:
    mprint("Les Crapeuleaux. (****)");
    if (nighttime()) mprint ("So sorry, we are closed 'til the morrow...");
    else {
      mprint("May I take your order? [yn]");
      if (ynq()=='y') {
	if (Player.cash < 1000)
	  mprint("So sorry, you have not the funds for dinner.");
	else {
	  mprint("Hope you enjoyed your tres expensive meal, m'sieur...");
	  Player.cash -= 1000;
	  dataprint();
	  Player.food += 6;
	  foodcheck();
	}
      }
    }
    break;
  case 4:
    mprint("The Centaur and Nymph -- J. Riley, prop.");
    if (nighttime()) {
      menuclear();
      menuprint("Riley says: Whataya have?\n\n");
      menuprint("a: Pint of Riley's ultra-dark 1Au\n");
      menuprint("b: Shot of Tullimore Dew 10Au\n");
      menuprint("c: Round for the House. 100Au\n");
      menuprint("d: Bed and Breakfast. 25Au\n");
      menuprint("ESCAPE: Leave this comfortable haven.\n");
      do response = mcigetc();
      while ((response != 'a') &&
	     (response != 'b') &&
	     (response != 'c') &&
	     (response != 'd') &&
	     (response != ESCAPE));
      switch (response) {
      case 'a':
	if (Player.cash < 1)
	  mprint("Aw hell, have one on me.");
	else {
	  hinthour == hour();
	  Player.cash -= 1;
	  dataprint();
	  if (hinthour!=hour()) {
	    if (random_range(3)) {
	      mprint("You overhear a rumor...");
	      hint();
	    }
	    else mprint("You don't hear much of interest.");
	    hinthour = hour();
	  }
	  else mprint("You just hear the same conversations again.");
	}
	break;
      case 'b':
	if (Player.cash < 10)
	  mprint("I don't serve the Dew on no tab, buddy!");
	else {
	  Player.cash -= 10;
	  mprint("Ahhhhh....");
	  if (Player.status[POISONED] || Player.status[DISEASED])
	    mprint("Phew! That's, er, smooth stuff!");
	  Player.status[POISONED] = 0;
	  Player.status[DISEASED] = 0;
	  showflags();
	}
	break;
      case 'c':
	if (Player.cash < 100) {
	  mprint("Whatta feeb!");
	  mprint("Outta my establishment.... Now!");
	  p_damage(random_range(20),UNSTOPPABLE,"Riley's right cross");
	}
	else {
	  Player.cash -= 100;
	  dataprint();
	  mprint("'What a guy!'"); morewait();
	  mprint("'Hey, thanks, fella.'"); morewait();
	  mprint("'Make mine a double...'"); morewait();
	  switch(random_range(4)) {
	  case 0:
	    mprint("'You are a real pal. Say, have you heard.... ");
	    hint();
	    break;
	  case 1:
	    mprint("A wandering priest of Dionysus blesses you...");
	    if ((Player.patron == ODIN) || (Player.patron == ATHENA))
	      Player.alignment++;
	    else if ((Player.patron == HECATE) || (Player.patron == SET))
	      Player.alignment--;
	    else if (Player.alignment > 0) Player.alignment--;
	    else Player.alignment++;
	    break;
	  case 2:
	    mprint("A thirsty bard promises to put your name in a song!");
	    gain_experience(20);
	    break;
	  case 3:
	    mprint("Riley draws you a shot of his 'special reserve'");
	    mprint("Drink it [yn]?");
	    if (ynq()=='y') {
	      if (Player.con < random_range(20)) {
		mprint("<cough> Quite a kick!");
		mprint("You feel a fiery warmth in your tummy....");
		Player.con++;
		Player.maxcon++;
	      }
	      else mprint("You toss it back nonchalantly.");
	    }
	  }		
	}
	break;
      case 'd':
	if (Player.cash < 25)
	  mprint("Pay in advance, mac!");
	else {
	  Player.cash -= 25;
	  mprint("How about a shot o' the dew for a nightcap?");
	  if (hour() > 12) Time += ((24-hour())+8) * 100;
	  else Time += ((9-hour())*100);
	  Player.status[POISONED] = 0;
	  Player.status[DISEASED] = 0;
	  Player.food = 18;
	  /* reduce temporary stat gains to max stat levels */
	  toggle_item_use(TRUE);
	  Player.str = min(Player.str,Player.maxstr);
	  Player.con = min(Player.con,Player.maxcon);
	  Player.agi = min(Player.agi,Player.maxagi);
	  Player.dex = min(Player.dex,Player.maxdex);
	  Player.iq = min(Player.iq,Player.maxiq);
	  Player.pow = min(Player.pow,Player.maxpow);
	  toggle_item_use(FALSE);
	  timeprint();
	  dataprint();
	  showflags();
	  mprint("The next morning.....");
	}
	break;
      default:
	mprint("So? Just looking? Go on!");
	break;
      }
    }
    else mprint("The pub don't open til dark, fella.");
    xredraw();
    break;
  }
}

void l_alchemist()
{
  int i,done=FALSE,mlevel;
  char response;
  pob obj;
  mprint("Ambrosias' Potions et cie.");
  if (nighttime()) 
    mprint("Ambrosias doesn't seem to be in right now.");
  else while (! done){
    menuclear();
    menuprint("a: Sell monster components\n");
    menuprint("b: Pay for transformation.\n");
    menuprint("ESCAPE: Leave this place.\n");
    response = mgetc();
    if (response == 'a') {
      done = TRUE;
      i = getitem(CORPSE);
      if ((i != ABORT) && (Player.possessions[i-'a'] != NULL)){
	obj = Player.possessions[i-'a'];
	if (Monsters[obj->aux].transformid == -1) {
	  mprint("I don't want such a thing.");
	  if (obj->basevalue > 0)
	    mprint("You might be able to sell it to someone else, though.");
	}
	else {
	  printm("\nI'll give you ");
	  mnumprint(obj->basevalue/3);
	  printm("Au for it. Take it? [ynq]");
	  if (ynq()=='y') {
	    Player.cash += (obj->basevalue/3);
	    conform_lost_objects(1,obj);
	  }
	  else mprint("Well, keep the smelly old thing, then!");
	}
      }
      else mprint("So nu?");
    }
    else if (response == 'b') {
      done = TRUE;
      i = getitem(CORPSE);
      if ((i != ABORT) && (Player.possessions[i-'a'] != NULL)){
	obj = Player.possessions[i-'a'];
	mlevel = Monsters[obj->aux].level;
	printm("\nIt'll cost you ");
 	mnumprint(max(10,obj->basevalue*2));
	printm("for the transformation. Pay it? [ynq]");
	if (ynq()=='y') {
 	  if (Player.cash < max(10,obj->basevalue*2))
	    mprint("You can't afford it!");
	  else if (Monsters[obj->aux].transformid == -1) {
 	      mprint("There is a puff of smoke, and the corpse disappears.");
 	      conform_lost_objects(1,obj);
 	    }
	  else {
	    mprint("Voila! A tap of the Philosopher's Stone...");
	    Player.cash -= obj->basevalue*2;
	    *obj = Objects[Monsters[obj->aux].transformid];
	    if ((obj->id >= STICKID) && (obj->id < STICKID+NUMSTICKS))
	      obj->charge = 20;
	    if (obj->plus == 0) obj->plus = mlevel;
	  }
	}
	else mprint("I don't need your business, anyhow.");
      }
      else mprint("So nu?");
    }
    else if (response == ESCAPE) done = TRUE;
  }
  xredraw();
}

void l_citygate()
{
  mprint("The huge gates are closed.");
  mprint("you hear the howl of a chaos storm outside.");
}

void l_chaosstorm()
{
  mprint("You are instantly reduced to a state of maximal entropy.");
  p_death("a chaos storm");
}
  


void l_dpw()
{
  static int lastday = -10;
  mprint("Rampart Department of Public Works.");
  if (Date - lastday < 7)
    printm("\nG'wan! Get a job!");
  else if (Player.cash < 100) {
    mprint("Do you want to go on the dole? [yn]");
    if (ynq()=='y') {
      mprint("Well, ok, but spend it wisely.");
      mprint("Please enter your name for our records.");
      if (strcmp(Player.name,msgscanstring()) != 0) {
	mprint("Aha! Welfare Fraud! It's off to gaol for you, lout!");
	send_to_jail();
      }
      else {
	mprint("Here's your handout, layabout!");
	lastday = Date;
	Player.cash = 99;
	dataprint();
      }
    }
  }
  else mprint("You're too well off for us to help you!");
}

void l_library()
{
  char response;
  int fee = 2000;
  mprint("Rampart Public Library.");
  if (nighttime())
    mprint("CLOSED");
  else {
    morewait();
    printm("\nLibrary Research Fee: 2000Au.");
    if (Player.maxiq < 18) {
      mprint("The Rampart student aid system has arranged a grant!");
      mprint("Your revised fee is:");
      mnumprint(fee=max(100,2000-(18-Player.maxiq)*250));
      mprint("Au.");
    }
    mprint("Pay it? [yn]");
    if (ynq()=='y') {
      if (Player.cash < fee) 
	mprint("No payee, No studee.");
      else {
	Player.cash -= fee;
	menuclear();
	menuprint("Peruse a scroll:\n");
	menuprint("a: Omegan Theology\n");
	menuprint("b: Guide to Rampart\n");
	menuprint("c: Attempt Advanced Research\n");
	do response = mgetc();
	while ((response < 'a') || (response > 'c'));
	if (response == 'a') {
	  mprint("You unfurl an ancient, yellowing scroll...");
	  morewait();
	  theologyfile();
	}
	else if (response == 'b') {
	  mprint("You unroll a slick four-color document...");
	  morewait();
	  cityguidefile();
	}
	else {
	  if (random_range(30) > Player.iq)  {
	    mprint("You feel more knowledgeable!");
	    Player.maxiq++;
	  }
	  else {
	    mprint("You find advice in an ancient tome:");
	    hint();
	  }
	}
      }
      xredraw();
    }
    else mprint("You philistine!");
  }
}


void l_pawn_shop()
{
  static pob items[10];
  static int lastitemgen = -1;
  int i,j,done = FALSE;
  char item,action;

  if (nighttime())
    mprint("Knight's Pawn Shop: Closed: Have a Nice (K)Night");
  else {
    if (lastitemgen < Date) {
      for (i=0;i<10;i++) {
	items[i] = create_object();
	if (items[i] != NULL) {
	  items[i]->known = 2;
	  if (items[i]->objchar == CASH)
	    items[i] = NULL;
	}
      }
      lastitemgen = Date;
    }
    while (! done) {
      mprint("\nKnight's Pawn Shop: Buy, Sell or Leave [b,s,ESCAPE] ");
      menuclear();
      for(i=0;i<10;i++) 
	if (items[i] != NULL)
	  menu_item_print(i,items[i]);
      action = mgetc();
      if (action == ESCAPE) 
	done = TRUE;
      else if (action == 'b') {
	printm("\nPurchase which item? [ESCAPE to quit] ");
	item = ' ';
	while ((item != ESCAPE) &&
	       ((item < 'a') || (item > 'k')))
	  item = mgetc();
	if (item != ESCAPE) {
	  i = item - 'a';
	  if (items[i] == NULL) mprint("No such item!");
	  else {
	    printm("\nThe low, low, cost is:");
	    mnumprint(true_item_value(items[i]));
	    mprint(" Buy it? [ynq] ");
	    if (ynq() == 'y') {
	      if (Player.cash < true_item_value(items[i]))
		mprint("No credit! Gwan, Beat it!");
	      else {
		Player.cash -= true_item_value(items[i]);
		Objects[items[i]->id].known = 1;
		add_item_to_pack(items[i]);
		items[i] = NULL;
	      }
	    }
	  }
	}
      }
      else if (action == 's') {
	printm("\nSell which item");
	i = getitem(NULL);
	if ((i != ABORT) && (Player.possessions[i-'a'] != NULL)) {
	  i -= 'a';
	  if (Player.possessions[i]->used)
	    mprint("You'll have to stop using it before you can sell it!");
	  else {
	    printm("\nYou can get ");
	    mnumprint((int) (item_value(Player.possessions[i]) / 4));
	    mprint(" gold pieces for it.");
	    mprint("Sell?");
	    if (ynq() == 'y') {
	      Player.cash += ((int)(item_value(Player.possessions[i]) / 4));
	      free((char *) items[0]);
	      for(j=0;j<9;j++)
		items[j] = items[j+1];
	      items[9] = Player.possessions[i];
	      items[9]->known = 2;	
	      conform_lost_objects(1,Player.possessions[i]);
	    }
	  }
	}
      }
      calc_melee();
    }
  }
  xredraw();
}



void l_cemetary()
{
  mprint("Rampart City Cemetary.");
  mprint("Closed -- Full.");
}

void l_condo()
{
  pol ol,prev=NULL;
  int i,done=FALSE,over=FALSE,weeksleep=FALSE;
  char response;

  if (! Soldcondo) {
    mprint("Rampart Arms.");
    mprint("Weekly Rentals and Purchases");
    mprint("Which are you interested in [r,p, or ESCAPE] ");
    response = mgetc();
    if (response == 'p') {
      mprint("Only 50,000Au. Buy it? [yn] ");
      if (ynq()=='y') {
	if (Player.cash < 50000) 
	  mprint("No mortgages, buddy.");
	else {
	  Soldcondo = TRUE;
	  Player.cash-=50000;
	  dataprint();
	  mprint("You are the proud owner of a luxurious condo penthouse.");
	  Condoitems = NULL;
	}
      }
    }
    else if (response == 'r') {
      mprint("Weekly Rental, 1000Au. Pay for it? [yn] ");
      if (ynq()=='y') {
	if (Player.cash < 1000)
	  mprint("Hey, pay the rent or out you go....");
	else {
	  weeksleep = TRUE;
	  Player.cash -=1000;
	  dataprint();
	}
      }
    }
    else mprint("Please keep us in mind for your housing needs.");
  }
  else {
    while (! done) {
      mprint("Home Sweet Home");
      menuclear();
      menuprint("a: Leave items in your safe.\n");
      menuprint("b: Retrieve items.\n");
      menuprint("c: Take a week off to rest.\n");
      menuprint("d: Retire permanently.\n");
      menuprint("ESCAPE: Leave this place.\n");
      response = mgetc();
      if (response == 'a') {
	i = getitem(NULL);
	if (i != ABORT) {
	  if (Player.possessions[i-'a']->blessing < 0)
	    mprint("The item just doesn't want to be stored away...");
	  else {
	    ol = ((pol) malloc(sizeof(oltype)));
	    ol->thing = Player.possessions[i-'a'];
	    ol->next = Condoitems;
	    Condoitems = ol;
	    conform_lost_object(Player.possessions[i-'a']);
	  }
	}
      }
      else if (response == 'b') {
	ol = Condoitems;
	while ((ol != NULL) && (! over)) {
	  printm("\n Retrieve");
	  printm(itemid(ol->thing));
	  printm("[ynq]");
	  response = mgetc();
	  if (response == 'y') {
	    add_item_to_pack(ol->thing);
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
      else if (response == 'c') weeksleep = TRUE;
      else if (response == 'd') {
	mprint("You sure you want to retire, now? [yn]");
	if (ynq() == 'y') {
	  p_win();
	}
      }
      else if (response == ESCAPE) done = TRUE;
    }
    xredraw();
  }
  if (weeksleep) {
    mprint("Taking a week off to rest...");
    Time+=100*24*7;
    toggle_item_use(TRUE);
    Player.hp = Player.maxhp;
    Player.str = Player.maxstr;
    Player.agi = Player.maxagi;
    Player.con = Player.maxcon;
    Player.dex = Player.maxdex;
    Player.iq = Player.maxiq;
    Player.pow = Player.maxpow;
    toggle_item_use(FALSE);
    Player.food = 23;
    for (i=0;i<NUMSTATI;i++)
      if (Player.status[i]<1000) Player.status[i]=0;
    mprint("You're once again fit and ready to continue your adventure.");
  }
}


void l_residence()
{
  pob newitem;
  if (Dungeon[Dlevel][Player.x][Player.y].aux == BURGLED) {
    mprint("There is a city guard detachment on the scene here.");
    mprint("You decide not to enter....");
  }
  else {
    mprint("A private home.");
    mprint("Burgle it?");
    if (ynq()=='y') {
      Player.alignment -= 2;
      if (random_range(100) > Player.dex*3+Player.rank[THIEVES]*10) {
	mprint("You set off the burglar alarm!");
	mprint("The city guard shows up and carts you off to jail!");
	send_to_jail();
      }
      else {
	Dungeon[Dlevel][Player.x][Player.y].aux = BURGLED;
	gain_experience(25);
	mprint("You find some cash!");
	Player.cash += random_range(500);
	if (random_range(2) == 1) {
	  mprint("You find a hidden cache! You find:");
	  do {
	    newitem = NULL;
	    while (newitem == NULL)
	      newitem = create_object();
	    morewait();
	    printm(itemid(newitem));
	    add_item_to_pack(newitem);
	  } while (random_range(3) == 1);
	}
      }
    }
    else mprint("Probably wise.");
  }
}


void gymtrain(maxstat,stat)
int *maxstat,*stat;
{
  if (*maxstat > *stat)
    mprint("Training is useless while you are debilitated.");
  else if (Gymcredit + Player.cash < 2000)
    mprint("You can't afford our training!");
  else {
    if (Gymcredit > 2000) Gymcredit -= 2000;
    else {
      Player.cash -= (2000-Gymcredit);
      Gymcredit = 0;
    }
    mprint("Sweat. Sweat.");
    if (*maxstat < random_range(30)) {
      mprint("The training pays off!");
      (*maxstat)++;
      (*stat)++;
    }
    else mprint("What a gyp! No improvement.");
  }
}

void healforpay()
{
  if (Player.cash < 50)
    mprint("You can't afford to be healed!");
  else {
    Player.cash -= 50;
    Player.hp += 20+random_range(20);
    if (Player.hp > Player.maxhp) 
      Player.hp = Player.maxhp;
    mprint("Another medical marvel....");
  }
  calc_melee();
}

void cureforpay()
{
  if (Player.cash < 250)
    mprint("You can't afford to be cured!");
  else {
    Player.cash -= 250;
    Player.status[DISEASED] = 0;
    mprint("Quarantine lifted....");
  }
}  



void l_jail()
{
  mprint("Rampart City Gaol -- always room for more.");
  mprint("You hear a loud 'click'");
  Dungeon[0][Player.x][Player.y].locchar = CLOSED_DOOR;
  Dungeon[0][Player.x][Player.y].aux = LOCKED;
  Player.y--;
  drawvision(Player.x,Player.y,Player.vision);
}

void send_to_jail()
{
  int i,j,found=FALSE;
  static int imprisonment=0;
  switch(imprisonment++){
  case 0:
    mprint("The Justiciar sternly reprimands you.");
    mprint("As a first-time offender, you are given probation.");
    Dlevel = 0;
    Player.y = 58;
    Player.x = 40;
    screencheck(58);
    break;
  default:
    mprint("The guards recognize you as a 'three-time-loser'"); 
    mprint("...and beat you up a little to teach you a lesson.");
    p_damage(random_range(imprisonment * 10),UNSTOPPABLE,"police brutality");
  case 1:
    mprint("The Justiciar expresses his shame for your conduct.");
    mprint("You are thrown in jail!");
    Player.y = 55;
    Player.x = 37 + (2*random_range(4));
    Dlevel = 0;
    screencheck(55);
    l_jail();
    break;
  }
}

void l_garden()
{
  mprint("Rampart Botanical Gardens---Do not deface statues.");
}        

void l_challenge()
{
  mprint("A voice booms out, shaking the dungeon walls:");
  mprint("'You have entered the Lairs of the Guardians'");
}

void l_guardian()
{
  mprint("It seems the guardian of this key is away. What luck.");
}

void l_adept()
{
  int i;
  mprint("You see a giant shimmering gate.....");
  mprint("Enter it? [yn]");
  if (ynq()=='n') {
    mprint("The Lords of Destiny spurn your cowardice....");
    p_death("the Lords of Destiny");
  }	
  else {
    mprint("Your head spins for a moment....");
    drawomega();
    mprint("and clears....");
    mprint("You hear a voice boom:");
    morewait();
    printm("\nWelcome to the Adept's Challenge!");
    mprint("A weird violet light illuminates you....");
    for(i=0;i<MAXITEMS;i++)
      if (Player.possessions[i]!=NULL)
        conform_lost_object(Player.possessions[i]);
    dispel(-1);
    Player.hp = Player.maxhp;
    Player.mana = calcmana();
    mprint("You are naked, and alone.");
    Player.x = 32;
    Player.y = 36;
  }
}

void l_warning()
{
  mprint("A voice resounds throughout the dungeon....");
  mprint("'Beware, O Adventurer, the final tests await!'");
}

void l_final_abyss()
{
  mprint("You boldly enter the final abyss....");
  mprint("You fall almost interminably.....");
  sleep(10);
  final_duel();
}


void l_portcullis()
{
  mprint("You walk under a heavy portcullis.");
}

void l_portcullis_trap()
{
  int i,j;
  mprint("You hear heavy walls slamming down!");
  for (i=0;i<WIDTH;i++)
    for(j=0;j<LENGTH;j++) {
      if (Dungeon[Dlevel][i][j].p_locf == L_PORTCULLIS)
	Dungeon[Dlevel][i][j].locchar = WALL;
      if (Dungeon[Dlevel][i][j].p_locf == L_PORTCULLIS_TRAP)
	Dungeon[Dlevel][i][j].p_locf = L_NO_OP;
    }
}

void l_trifid()
{
  int i,damage=0,stuck=TRUE;
  mprint("The hedge comes alive with a surge of alien growth!");
  while (stuck) {
    dataprint();
    damage += Dlevel/2+1;
    mprint("Razor-edged vines covered in suckers attach themselves to you.");
    if ((i=inpack(THINGID+6,-1))+1) {
      mprint("Thinking fast, you toss salt water on the trifid...");
      mprint("The trifid disintegrates with a frustrated sigh.");
      Dungeon[Dlevel][Player.x][Player.y].locchar = FLOOR;
      Dungeon[Dlevel][Player.x][Player.y].p_locf = HEDGE;
      gain_experience(1000);
      stuck = FALSE;
      conform_lost_objects(1,Player.possessions[i]);
    }
    else {
      p_damage(damage,UNSTOPPABLE,"a trifid");
      mprint("You are entangled in tendrils...");
      menuclear();
      menuprint("a: Try to break free.\n");
      menuprint("b: Hang limp and hope the tendrils uncoil.\n");
      menuprint("c: Pray for assistance.\n");
      menuprint("d: Attempt to bargain with the hedge.\n");
      menuprint("e: Click your heels together and wish for escape.\n");
      menuprint("ANYTHING ELSE: writhe and scream hopelessly.\n");
      switch(menugetc()) {
      case 'a': 
	if (Player.str > random_range(200)) {
	  mprint("Amazing! You're now free.");
	  mprint("The trifid writhes hungrily at you.");
	  stuck = FALSE;
	}
	else mprint("Well, THAT didn't work.");
	break;
      case 'b': 
	mprint("Well, at least you're facing your fate with dignity.");
	break;
      case 'c': 
	mprint("You receive no divine aid as yet.");
	break;
      case 'd':
	mprint("The hedge doesn't answer your entreaties.");
	break;
      case 'e':
	mprint("You forgot your ruby slippers, stupid.");
	break;
      default:
	mprint("The hedge enjoys your camp play-acting....");
	break;
      }
    }
  }
  xredraw();
}


void l_maze()
{
  mprint("Hedge maze closed for trifid extermination.");
}


void l_vault()
{
  mprint("You come to a thick vault door with a complex time lock.");
  if ((hour()==23)) {
    mprint("The door is open.");
    Dungeon[0][Player.x][Player.y+1].locchar = FLOOR;
  }
  else {
    mprint("The door is closed.");
    Dungeon[0][Player.x][Player.y+1].locchar = WALL;
    mprint("Try to crack it? [yn]");
    if (ynq()=='y') {
      if (random_range(100) < Player.rank[THIEVES]*Player.rank[THIEVES]) {
	mprint("The lock clicks open!!!");
	gain_experience(5000);
	Dungeon[0][Player.x][Player.y+1].locchar = FLOOR;
      }	
      else {
	mprint("Uh, oh, set off the alarm.... The castle guard arrives....");
	send_to_jail();
      }
    }
    else mprint("Good move.");
  }
}


void l_brothel()
{
  char response;
  mprint("You come to a heavily reinforced inner door.");
  mprint("A sign reads `The House of the Eclipse'");
  mprint("Try to enter? [yn]");
  if (ynq()=='y') {
    menuclear();
    menuprint("a:knock on the door.\n");
    menuprint("b:try to pick the lock.\n");
    menuprint("c:bash down the door.\n");
    menuprint("ESCAPE: Leave this house of ill repute.\n");
    do 
      response = menugetc();
    while ((response != 'a') &&
	   (response != 'b') &&
	   (response != 'c') &&
	   (response != ESCAPE));
    xredraw();
    if (response == 'a') {
      if (!nighttime())
	mprint("There is no reponse.");
      else {
	mprint("A window opens in the door.");
	mprint("`500Au, buddy. For the night.' pay it [yn]?");
	if (ynq()=='y') {
	  if (Player.cash < 500) {
	    mprint("`What, no roll?!'");
	    mprint("The bouncer bounces you a little and lets you go.");
	    p_damage(25,UNSTOPPABLE,"the bouncer");
	  }
	  else {
	    Player.cash -= 500;
	    mprint("You are ushered into an opulently appointed hall.");
	    mprint("After an expensive dinner (takeout from Les Crapuleux)");
	    mprint("you spend an enjoyable and educational evening with");
	    if (Player.preference == 'm') 
	      switch(random_range(4)) {
	      case 0: 
		mprint("Skarn the Insatiable, a satyr.");
		break;
	      case 1: 
		mprint("Dryden the Defanged, an incubus.");
		break;
	      case 2: 
		mprint("Gorgar the Equipped, a centaur.");
		break;
	      case 3: 
		mprint("Hieronymus, the mendicant priest of Eros.");
		break;
	      }	
	    else switch(random_range(4)) {
	    case 0: 
	      mprint("Noreen the Nymph (omaniac)");
	      break;
	    case 1: 
	      mprint("Angelface, a recanted succubus.");
	      break;
	    case 2: 
	      mprint("Corporal Sue of the City Guard (moonlighting).");
	      break;
	    case 3: 
	      mprint("Sheena the Queena the Jungle, a wereleopard.");
	      break;
	    }	
	    if (hour() > 12) Time += ((24-hour())+8) * 100;
	    else Time += ((9-hour())*100);
	    Player.food = 18;
	    Player.status[DISEASED] = 0;
	    /* reduce temporary stat gains to max stat levels */
	    toggle_item_use(TRUE);
	    Player.str = min(Player.str,Player.maxstr);
	    Player.con = min(Player.con,Player.maxcon);
	    Player.agi = min(Player.agi,Player.maxagi);
	    Player.dex = min(Player.dex,Player.maxdex);
	    Player.iq = min(Player.iq,Player.maxiq);
	    Player.pow = min(Player.pow,Player.maxpow);
	    toggle_item_use(FALSE);
	    Player.con++;
	    gain_experience(100);
	    timeprint();
	    dataprint();
	    showflags();
	    mprint("You arise, tired but happy, the next morning...");
	  }
	}
	else mprint("What are you, some kinda prude?");
      }
    }
    else if (response == 'b') {
      if (nighttime()) {
	mprint("As you fumble at the lock, the door opens....");
	mprint("The bouncer tosses you into the street.");
      }
      else mprint("The door appears to be bolted and barred from behind.");
    }
    else if (response == 'c') {
      if (nighttime()) {
	mprint("As you charge toward the door it opens....");
	mprint("Yaaaaah! Thud!");
	mprint("You run passed the startled bouncer into a wall.");
	p_damage(20,UNSTOPPABLE,"a move worthy of Clouseau");
	mprint("The bouncer tosses you into the street.");
      }
      else {
	mprint("Ouch! The door resists your efforts.");
	p_damage(1,UNSTOPPABLE,"a sturdy door");
	mprint("You hear an irritated voice from inside:");
	mprint("'Keep it down out there! Some of us are trying to sleep!'");
      }
    }
  }
}

