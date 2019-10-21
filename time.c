/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* time.c */

/* this file deals with the passage of time in omega */

#include "glob.h"

/* This function coordinates monsters and player actions, as well as
random events. Each tick is a second. There are therefore 60 ticks to
the minute and 60 minutes to the hour.
*/

void time_clock(reset)
int reset;
{
  int env;
  pml ml, *prev;

  if (++Tick > 60) {
    Tick = 0;
    minute_status_check(); /* see about some player statuses each minute */
    if (++Time % 10 == 0) tenminute_check();
  }
  
  if (reset) Tick = (Player.click = 0);

  env = Current_Environment;
  while ((Tick == Player.click) && (Current_Environment != E_COUNTRYSIDE) &&
    Current_Environment == env) {
    if (! gamestatusp(SKIP_PLAYER))
      do {
	resetgamestatus(SKIP_MONSTERS);
	if ((! Player.status[SLEPT])  && 
	    (Current_Environment != E_COUNTRYSIDE)) p_process(); 
      } while (gamestatusp(SKIP_MONSTERS) && 
	       (Current_Environment != E_COUNTRYSIDE));
    else resetgamestatus(SKIP_PLAYER);
    Player.click = (Player.click + Command_Duration) % 60;
  }

  /* klugy but what the heck. w/o this line, if the player caused
  a change-environment to the country, the monsters on the old Level
  will still act, causing all kinds of anomalies and core dumps,
  intermittently. However, any other environment change will reset
  Level appropriately, so only have to check for countryside */

  if (Current_Environment != E_COUNTRYSIDE) {

    prev = &(Level->mlist);
    ml = *prev;
    while (ml)
      if (ml->m->hp > 0) {
	/* following is a hack until I discover source of phantom monsters */
	if (Level->site[ml->m->x][ml->m->y].creature != ml->m)
	  fix_phantom(ml->m); 
	if (Tick == ml->m->click) {
	  ml->m->click += ml->m->speed;
	  while (ml->m->click > 60) ml->m->click -= 60;
	  m_pulse(ml->m);
	}
	prev = &(ml->next);
	ml = ml->next;
      }
      else if (ml->m != Arena_Monster) {
	*prev = ml->next;
	free((char *) ml->m);
	free((char *) ml);
	ml = *prev;
      }
      else
	ml = ml->next;
  }
}





/* remedies occasional defective monsters */
void fix_phantom(m)
struct monster *m;
{
  if (Level->site[m->x][m->y].creature == NULL) {
    mprint("You hear a sound like a sigh of relief....");
    Level->site[m->x][m->y].creature = m;
  }
  else {
    mprint("You hear a puff of displaced air....");
    findspace(&(m->x),&(m->y),-1);
    Level->site[m->x][m->y].creature = m;
    m_death(m);
  }
}
