/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* priest.c */
/* functions for clerics. */

#include "glob.h"

/* prayer occurs at altars, hence name of function */
void l_altar()
{
  int i,deity;
  char response;

  if (Current_Environment == E_COUNTRYSIDE) deity = DRUID;
  else deity = Level->site[Player.x][Player.y].aux;

  switch(deity) {
    default:
      print1("This rude altar has no markings.");
      break;
    case ODIN:
      print1("This granite altar is graven with a gallows.");
      break;
    case SET:
      print1("This sandstone altar has a black hand drawn on it.");
      break;
    case HECATE:
      print1("This silver altar is inlaid with a black crescent moon.");
      break;
    case ATHENA:
      print1("This golden altar is inscribed with an owl.");
      break;
    case DESTINY:
      print1("This crystal altar is in the form of an omega.");
      break;
    case DRUID:
      print1("This oaken altar is ornately engraved with leaves.");
      break;
  }
  print2("Worship at this altar? [yn] ");
  if (ynq2() == 'y') {
    if (Player.rank[PRIESTHOOD] == 0)
      increase_priest_rank(deity);
    else if (! check_sacrilege(deity)) {
      if (Blessing) print1("You have a sense of immanence.");
      print2("Request a Blessing, Sacrifice an item, or just Pray [b,s,p] ");
      do response = (char) mcigetc();
      while ((response != 'b') && 
	     (response != 's') && 
	     (response != 'p') && 
	     (response != ESCAPE));
      if (response == 'b') {
	print1("You beg a heavenly benefice.");
	print2("You hear a gong resonating throughout eternity....");
	morewait();
	if (Blessing) {
	  print1("A shaft of lucent radiance lances down from the heavens!");
	  print2("You feel uplifted....");
	  morewait();
	  gain_experience(Player.rank[PRIESTHOOD]*Player.rank[PRIESTHOOD]*50);
	  cleanse(1);
	  heal(10);
	  bless(1);
	  Blessing = FALSE;
	  increase_priest_rank(deity);
	}
	else {
	  print1("Your ardent plea is ignored.");
	  print2("You feel ashamed.");
	  Player.xp -= (Player.xp/4);
	}
	calc_melee();
      }
      else if (response == 's') {
	print1("Which item to Sacrifice?");
	i = getitem(NULL_ITEM);
	if (i==ABORT) i = 0;
	if (Player.possessions[i] == NULL) {
	  print1("You have insulted your deity!");
	  print2("Not a good idea, as it turns out...");
	  dispel(-1);
	  p_damage(Player.hp-1,UNSTOPPABLE,"a god's pique");
	}
	else if (true_item_value(Player.possessions[i]) >
		 (long) (Player.rank[PRIESTHOOD] *
		  Player.rank[PRIESTHOOD] *
		  Player.rank[PRIESTHOOD] * 50)) {
	  print1("With a burst of blue flame, your offering vanishes!");
	  dispose_lost_objects(1,Player.possessions[i]);
	  print2("A violet nimbus settles around your head and slowly fades.");
	  morewait();
	  Blessing = TRUE;
	}
	else {
	  print1("A darkling glow envelopes your offering!");
	  print2("The glow slowly fades....");
	  morewait();
	  setgamestatus(SUPPRESS_PRINTING);
	  if (Player.possessions[i]->used) {
	    Player.possessions[i]->used = FALSE;
	    item_use(Player.possessions[i]);
	    Player.possessions[i]->blessing =
	      -1 - abs(Player.possessions[i]->blessing);
	    Player.possessions[i]->used = TRUE;
	    item_use(Player.possessions[i]);
	  }
	  else Player.possessions[i]->blessing =
	    -1 - abs(Player.possessions[i]->blessing);
	  resetgamestatus(SUPPRESS_PRINTING);
	}
      }
      else if (response == 'p') {
	if (deity != Player.patron) 
	  print1("Nothing seems to happen.");
	else if (!increase_priest_rank(deity))
	  answer_prayer();
      }
    }
  }
}




int check_sacrilege(deity)
int deity;
{
  int i,sacrilege=FALSE;
  if ((Player.patron != deity) && (Player.patron > 0)) {
    sacrilege=TRUE;
    Player.pow--;
    Player.maxpow--;
    switch(Player.patron) {
    case ODIN:
      print1("Odin notices your lack of faith! ");
      morewait();
      if (deity == ATHENA) {
	print2("However, Athena intercedes on your behalf.");
	sacrilege = FALSE;
      }
      else {
	print2("You are struck by a thunderbolt!");
	p_damage(Player.level*5,UNSTOPPABLE,"Odin's wrath");
	if (Player.hp > 0) {
	  morewait();
	  print2("The bolt warps your feeble frame....");
	  Player.maxcon = Player.maxcon/2;
	  Player.con = min(Player.con,Player.maxcon);
	  Player.maxstr = Player.maxstr/2;
	  Player.con = min(Player.str,Player.maxstr);
	}
      }
      morewait();
      break;
    case SET:
      print1("Set notices your lack of faith! ");
      morewait();
      if (deity == HECATE) {
	print1("But since you pray to a friendly deity,");
	print2("Set decides not to punish you.");
	sacrilege = FALSE;
      }
      else {
	print2("You are blasted by a shaft of black fire!");
	p_damage(Player.level*5,UNSTOPPABLE,"Set's anger");
	if (Player.hp > 0) {
	  morewait();
	  print1("You are wreathed in clouds of smoke.");
	  for(i=0;i<MAXITEMS;i++) 
	    if ((Player.possessions[i] != NULL) &&
		(Player.possessions[i]->blessing > -1))
	      conform_lost_object(Player.possessions[i]);
	  morewait();
	  print2("You feel Set's Black Hand on your heart....");
	  Player.con = Player.maxcon = Player.maxcon / 4;
	}
      }
      morewait();
      break;
    case HECATE:
      print1("Hecate notices your lack of faith! ");
      morewait();
      if (deity == SET) {
	print1("But ignores the affront since she likes Set.");
	sacrilege = FALSE;
      }
      else {
	print1("You are zapped by dark moonbeams!");
	p_damage(Player.level*5,UNSTOPPABLE,"Hecate's malice");
	if (Player.hp > 0) {
	  print2("The beams leach you of magical power!");
	  Player.maxpow = Player.maxpow/5;
	  Player.pow = min(Player.pow,Player.maxpow);
	  for(i=0;i<NUMSPELLS;i++) 
	    Spells[i].known = FALSE;
	}
      }
      morewait();
      break;
    case ATHENA:
      print1("Athena notices your lack of faith! ");
      morewait();
      if (deity == ODIN) {
	print2("But lets you off this time since Odin is also Lawful.");
	sacrilege = FALSE;
      }
      else {
	print2("You are zorched by godsfire!");
	if (Player.hp > 0) {
	  morewait();
	  print1("The fire burns away your worldly experience!");
		Player.level = 0;
	  Player.xp = 0;
	  Player.maxhp = Player.hp = Player.con;
	  print2("Your power is reduced by the blast!!!");
	  Player.pow = Player.maxpow = Player.maxpow/3;
	  Player.mana = min(Player.mana,calcmana());
	}
      }
      morewait();
      break;
    case DESTINY:
      print2("The Lords of Destiny ignore your lack of faith.");
      sacrilege = FALSE;
      morewait();
      break;
    case DRUID:
      print2("Your treachery to the ArchDruid has been noted.");
      if (random_range(2) == 1)
	Player.alignment += 40;
      else Player.alignment -= 40;
      morewait();
      break;
    }
    if (sacrilege) {
      Player.patron = 0;
      Player.rank[PRIESTHOOD] = 0;
    }
  }
  return(sacrilege);
}

int increase_priest_rank(deity)
int deity;
{
  if (Player.rank[PRIESTHOOD] == 0) switch(deity) {
  default:
    print2("Some nameless god blesses you....");
    Player.hp = max(Player.hp, Player.maxhp);
    morewait();
    print2("The altar crumbles to dust and blows away.");
    Level->site[Player.x][Player.y].locchar = FLOOR;
    Level->site[Player.x][Player.y].p_locf = L_NO_OP;
    lset(Player.x, Player.y, CHANGED);
    break;
  case ODIN:
    if (Player.alignment > 0) {
      print1("Odin hears your prayer!");
      print2(Priest[ODIN]);
      nprint2(" personally blesses you.");
      nprint2(" You are now a lay devotee of Odin.");
      Player.patron = ODIN;
      Player.rank[PRIESTHOOD] = LAY;
      Player.guildxp[PRIESTHOOD] = 1;
      morewait();
      learnclericalspells(ODIN,LAY);
    }
    else print1("Odin ignores you.");
    break;
  case SET:
    if (Player.alignment < 0) {
      print1("Set hears your prayer!");
      print2(Priest[SET]);
      nprint2(" personally blesses you. ");
      nprint2(" You are now a lay devotee of Set.");
      Player.patron = SET;
      Player.rank[PRIESTHOOD] = LAY;
      Player.guildxp[PRIESTHOOD] = 1;
      morewait();
      learnclericalspells(SET,LAY);
    }
    else print1("Set ignores you.");
    break;
  case ATHENA:
    if (Player.alignment > 0) {
      print1("Athena hears your prayer!");
      print2(Priest[ATHENA]);
      nprint2(" personally blesses you.");
      nprint2(" You are now a lay devotee of Athena.");
      Player.patron = ATHENA;
      Player.rank[PRIESTHOOD] = LAY;
      Player.guildxp[PRIESTHOOD] = 1;
      morewait();
      learnclericalspells(ATHENA,LAY);
    }
    else print1("Athena ignores you.");
    break;
  case HECATE:
    if (Player.alignment < 0) {
      print1("Hecate hears your prayer!");
      print2(Priest[HECATE]);
      nprint2(" personally blesses you.");
      nprint2(" You are now a lay devotee of Hecate.");
      Player.patron = HECATE;
      Player.rank[PRIESTHOOD] = LAY;
      Player.guildxp[PRIESTHOOD] = 1;
      morewait();
      learnclericalspells(HECATE,LAY);
    }
    else print1("Hecate ignores you.");
    break;
  case DRUID:
    if (abs(Player.alignment) < 10) {
      print1(Priest[DRUID]);
      nprint1(" personally blesses you.");
      print2("You are now a lay devotee of the Druids.");
      Player.patron = DRUID;
      Player.rank[PRIESTHOOD] = LAY;
      Player.guildxp[PRIESTHOOD] = 1;
      morewait();
      learnclericalspells(DRUID,LAY);
    }
    else {
      print1("You hear a voice....");
      morewait();
      print2("'Only those who embody the Balance may become Druids.'");
    }
    break;
  case DESTINY:
    print1("The Lords of Destiny could hardly care less.");
    print2("But you can consider yourself now to be a lay devotee.");
    Player.patron = DESTINY;
    Player.rank[PRIESTHOOD] = LAY;
      Player.guildxp[PRIESTHOOD] = 1;
    break;
  }
  else if (deity == Player.patron) {
    if ((((deity == ODIN) || (deity == ATHENA)) && 
	 (Player.alignment < 1)) ||
	(((deity == SET) || (deity == HECATE)) && 
	 (Player.alignment > 1)) ||
	((deity == DRUID) && (abs(Player.alignment) > 10))) {
      print1("You have swerved from the One True Path!");
      print2("Your deity is greatly displeased...");
      Player.xp -= Player.level*Player.level;
      Player.xp = max(0,Player.xp);
    }
    else if (Player.rank[PRIESTHOOD]== HIGHPRIEST) return 0;
    else if (Player.rank[PRIESTHOOD]== SPRIEST) {
      if (Player.level > Priestlevel[deity])
	hp_req_test();
      else return 0;
    }
    else if (Player.rank[PRIESTHOOD]==PRIEST) {
      if (Player.guildxp[PRIESTHOOD] >= 4000) {
	print1("An heavenly fanfare surrounds you!");
	print2("Your deity raises you to the post of Senior Priest.");
	hp_req_print();
	Player.rank[PRIESTHOOD] = SPRIEST;
	morewait();
	learnclericalspells(deity,SPRIEST);
      }
      else return 0;
    }
    else if (Player.rank[PRIESTHOOD]==ACOLYTE) {
      if (Player.guildxp[PRIESTHOOD] >= 1500) {
	print1("A trumpet sounds in the distance.");
	print2("Your deity raises you to the post of Priest.");
	Player.rank[PRIESTHOOD] = PRIEST;
	morewait();
	learnclericalspells(deity,PRIEST);
      }
      else return 0;
    }
    else if (Player.rank[PRIESTHOOD]==LAY) {
      if (Player.guildxp[PRIESTHOOD] >= 400) {
	print1("A mellifluous chime sounds from above the altar.");
	print2("Your deity raises you to the post of Acolyte.");
	Player.rank[PRIESTHOOD] = ACOLYTE;
	morewait();
	learnclericalspells(deity,ACOLYTE);
      }
      else return 0;
    }
  }
  return 1;
}


void answer_prayer()
{
  clearmsg();
  switch(random_range(12)) {
  case 0: print1("You have a revelation!"); break;
  case 1: print1("You feel pious."); break;
  case 2: print1("A feeling of sanctity comes over you."); break;
  default: print1("Nothing unusual seems to happen."); break;
  }
}


void hp_req_test()
{
  pob o;
  switch  (Player.patron) {
  case ODIN:
    if (find_item(&o,ARTIFACTID+15,-1))
      make_hp(o);
    else hp_req_print();
    break;
  case SET:
    if (find_item(&o,ARTIFACTID+14,-1))
      make_hp(o);
    else hp_req_print();
    break;
  case ATHENA:
    if (find_item(&o,ARTIFACTID+17,-1))
      make_hp(o);
    else hp_req_print();
    break;
  case HECATE:
    if (find_item(&o,ARTIFACTID+16,-1))
      make_hp(o);
    else hp_req_print();
    break;
  case DRUID:
    if (find_item(&o,ARTIFACTID+14,-1))
      make_hp(o);
    else if (find_item(&o,ARTIFACTID+15,-1))
      make_hp(o);
    else if (find_item(&o,ARTIFACTID+16,-1))
      make_hp(o);
    else if (find_item(&o,ARTIFACTID+17,-1))
      make_hp(o);
    else hp_req_print();
    break;
  case DESTINY:
    if (find_item(&o,ARTIFACTID+19,-1))
      make_hp(o);
    else hp_req_print();
    break;
  }
}


void hp_req_print()
{
  morewait();
  print1("To advance further, you must obtain the Holy Symbol of ");
  switch(Player.patron) {
  case ODIN:
    nprint1(Priest[SET]);
    print2("who may be found in the main Temple of Set.");
    break;
  case SET:
    nprint1(Priest[ODIN]);
    print2("who may be found in the main Temple of Odin.");
    break;
  case ATHENA:
    nprint1(Priest[HECATE]);
    print2("who may be found in the main Temple of Hecate.");
    break;
  case HECATE:
    nprint1(Priest[ATHENA]);
    print2("who may be found in the main Temple of Athena.");
    break;
  case DRUID:
    print2("any of the aligned priests who may be found in their main Temples.");
    break;
  case DESTINY:
    nprint1(Priest[DESTINY]);
    print2("who may be found in the main Temple of Destiny.");
    break;
  }
}

void make_hp(o)
pob o;
{
  print1("A full-scale heavenly choir chants 'Hallelujah' all around you!");
  print2("You notice a change in the symbol you carry....");
  switch(Player.patron) {
  case ODIN:
    *o = Objects[ARTIFACTID+14];
    break;
  case SET:
    *o = Objects[ARTIFACTID+15];
    break;
  case ATHENA:
    *o = Objects[ARTIFACTID+16];
    break;
  case HECATE:
    *o = Objects[ARTIFACTID+17];
    break;
  case DRUID:
    *o = Objects[ARTIFACTID+18];
    break;
  case DESTINY:
    *o = Objects[ARTIFACTID+19];
    break;
  }
  o->known = 2;
  o->charge = 17; /* random hack to convey bit that symbol is functional */
  morewait();
  if (Player.patron == DRUID)
    print1("Your deity raises you to the post of ArchDruid!");
  else print1("Your deity raises you to the post of High Priest!");
  print2("You feel holy.");
  strcpy(Priest[Player.patron],Player.name);
  Priestlevel[Player.patron] = Player.level;
  Player.rank[PRIESTHOOD] = HIGHPRIEST;
  morewait();
  Priestbehavior[Player.patron] = fixnpc(4);
  save_hiscore_npc(Player.patron);
  learnclericalspells(Player.patron,HIGHPRIEST);
}
