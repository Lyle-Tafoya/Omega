/* omega copyright (C) by Laurence Raphael Brothers, 1987 */
/* oetc.c */
/* grab bag of random functions used in random places */

#include <strings.h>
#include "oglob.h"

/* from oscr */
extern void mprint();

/* from outil */
extern int random_range();

void hint(),learnclericalspells(),add_new_npc(),nameprint();
char *slotstr(),*wordnum();

/* there are various ways for the player to receive one of these hints */
void hint()
{
  switch(random_range(60)) {
    case 0:mprint("There is a dungeon entrance in the Garden.");break;
    case 1:mprint("Statues can be dangerous.");break;
    case 2:mprint("Identify Artifacts.");break;
    case 3:mprint("The higher form of mercury is desirable.");break;
    case 4:mprint("A sense of unease is a good thing to have.");break;
    case 5:mprint("If you dig too much, you might cause a cave in!.");break;
    case 6:mprint("Be Lawful: Live and Let Live.");break;
    case 7:mprint("Be Chaotic: Live and Let Die."); break;
    case 8:mprint("The world doesn't slow down; you speed up.");break;
    case 9:mprint("Security is a sense of dislocation.");break;
    case 10:mprint("Tullimore Dew is a panacea.");break;
    case 11:mprint("Thieves hide behind locked doors.");break;
    case 12:mprint("`No jail is escapeproof' -- John Dillinger.");break;
    case 13:mprint("Oh, to have an apartment of your own!");break;
    case 14:mprint("Some homes have money and treasure.");break;
    case 15:mprint("Some homes have burglar alarms.");break;
    case 16:mprint("Oh, how I wish I was more powerful!"); break;
    case 17:mprint("Oh, how I wish I was more wealthy!");break;
    case 18:mprint("Oh, how I wish I was more skillful!"); break;
    case 19:mprint("Oh, how I wish I was more balanced!"); break;
    case 20:mprint("Don't anger fnords or fungi.");break;
    case 21:mprint("Some fairies are good.");break;
    case 22:mprint("An affair with a demon can be heartbreaking."); break;
    case 23:mprint("The Explorer's Club knows a useful spell."); break;
    case 24:mprint("Some Lyzzard partes are rotten."); break;
    case 25:mprint("Magic pools are totally random."); break;
    case 26:mprint("^F aborts the spell of Shadow Form."); break;
    case 27:mprint("Humans can be good or evil, lawful or chaotic."); break;
    case 28:mprint("There are six kinds of wishes."); break;
    case 29:mprint("The fifth level is the lair of the Goblin King."); break;
    case 30:mprint("Donaldson's Giants can withstand lava.");break;
    case 31:mprint("Ritual magic can have many different effects.");break;
    case 32:mprint("The Mercenaries are the best equipped fighters."); break;
    case 33:mprint("The Gladiators are the most skilled fighters."); break;
    case 34:mprint("Rent a flat and lose any bad stati you may have."); break;
    case 35:mprint("Some junk may be worth a fortune if identified."); break;
    case 36:mprint("Identify humans by talking to them."); break;
    case 37:mprint("They say the Duke has a treasure trove."); break;
    case 38:mprint("If you yield, your opponent will gain experience."); break;
    case 39:mprint("The 33d level is the lair of the Dragon Lord."); break;
    case 40:mprint("A full moon bodes well for the followers of Law."); break;
    case 41:mprint("A new moon omens evil for the Law-abiding."); break;
    case 42:mprint("Druids revere the half-moon."); break;
    case 43:mprint("Most grot is useless."); break;
    case 44:mprint("Cash can sometimes be found in the walls."); break;
    case 45:mprint("Pointy weapons break often but dig better."); break;
    case 46:mprint("The DREADED AQUAE MORTIS is invulnerable."); break;
    case 47:mprint("There must be *some* reason to worship Destiny!"); break;
    case 48:mprint("Kill a trifid? A puzzle! Try a saline solution!"); break;
    case 49:mprint("Beware! The Eater of Souls inhabits the abyss!"); break;
    case 50:mprint("They say there's a red-light district."); break;
    case 51:mprint("The House of the Eclipse is behind a closed door.");
    case 52:mprint("Don't block and attack in the same line."); break;
    case 53:mprint("The Champion should never refuse a challenge."); break;
    case 54:mprint("They say that the autoteller program is buggy."); break;
    case 55:mprint("It's better not to sleep on the ground."); break;
    case 56:mprint("Try ritual magic in a room with charred walls."); break;
    case 57:mprint("Breaking down a wall by bashing it is a bad idea!"); break;
    case 58:mprint("Don't be a three-time loser!"); break;
    case 59:mprint("The Archdruid is always glad to welcome visitors.");break;
  }
}

/* for when a deity teaches spells to a devotee */
void learnclericalspells(deity,level)
int deity,level;
{
  mprint("With your new clerical rank comes knowledge of magic...");
  Player.pow+=level;
  Player.maxpow+=level;
  switch(level) {
    case LAY: 
      if (deity==ODIN) 
	Spells[S_MISSILE].known = TRUE;
      else if (deity==SET)
	Spells[S_INVISIBLE].known = TRUE;
      else if (deity==ATHENA)
	Spells[S_IDENTIFY].known = TRUE;
      else if (deity==HECATE)
	Spells[S_DRAIN].known = TRUE;
      else if (deity==DRUID) {
	Spells[S_KNOWLEDGE].known = TRUE;
	Spells[S_MON_DET].known = TRUE;
      }
      break;
    case ACOLYTE: 
      if (deity==ODIN) {
	Spells[S_LBALL].known = TRUE;
	Spells[S_TRUESIGHT].known = TRUE;
      }
      else if (deity==SET) {
	Spells[S_SUMMON].known = TRUE;
	Spells[S_FIREBOLT].known = TRUE;
      }
      else if (deity==ATHENA) {
	Spells[S_HEAL].known = TRUE;
	Spells[S_SANCTUARY].known = TRUE;
      }
      else if (deity==HECATE) {
	Spells[S_SLEEP].known = TRUE;
	Spells[S_DISPEL].known = TRUE;
      }
      else if (deity==DRUID) {
	Spells[S_HEAL].known = TRUE;
	Spells[S_CURE].known = TRUE;
      }
      else if (deity==DESTINY)
	mprint("An acolyte of the Lords of Destiny. Gee whiz.");
      break;
    case PRIEST: 
      Spells[S_SANCTIFY].known = TRUE;
      if (deity==ODIN) {
	Spells[S_HERO].known = TRUE;
	Spells[S_HEAL].known = TRUE;
      }
      else if (deity==SET) {
	Spells[S_INVISIBLE].known = TRUE;
	Spells[S_DISPEL].known = TRUE;
      }
      else if (deity==ATHENA) {
	Spells[S_INVULNERABLE].known = TRUE;
	Spells[S_ACCURACY].known = TRUE;
      }
      else if (deity==HECATE) {
	Spells[S_SHADOWFORM].known = TRUE;
	Spells[S_CURE].known = TRUE;
      }
      else if (deity==DRUID) {
	Spells[S_DISRUPT].known = TRUE;
	Spells[S_ALERT].known = TRUE;
	Spells[S_CLAIRVOYANCE].known = TRUE;
      }
      else if (deity==DESTINY)
	mprint("How useless, a new priest of the Lords of Destiny.");
      break;
    case SPRIEST:
      Spells[S_BLESS].known = TRUE;
      if (deity == ODIN) 
	Spells[S_ACCURACY].known = TRUE;
      else if (deity == SET)
	Spells[S_INVULNERABLE].known = TRUE;
      else if (deity == ATHENA)
	Spells[S_HERO].known = TRUE;
      else if (deity == HECATE)
	Spells[S_POLYMORPH].known = TRUE;
      else if (deity == DRUID) {
	Spells[S_POLYMORPH].known = TRUE;	
	Spells[S_LEVITATE].known = TRUE;
      }
      else if (deity == DESTINY)
	mprint("Wow, a new senior priest of the Lords of Destiny.");
      break;
    case HIGHPRIEST:
      if (deity == ODIN)
	Spells[S_INVULNERABLE].known = TRUE;
      else if (deity == SET)
	Spells[S_HELLFIRE].known = TRUE;
      else if (deity == ATHENA)
	Spells[S_HELLFIRE].known = TRUE;
      else if (deity == HECATE)
	Spells[S_DESECRATE].known = TRUE;
      else if (deity == DRUID) {
	Spells[S_DISINTEGRATE].known = TRUE;
	Spells[S_HERO].known = TRUE;
      }
      else if (deity == DESTINY) {
	mprint("So you're now the high priest of the Lords of Destiny.");
	mprint("You didn't think you were going to get anything, did you?");
      }
    }
}

/* for the use of the casino slot machine */
char *slotstr(num)
int num;
{
  switch(num) {
  case 0:
    return("<Slime Mold>");
    break;
  case 1:
    return("<Lemon>");
    break;
  case 2:
    return("<Copper>");
    break;
  case 3:
    return("<Nymph>");
    break;
  case 4:
    return("<Sword>");
    break;
  case 5:
    return("<Shield>");
    break;
  case 6:
    return("<Chest>");
    break;
  case 7:
    return("<Bar>");
    break;
  case 8:
    return("<Orb>");
    break;
  case 9:
    return("<Mithril Nugget>");
    break;
  }
}

/* random names for various uses */
void nameprint()
{
  switch(random_range(40)) {
  case 0:mprint("Orion Splash");
  case 1:mprint("Gorgar");break;
  case 2:mprint("Hieronymous");break;
  case 3:mprint("Quantifor Quotron");break;
  case 4:mprint("Leon");break;
  case 5:mprint("Joyce");break;
  case 6:mprint("Leticia Smiley");break;
  case 7:mprint("Ogilvy the Grim");break;
  case 8:mprint("Salara Storn");break;
  case 9:mprint("Murgo");break;
  case 10:mprint("Jonathan Atwilder");break;
  case 11:mprint("Xylos the Tan");break;
  case 12:mprint("Terence");break;
  case 13:mprint("Toronado");break;
  case 14:mprint("Kelly");break;
  case 15:mprint("Cantinflas");break;
  case 16:mprint("Ixel");break;
  case 17:mprint("Toto");break;
  case 18:mprint("Frost");break;
  case 19:mprint("Aliera Erinyes");break;
  case 20:mprint("Godel");break;
  case 21:mprint("Kerst Blackblade");break;
  case 22:mprint("Ebenezer");break;
  case 23:mprint("Jeremiah");break;
  case 24:mprint("Triskelion Shadow");break;
  case 25:mprint("Eleskir Eremar");break;
  case 26:mprint("Tyron");break;
  case 27:mprint("Morgon");break;
  case 28:mprint("Achmed");break;
  case 29:mprint("Chin");break;
  case 30:mprint("Fujimoto");break;
  case 31:mprint("Dos Santos");break;
  case 32:mprint("Federico");break;
  case 33:mprint("Jaime");break;
  case 34:mprint("Siobhan");break;
  case 35:mprint("Hans");break;
  case 36:mprint("Gurkov");break;
  case 37:mprint("Krilos the Slayer");break;
  case 38:mprint("Oxxblud");break;
  case 39:mprint("Dorian");break;
  }
}


/* returns english string equivalent of number */
char *wordnum(num)
{
  switch(num) {
  case 0: return("zero "); break;
  case 1: return("one "); break;
  case 2: return("two "); break;
  case 3: return("three "); break;
  case 4: return("four "); break;
  case 5: return("five "); break;
  case 6: return("six "); break;
  case 7: return("seven "); break;
  case 8: return("eight "); break;
  case 9: return("nine "); break;
  case 10: return("ten "); break;
  default: return(""); break;
  }
}
