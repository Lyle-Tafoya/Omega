/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* effect2.c */

#include "glob.h"



void knowledge(blessing)
int blessing;
{
  if (blessing < 0)
    mprint("You feel ignorant.");
  else {
    mprint("You feel knowledgeable!");
    menuclear();
    menuprint("Current Point Total: ");
    menulongprint(calc_points());
    menuprint("\nAlignment:");
    if (Player.alignment == 0)
      menuprint("Neutral, embodying the Cosmic Balance");
    else if (abs(Player.alignment) < 10)
      menuprint("Neutral, tending toward ");
    else if (abs(Player.alignment) < 50)
      menuprint("Neutral-");
    else if (abs(Player.alignment) < 100) ;
    else if (abs(Player.alignment) < 200)
      menuprint("Servant of ");
    else if (abs(Player.alignment) < 400)
      menuprint("Master of ");
    else if (abs(Player.alignment) < 800)
      menuprint("The Essence of ");
    else menuprint("The Ultimate Avatar of ");
    if (Player.alignment < 0) menuprint("Chaos\n");
    else if (Player.alignment > 0) menuprint("Law\n");
    showmenu();
    morewait();
    menuclear();
    menuprint("Current stati:\n");
    if (Player.status[BLINDED])
      menuprint("Blinded\n");
    if (Player.status[SLOWED])
      menuprint("Slowed\n");
    if (Player.status[HASTED])
      menuprint("Hasted\n");
    if (Player.status[DISPLACED])
      menuprint("Displaced\n");
    if (Player.status[SLEPT])
      menuprint("Slept\n");
    if (Player.status[DISEASED])
      menuprint("Diseased\n");
    if (Player.status[POISONED])
      menuprint("Poisoned\n");
    if (Player.status[BREATHING])
      menuprint("Breathing\n");
    if (Player.status[INVISIBLE])
      menuprint("Invisible\n");
    if (Player.status[REGENERATING])
      menuprint("Regenerating\n");
    if (Player.status[VULNERABLE])
      menuprint("Vulnerable\n");
    if (Player.status[BERSERK])
      menuprint("Berserk\n");
    if (Player.status[IMMOBILE])
      menuprint("Immobile\n");
    if (Player.status[ALERT])
      menuprint("Alert\n");
    if (Player.status[AFRAID])
      menuprint("Afraid\n");
    if (Player.status[ACCURATE])
      menuprint("Accurate\n");
    if (Player.status[HERO])
      menuprint("Heroic\n");
    if (Player.status[LEVITATING])
      menuprint("Levitating\n");
    if (Player.status[TRUESIGHT]) /* FIXED! 12/30/98 DG */
      menuprint("Sharp\n");
    if (Player.status[SHADOWFORM])
      menuprint("Shadowy\n");
    if (Player.status[ILLUMINATION])
      menuprint("Glowing\n");
    if (Player.status[DEFLECTION])
      menuprint("Buffered\n");
    if (Player.status[RETURNING])
      menuprint("Returning\n");
    showmenu();
    morewait();
    menuclear();
    menuprint("Immunities:\n");
    if (p_immune(NORMAL_DAMAGE))
      menuprint("Normal Damage\n");
    if (p_immune(FLAME))
      menuprint("Flame\n");
    if (p_immune(ELECTRICITY))
      menuprint("Electricity\n");
    if (p_immune(COLD))
      menuprint("Cold\n");
    if (p_immune(POISON))
      menuprint("Poison\n");
    if (p_immune(ACID))
      menuprint("Acid\n");
    if (p_immune(FEAR))
      menuprint("Fear\n");
    if (p_immune(SLEEP))
      menuprint("Sleep\n");
    if (p_immune(NEGENERGY))
      menuprint("Negative Energies\n");
    if (p_immune(THEFT))
      menuprint("Theft\n");
    if (p_immune(GAZE))
      menuprint("Gaze\n");
    if (p_immune(INFECTION))
      menuprint("Infection\n");
    showmenu();
    morewait();
    menuclear();
    menuprint("Ranks:\n");
    switch(Player.rank[LEGION]) {
    case COMMANDANT:
      menuprint("Commandant of the Legion"); 
      break;
    case COLONEL:    
      menuprint("Colonel of the Legion");
      break;
    case FORCE_LEADER:
      menuprint("Force Leader of the Legion");
      break;
    case CENTURION:
      menuprint("Centurion of the Legion");
      break;
    case LEGIONAIRE:
      menuprint("Legionaire");
      break;
    }
    if (Player.rank[LEGION] > 0) {
      menuprint(" (");
      menunumprint(Player.guildxp[LEGION]);
      menuprint(" XP).\n");
    }
    switch(Player.rank[ARENA]) {
    case -1:
      menuprint("Ex-gladiator\n");
      break;
    case CHAMPION:
      menuprint("Gladiator Champion");
      break;
    case GLADIATOR:    
      menuprint("Gladiator of the Arena");
      break;
    case RETIARIUS:
      menuprint("Retiarius of the Arena");
      break;
    case BESTIARIUS:
      menuprint("Bestiarius of the Arena");
      break;
    case TRAINEE:
      menuprint("Gladiator Trainee of the Arena");
      break;
    }
    if (Player.rank[ARENA] > 0) {
      menuprint(" (Opponent ");
      menunumprint(Arena_Opponent);
      menuprint(")\n");
    }
    switch(Player.rank[COLLEGE]) {
    case ARCHMAGE:
      menuprint("Archmage of the Collegium Magii");
      break;
    case MAGE:
      menuprint("Collegium Magii: Mage");
      break;
    case PRECEPTOR:
      menuprint("Collegium Magii: Preceptor");
      break;
    case STUDENT:
      menuprint("Collegium Magii: Student");
      break;
    case NOVICE:
      menuprint("Collegium Magii: Novice");
      break;
    }
    if (Player.rank[COLLEGE] > 0) {
      menuprint(" (");
      menunumprint(Player.guildxp[COLLEGE]);
      menuprint(" XP).\n");
    }
    switch(Player.rank[NOBILITY]) {
    case DUKE:
      menuprint("Duke of Rampart");
      break;
    case LORD:
      menuprint("Peer of the Realm");
      break;
    case KNIGHT:
      menuprint("Order of the Knights of Rampart");
      break;
    case ESQUIRE:
      menuprint("Squire of Rampart");
      break;
    case COMMONER:
      menuprint("Commoner");
      break;
    default:
      menuprint("Lowly Commoner\n");
      break;
    }
    if (Player.rank[NOBILITY] > 1) {
      menuprint(" (");
      menunumprint(Player.rank[NOBILITY] - 1);
      menuprint(ordinal(Player.rank[NOBILITY] - 1));
      menuprint(" Quest Completed)\n");
    }
    else if (Player.rank[NOBILITY] == 1) {
      menuprint(" (1st Quest Undertaken)\n");
    }
    switch(Player.rank[CIRCLE]) {
    case -1:
      menuprint("Former member of the Circle.\n");
      break;
    case PRIME:
      menuprint("Prime Sorceror of the Inner Circle");
      break;
    case HIGHSORCEROR:
      menuprint("High Sorceror of the Inner Circle");
      break;
    case SORCEROR:
      menuprint("Member of the Circle of Sorcerors");
      break;
    case ENCHANTER:
      menuprint("Member of the Circle of Enchanters");
      break;
    case INITIATE:
      menuprint("Member of the Circle of Initiates");
      break;
    }
    if (Player.rank[CIRCLE] > 0) {
      menuprint(" (");
      menunumprint(Player.guildxp[CIRCLE]);
      menuprint(" XP).\n");
    }
    switch(Player.rank[ORDER]) {
    case -1:
      menuprint("Washout from the Order of Paladins\n");
      break;
    case JUSTICIAR:
      menuprint("Justiciar of the Order of Paladins");
      break;
    case PALADIN:
      menuprint("Paladin of the Order");
      break;
    case CHEVALIER:
      menuprint("Chevalier of the Order");
      break;
    case GUARDIAN:
      menuprint("Guardian of the Order");
      break;
    case GALLANT:
      menuprint("Gallant of the Order");
      break;
    }
    if (Player.rank[ORDER] > 0) {
      menuprint(" (");
      menunumprint(Player.guildxp[ORDER]);
      menuprint(" XP).\n");
    }
    switch(Player.rank[THIEVES]) {
    case SHADOWLORD:
      menuprint("Guild of Thieves: Shadowlord");
      break;
    case TMASTER:
      menuprint("Guild of Thieves: Master Thief");
      break;
    case THIEF:
      menuprint("Guild of Thieves: Thief");
      break;
    case ATHIEF:
      menuprint("Guild of Thieves: Apprentice Thief");
      break;
    case TMEMBER:
      menuprint("Guild of Thieves: Candidate Member");
      break;
    }
    if (Player.rank[THIEVES] > 0) {
      menuprint(" (");
      menunumprint(Player.guildxp[THIEVES]);
      menuprint(" XP).\n");
    }
    switch(Player.rank[PRIESTHOOD]) {
      case LAY:
        menuprint("A lay devotee of ");
	break;
      case ACOLYTE:
        menuprint("An Acolyte of ");
	break;
      case PRIEST:
        menuprint("A Priest of ");
	break;
      case SPRIEST:
        menuprint("A Senior Priest of ");
	break;
      case HIGHPRIEST:
        menuprint("The High Priest of ");
	break;
      }
    switch(Player.patron) {
      case ODIN:
        menuprint("Odin");
	break;
      case SET:
        menuprint("Set");
	break;
      case ATHENA:
        menuprint("Athena");
	break;
      case HECATE:
        menuprint("Hecate");
	break;
      case DRUID:
        menuprint("Druidism");
	break;
      case DESTINY:
        menuprint("the Lords of Destiny");
	break;
    }
    if (Player.rank[PRIESTHOOD] > 0) {
      menuprint(" (");
      menunumprint(Player.guildxp[PRIESTHOOD]);
      menuprint(" XP).\n");
    }
    if (Player.rank[ADEPT] > 0) 
      menuprint("**************\n*Omegan Adept*\n**************\n");
    showmenu();
    morewait();
    xredraw();
  }
}


/* Recreates the current level */
void flux(blessing)
int blessing;
{
  mprint("The universe warps around you!");
  if (Current_Environment == E_CITY) {
    mprint("Sensing dangerous high order magic, the Collegium Magii");
    mprint("and the Circle of Sorcerors join forces to negate the spell.");
    mprint("You are zapped by an antimagic ray!!!");
    dispel(-1);
    mprint("The universe unwarps itself....");
  }
  else if (Current_Environment != Current_Dungeon)
    mprint("Odd.... No effect!");
  else {
    mprint("You stagger as the very nature of reality warps!");
    erase_level();
    Level->generated = FALSE;
    mprint("The fabric of spacetime reknits....");
    change_level(Level->depth-1,Level->depth,TRUE);
  }
}

/*Turns on displacement status for the player */
void displace(blessing)
int blessing;
{
  if (blessing > -1) {
      mprint("You feel a sense of dislocation.");
      Player.status[DISPLACED] = blessing + random_range(6);
    }
  else {
    mprint("You feel vulnerable");
    Player.status[VULNERABLE] += random_range(6) - blessing;
  }
}

  
	
void invisible(blessing)
int blessing;
{  
  if (blessing > -1) {
    mprint("You feel transparent!");
    Player.status[INVISIBLE]+= 2+5*blessing;
  }
  else {
    mprint("You feel dangerous!");
    Player.status[VULNERABLE] +=
      random_range(10)+1;
  }
}


void warp(blessing)
int blessing;
{
  int newlevel;
  if (Current_Environment != Current_Dungeon) 
    mprint("How strange! No effect....");
  else {
    mprint("Warp to which level? ");
    newlevel = (int) parsenum();
    if (newlevel >= MaxDungeonLevels || blessing < 0 || newlevel < 1) {
      mprint("You have been deflected!");
      newlevel=random_range(MaxDungeonLevels - 1) + 1;
    }
    mprint("You dematerialize...");
    change_level(Level->depth,newlevel,FALSE);
  }
  roomcheck();
}

void alert(blessing)
 int blessing;
{
  if (blessing > -1) {
    mprint("You feel on-the-ball.");
    Player.status[ALERT]+= 4+(5*blessing);
  }
  else sleep_player(abs(blessing)+3);
}

void regenerate(blessing)
int blessing;
{
  if (blessing < 0)
    heal(blessing * 10);
  else {
    mprint("You feel abnormally healthy.");
    Player.status[REGENERATING] += (blessing+1)*50;
  }
}

void haste(blessing)
int blessing;
{
  if (blessing > -1) {
    if (! Player.status[HASTED]) 
      mprint("The world slows down!"); 
    else mprint("Nothing much happens.");
    if (Player.status[SLOWED]) 
      Player.status[SLOWED] = 0;
    Player.status[HASTED] += (blessing*100)+random_range(250);
  }
  else {
    mprint("You feel slower.");
    if (Player.status[HASTED] > 0) mprint("...but the feeling quickly fades.");
    else Player.status[SLOWED] += random_range(250)+250;
  }
}


void recover_stat(blessing)
int blessing;
{
  if (blessing < 0) {
    mprint("You feel a cold surge!");
    switch(random_range(6)) {
      case 0: Player.str = min(Player.str-1,Player.maxstr-1); break;
      case 1: Player.con = min(Player.con-1,Player.maxcon-1); break;
      case 2: Player.dex = min(Player.dex-1,Player.maxdex-1); break;
      case 3: Player.agi = min(Player.agi-1,Player.maxagi-1); break;
      case 4: Player.iq = min(Player.iq-1,Player.maxiq-1); break;
      case 5: Player.pow = min(Player.pow-1,Player.maxpow-1); break;
    }
  }
  else {
    mprint("You feel a warm tingle!");
    Player.str = max(Player.str,Player.maxstr);
    Player.con = max(Player.con,Player.maxcon);
    Player.dex = max(Player.dex,Player.maxdex);
    Player.agi = max(Player.agi,Player.maxagi);
    Player.iq = max(Player.iq,Player.maxiq);
    Player.pow = max(Player.pow,Player.maxpow);
    }
  calc_melee();
}

void augment(blessing)
int blessing;
{
  if (blessing < 0) {
    mprint("You feel a cold surge!");
    switch(random_range(6)) {
      case 0: Player.str = min(Player.str-1,Player.maxstr-1); break;
      case 1: Player.con = min(Player.con-1,Player.maxcon-1); break;
      case 2: Player.dex = min(Player.dex-1,Player.maxdex-1); break;
      case 3: Player.agi = min(Player.agi-1,Player.maxagi-1); break;
      case 4: Player.iq = min(Player.iq-1,Player.maxiq-1); break;
      case 5: Player.pow = min(Player.pow-1,Player.maxpow-1); break;
    }
  }
  else if (blessing == 0) {
    mprint("You feel a warm tingle!");
    switch(random_range(6)) {
      case 0: Player.str = max(Player.str+1,Player.maxstr+1); break;
      case 1: Player.con = max(Player.con+1,Player.maxcon+1); break;
      case 2: Player.dex = max(Player.dex+1,Player.maxdex+1); break;
      case 3: Player.agi = max(Player.agi+1,Player.maxagi+1); break;
      case 4: Player.iq = max(Player.iq+1,Player.maxiq+1); break;
      case 5: Player.pow = max(Player.pow+1,Player.maxpow+1); break;
    }
  }
  else {
    mprint("You feel a hot flash!");
    Player.str = max(Player.str+1,Player.maxstr+1);
    Player.con = max(Player.con+1,Player.maxcon+1);
    Player.dex = max(Player.dex+1,Player.maxdex+1);
    Player.agi = max(Player.agi+1,Player.maxagi+1);
    Player.iq = max(Player.iq+1,Player.maxiq+1);
    Player.pow = max(Player.pow+1,Player.maxpow+1);
  }
  calc_melee();
}

void breathe(blessing)
int blessing;
{
  if (blessing > -1) {
      mprint("Your breath is energized!");
      Player.status[BREATHING] += 6+blessing;
    }
  else {
    mprint("You choke as your lungs fill with water!");
    p_damage(50,UNSTOPPABLE,"drowning");
  }
}


void i_chaos(o)
pob o;
{
  if (Player.alignment < 0) {
    Player.alignment -= random_range(20);
    mprint("You feel deliciously chaotic!");
    gain_experience(abs(Player.alignment)*10);
  }
  else {
    mprint("You feel a sense of inner turmoil!");
    Player.alignment -= random_range(20);
  }
}

void i_law(o)
pob o;
{
  if (Player.alignment > 0) {
    Player.alignment += random_range(20);
    mprint("You feel wonderfully lawful!");
    gain_experience(Player.alignment*10);
  }
  else {
    mprint("You feel a sense of inner constraint!");
    Player.alignment += random_range(20);
  }
}

void sanctify(blessing)
int blessing;
{
  if (blessing > -1) {
    if (Level->environment == E_TEMPLE) 
      mprint("Odd, the spell has no effect. I wonder why.");
    else if (Level->site[Player.x][Player.y].locchar == ALTAR) 
      mprint("This site can't get any holier!");
    else if (Player.patron == 0) {
      mprint("The gods are angered!");
      Level->site[Player.x][Player.y].locchar = LAVA;
      Level->site[Player.x][Player.y].p_locf = L_LAVA;
      lset(Player.x, Player.y, CHANGED);
      p_movefunction(L_LAVA);
    }
    else {
      Level->site[Player.x][Player.y].locchar = ALTAR;
      Level->site[Player.x][Player.y].aux = Player.patron;
      Level->site[Player.x][Player.y].p_locf = L_ALTAR;
      lset(Player.x, Player.y, CHANGED);
      mprint("You are standing on sacred ground!");
    }
  }
  else {
    if (Level->site[Player.x][Player.y].locchar == ALTAR) {
      mprint("The altar crumbles before your unholy blast....");
      Level->site[Player.x][Player.y].locchar = FLOOR;
      Level->site[Player.x][Player.y].p_locf = L_NO_OP;
      lset(Player.x, Player.y, CHANGED);
      if (Level->site[Player.x][Player.y].aux == Player.patron) {
	mprint("Your deity is not amused....");
	p_damage(Player.hp-1,UNSTOPPABLE,"Divine Wrath");
      }
      else if ((Player.patron == ATHENA) || (Player.patron == ODIN)) {
	if ((Level->site[Player.x][Player.y].aux == SET) ||
	    (Level->site[Player.x][Player.y].aux == HECATE)) {
	  mprint("Your deity applauds the eradication of Chaos' taint");
	  gain_experience(1000);
	}
	else {
	  mprint("Your deity approves of your action.");
	  gain_experience(100);
	}
      }
      else if ((Player.patron == SET) || (Player.patron == HECATE)) {
	if ((Level->site[Player.x][Player.y].aux == ODIN) ||
	    (Level->site[Player.x][Player.y].aux == ATHENA)) {
	  mprint("Your deity applauds the obliteration of Law");
	  gain_experience(1000);
	}
	else {
	  mprint("Your deity approves of your action.");
	  gain_experience(100);
	}
      }
      else if (Player.patron == DRUID) {
	mprint("Your attempt to maintain the Balance is applauded....");
	gain_experience(250);
      }
      else mprint("Nothing much happens");
    }
    else mprint("You feel an aura of unholiness arising from this spot....");
  }
}

void accuracy(blessing)
int blessing;
{
  if (blessing > -1) {
      mprint("Everything seems covered in bullseyes!");
      Player.status[ACCURACY] += random_range(5)+1+blessing*5;
      calc_melee();
    }
  else {
    Player.status[ACCURACY]=0;
    calc_melee();
    mprint("Your vision blurs...");
  }
}
