/* omega copyright (c) 1987 by Laurence Raphael Brothers */
/* omonf.c */
/* monster: talk, move, melee, range, and special functions */

#include <strings.h>
#include "oglob.h"

/* from oscr */
extern void mprint(),printm(),prepmsg(),drawspot(),drawmon(),locprint();
extern void drawvision(),erase_level(),mnumprint();
extern void showflags(),menuprint(),plotspot();
extern char ynq();

/* from ocity */
extern void load_city();

/* from omovef */
extern void send_to_jail();

/* from oetc */
extern void hint();

/* from outil */
extern int random_range(),sign(),max(),distance(),m_unblocked(),bitp();
extern void do_los(),bitset(),bitreset();
extern int pow2();

/* from oaux */
extern void p_damage(),p_death(),drop_at(),p_poison(),p_teleport();
extern void sleep_player(),conform_lost_object(),darkenroom();
extern void gain_experience(),calc_melee();
extern char *itemid();
extern int p_immune();


/* from omove */
extern void l_whirlwind();

/* from oeffect (1 or 2) */
extern void lball(),fbolt(),fball(),summon(),aggravate(),findspace();
extern void acid_cloud(),snowball(),nbolt(),level_drain(),manastorm();
extern void bless(),acquire(),disrupt(),disintegrate(),wish();
extern void cure(),dispel(),haste(),enchant();



/* from oitem */
extern pob create_object();

/* from oinv */
extern void add_item_to_pack();

/* from omon */
extern int m_statusp(),m_immunityp();
extern int m_open_door();
extern void m_pulse(),m_status_set(),m_damage();
extern void m_death(),m_hit(),m_miss(),m_pickup(),m_dropstuff();
extern void m_status_reset(),monster_strike(),movemonster();
extern void monster_talk(),monster_melee(),monster_move(),monster_special();

/* omonf functions */
void m_normal_melee(),m_normal_move(),m_no_op();
void m_smart_move(),m_simple_move(),m_spirit_move(),m_vanish();
void m_fire_melee(),m_firebolt(),m_guard_talk(),m_random_move();
void m_flutter_move(),m_nbolt(),m_sp_surprise(),m_dragon_melee();
void m_stupid_talk(),m_hungry_talk(),m_silent_talk(),m_greedy_talk();
void m_mp_talk(),m_follow_move(),m_mp_melee(),m_sp_mp(),m_evil_talk();
void m_elec_melee(),m_lball(),m_fireball(),m_poison_melee(),m_titter();
void m_ng_melee(),m_sp_poison_cloud(),m_huge_sounds(),m_robot_talk();
void m_sleep_melee(),m_blind_strike(),m_im_talk(),m_confused_move();
void m_thief_f(),m_man_talk(),m_spectral_melee(),m_summon();
void m_teleport(),m_disease_melee(),m_illusion(),m_aggravate();
void m_sp_explode(),m_cold_melee(),m_sp_acid_cloud();
void m_sp_whirl(),m_sp_escape(),m_sp_ghost(),m_ghost_talk();
void m_mimsy_talk(),m_slithy_talk(),m_burble_talk(),m_sp_spell();
void m_scaredy_move(),m_good_fairy_talk(),m_evil_fairy_talk();
void m_sp_seductor(),m_demonlover_talk(),m_seductor_talk();
void m_sp_eater(),m_sp_dragonlord(),m_ninja_talk(),m_assassin_talk();
void m_sp_blackout();

void m_no_op(m)
struct monster *m;
{
}


void m_silent_talk(m)
struct monster *m;
{

  int reply = random_range(4);
  
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  switch (reply) {
    case 0:strcat(Str2," does not reply. "); break;
    case 1:strcat(Str2," shrugs silently. "); break;
    case 2:strcat(Str2," hold a finger to his mouth. "); break;
    case 3:strcat(Str2," glares at you but says nothing. "); break;
  }      
  mprint(Str2);
}

void m_stupid_talk(m)
struct monster *m;
{

  int reply = random_range(4);
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  switch (reply) {
    case 0:strcat(Str2," looks at you with mute incomprehension. "); break;
    case 1:strcat(Str2," growls menacingly and ignores you. "); break;
    case 2:strcat(Str2," does not seem to have heard you. "); break;
    case 3:strcat(Str2," could not be expected to talk!!!. "); break;
  }      
  mprint(Str2);
}

void m_greedy_talk(m)
struct monster *m;
{

  int reply = random_range(4);
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  switch (reply) {
    case 0:strcat(Str2," says: Give me a treasure.... ");break;
    case 1:strcat(Str2," says: Stand and deliver, knave! "); break;
    case 2:strcat(Str2," says: Your money or your life! "); break;
    case 3:strcat(Str2," says: Yield or Die! "); break;
  }      
  mprint(Str2);
}

void m_hungry_talk(m)
struct monster *m;
{

  int reply = random_range(4);
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  switch (reply) {
    case 0:strcat(Str2," says: I hunger, foolish adventurer! "); break;
    case 1:strcat(Str2," drools menacingly at you. "); break;
    case 2:strcat(Str2," says: You're invited to be lunch! "); break;
    case 3:strcat(Str2," says: Feeed Meee! "); break;
  }      
  mprint(Str2);
}




/* like m_normal_move, but can open doors */
void m_smart_move(m)
struct monster *m;
{
  m_simple_move(m,TRUE);
}

/* not very smart, but not altogether stupid movement */
void m_normal_move(m)
struct monster *m;
{
  m_simple_move(m,FALSE);
}


/* used by both m_normal_move and m_smart_move */
void m_simple_move(m,canopen)
struct monster *m;
int canopen;
{
  int dx = sign(Player.x - m->x);
  int dy = sign(Player.y - m->y);

  if (m_statusp(m,COWARDLY) || (m->hp < ((int) (Monsters[m->id].hp/6)))) {
    dx = -dx;
    dy = -dy;
  }

  if ((! m_statusp(m,HOSTILE)) || 
      (Player.status[INVISIBLE] > 0)) m_random_move(m);
  else {
    if ((m_open_door(m,m->x+dx,m->y+dy,canopen)) &&
	(m_unblocked(m,m->x+dx,m->y+dy)))
      movemonster(m,m->x+dx,m->y+dy);
    else if (dx == 0) {
      if ((m_open_door(m,m->x+dx,m->y+dy,canopen)) &&
	  m_unblocked(m,m->x+1,m->y+dy))
	movemonster(m,m->x+1,m->y+dy);
      else if ((m_open_door(m,m->x+dx,m->y+dy,canopen)) &&
	       m_unblocked(m,m->x-1,m->y+dy))
	movemonster(m,m->x-1,m->y+dy);
    }
    
    else if (dy == 0) {
      if ((m_open_door(m,m->x+dx,m->y+dy,canopen)) &&
	  m_unblocked(m,m->x+dx,m->y+1))
	movemonster(m,m->x+dx,m->y+1);
      else if ((m_open_door(m,m->x+dx,m->y+dy,canopen)) &&
	       m_unblocked(m,m->x+dx,m->y-1))
	movemonster(m,m->x+dx,m->y-1);
    }
    
    else {
      if ((m_open_door(m,m->x+dx,m->y+dy,canopen)) && m_unblocked(m,m->x+dx,m->y))
	movemonster(m,m->x+dx,m->y);
      else if ((m_open_door(m,m->x+dx,m->y+dy,canopen)) &&
	       m_unblocked(m,m->x,m->y+dy))
	movemonster(m,m->x,m->y+dy);
    }
  }
}

/* for spirits (and earth creatures) who can ignore blockages because
   either they are noncorporeal or they can move through stone */
void m_spirit_move(m)
struct monster *m;
{
  int dx = sign(Player.x - m->x);
  int dy = sign(Player.y - m->y);

  if (m_statusp(m,COWARDLY) || (m->hp < ((int) (Monsters[m->id].hp/6)))) {
    dx = -dx;
    dy = -dy;
  }

  if (Player.status[INVISIBLE] > 0) m_random_move(m);
  else movemonster(m,m->x+dx,m->y+dy);
}


  
/* fluttery dumb movement */
void m_flutter_move(m)
struct monster *m;
{
  int trange,range = distance(m->x,m->y,Player.x,Player.y);
  int i,tx,ty,nx=m->x,ny=m->y;

  if (Player.status[INVISIBLE] > 0) m_random_move(m);
  else {
    for (i=0;i<8;i++) {
      tx = m->x+Dirs[0][i];
      ty = m->y+Dirs[1][i];
      trange = distance(tx,ty,Player.x,Player.y);
      if (m_statusp(m,COWARDLY) || (m->hp < ((int) (Monsters[m->id].hp/6)))) {
	if ((trange > range) && m_unblocked(m,tx,ty)) {
	  range = trange;
	  nx = tx;
	  ny = ty;
	}
      }
      else if ((trange <= range) && m_unblocked(m,tx,ty)) {
	range = trange;
	nx = tx;
	ny = ty;
      }
    }
    movemonster(m,nx,ny);
  }
}

/* dragon type attacks */
void m_dragon_melee(m)
struct monster *m;
{
  m_fire_melee(m);
  m_normal_melee(m);
}



void m_fire_melee(m)
struct monster *m;
{

  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  if (hitp(m->hit,Player.defense) || (Player.status[VULNERABLE]>0)){
    strcat(Str2," blasts you with fire!");
    if (Tacmode) {
      menuprint("\n");
      menuprint(Str2);
    }
    else mprint(Str2);
    m_hit(m,FLAME);
  }
  else {
    strcat(Str2," misses you with a fiery blast!");
    if (Tacmode) {
      menuprint("\n");
      menuprint(Str2);
    }
    else mprint(Str2);
  }
}




void m_cold_melee(m)
struct monster *m;
{

  int hit = m->hit+(Tacmode ? 0 :m->actions*2);
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  if (hitp(hit,Player.defense) || (Player.status[VULNERABLE]>0)){
    strcat(Str2," freezes you with cold!");
    if (Tacmode) {
      menuprint("\n");
      menuprint(Str2);
    }
    else mprint(Str2);
    m_hit(m,COLD);
  }
  else {
    strcat(Str2," misses you with a frosty blast!");
    if (Tacmode) {
      menuprint("\n");
      menuprint(Str2);
    }
    else mprint(Str2);
  }
}




void m_elec_melee(m)
struct monster *m;
{

  int hit = m->hit+(Tacmode ? 0 :m->actions*2);
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  if (hitp(hit,Player.defense) || (Player.status[VULNERABLE]>0)){
    strcat(Str2," jolts you with electricity!");
    if (Tacmode) {
      menuprint("\n");
      menuprint(Str2);
    }
    else mprint(Str2);
    m_hit(m,ELECTRICITY);
  }
  else {
    strcat(Str2," misses you.");
    if (Tacmode) {
      menuprint("\n");
      menuprint(Str2);
    }
    else mprint(Str2);
  }
}


void m_normal_melee(m)
struct monster *m;
{

  int hit = m->hit+(Tacmode ? 0 :m->actions*2);
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  if (hitp(hit,Player.defense) || (Player.status[VULNERABLE]>0)){
    strcat(Str2," hit you.");
    if (Tacmode) {
      menuprint(Str2);
      menuprint("\n");
    }
    else mprint(Str2);
    m_hit(m,NORMAL_DAMAGE);
  }
  else {
    strcat(Str2," missed you.");
    if (Tacmode) {
      menuprint(Str2);
      menuprint("\n");
    }
    else mprint(Str2);
  }
}



void m_firebolt(m)
struct monster *m;
{
  fbolt(m->x,m->y,Player.x,Player.y,m->hit,m->dmg);
}

void m_nbolt(m)
struct monster *m;
{
  nbolt(m->x,m->y,Player.x,Player.y,m->hit,m->dmg);
}


void m_lball(m)
struct monster *m;
{
  lball(m->x,m->y,Player.x,Player.y,m->dmg);
}

void m_fireball(m)
struct monster *m;
{
  fball(m->x,m->y,Player.x,Player.y,m->dmg);
}


void m_snowball(m)
struct monster *m;
{
  snowball(m->x,m->y,Player.x,Player.y,m->dmg);
}
    


    
    


void m_guard_talk(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE)) {
    mprint("Surrender in the name of the Law!");
    mprint("Do it? [yn]");
    if (ynq()=='y') {
      Player.alignment++;
      mprint("Go directly to jail. Do not pass go, do not collect 200Au.");
      erase_level();
      Dlevel=0;
      if (! Leveldata[0].generated) load_city();
      mprint("You are taken to the city gaol.");
      send_to_jail();
      locprint("The City of Rampart.");
      drawvision(Player.x,Player.y,Player.vision);
    }
    else mprint("All right, you criminal scum, you asked for it!");
  }
  else mprint("Move it right along, stranger!");
}




void m_follow_move(m)
struct monster *m;
{
  if (! m_statusp(m,HOSTILE))
    m_normal_move(m);
}
    

void m_mp_talk(m)
struct monster *m;
{
  strcpy(m->monstring,"mendicant priest");
  if (m->monchar == 'w')
    mprint("The old woman asks you to spare some treasure for the needy");
  else 
    mprint("The old man asks you to spare some treasure for the needy");
}

void m_mp_melee(m)
struct monster *m;
{
  if (m->monchar == 'w')  
    mprint("The old woman impotently waves her cane at you.");
  else
    mprint("The old man impotently waves his cane at you.");
}


void m_sp_mp(m)
struct monster *m;
{
  if (m->attacked && (random_range(3) == 1)) {
    mprint("You feel cursed!");
    p_damage(10,UNSTOPPABLE,"a mendicant priest's curse");
    m_vanish(m);
  }
  else if (! m_statusp(m,NEEDY)) {
    mprint("The mendicant priest makes a mystical gesture....");
    mprint("You feel impressed...");
    Player.alignment += 5;
    if (Player.alignment > 20)
      Player.hp = max(Player.hp,Player.maxhp);
    m_vanish(m);
  }
}

void m_huge_sounds(m)
struct monster *m;
{
  if (m_statusp(m,AWAKE) &&
      (! los_p(m->x,m->y,Player.x,Player.y)) &&
      (random_range(10) == 1))
    mprint("The dungeon shakes!");
}

void m_titter(m)
struct monster *m;
{

  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  strcat(Str2," titters obscenely at you.");
  mprint(Str2);
}

void m_ng_melee(m)
struct monster *m;
{

  int hit = m->hit+(Tacmode ? 0 :m->actions*2);
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  if (hitp(hit,Player.defense) || (Player.status[VULNERABLE]>0)) {
    strcat(Str2," hit you.");
    if (Tacmode) {
      menuprint(Str2);
      menuprint("\n");
    }
    else mprint(Str2);
    m_hit(m,NORMAL_DAMAGE);
    if ((random_range(5) == 1) || (Player.status[VULNERABLE]>0)) {
      mprint("It grabs you and carries you off!");
      mprint("Its leathery wings flap and flap, and it giggles insanely.");
      mprint("It tickles you cunningly to render you incapable of escape.");
      mprint("Finally, it deposits you in a strange place.");
      Tacmode = FALSE;
      p_teleport(0);
    }
  }
  else {
    strcat(Str2," missed you.");
    if (Tacmode) {
      menuprint(Str2);
      menuprint("\n");
    }
    else mprint(Str2);
  }
}


void m_poison_melee(m)
struct monster *m;
{

  int hit = m->hit+(Tacmode ? 0 :m->actions*2);
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  if (hitp(hit,Player.defense) || (Player.status[VULNERABLE]>0)) {
    strcat(Str2," hit you.");
    if (Tacmode) {
      menuprint(Str2);
      menuprint("\n");
    }
    else mprint(Str2);
    m_hit(m,NORMAL_DAMAGE);
    if ((random_range(3) == 1) || (Player.status[VULNERABLE]>0)) {
      mprint("You've been poisoned!");
      p_poison(10);
    }
  }
  else {
    strcat(Str2," missed you.");
    if (Tacmode) {
      menuprint(Str2);
      menuprint("\n");
    }
    else mprint(Str2);
  }
}


void m_sp_poison_cloud(m)
struct monster *m;
{
  if (distance(m->x,m->y,Player.x,Player.y) < 3) {
    mprint("A cloud of poison gas surrounds you!");
    if (Player.status[BREATHING] > 0)
      mprint("You can breathe freely, however.");
    else p_poison(7);
  }
}

void m_sleep_melee(m)
struct monster *m;
{

  int hit = m->hit+(Tacmode ? 0 :m->actions*2);
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  if (hitp(hit,Player.defense) || (Player.status[VULNERABLE]>0)){
    strcat(Str2," hit you.");
    if (Tacmode) {
      menuprint(Str2);
      menuprint("\n");
    }
    else mprint(Str2);
    m_hit(m,NORMAL_DAMAGE);
    sleep_player(m->level);
  }
  else {
    strcat(Str2," missed you.");
    if (Tacmode) {
      menuprint(Str2);
      menuprint("\n");
    }
    else mprint(Str2);
  }
}

void m_blind_strike(m)
struct monster *m;
{

  pml ml;
  if ((Player.status[BLINDED] == 0) &&
      los_p(m->x,m->y,Player.x,Player.y) &&
      (distance(m->x,m->y,Player.x,Player.y) < 5)) {
    if (m->uniqueness == COMMON) {
      strcpy(Str2,"The ");
      strcat(Str2,m->monstring);
    }
    else strcpy(Str2,m->monstring);
    strcat(Str2," gazes at you menacingly");
    mprint(Str2);
    if (! p_immune(GAZE)) {
      mprint("You've been blinded!");
      Player.status[BLINDED] = random_range(4)+1;
      for(ml=Mlist[Dlevel];ml!=NULL;ml=ml->next) 
	plotspot(ml->m->x,ml->m->y,FALSE);
    }
    else mprint("You gaze steadily back....");
  }
}

void m_ninja_talk(m)
struct monster *m;
{
  mprint("The black-garbed figure says apologetically:");
  mprint("'Situree simasita, wakarimasen.'");
  strcpy(m->monstring,"ninja");
}



void m_thief_talk(m)
struct monster *m;
{
  if (Player.rank[THIEVES]) {
    if (m->level == 2)
      strcpy(m->monstring,"sneak thief");
    else strcpy(m->monstring,"master thief");
    mprint("The cloaked figure makes a gesture which you recognize...");
    mprint("...the thieves' guild recognition signal!");
    mprint("'Sorry, mate, thought you were a mark....'");
    m_vanish(m);
  }
  else m_man_talk(m);

}



void m_assassin_talk(m)
struct monster *m;
{
  strcpy(m->monstring,"master assassin");
  mprint("The ominous figure does not reply, but hands you an embossed card:");
  mprint("'Guild of Assassins Ops are forbidden to converse with targets.'");
}
    

void m_im_talk(m)
struct monster *m;
{
  strcpy(m->monstring,"itinerant merchant");
  if (m->possessions == NULL)
    mprint("The merchant says: Alas! I have nothing to sell!");
  else {
    m->possessions->thing->known = 2;
    printm("\nI have a fine");
    mprint(itemid(m->possessions->thing));
    mprint("for only");
    mnumprint(max(10,4*true_item_value(m->possessions->thing)));
    mprint("Au.");
    mprint("Want it? [yn]");
    if (ynq()=='y') {
      if (Player.cash < (max(10,4*true_item_value(m->possessions->thing)))) {
	if (Player.alignment > 10) {
	  mprint("Well, I'll let you have it for what you've got.");
	  Player.cash = 0;
	  add_item_to_pack(m->possessions->thing);
	  m->possessions = NULL;
	}
	else mprint("Beat it, you deadbeat!");
      }
      else {
	mprint("Here you are. Have a good day.");
	Player.cash -= max(10,(4*item_value(m->possessions->thing)));
	add_item_to_pack(m->possessions->thing);
	m->possessions = NULL;
      }
    }
    else mprint("Well then, I must be off. Good day.");
    m_vanish(m);
  }
}


void m_man_talk(m)
struct monster *m;
{

  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  switch (random_range(4)) {
    case 0:strcat(Str2," asks you for the way home."); break;
    case 1:strcat(Str2," wishes you a pleasant day."); break;
    case 2:strcat(Str2," sneers at you contemptuously."); break;
    case 3:strcat(Str2," smiles and nods."); break;
  }
  mprint(Str2);
}


void m_evil_talk(m)
struct monster *m;
{

  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  switch (random_range(4)) {
    case 0:strcat(Str2," says: 'THERE CAN BE ONLY ONE!'"); break;
    case 1:strcat(Str2," says: 'Prepare to die, Buckwheat!'"); break;
    case 2:strcat(Str2," says: 'Time to die!'"); break;
    case 3:strcat(Str2," says: 'There will be no mercy.'"); break;
  }
  mprint(Str2);
}


void m_robot_talk(m)
struct monster *m;
{
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  switch (random_range(4)) {
    case 0:strcat(Str2," says: 'exterminate...Exterminate...EXTERMINATE!!!'");
      break;
    case 1:strcat(Str2," says: 'Kill ... Crush ... Destroy'"); break;
      break;
    case 2:strcat(Str2," says: 'Danger -- Danger'"); break;
      break;
    case 3:strcat(Str2," says: 'Yo Mama -- core dumped.'"); break;
      break;
  }
  mprint(Str2);
}


void m_thief_f(m)
struct monster *m;
{
  int i = random_item();
  if (random_range(3) == 1) {
    if (distance(Player.x,Player.y,m->x,m->y) < 2) {
      if (p_immune(THEFT) || (Player.level > (m->level*2)+random_range(20))) {
	if (m->level == 2)
	  strcpy(m->monstring,"sneak thief");
	else strcpy(m->monstring,"master thief");
	mprint("You feel secure.");
	}
      else {
	if (i == ABORT)
	  mprint("You feel fortunate.");
	else if ((Player.possessions[i]->used) || 
		 (Player.dex < m->level*random_range(10))) {
	  mprint("You feel a sharp tug.... You hold on!");
	  if (m->level == 2)
	    strcpy(m->monstring,"sneak thief");
	  else strcpy(m->monstring,"master thief");
	}
	else {
	  mprint("You feel uneasy for a moment.");
	  mprint("The thief runs away!");
	  m_teleport(m);
	  m->movef = M_MOVE_SCAREDY;
	  m->specialf = M_MOVE_SCAREDY;
	  m_pickup(m,Player.possessions[i]);
	  conform_lost_object(Player.possessions[i]);
	}
	if (m->id == ML2+2) { /* the sneak thief, naturally */
	  m->monchar = 't';
	  strcpy(m->monstring,"sneak thief");
	  m->talkf = M_TALK_GREEDY;
	}
      }
    }
  }
}

void m_spectral_melee(m)
struct monster *m;
{

  int hit = m->hit+(Tacmode ? 0 :m->actions*2);
  m_normal_melee(m);
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  if (hitp(hit,Player.defense) || (Player.status[VULNERABLE]>0)){
    strcat(Str2," touches you.");
    if (Tacmode) {
      menuprint(Str2);
      menuprint("\n");
    }
    else mprint(Str2);
    mprint("You feel cold!");
    if (p_immune(NEGENERGY))
      mprint("... but the feeling quickly fades.");
    else {
      switch(random_range(2)) {
      case 0:
	mprint("The coldness spreads throughout your body...");
	Player.str--;
	Player.con--;
	if ((Player.str < 3) || (Player.con < 3)) {
	  mprint("You suffer a fatal heart attack!!!");
	  Player.hp = 0;
	  if (m->uniqueness == COMMON) {
	    strcpy(Str2,"a ");
	    strcat(Str2,m->monstring);
	  }
	  else strcpy(Str2,m->monstring);
	  p_death(Str2);
	}
	break;
      case 1: 
	mprint("The coldness saps your very soul...");
	if (m->uniqueness == COMMON) {
	  strcpy(Str2,"a ");
	  strcat(Str2,m->monstring);
	}
	else strcpy(Str2,m->monstring);
	level_drain(m->level,Str2);
	break;
      }
    }
  }
  else {
    strcat(Str2," missed you.");
    if (Tacmode) {
      menuprint(Str2);
      menuprint("\n");
    }
    else mprint(Str2);
  }
}


void m_summon(m)
struct monster *m;
{
  if ((distance(Player.x,Player.y,m->x,m->y) < 2) &&
      (random_range(3) == 1)) {
	summon(0,-1);
	summon(0,-1);
      }
}	  


void m_disease_melee(m)
struct monster *m;
{

  int hit = m->hit+(Tacmode ? 0 :m->actions*2);
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  if (hitp(hit,Player.defense) || (Player.status[VULNERABLE]>0)) {
    strcat(Str2," bit you.");
    if (Tacmode) {
      menuprint(Str2);
      menuprint("\n");
    }
    else mprint(Str2);
    m_hit(m,NORMAL_DAMAGE);
    if (random_range(30) > Player.con) {
      mprint("You feel ill.");
      if (p_immune(INFECTION))
	mprint("...but the feeling quickly fades.");
      else {
	mprint("You begin to shake with ague.");
	Player.status[DISEASED] += m->level;
      }
    }
    showflags();
  }
  else {
    strcat(Str2," missed you.");
    if (Tacmode) {
      menuprint(Str2);
      menuprint("\n");
    }
    else mprint(Str2);
  }
}

void m_aggravate(m)
struct monster *m;
{

  if (m_statusp(m,HOSTILE)) {
    if (m->uniqueness == COMMON) {
      strcpy(Str2,"The ");
      strcat(Str2,m->monstring);
    }
    else strcpy(Str2,m->monstring);
    strcat(Str2," emits an irritating humming sound.");
    mprint(Str2);
    aggravate();
    m_status_reset(m,HOSTILE);
  }
}


void m_teleport(m)
struct monster *m;
{
  if (m_statusp(m,AWAKE)) {
    Dungeon[Dlevel][m->x][m->y].creature = NULL;
    findspace(&(m->x),&(m->y));
    Dungeon[Dlevel][m->x][m->y].creature = m;
  }
}


void m_illusion(m)
struct monster *m;
{
  int i = random_range(NUMMONSTERS);
  if (i==ML0+4) i = m->id; /* can't imitate NPC */
  if (Player.status[TRUESIGHT]) {
    m->monchar = Monsters[m->id].monchar;
    strcpy(m->monstring,Monsters[m->id].monstring);    
  }
  else  if (random_range(5) == 1) {
    m->monchar = Monsters[i].monchar;
    strcpy(m->monstring,Monsters[i].monstring);
  }
}


/* allows monsters to fall into pools, revealed traps, etc */
void m_confused_move(m)
struct monster *m;
{
  int i,nx,ny,done=FALSE;
  for (i=0;((i<8)&&(! done));i++) {
    nx = m->x+random_range(3)-1;
    ny = m->y+random_range(3)-1;
    if (unblocked(nx,ny) && 
	((nx != Player.x) || 
	 (ny != Player.y))) {
      done = TRUE;
      movemonster(m,nx,ny);
    }
  }
}

void m_random_move(m)
struct monster *m;
{
  int i,nx,ny,done=FALSE;
  for (i=0;((i<8)&&(! done));i++) {
    nx = m->x+random_range(3)-1;
    ny = m->y+random_range(3)-1;
    if (m_unblocked(m,nx,ny) && 
	((nx != Player.x) || 
	 (ny != Player.y))) {
      done = TRUE;
      movemonster(m,nx,ny);
    }
  }
}

void m_sp_explode(m)
struct monster *m;
{
  if ((distance(Player.x,Player.y,m->x,m->y)<2) &&
      (m-> hp > 0) && 
      (m->hp < Monsters[m->id].hp))
    fball(m->x,m->y,m->x,m->y,m->hp);
}



void m_sp_demon(m)
struct monster *m;
{
  int mid;

  if ((m->id != ML4+6) && /*succubi don't give fear */
      los_p(m->x,m->y,Player.x,Player.y) &&
      (random_range(30) > Player.level+10) &&
      (Player.status[AFRAID] == 0)) {
	mprint("You are suddenly stricken with gibbering terror!");
	if (! p_immune(FEAR)) Player.status[AFRAID] += m->level;
	else mprint("You master your reptile brain and stand fast.");
      }
  if ((m->hp < (m->level * 5)) && (m->hp > 1)) {
    mprint("The demon uses its waning lifeforce to summon help!");
    m->hp = 1;
  switch(m->level) {
      case 3: mid = ML2+1; break; /* night gaunt */
      case 4:
      case 5: mid = ML3+2; break; /* lesser frost demon */
      case 8: mid = ML5+4; break; /* lesser frost demon */
    }
    summon(-1,mid);
    summon(-1,mid);
  }
}


void m_sp_acid_cloud(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE) &&
      (distance(m->x,m->y,Player.x,Player.y) < 3))
    acid_cloud();
}

	  
void m_sp_escape(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE))
    m_vanish(m);
}
    

void m_vanish(m)
struct monster *m;
{
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  strcat(Str2," vanishes in the twinkling of an eye!");
  mprint(Str2);
  Dungeon[Dlevel][m->x][m->y].creature = NULL;
  Dungeon[Dlevel][m->x][m->y].showchar = ' ';
  drawspot(m->x,m->y);
  m->hp = -1;
}


void m_slithy_talk(m)
struct monster *m;
{
  mprint("It can't talk -- it's too slithy!");
}


void m_mimsy_talk(m)
struct monster *m;
{
  mprint("It can't talk -- it's too mimsy!");
}



void m_burble_talk(m)
struct monster *m;
{

  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  strcat(Str2," burbles hatefully at you.");
  mprint(Str2);
}


void m_sp_ghost(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE)) {
    mprint("The ghost moans horribly....");
    p_damage(3,FEAR,"a ghost-inspired heart attack");
    mprint("You are suddenly stricken with gibbering terror!");
    if (! p_immune(FEAR)) Player.status[AFRAID] += m->level;
    else mprint("You master your reptile brain and stand fast.");
  }
}



void m_ghost_talk(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE))
    mprint("The ghost doesn't seem interested in conversation");
  else {
    mprint("The ghost passes on some useful information!");
    m->talkf = M_TALK_SILENT;
    hint();
  }
}


/* random spell cast by monster */
void m_sp_spell(m)
struct monster *m;
{
  char action[80];
  int spell;
  if (m_statusp(m,HOSTILE) && los_p(Player.x,Player.y,m->x,m->y)) {
    if (m->uniqueness == COMMON) strcpy(action,"The ");
    else strcpy(action,"");
    strcat(action,m->monstring);
    strcat(action," casts a spell...");
    mprint(action);
    
    switch (random_range(m->level+7)) {
    case 0: 
      nbolt(m->x,m->y,Player.x,Player.y,m->hit,10);
      break;
    case 1:
      mprint("It seems stronger..."); 
      m->hp += random_range(m->level*m->level);
      break;
    case 2:
      haste(-1);
      break;
    case 3:
      cure(-1);
      break;
    case 4:
      lball(m->x,m->y,Player.x,Player.y,20);
      break;
    case 5:
      enchant(-1);
    case 6:
      snowball(m->x,m->y,Player.x,Player.y,30);
      break;
    case 7:
      bless(0-m->level);
      break;
    case 8:
      p_poison(m->level);
      break;
    case 9:
      sleep_player(m->level/2);
      break;
    case 10:
      fbolt(m->x,m->y,Player.x,Player.y,m->hit*3,50);
      break;
    case 11:
      acquire(0-m->level);
      break;
    case 12:
      dispel(-1);
      break;
    case 13:
      disrupt(Player.x,Player.y,50);
      break;
    case 14:
      if (m->uniqueness == COMMON) {
	strcpy(Str2,"a ");
	strcat(Str2,m->monstring);
      }
      else strcpy(Str2,m->monstring);
      level_drain(m->level,Str2);
      break;
    case 15:
    case 16:
      disintegrate(Player.x,Player.y);
      break;
    }
  }
}


void m_scaredy_move(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE) && (m->hp < m->level*4)) 
    m_vanish(m);
  else m_normal_move(m);
}

void m_beg_talk(m)
struct monster *m;
{
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  strcat(Str2," asks you for alms.");
  mprint(Str2);
}


void m_hint_talk(m)
struct monster *m;
{
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  strcat(Str2,"whispers in your ear.");
  mprint(Str2);
  hint();
  m->talkf = M_TALK_SILENT;
}

void m_good_fairy_talk(m)
struct monster *m;
{
  mprint("The good fairy glints: Would you like a wish?");
  if (ynq()=='y') {
    mprint("The good fairy glows: Are you sure?");
    if (ynq()=='y') {
      mprint("The good fairy radiates: Really really sure?");
      if (ynq()=='y') {
	mprint("The good fairy beams: I mean, like, sure as sure can be?");
	if (ynq()=='y') {
	  mprint("The good fairy dazzles: You don't want a wish, right?");
	  if (ynq()=='y') mprint("The good fairy laughs: I thought not.");
	  else wish(0);
	}
      }
    }
  }
  mprint("In a flash of sweet-smelling light, the fairy vanishes....");
  Player.hp = max(Player.hp,Player.maxhp);
  Player.mana = max(Player.mana,calcmana());
  mprint("You feel mellow.");
  m_vanish(m);
}

void m_evil_fairy_talk(m)
struct monster *m;
{
  mprint("The evil fairy roils: Eat my pixie dust!");
  mprint("She waves her black-glowing wand, which screams thinly....");
  m->movef=M_MOVE_SMART;
  m->meleef=M_MELEE_POISON;
  m->specialf=M_SP_THIEF;
  acquire(-1);
  bless(-1);
  sleep_player(m->level/2);
  summon(-1,-1);
  summon(-1,-1);
  summon(-1,-1);
  summon(-1,-1);
}


void m_seductor_talk(m)
struct monster *m;
{
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  strcat(Str2," beckons seductively...");
  mprint(Str2);
  mprint("Flee? [yn] ");
  if (ynq()=='y') {
    mprint("You feel stupid.");
  }
  else {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
    strcat(Str2," shows you a good time....");
    mprint(Str2);
    gain_experience(500);
    Player.con++;
  }
  m_vanish(m);
}

/* monsters with this have some way to hide, camouflage, etc until they 
   attack */
void m_sp_surprise(m)
struct monster *m;
{
  if (m->attacked) {
    if (m_statusp(m,HOSTILE) && 
	(! Player.status[TRUESIGHT]) &&
	m_statusp(m,M_INVISIBLE)) {
      m->monchar = Monsters[m->id].monchar;
      if ((! Tacmode) && (! Player.status[ALERT])) {
	switch(random_range(4)) {
	case 0: 
	  mprint("You are surprised by a sudden treacherous attack!");
	  break;
	case 1: 
	  mprint("You are shocked out of your reverie by the scream of battle!");
	  break;
	case 2: 
	  mprint("Suddenly, from out of the shadows, a surprise attack!");
	  break;
	case 3: 
	  mprint("A shriek of hatred causes you to momentarily freeze up!");
	  break;
	}
	strcpy(Str1,"You find yourself in close-combat with ");
	if (m->uniqueness == COMMON) strcat(Str1,"a ");
	strcat(Str1,m->monstring);
	mprint(Str1);
	morewait();
	m->actions++;
	tacmode(m);
	m->actions--;
	m_status_reset(m,M_INVISIBLE);
      }
      else {
	mprint("You alertly sense the presence of an attacker!");
	m_status_reset(m,M_INVISIBLE);
      }	
    }
  }
}

void m_sp_seductor(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE)) {
    if (m->uniqueness == COMMON) {
      strcpy(Str2,"The ");
      strcat(Str2,m->monstring);
    }
    else strcpy(Str2,m->monstring);
    strcat(Str2," runs away screaming for help....");
    mprint(Str2);
    m_vanish(m);
    summon(-1,-1);
    summon(-1,-1);
    summon(-1,-1);
  }
  else if (distance(Player.x,Player.y,m->x,m->y) < 2) 
    m_seductor_talk(m);

}

void m_demonlover_talk(m)
struct monster *m;
{
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  strcat(Str2," beckons seductively...");
  mprint(Str2);
  mprint("Flee? [yn] ");
  if (ynq()=='y') 
    mprint("You feel fortunate....");
  else {
    if (m->uniqueness == COMMON) {
      strcpy(Str2,"The ");
      strcat(Str2,m->monstring);
    }
    else strcpy(Str2,m->monstring);
    strcat(Str2," shows you a good time....");
    mprint(Str2);
    mprint("You feel your life energies draining...");
    level_drain(random_range(3)+1,"a demon's kiss");
  }
  m->talkf = M_TALK_EVIL;
  m->meleef = M_MELEE_SPIRIT;
  m->specialf = M_SP_DEMON;
  if (m->monchar == 's') {
    m->monchar = 'I';
    strcpy(m->monstring,"incubus");
  }
  else {
    m->monchar = 'S';
    strcpy(m->monstring,"succubus");
  }
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  strcat(Str2," laughs insanely.");
  mprint(Str2);
  mprint("You now notice the fangs, claws, batwings...");
}

void m_sp_demonlover(m)
struct monster *m;
{
  if (distance(Player.x,Player.y,m->x,m->y) < 2) 
    m_demonlover_talk(m);
}

void m_sp_eater(m)
struct monster *m;
{
  int i;
  if (Player.rank[COLLEGE]) m_status_set(m,HOSTILE);
  if (m_statusp(m,HOSTILE))
    if (los_p(m->x,m->y,Player.x,Player.y)) {
      mprint("A strange numbing sensation comes over you...");
      Player.mana = Player.mana / 2;
      enchant(-1);
      Player.pow--;
      if (--Player.pow < 1) p_death("The Eater of Magic");
    }
  if (m->hp < 10) {
    mprint("The Eater explodes in a burst of mana!");
    for(i=0;i<9;i++) 
      manastorm(m->x+Dirs[0][i],m->y+Dirs[0][i],100);
  }
}


void m_sp_dragonlord(m)
struct monster *m;
{
  static int constriction=0;
  if (m_statusp(m,HOSTILE)) {
    if (distance(m->x,m->y,Player.x,Player.y)<2) {
      if (! Player.status[IMMOBILE]) {
	mprint("A gust of wind from the Dragonlord's wings knocks you down!");
	p_damage(25,NORMAL_DAMAGE,"a gust of wind");
	Skipplayer=TRUE;
	Player.status[IMMOBILE]+=2;
      }
      else if (! constriction) {
	mprint("The Dragonlord grabs you with his tail!");
	constriction = 25;
	Player.status[IMMOBILE]+=1;
      }
      else if (random_range(2)) {
	mprint("The coils squeeze tighter and tighter...");
	p_damage(constriction,NORMAL_DAMAGE,"The Dragonlord");
	Player.status[IMMOBILE]+=1;
	constriction *=2;
      }
      else {
	mprint("The dragonlord hurls you to the ground!");
	p_damage(2*constriction,NORMAL_DAMAGE,"The Dragonlord");
	constriction = 0;
      }
      m_sp_spell(m);
    }
    else {
      constriction = 0;
      if (view_los_p(m->x,m->y,Player.x,Player.y)) {
	if ((! Player.immunity[FEAR]) && (! Player.status[AFRAID])) {
	  mprint("You are awestruck at the sight of the Dragonlord.");
	  Player.status[AFRAID]+=5;
	}
	if (random_range(3)) {
	  m_sp_spell(m);
	  m_sp_spell(m);
	}
      }
    }
  }
  else if (distance(m->x,m->y,Player.x,Player.y)<2)
    mprint("You are extremely impressed at the sight of the Dragonlord.");
}

void m_sp_whirl(m)
struct monster *m;
{
  if (distance(m->x,m->y,Player.x,Player.y) < 2) {
    mprint("The whirlwind swirls you up!");
    l_whirlwind();
  }
}

void m_sp_blackout(m)
struct monster *m;
{
  int r = Dungeon[Dlevel][m->x][m->y].roomnumber;
  if (r>0) 
    if (r == Dungeon[Dlevel][Player.x][Player.y].roomnumber)
      if (Leveldata[Dlevel].rooms[r].lighted) {
	mprint("The room is plunged into darkness.");
	darkenroom(r);
	Leveldata[Dlevel].rooms[r].lighted = FALSE;
      }
}
