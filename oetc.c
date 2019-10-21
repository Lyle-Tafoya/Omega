/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988 */
/* oetc.c */
/* grab bag of random functions used in random places */

#include "oglob.h"

/* there are various ways for the player to receive one of these hints */
void hint()
{
  switch(random_range(95)) {
  case 0:mprint("There is an entrance to the sewers in the Garden.");break;
  case 1:mprint("Statues can be dangerous.");break;
  case 2:mprint("Unidentified Artifacts can be dangerous.");break;
  case 3:mprint("The higher form of mercury is desirable.");break;
  case 4:mprint("A sense of unease is a good thing to have.");break;
  case 5:mprint("If you dig too much, you might cause a cave in!.");break;
  case 6:mprint("Be Lawful: Live and Let Live.");break;
  case 7:mprint("Be Chaotic: Live and Let Die."); break;
  case 8:mprint("The world doesn't slow down; you speed up.");break;
  case 9:mprint("Security is a sense of dislocation.");break;
  case 10:mprint("Tullimore Dew is a panacea.");break;
  case 11:mprint("Thieves hide behind closed doors.");break;
  case 12:mprint("`No jail is escapeproof' -- John Dillinger.");break;
  case 13:mprint("Oh, to have an apartment of your own!");break;
  case 14:mprint("Some homes have money and treasure.");break;
  case 15:mprint("Some homes are defended.");break;
  case 16:mprint("Sometimes you could just wish for Death."); break;
  case 17:mprint("A cursed wish can be fatal.");break;
  case 18:mprint("The way you play, you should wish for Skill."); break;
  case 19:mprint("A druid might wish for Balance."); break;
  case 20:mprint("Mages always wish for Knowledge.");break;
  case 21:mprint("Some fairies are good.");break;
  case 22:mprint("An affair with a demon can be heartbreaking."); break;
  case 23:mprint("The Explorer's Club knows a useful spell."); break;
  case 24:mprint("They say some famous people live in mansions."); break;
  case 25:mprint("Magic pools are totally random."); break;
  case 26:mprint("There are five elements, including Void."); break;
  case 27:mprint("Humans can be good or evil, lawful or chaotic."); break;
  case 28:mprint("There are many kinds of wishes. Case counts, you know.");
    break;
  case 29:mprint("There are caves due south of Rampart"); break;
  case 30:mprint("Donaldson's Giants can withstand lava.");break;
  case 31:mprint("Ritual magic can have many different effects.");break;
  case 32:mprint("The Mercenaries are the best equipped fighters."); break;
  case 33:mprint("The Gladiators are the most skilled fighters."); break;
  case 34:mprint("Rent a flat and lose any bad stati you may have."); break;
  case 35:mprint("Some junk may be worth a fortune if identified."); break;
  case 36:mprint("Identify humans by talking to them."); break;
  case 37:mprint("They say the Duke has a treasure trove."); break;
  case 38:mprint("If you yield, your opponent will gain experience."); break;
  case 39:mprint("The Dragon Lord lives in the Waste of Time."); break;
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
  case 50:mprint("They say there's a red-light district in town."); break;
  case 51:mprint("The House of the Eclipse is behind a closed door."); break;
  case 52:mprint("The Orbs may be encountered on the Astral Plane."); break;
  case 53:mprint("The Champion should never refuse a challenge."); break;
  case 54:mprint("They say that the autoteller program is buggy."); break;
  case 55:mprint("It's better not to sleep on the ground."); break;
  case 56:mprint("Try ritual magic in different kinds of rooms."); break;
  case 57:mprint("Breaking down a wall by bashing it is a bad idea!"); break;
  case 58:mprint("Follow the Oracle's advice; she is all-wise."); break;
  case 59:mprint("The ArchDruid lives in the northern forest.");break;
  case 60:mprint("A search of the mountains may reveal a secret pass.");break;
  case 61:mprint("Star Peak is to the far North-East."); break;
  case 62:mprint("The Archmage lives in the far North-West beyond a pass.");
    break;
  case 63:mprint("There is a volcano in the southern marshes."); break;
  case 64:mprint("The Demon Emperor resides in the Volcano."); break;
  case 65:mprint("The Lawgiver can be found at Star Peak."); break;
  case 66:mprint("The Temple of Athena is to the North-East."); break;
  case 67:mprint("The Temple of Set can be found in a desert.");break;
  case 68:mprint("The Temple of Hecate is in the swamp."); break;
  case 69:mprint("The Temple of Odin is to the South in some mountains.");
    break;
  case 70:mprint("There is a curious island off a promontory of the swamp.");
    break;
  case 71:mprint("The Eater of Magic can be found on an island.");break;
  case 72:mprint("The Temple of Destiny is practically inaccessible.");break;
  case 73:mprint("Each sect has its own main temple outside the city.");break;
  case 74:mprint("The aligned temples are dangerous to unbelievers.");break;
  case 75:mprint("If you are poor, maybe you should wish for Wealth.");break;
  case 76:mprint("Need mana? Wish for Power.");break;
  case 77:mprint("Wishing for Law, Balance, or Chaos alters alignment.");break;
  case 78:mprint("Feeling out of sorts? Wish for Health.");break;
  case 79:mprint("Challenge the abyss at the Temple of Destiny.");break;
  case 80:mprint("The Circle of Sorcerors has an Astral HQ");break;
  case 81:mprint("The Star Gem is the only way back from the Astral Plane.");
    break;
  case 82:mprint("The Star Gem is guarded by the Circle of Sorcerors.");break;
  case 83:mprint("The Star Gem is rightfully the property of the LawBringer.");
    break;
  case 84:mprint("They say the Demon Emperor owns the Amulet of the Planes.");
    break;
  case 85:mprint("An Amulet might get you to the Temple of Destiny.");break;
  case 86:mprint("A wish for Location might help you become Adept.");break;
  case 87:mprint("Some Artifacts may be used only once per day.");break;
  case 88:mprint("Overusing Artifacts can be a bad move."); break;
  case 89:mprint("You might starve in the Swamp or the Mountains!");break;
  case 90:mprint("You would have to be very chaotic to attack a guard!");break;
  case 91:mprint("You would have to be very foolhardy to attack a guard!");
    break;
  case 92:mprint("Only a master of chaos would kill all the city guards!");
    break;
  case 93:mprint("The Order depends on the force of the LawGiver");break;
  case 94:mprint("City Guards are employees of the Order");break;
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
	Spells[S_REGENERATE].known = TRUE;
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
	Spells[S_SHADOWFORM].known = TRUE;
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
	Spells[S_RESTORE].known = TRUE;
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
char *nameprint()
{
  switch(random_range(40)) {
  case 0:strcpy(Str3,"Orion Splash");break;
  case 1:strcpy(Str3,"Gorgar");break;
  case 2:strcpy(Str3,"Hieronymous");break;
  case 3:strcpy(Str3,"Quantifor Quotron");break;
  case 4:strcpy(Str3,"Leon");break;
  case 5:strcpy(Str3,"Joyce");break;
  case 6:strcpy(Str3,"Leticia Smiley");break;
  case 7:strcpy(Str3,"Ogilvy the Grim");break;
  case 8:strcpy(Str3,"Salara Storn");break;
  case 9:strcpy(Str3,"Murgo");break;
  case 10:strcpy(Str3,"Jonathan Atwilder");break;
  case 11:strcpy(Str3,"Xylos the Tan");break;
  case 12:strcpy(Str3,"Terence");break;
  case 13:strcpy(Str3,"Toronado");break;
  case 14:strcpy(Str3,"Kelly");break;
  case 15:strcpy(Str3,"Cantinflas");break;
  case 16:strcpy(Str3,"Ixel");break;
  case 17:strcpy(Str3,"Toto");break;
  case 18:strcpy(Str3,"Frost");break;
  case 19:strcpy(Str3,"Aliera Erinyes");break;
  case 20:strcpy(Str3,"Godel");break;
  case 21:strcpy(Str3,"Kerst Blackblade");break;
  case 22:strcpy(Str3,"Ebenezer");break;
  case 23:strcpy(Str3,"Jeremiah");break;
  case 24:strcpy(Str3,"Triskelion Shadow");break;
  case 25:strcpy(Str3,"Eleskir Eremar");break;
  case 26:strcpy(Str3,"Tyron");break;
  case 27:strcpy(Str3,"Morgon");break;
  case 28:strcpy(Str3,"Achmed");break;
  case 29:strcpy(Str3,"Chin");break;
  case 30:strcpy(Str3,"Fujimoto");break;
  case 31:strcpy(Str3,"Dos Santos");break;
  case 32:strcpy(Str3,"Federico");break;
  case 33:strcpy(Str3,"Jaime");break;
  case 34:strcpy(Str3,"Siobhan");break;
  case 35:strcpy(Str3,"Hans");break;
  case 36:strcpy(Str3,"Gurkov");break;
  case 37:strcpy(Str3,"Krilos the Slayer");break;
  case 38:strcpy(Str3,"Oxxblud");break;
  case 39:strcpy(Str3,"Dorian");break;
  }
  return(Str3);
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
