/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988 */
/* oguild2.c */
/* L_ functions  */ 

/* These functions implement the various guilds. */
/* They are all l_ functions since they are basically activated*/
/* at some site or other. */

#include "oglob.h"


void l_thieves_guild()
{
  int fee,count,i,number,done=FALSE,dues=1000;
  char c,action;
  pob lockpick;
  print1("You have penetrated to the Lair of the Thieves' Guild.");
  if (! nighttime()) 
    print2("There aren't any thieves around in the daytime.");
  else {
    if ((Player.rank[THIEVES]==TMASTER) &&
	(Player.level > Shadowlordlevel) &&
	find_and_remove_item(THINGID+16,-1)) {
      print2("You nicked the Justiciar's Badge!");
      morewait();
      print1("The Badge is put in a place of honor in the Guild Hall.");
      print2("You are now the Shadowlord of the Thieves' Guild!");
      morewait();
      print1("Who says there's no honor among thieves?");
      strcpy(Shadowlord,Player.name);
      Shadowlordlevel = Player.level;
      morewait();
      clearmsg();
      print1("You learn the Spell of Shadowform.");
      Spells[S_SHADOWFORM].known = TRUE;
      morewait();
      clearmsg();
      Player.rank[THIEVES]=SHADOWLORD;
      Player.maxagi += 2;
      Player.maxdex += 2;
      Player.agi += 2;
      Player.dex += 2;
    }
    while (! done) {
      menuclear();
      menuprint("a: Join the Thieves' Guild.\n");
      menuprint("b: Raise your Guild rank.\n");
      menuprint("c: Get an item identified.\n");
      if (Player.rank[THIEVES] > 0)
	menuprint("d: Fence an item.\n");
      menuprint("ESCAPE: Leave this Den of Iniquity.");
      action = mgetc();
      if (action == ESCAPE) done = TRUE;
      else if (action == 'a') {
	done = TRUE;
	if (Player.rank[THIEVES]> 0)
	  print2("You are already a member!");
	else if (Player.alignment > 10) 
	  print2("You are too lawful to be a thief!");
	else {
	  dues = (int) (dues * (1+(12-Player.dex)/9.0));
	  dues += Player.alignment*5;
	  dues = max(100,dues);
	  print1("Dues are ");
	  mnumprint(dues);
	  nprint1("Au. Pay it? [yn] ");
	  if (ynq1() =='y') {
	    if (Player.cash < dues) {
	      print1("You can't cheat the Thieves' Guild!");
	      print2("... but the Thieves' Guild can cheat you....");
	      Player.cash = 0;
	    }
	    else {
	      print1("Shadowlord ");
	      nprint1(Shadowlord);
	      print2("enters your name into the roll of the Guild."); 
	      morewait();
	      clearmsg();
	      print1("As a special bonus, you get a free lockpick.");
	      print2("You are taught the spell of Object Detection.");
	      morewait();
	      Spells[S_OBJ_DET].known = TRUE; 
	      lockpick = ((pob) malloc(sizeof(objtype)));
	      *lockpick = Objects[THINGID+2]; /* lock pick */
	      gain_item(lockpick);
	      Player.cash -= dues;
	      dataprint();
	      Player.guildxp[THIEVES]=1;
	      Player.rank[THIEVES]=TMEMBER;
	      Player.maxdex++;Player.dex++;Player.agi++;Player.maxagi++;
	    }
	  }
	}
      }
      else if (action == 'b') {
	if (Player.rank[THIEVES]==0)
	  print2("You are not even a member!");
	else if (Player.rank[THIEVES]==SHADOWLORD) 
	  print2("You can't get any higher than this!");
	else if (Player.rank[THIEVES]==TMASTER) {
	  if (Player.level <= Shadowlordlevel)
	    print2("You are not experienced enough to advance.");
	  else print2("You must bring back the Justiciar's Badge!");
	}
	else if (Player.rank[THIEVES]==THIEF) {
	  if (Player.guildxp[THIEVES] < 4000)
	    print2("You are not experienced enough to advance.");
	  else  {
	    print1("You are now a Master Thief of the Guild!");
	    print2("You are taught the Spell of Apportation.");
	    morewait();
	    print1("To advance to the next level you must return with");
	    print2("the badge of the Justiciar (cursed be his name).");
	    morewait();
	    clearmsg();
	    print1("The Justiciar's office is just south of the gaol.");
	    Spells[S_APPORT].known = TRUE;
	    Player.rank[THIEVES]=TMASTER;
	    Player.maxagi++;
	    Player.maxdex++;
	    Player.agi++;
	    Player.dex++;
	  }
	}
	else if (Player.rank[THIEVES]==ATHIEF) {	
	  if (Player.guildxp[THIEVES] < 1500)
	    print2("You are not experienced enough to advance.");
	  else  {
	    print1("You are now a ranking Thief of the Guild!");
	    print2("You learn the Spell of Invisibility.");
	    Spells[S_INVISIBLE].known = TRUE;
	    Player.rank[THIEVES]=THIEF;
	    Player.agi++;
	    Player.maxagi++;
	  }
	}
	else if (Player.rank[THIEVES]==TMEMBER) {
	  if (Player.guildxp[THIEVES] < 400)
	    print2("You are not experienced enough to advance.");
	  else {
	    print1("You are now an Apprentice Thief!");
	    print2("You are taught the Spell of Levitation.");
	    Spells[S_LEVITATE].known = TRUE;
	    Player.rank[THIEVES]=ATHIEF;
	    Player.dex++;
	    Player.maxdex++;
	  }
	}
      }
      else if (action == 'c') {
	if (Player.rank[THIEVES]==0) {
	  print1("RTG, Inc, Appraisers. Identification Fee: 50Au/item.");
	  fee = 50;
	}
	else {
	  fee = 5;
	  print1("The fee is 5Au per item.");
	}
	print2("Identify one item, or all possessions? [ip] ");
	if (mcigetc()=='i') {
	  if (Player.cash < fee)
	    print2("Try again when you have the cash.");
	  else {
	    Player.cash -= fee;
	    dataprint();
	    identify(0);
	  }
	}
	else {
	  count = 0;
	  for(i=1;i<MAXITEMS;i++)
	    if (Player.possessions[i] != NULL)
	      if (Player.possessions[i]->known < 2)
		count++;
	  for(i=0;i<Player.packptr;i++)
	    if (Player.pack[i] != NULL)
	      if (Player.pack[i]->known < 2)
		count++;
	  print1("The fee will be: ");
	  mnumprint(max(count*fee,fee));
	  nprint1("Au. Pay it? [yn] ");
	  if (ynq1()=='y')
	  if (Player.cash < max(count*fee,fee))
	    print2("Try again when you have the cash.");
	  else {
	    Player.cash -= max(count*fee,fee);
	    dataprint();
	    identify(1);
	  }
	}
      }
      else if (action == 'd') {
	if (Player.rank[THIEVES]==0)
	  print2("Fence? Who said anything about a fence?");
	else {
	  print1("Fence one item or go through pack? [ip] ");
	  if (mcigetc()=='i') {
	    i = getitem(NULL);
	    if ((i==ABORT) || (Player.possessions[i] == NULL))
	      print2("Huh, Is this some kind of set-up?");
	    else if (Player.possessions[i]->blessing < 0) 
	      print2("I don't want to buy a cursed item!");
	    else {
	      print1("I'll give you ");
	      mnumprint(2 * item_value(Player.possessions[i]) / 3);
	      nprint1("Au each. OK? [yn] ");
	      if (ynq1() == 'y') {
		number = getnumber(Player.possessions[i]->number);
		if ((number >= Player.possessions[i]->number) &&
		    Player.possessions[i]->used) {
		  Player.possessions[i]->used = FALSE;
		  item_use(Player.possessions[i]);
		}
		Player.cash += number*2*item_value(Player.possessions[i])/3;
		dispose_lost_objects(number,Player.possessions[i]);
		dataprint();
	      }
	      else print2("Hey, gimme a break, it was a fair price!");
	    }
	  }
	  else {
	    for(i=0;i<Player.packptr;i++) {
	      if (Player.pack[i]->blessing > -1) {
		print1("Sell ");
		nprint1(itemid(Player.pack[i]));
		nprint1(" for ");
		mnumprint(2*item_value(Player.pack[i])/3);
		nprint1("Au each? [ynq] ");
		if ((c=ynq1())=='y') {
		  number = getnumber(Player.pack[i]->number);
		  Player.cash += 2*number * item_value(Player.pack[i]) / 3;
		  Player.pack[i]->number -= number;
		  if (Player.pack[i]->number < 1) {
		    free((char *)Player.pack[i]);
		    Player.pack[i] = NULL;
		  }
		  dataprint();
		}
		else if (c=='q') break;
	      }
	    }
	    fixpack();
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
  int done=FALSE,enrolled = FALSE;
  print1("The Collegium Magii. Founded 16937, AOF.");
  if (nighttime())
    print2("The Registration desk is closed at night....");
  else {
    while (! done) {
      if ((Player.rank[COLLEGE]==MAGE) &&
	  (Player.level > Archmagelevel) &&
	  find_and_remove_item(CORPSEID,ML10+1)) {
	print1("You brought back the heart of the Eater of Magic!");
	morewait();
	print1("The Heart is sent to the labs for analysis.");
	print2("The Board of Trustees appoints you Archmage!");
	morewait();
	clearmsg();
	strcpy(Archmage,Player.name);
	Archmagelevel = Player.level;
	Player.rank[COLLEGE] = ARCHMAGE;
	Player.maxiq += 5;
	Player.iq += 5;
	Player.maxpow += 5;
	Player.pow += 5;
      }
      menuclear();
      menuprint("May we help you?\n\n");
      menuprint("a: Enroll in the College.\n");
      menuprint("b: Raise your College rank.\n");
      menuprint("c: Do spell research.\n");
      menuprint("ESCAPE: Leave these hallowed halls.\n");
      action = mgetc();
      if (action == ESCAPE) done = TRUE;
      else if (action == 'a') {
	if (Player.rank[COLLEGE] > 0)
	  print2("You are already enrolled!");
	else if (Player.iq < 13) 
	  print2("Your low IQ renders you incapable of being educated.");
	else {
	  if (Player.iq > 17) {
	    print2("You are given a scholarship!");
	    morewait();
	    enrolled=TRUE;
	  }
	  else {
	    print1("Tuition is 1000Au. ");
	    nprint1("Pay it? [yn] ");
	    if (ynq1() =='y') {
	      if (Player.cash < 1000)
		print2("You don't have the funds!");
	      else {
		Player.cash -= 1000;
		enrolled = TRUE;
		dataprint();
	      }		
	    }
	  }
	  if (enrolled) {
	    print1("Archmage ");
	    nprint1(Archmage);
	    nprint1(" greets you and congratulates you on your acceptance.");
	    print2("You are now enrolled in the Collegium Magii!");
	    morewait();
	    print1("You are now a Novice.");
	    print2("You may research 1 spell, for your intro class.");
	    Spellsleft = 1;
	    Player.rank[COLLEGE] = INITIATE;
	    Player.guildxp[COLLEGE] = 1;
	    Player.maxiq += 1;
	    Player.iq += 1;
	    Player.maxpow += 1;
	    Player.pow += 1;
	  }
	}
      }
      else if (action == 'b') {
	if (Player.rank[COLLEGE] == 0)
	  print2("You have not even been initiated, yet!");
	else if (Player.rank[COLLEGE]==ARCHMAGE) 
	  print2("You are at the pinnacle of mastery in the Collegium.");
	else if (Player.rank[COLLEGE]==MAGE) {
	  if (Player.level <= Archmagelevel)
	    print2("You are not experienced enough to advance.");
	  else
	    print2("You must return with the heart of the Eater of Magic!");
	}
	else if (Player.rank[COLLEGE]==PRECEPTOR) {
	  if (Player.guildxp[COLLEGE] < 4000)
	    print2("You are not experienced enough to advance.");
	  else  {
	    print1("You are now a Mage of the Collegium Magii!");
	    print2("You may research 6 spells for postdoctoral research.");
	    Spellsleft += 6;
	    morewait();
	    print1("To become Archmage, you must return with the");
	    print2("heart of the Eater of Magic");
	    morewait();
	    clearmsg();
	    print1("The Eater may be found on a desert isle somewhere.");
	    Player.rank[COLLEGE] = MAGE;
	    Player.maxiq += 2;
	    Player.iq += 2;
	    Player.maxpow += 2;
	    Player.pow += 2;
	  }
	}
	else if (Player.rank[COLLEGE]==STUDENT) {
	  if (Player.guildxp[COLLEGE] < 1500)
	    print2("You are not experienced enough to advance.");
	  else  {
	    print1("You are now a Preceptor of the Collegium Magii!");
	    print2("You are taught the basics of ritual magic.");
	    morewait();
	    clearmsg();
	    print1("Your position allows you to research 4 spells.");
	    Spellsleft +=4;
	    Spells[S_RITUAL].known = TRUE;
	    Player.rank[COLLEGE] = PRECEPTOR;
	    Player.maxiq += 1;
	    Player.iq += 1;
	    Player.maxpow += 1;
	    Player.pow += 1;
	  }
	}
	else if (Player.rank[COLLEGE]==NOVICE) {
	  if (Player.guildxp[COLLEGE] < 400)
	    print2("You are not experienced enough to advance.");
	  else  {
	    print1("You are now a Student at the Collegium Magii!");
	    print2("You are taught the spell of identification.");
	    morewait();
	    clearmsg();
	    print1("Thesis research credit is 2 spells.");
	    Spellsleft+=2;
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
	clearmsg();
	if (Spellsleft > 0) {
	  print1("Research permitted: ");
	  mnumprint(Spellsleft);
	  nprint1(" Spells.");
	  morewait();
	}
	if (Spellsleft < 1) {
	  print1("Extracurricular Lab fee: 2000 Au. ");
	  nprint1("Pay it? [yn] ");
	  if (ynq1()=='y') {
	    if (Player.cash < 2000) 
	      print1("Try again when you have the cash.");
	    else {
	      Player.cash -= 2000;
	      dataprint();
	      Spellsleft = 1;
	    }
	  }
	}
	if (Spellsleft > 0) {
	  learnspell(0);
	  Spellsleft--;
	}
      }
    }
  }
  xredraw();
}



void l_sorcerors()
{
  char action;
  int done=FALSE,fee=3000,total;
  print1("The Circle of Sorcerors.");
  if (Player.rank[CIRCLE] == -1) {
    print2("Fool! Didn't we tell you to go away?");
    Player.mana = 0;
    dataprint();
  }
  else  while (! done) {
    if ((Player.rank[CIRCLE]==HIGHSORCEROR) &&
	(Player.level > Primelevel) &&
	find_and_remove_item(CORPSEID,ML10+2)) {
      print2("You obtained the Crown of the Lawgiver!");
      morewait();
      print1("The Crown is ritually sacrificed to the Lords of Chaos.");
      print2("You are now the Prime Sorceror of the Inner Circle!");
      strcpy(Prime,Player.name);
      Primelevel = Player.level;
      morewait();
      clearmsg();
      print1("You learn the Spell of Disintegration!");
      morewait();
      clearmsg();
      Spells[S_DISINTEGRATE].known = TRUE;
      Player.rank[CIRCLE] = PRIME;
      Player.maxpow += 10;
      Player.pow += 10;
    }
    menuclear();
    menuprint("May we help you?\n\n");
    menuprint("a: Become an Initiate of the Circle.\n");
    menuprint("b: Raise your rank in the Circle.\n");
    menuprint("c: Restore mana points\n");
    menuprint("ESCAPE: Leave these Chambers of Power.\n");
    action = mgetc();
    if (action == ESCAPE) done = TRUE;
    else if (action == 'a') {
      if (Player.rank[CIRCLE] > 0)
	  print2("You are already an initiate!");
      else if (Player.alignment > 0)
	print2("You may not join -- you reek of Law!");
      else {
	fee += Player.alignment*100;
	fee = (int) (fee * (1+(12-Player.pow)/9.0));
	fee = max(100,fee);
	print1("For you, there is an initiation fee of ");
	mnumprint(fee);
	nprint1("Au.");
	print2("Pay it? [yn] ");
	if (ynq2() =='y') {
	  if (Player.cash < fee) 
	    print3("Try again when you have the cash!");
	  else {
	    print1("Prime Sorceror ");
	    nprint1(Prime);
	    print2("conducts your initiation into the circle of novices.");
	    morewait();
	    clearmsg();
	    print1("You learn the Spell of Magic Missiles.");
	    Spells[S_MISSILE].known = TRUE;
	    Player.cash -= fee;
	    dataprint();
	    Player.rank[CIRCLE] = INITIATE;
	    Player.guildxp[CIRCLE] = 1;
	    Player.maxpow++;
	    Player.pow++;
	  }
	}
      }
    }
    else if (action == 'b') {
      if (Player.rank[CIRCLE] == 0)
	print2("You have not even been initiated, yet!");
      else if (Player.alignment > -1) {
	print1("Ahh! You have grown too lawful!!!");
	print2("You are hereby blackballed from the Circle!");
	Player.rank[CIRCLE] = -1;
	morewait();
	clearmsg();
	print1("A pox upon thee!");
	if (! Player.immunity[INFECTION])
	  Player.status[DISEASED]+=100;
	print2("And a curse on your possessions!");
	morewait();
	clearmsg();
	acquire(-1);
	clearmsg();
	enchant(-1);
	bless(-1);
	print3("Die, false sorceror!");
	p_damage(25,UNSTOPPABLE,"a sorceror's curse");
      }
      else if (Player.rank[CIRCLE]==PRIME) 
	print2("You are at the pinnacle of mastery in the Circle.");
      else if (Player.rank[CIRCLE]==HIGHSORCEROR) {
	if (Player.level <= Primelevel)
	  print2("You are not experienced enough to advance.");
	else 
	  print2("You must return with the Crown of the LawBringer!");
      }
      else if (Player.rank[CIRCLE]==SORCEROR) {
	if (Player.guildxp[CIRCLE] < 4000)
	  print2("You are not experienced enough to advance.");
	else  {
	  print1("You are now a High Sorceror of the Inner Circle!");
	  print2("You learn the Spell of Disruption!");
	  morewait();
	  clearmsg();
	  print1("To advance you must return with the LawBringer's Crown!");
	  print2("The LawBringer resides on Star Peak.");
	  Spells[S_DISRUPT].known = TRUE;
	  Player.rank[CIRCLE] = HIGHSORCEROR;
	  Player.maxpow += 5;
	  Player.pow += 5;
	}
      }
      else if (Player.rank[CIRCLE]==ENCHANTER) {
	if (Player.guildxp[CIRCLE] < 1500)
	  print2("You are not experienced enough to advance.");
	else  {
	  print1("You are now a member of the Circle of Sorcerors!");
	  print2("You learn the Spell of Ball Lightning!");
	  Spells[S_LBALL].known = TRUE;
	  Player.rank[CIRCLE] = SORCEROR;
	  Player.maxpow += 2; 
	  Player.pow+=2;
	}
      }
      else if (Player.rank[CIRCLE]==INITIATE) {
	if (Player.guildxp[CIRCLE] < 400)
	  print2("You are not experienced enough to advance.");
	else  {
	  print1("You are now a member of the Circle of Enchanters!");
	  print2("You learn the Spell of Firebolts.");
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
      print1("That will be: ");
      mnumprint(fee);
      nprint1("Au. Pay it? [yn] ");
      if (ynq1()=='y') {
	if (Player.cash < fee) 
	  print2("Begone, deadbeat, or face the wrath of the Circle!");
	else {
	  Player.cash -= fee;
	  total = calcmana();
	  while (Player.mana < total) {
	    Player.mana++;
	    dataprint();
	  }
	  print2("Have a sorcerous day, now!");
	}
      }
      else print2("Be seeing you!");
    }
  }
  xredraw();
}




void l_order()
{
  pob newitem;
  print1("The Headquarters of the Order of Paladins.");
  morewait();
  if ((Player.rank[ORDER]==PALADIN) &&
      (Player.level > Justiciarlevel) &&
      gamestatusp(GAVE_STARGEM) &&
      Player.alignment > 300) {
    print1("You have succeeded in your quest!");
    morewait();
    print1("The previous Justiciar steps down in your favor.");
    print2("You are now the Justiciar of Rampart and the Order!");
    strcpy(Justiciar,Player.name);
    Justiciarlevel = Player.level;
    morewait();
    clearmsg();
    print1("You are awarded a blessed shield of deflection!");
    morewait();
    newitem = ((pob) malloc(sizeof(objtype)));
    *newitem = Objects[SHIELDID+6]; /* shield of deflection */
    newitem->blessing = 9;
    gain_item(newitem);
    morewait();
    Player.rank[ORDER] = PRIME;
    Player.maxpow += 10;
    Player.pow += 10;
  }
  if ((Player.rank[ORDER] == -1) || (Player.alignment < 1)) 
    print1("Get thee hence, minion of chaos!");
  else if (Player.rank[ORDER] == 0) {
    if (Player.rank[ARENA] != 0) 
      print1("We do not accept blood stained gladiators into our Order.");
    else if (Player.rank[LEGION] != 0) 
      print1("Go back to your barracks, mercenary!");
    else {
      print1("Dost thou wish to join our Order? [yn] ");
      if (ynq1()=='y') {
	print1("Justiciar ");
	nprint1(Justiciar);
	nprint1(" welcomes you to the Order.");
	print2("'Mayest thou always follow the sublime path of Law.'");
	morewait();
	print1("You are now a Gallant in the Order.");
	print2("You are given a horse and a blessed spear.");
	morewait();
	Player.rank[ORDER] = GALLANT;
	Player.guildxp[ORDER] = 1;
	setgamestatus(MOUNTED);
	newitem = ((pob) malloc(sizeof(objtype)));
	*newitem = Objects[WEAPONID+19]; /* spear */
	newitem->blessing = 9;
	newitem->plus = 1;
	newitem->known = 2;
	gain_item(newitem);
      }
    }
  }
  else {
    if (Player.alignment < 1) {
      print1("You have been tainted by chaos!");
      print2("You are stripped of your rank in the Order!");
      morewait();
      Player.rank[ORDER]= -1;
      send_to_jail();
    }
    else {
      print1("'Welcome back, Paladin.'");
      if (!gamestatusp(MOUNTED)) {
	print2("You are given a new steed.");
	setgamestatus(MOUNTED);
      }
      morewait();
      clearmsg();
      if ((Player.hp < Player.maxhp) || (Player.status[DISEASED]))
	print1("Your wounds are treated by a medic.");
      cleanse(1);
      Player.hp = Player.maxhp;
      Player.food = 40;
      print2("You get a hot meal from the refectory.");
      morewait();
      clearmsg();
      if (Player.rank[ORDER]==PALADIN) {
	if (Player.level <= Justiciarlevel)
	  print2("You are not experienced enough to advance.");
	else if (Player.alignment < 300)
	  print2("You are not sufficiently Lawful as yet to advance.");
	else print2("You must give the Star Gem to the LawBringer.");
      }
      else if (Player.rank[ORDER]==CHEVALIER) {
	if (Player.guildxp[ORDER] < 4000)
	  print2("You are not experienced enough to advance.");
	else if (Player.alignment < 200)
	  print2("You are not sufficiently Lawful as yet to advance.");
	else {
	  print1("You are made a Paladin of the Order!");
	  print2("You learn the Spell of Heroism and get Mithril Plate!");
	  morewait();
	  newitem = ((pob) malloc(sizeof(objtype)));
	  *newitem = Objects[ARMORID+11]; /* mithril plate armor */
	  newitem->blessing = 9;
	  newitem->known = 2;
	  gain_item(newitem);
	  morewait();
	  clearmsg();
	  print1("To advance you must rescue the Star Gem and return it");
	  print2("to its owner, the LawBringer, who resides on Star Peak.");
	  morewait();
	  print1("The Star Gem was stolen by the cursed Prime Sorceror,");
	  print2("whose headquarters may be found beyond the Astral Plane.");
	  morewait();
	  print1("The Oracle will send you to the Astral Plane if you");
	  print2("prove yourself worthy to her.");
	  morewait();
	  Spells[S_HERO].known = TRUE;
	  Player.rank[ORDER] = PALADIN;
	}
      }
      else if (Player.rank[ORDER]==GUARDIAN) {
	if (Player.guildxp[ORDER] < 1500)
	  print2("You are not experienced enough to advance.");
	else if (Player.alignment < 125)
	  print2("You are not yet sufficiently Lawful to advance.");
	else {
	  Player.rank[ORDER] = CHEVALIER;
	  print1("You are made a Chevalier of the Order!");
	  print2("You are given a Mace of Disruption!");
	  morewait();
	  clearmsg();
	  newitem = ((pob) malloc(sizeof(objtype)));
	  *newitem = Objects[WEAPONID+25]; /* mace of disruption */
	  newitem->known = 2;
	  gain_item(newitem);
	}
      }
      else if (Player.rank[ORDER]==GALLANT) {
	if (Player.guildxp[ORDER] < 400)
	  print2("You are not experienced enough to advance.");
	else if (Player.alignment < 50)
	  print2("You are not Lawful enough to advance.");
	else  {
	  print1("You are made a Guardian of the Order of Paladins!");
	  print2("You are given a Holy Hand Grenade (of Antioch).");
	  morewait();
	  print1("You hear a nasal monotone in the distance....");
	  print2("'...and the number of thy counting shall be 3...'");
	  morewait();
	  clearmsg();
	  Player.rank[ORDER] = GUARDIAN;
	  newitem = ((pob) malloc(sizeof(objtype)));
	  *newitem = Objects[ARTIFACTID+7]; /* holy hand grenade. */
	  newitem->known = 2;
	  gain_item(newitem);
	}
      }
    }
  }
}
