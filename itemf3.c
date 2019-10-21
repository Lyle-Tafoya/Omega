/* omega copyright (C) 1987,1988,1989 by Laurence Raphael Brothers */
/* itemf3.c */

/* mostly artifact and stick functions */

#include "glob.h"

/* amulet of the planes */
void i_planes(o)
pob o;
{
  if (Player.mana < 1) print1("The amulet spits some multicolored sparks.");
  else {
    print1("You focus mana into the amulet....");
    Player.mana = max(0,Player.mana-100);
    dataprint();
    morewait();
    strategic_teleport(1);
  }
}


/* the sceptre of high magic */
void i_sceptre(o)
pob o;
{
  if (HiMagicUse == Date) 
    print1("The Sceptre makes a sort of dull 'thut' noise.");
  else if (Current_Environment == E_CIRCLE || Current_Environment == E_ASTRAL)
  {
    HiMagicUse = Date; /* WDT: this looks like it's a good place to use
                        * the batteries. */
    print1("The Sceptre warps strangely for a second, and then subsides.");
    morewait();
    print2("You smell ozone."); /* WDT: explain the battery use. */
  }
  else {
    HiMagicUse = Date;
    print1("With a shriek of tearing aether, a magic portal appears!");
    print2("Step through? [yn] ");
    if (ynq()=='y') change_environment(E_COURT);
    print1("The sceptre seems to subside. You hear a high whine, as of");
    print2("capacitors beginning to recharge.");
    morewait();
  }
}


/* the star gem */
void i_stargem(o)
pob o;
{
  if (StarGemUse == Date) {
    print1("The Star Gem glints weakly as if to say:");
    print2("'You have used me overmuch.'");
    print3("and it vanishes a puff of regret.");
    Objects[o->id].uniqueness = UNIQUE_UNMADE;
		/* it's now out there, somewhere */
    dispose_lost_objects(1,o);
  }
  else {
    StarGemUse = Date;
    if (o->blessing < 1) {
      print1("The Star Gem shines brightly and emits a musical tone.");
      print2("You see a dark cloud roil away from it.");
      morewait();
      o->blessing = 10;
    }
    print1("The star gem flares with golden light!");
    morewait();
    if (Player.status[ILLUMINATION] < 1000) {
      print1("Interesting, you seem to be permanently accompanied");
      print2("by a friendly lambent glow....");
      morewait();
      Player.status[ILLUMINATION] = 1500;
    }
    print1("You suddenly find yourself whisked away by some unknown force!");
    morewait();
    setgamestatus(COMPLETED_ASTRAL);
    change_environment(E_COUNTRYSIDE);
    Player.x = 61;
    Player.y = 3;
    screencheck(3);
    drawvision(Player.x,Player.y);
    locprint("Star Peak");
    Country[Player.x][Player.y].current_terrain_type =
      Country[Player.x][Player.y].base_terrain_type;
    c_set(Player.x, Player.y, CHANGED);
    print2("The Star Gem's brilliance seems to fade.");
  }
}


/* wand of fear */
void i_fear(o)
pob o;
{
  int x=Player.x,y=Player.y;
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  inflict_fear(x,y);
}

	

void i_juggernaut(o)
pob o;
{
  int d,x=Player.x,y=Player.y;
  int seen = 1, not_seen = 0;
  int tunneled = 0;

  print1("You activate the Juggernaut of Karnak!");
  if (! o->known) {
    print2("Uh, oh, it's coming this way!");
    p_death("the Juggernaut of Karnak");
  }
  else {
    d = getdir();
    if (d == ABORT)
      print2("You deactivate the Juggernaut before it escapes.");
    else {
      print1("Vroom! ");
      while (inbounds(x+Dirs[0][d],y+Dirs[1][d])) {
	x+=Dirs[0][d];
	y+=Dirs[1][d];
	if (!view_unblocked(x, y) || offscreen(y))
	  seen = 0;
	if (Level->site[x][y].locchar == WALL)
	  tunneled++;
	if (Level->site[x][y].locchar != WATER &&
	  Level->site[x][y].locchar != VOID_CHAR &&
	  Level->site[x][y].locchar != ABYSS &&
	  Level->site[x][y].locchar != SPACE &&
	  Level->site[x][y].locchar != LAVA)
	{
	  Level->site[x][y].locchar = FLOOR;
	  Level->site[x][y].p_locf = L_NO_OP;
	}
	lreset(x, y, SECRET);
	lset(x, y, CHANGED);
	if (Level->site[x][y].creature != NULL) {
	  if (seen)
	    nprint1("Splat! ");
	  else
	    not_seen++;
	  setgamestatus(SUPPRESS_PRINTING);
	  m_death(Level->site[x][y].creature);
	  resetgamestatus(SUPPRESS_PRINTING);
	}
	plotspot(x, y, FALSE);
	omshowcursor(x, y);
      }
      if (not_seen > 6)
	print2("You hear many distant screams...");
      else if (not_seen > 3)
	print2("You hear several distant screams...");
      else if (not_seen > 1)
	print2("You hear a couple of distant screams...");
      else if (not_seen == 1)
	print2("You hear a distant scream...");
      gain_experience(1000);
      dispose_lost_objects(1,o);
      Level->tunnelled += tunneled - 1;
      tunnelcheck();
    }
  }
}


void i_symbol(o)
pob o;
{
  int i;
  if (! o->known)
    print1("Nothing seems to happen.");
  /* if o->charge != 17, then symbol was stolen from own high priest! */
  else if ((o->aux != Player.patron) || (o->charge != 17)){
    print1("You invoke the deity...");
    print2("...who for some reason seems rather annoyed at you...");
    print3("You are enveloped in Godsfire!");
    morewait();
    for(;Player.hp>1;Player.hp--) {
      dataprint();
      morewait();
      for(i=0;i<MAXITEMS;i++)
	if (Player.possessions[i] != NULL)
	  dispose_lost_objects(Player.possessions[i]->number,
			       Player.possessions[i]);
      Player.mana = 0;
    }
  }
  else if (SymbolUseHour == hour()) {
    print1("Your deity frowns upon this profligate use of power...");
    print2("Shazam! A bolt of Godsfire! Your symbol shatters!");
    dispose_lost_objects(1,o);
    Player.hp = 1;
    dataprint();
  }
  else {
    print1("A mystic flow of theurgic energy courses through your body!");
    SymbolUseHour = hour();
    cleanse(1);
    heal(10);
    Player.mana = max(Player.mana,calcmana());
  }
}



void i_crystal(o)
pob o;
{
  if (!o->known) print1("You can't figure out how to activate this orb.");
  else {
    print1("You gaze into your crystal ball.");
    if (ViewHour == hour()) print2("All you get is Gilligan's Island reruns.");
    else if ((o->blessing<0) || (Player.iq+Player.level < random_range(30))) {
      ViewHour = hour();
      print2("Weird interference patterns from the crystal fog your mind....");
      amnesia();
    }
    else {
      ViewHour = hour();
      print2("You sense the presence of life...");
      mondet(1);
      morewait();
      print2("You sense the presence of objects...");
      objdet(1);
      morewait();
      print2("You begin to see visions of things beyond your ken....");
      hint();
    }
  }
}
    
void i_antioch(o)
pob o;
{
  int x=Player.x,y=Player.y;
  int count;
  if (!o->known){
    print1("Ka-Boom!");
    print2("You seem to have annihilated yourself.");
    p_death("the Holy Hand-Grenade of Antioch");
  }
  else {
    print1("Bring out the Holy Hand-Grenade of Antioch!");
    setspot(&x,&y);
    print2("Ok, you pull the pin.....");
    morewait();
    print1("What do you count up to? ");
    count = (int) parsenum();
    if ((count < 3)&&(Level->site[x][y].creature!=NULL)) {
      print1("`Three shall be the number of thy counting....");
      print2("And the number of thy counting shall be three.'");
      print3("Your target picks up the grenade and throws it back!");
      morewait();
      clearmsg();
      print1("Ka-Boom!");
      p_death("the Holy Hand-Grenade of Antioch");
    }
    else if (count>3) {
      print1("`Three shall be the number of thy counting.");
      print2("And the number of thy counting shall be three.'");
      morewait();
      clearmsg();
      print1("Ka-Boom!");
      p_death("the Holy Hand-Grenade of Antioch");
    }
    else {
      print1("Ka-Boom!");
      gain_experience(1000);
      Level->site[x][y].locchar = TRAP;
      Level->site[x][y].p_locf = L_TRAP_DOOR;
      lset(x, y, CHANGED);
      if (Level->site[x][y].creature != NULL) {
	m_death(Level->site[x][y].creature);
	print2("You are covered with gore.");
      }
      Level->site[x][y].things = NULL;
    }
  }
  dispose_lost_objects(1,o);
}

void i_kolwynia(o)
pob o;
{
  int i;
  if (! o->known) {
    print1("You destroy youself with a mana storm. How sad.");
    p_death("Kolwynia, The Key That Was Lost");
  }
  else {
    gain_experience(5000);
    print1("You seem to have gained complete mastery of magic.");
    Player.pow = Player.maxpow = 2*Player.maxpow;
    for(i=0;i<NUMSPELLS;i++)
      Spells[i].known = TRUE;
  }
  dispose_lost_objects(1,o);
}

void i_enchantment(o)
pob o;
{
  char response;
  if (ZapHour == hour()) 
    print1("The staff doesn't seem to have recharged yet.");
  else if (! o->known) {
    ZapHour = hour();
    print1("You blast the staff backwards....");
    dispel(-1);
  }
  else {
    ZapHour = hour();
    print1("Zap with white or black end [wb] ");
    do response = (char) mcigetc();
    while ((response != 'w') && (response != 'b'));
    print2("The staff discharges!");
    if (response == 'w') enchant(o->blessing*2+1);
    else dispel(o->blessing);
  }
}

void i_helm(o)
pob o;
{
  if (HelmHour == hour()) 
    print1("The helm doesn't seem to have recharged yet.");
  else if (! o->known) {
    HelmHour = hour();
    print1("You put the helm on backwards....");
    p_teleport(-1);
  }
  else {
    HelmHour = hour();
    print1("Your environment fades.... and rematerializes.");
    p_teleport(o->blessing);
  }
}
 

void i_death(o)
pob o;
{
  clearmsg();
  print1("Bad move...");
  p_death("the Potion of Death");
}

void i_life(o)
pob o;
{
  clearmsg();
  print1("Good move.");
  Player.hp = Player.maxhp = 2 * Player.maxhp;
  dispose_lost_objects(1,o);
}



/* f = fire, w = water, e = earth, a = air, m = mastery */
int orbcheck(element)
char element;
{
  char response;
  print1("The orb begins to glow with increasing intensity!");
  print2("You have the feeling you need to do something more....");
  morewait();
  print1("Burn it in fire [f] ");
  print2("Douse it with water [w] ");
  morewait();
  print1("Smash it against the earth [e] ");
  print2("Toss is through the air [a] ");
  morewait();
  print1("Mix the above actions, doing them in sequence [m] ");
  do {
    print2("Which one [f,w,e,a,m] ");
    response = (char) mcigetc();
  } while ((response != 'f') &&
	   (response != 'w') &&
	   (response != 'e') &&
	   (response != 'a') &&
	   (response != 'm'));
  return(response == element);
}

/* orb functions */
void i_orbfire(o)
pob o;
{
  if (! orbcheck('f')) {
    print1("Bad choice!");
    print2("The Orb of Fire blasts you!");
    fball(Player.x,Player.y,Player.x,Player.y,250);
    o->known = 1;
  }
  else {
    print1("The Orb of Fire flares a brilliant red!");
    Spells[S_FIREBOLT].known = TRUE;
    gain_experience(10000);
    Player.immunity[FLAME]+=100;
    print2("You feel fiery!");
    o->plus = 100;
    o->blessing = 100;
    i_firebolt(o);
  }
  *o = Objects[ARTIFACTID+5];
}


void i_orbwater(o)
pob o;
{
  if (! orbcheck('w')) {
    print1("A serious mistake!");
    print2("The Orb of Water blasts you!");
    heal(-250);
    o->known = 1;
  }
  else {
    print1("The Orb of Water pulses a deep green!");
    Spells[S_DISRUPT].known = TRUE;
    Player.immunity[POISON]+=100;
    gain_experience(10000);
    print2("You feel wet!");
    o->plus = 100;
    o->blessing = 100;
    i_disrupt(o);
  }
  *o = Objects[ARTIFACTID+5];
}






void i_orbearth(o)
pob o;
{
  int i;
  if (! orbcheck('e')) {
    print1("What a maroon!");
    print2("The Orb of Earth blasts you!");
    Player.con -= 10;
    if (Player.con < 3)
      p_death("congestive heart failure");
    else {
      print3("Your possessions disintegrate!");
      for (i=0;i<MAXITEMS;i++)
	if (Player.possessions[i] != NULL) 
	  dispose_lost_objects(Player.possessions[i]->number,
			       Player.possessions[i]);
      for (i=0;i<MAXPACK;i++) 
	if (Player.pack[i] != NULL) {
	  free((char *) Player.pack[i]);
	  Player.pack[i] = NULL;
	}
      Player.packptr = 0;
      o->known = 1;
    }
  }
  else {
    print1("The Orb of Earth emanates a brownish aura!");
    Spells[S_DISINTEGRATE].known = TRUE;
    gain_experience(10000);
    Player.immunity[NEGENERGY]+=100;
    print2("You feel earthy!");
    o->plus = 100;
    o->blessing = 100;
    i_disintegrate(o);
  }
  *o = Objects[ARTIFACTID+5];
}


void i_orbair(o)
pob o;
{
  if (! orbcheck('a')) {
    print1("You lose!");
    print2("The Orb of Air blasts you!");
    lball(Player.x,Player.y,Player.x,Player.y,100);
    o->known = 1;
  }
  else {
    print1("The Orb of Air flashes blue!");
    Spells[S_LBALL].known = TRUE; /* lball */
    gain_experience(10000);
    print2("You feel airy!");
    Player.immunity[ELECTRICITY]+=100;
    o->plus = 100;
    o->blessing = 100;
    i_invisible(o);
    i_lbolt(o);
  }
  *o = Objects[ARTIFACTID+5];
}


void i_orbmastery(o)
pob o;
{

  if (! orbcheck('m')) {
    print1("A fatal error!");
    print2("The Orb of Mastery blasts you to cinders!");
    p_death("playing with the Orb of Mastery");
    o->known = 1;
  }
  else if ((find_and_remove_item(ARTIFACTID+1,-1)) &&
	   (find_and_remove_item(ARTIFACTID+2,-1)) &&
	   (find_and_remove_item(ARTIFACTID+3,-1)) &&
	   (find_and_remove_item(ARTIFACTID+4,-1))) {
    print1("The Orb of Mastery radiates rainbow colors!");
    print2("You feel godlike.");
    Player.iq = Player.maxiq = 2 * Player.maxiq;
    Player.pow = Player.maxpow = 2 * Player.maxpow;
    Player.str = Player.maxstr = 2 * Player.maxstr;
    Player.dex = Player.maxdex = 2 * Player.maxdex;
    Player.con = Player.maxcon = 2 * Player.maxcon;
    Player.agi = Player.maxagi = 2 * Player.maxagi;
    dataprint();
    morewait();
    print1("You have been imbued with a cosmic power....");
    morewait();
    wish(1);
    morewait();
    clearmsg();
    print2("You feel much more experienced.");
    gain_experience(20000);
    *o = Objects[ARTIFACTID+5];
  }
  else {
    print1("The Orb of Mastery's power is unbalanced!");
    print2("The Orb of Mastery blasts you to cinders!");
    p_death("playing with the Orb of Mastery");
  }  
}


void i_orbdead(o)
pob o;
{
  int i;
  print1("The burnt-out orb drains all your energy!");
  for(i=0;i<NUMSPELLS;i++)
    Spells[i].known = FALSE;
  print2("You feel not at all like a mage.");
  for(i=0;i<MAXITEMS;i++) {
    if (Player.possessions[i] != NULL) {
      Player.possessions[i]->plus = 0;
      if (Player.possessions[i]->usef > 100)
	Player.possessions[i]->usef = I_NOTHING;
    }
  }
  print3("A storm of mundanity surounds you!");
  level_drain(Player.level-1,"a Burnt-out Orb");
  Player.mana = 0;
  Player.pow -= 10;
}



void i_dispel(o)
pob o;
{
  dispel((o->blessing > -1) ? o->blessing+random_range(3): o->blessing);
}


/* stick functions */


/* wand of apportation */
void i_apport(o)
pob o;
{
  o->known = max(1,o->known);
  Objects[o->id].known = 1;
  apport(o->blessing);
}

/* staff of firebolts */
void i_firebolt(o)
pob o;
{
  int x=Player.x,y=Player.y;
  o->known = max(1,o->known);
  Objects[o->id].known = 1;
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  fbolt(Player.x,Player.y,x,y,Player.dex*2+Player.level,75);
}


void i_disintegrate(o)
pob o;
{
  int x=Player.x,y=Player.y;
  o->known = max(1,o->known);
  Objects[o->id].known = 1;
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  disintegrate(x,y);
}

void i_disrupt(o)
pob o;
{
  int x=Player.x,y=Player.y;
  o->known = max(1,o->known);
  Objects[o->id].known = 1;
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  disrupt(x,y,100);
}


/* staff of lightning bolts */
void i_lbolt(o)
pob o;
{
  int x=Player.x,y=Player.y;
  o->known = max(1,o->known);
  Objects[o->id].known = 1;
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  lbolt(Player.x,Player.y,x,y,Player.dex*2+Player.level,75);
}

/* wand of magic missiles */
void i_missile(o)
pob o;
{
  int x=Player.x,y=Player.y;
  o->known = max(1,o->known);
  Objects[o->id].known = 1;
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  nbolt(Player.x,Player.y,x,y,Player.dex*2+Player.level,20);
}

/* wand of fire balls */
void i_fireball(o)
pob o;
{
  int x=Player.x,y=Player.y;
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  fball(Player.x,Player.y,x,y,35);
}


/* wand of snowballs */
void i_snowball(o)
pob o;
{
  int x=Player.x,y=Player.y;
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  snowball(Player.x,Player.y,x,y,20);
}

/* wand of lightning balls */
void i_lball(o)
pob o;
{
  int x=Player.x,y=Player.y;
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  lball(Player.x,Player.y,x,y,50);
}

/* staff of sleep */
void i_sleep_other(o)
pob o;
{
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  sleep_monster(o->blessing);
}

/* rod of summoning */
/* rod of summoning now always summons as if cursed */
void i_summon(o)
pob o;
{
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  summon(-1,-1);
}

void i_hide(o)
pob o;
{
  int x=Player.x,y=Player.y;
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  setspot(&x,&y);
  hide(x,y);
}

void i_polymorph(o)
pob o;
{
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  polymorph(o->blessing);
}
