/* omega copyright (c) 1987 by Laurence Raphael Brothers */

/* this file deals with the passage of time in omega */
/* This marks a change toward event-driven action */
/* otime.c */

#include <strings.h>
#include "oglob.h"

/* from oscr */
void drawmonsters(),dataprint();;

/* from omon */
void m_pulse();

/* from oaux */
void move_status_check(),hourly_check();

void time_clock();


/* This function coordinates monsters and player actions,
as well as random events.

There are 10 clicks to the turn; depending on the speed rating of
monsters and player, they may "go off" from any speed from 1/10 turns
to 10 times per turn */

void time_clock()
{
  pml ml,tempml,prevml;

  Tick = ((Tick+1) % 50);

  if (Tick % 5 == 0) {
    move_status_check(); /* see about some player statuses each turn */
    Time++;
    if (Time % 100 == 0) hourly_check(); /* 100 moves == 1 hour */
  }
  
  Skipmonsters = FALSE;
  if (Tick == Player.click) {
    Player.click = (Player.click + Player.speed) % 50;
    if (! Skipplayer) do p_process(); while (Skipmonsters);
  }
  Skipplayer = FALSE;

  drawmonsters(FALSE);

  /* Check all monsters */
  ml = Mlist[Dlevel];
  prevml = ml;
  while(ml != NULL) {
    if (ml->m->hp < 1) {
      tempml = ml;
      if (ml == Mlist[Dlevel]) Mlist[Dlevel] = Mlist[Dlevel]->next;
      else prevml->next = ml->next;
      ml = ml->next;
      free((char *) tempml->m);
      free((char *) tempml);
    }
    else if (Tick == ml->m->click) {
      ml->m->click = (ml->m->click + ml->m->speed) % 50;
      m_pulse(ml->m);
    }
    prevml = ml;
    ml = ml->next;
  }
}

