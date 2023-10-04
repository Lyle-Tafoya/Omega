/*
Omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989
Modifications copyright (C) by Lyle Tafoya, 2019, 2021-2023

This file is part of Omega.

Omega is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Omega is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Omega. If not, see <https://www.gnu.org/licenses/>.
*/


#include "defs.h"

#include <string>

void   append_message(const std::string &, bool = false);
int    mcigetc(void);
long   parsenum(void);
std::string msgscanstring();
Symbol getspot(int, int, int);
char   menugetc(void);
int    mgetc(void);
int    ynq(void);
int    ynq1(void);
int    ynq2(void);
int    getnumber(int);
int    litroom(int, int);
int    move_slot(int, int, int);
int    stillonblock(void);
void   blankoutspot(int, int);
void   blotspot(int, int);
void   bufferprint(void);
void   calculate_offsets(int, int);
void   clear_screen(void);
void   colour_on(void);
void   colour_off(void);
void   commanderror(void);
void   dataprint(void);
void   deathprint(void);
void   display_bigwin(void);
void   display_death(const std::string &);
void   display_option_slot(int);
void   display_options(void);
void   display_pack(void);
void   display_quit(void);
void   display_win(void);
void   dobackspace(void);
void   dodrawspot(int, int);
void   draw_explosion(Symbol, int, int);
void   drawmonsters(int);
void   drawomega(void);
void   drawplayer(void);
void   drawscreen(void);
void   drawspot(int, int);
void   drawvision(int, int);
void   enable_attr(WINDOW *, attr_t);
void   endgraf(void);
void   erase_level(void);
void   erase_monster(struct monster *);
void   expand_message_window();
int    get_message_input();
int    get_level_input();
void   hide_line(int);
void   initgraf(void);
void   levelrefresh(void);
void   lightspot(int, int);
void   locprint(const std::string &);
void   maddch(char);
void   menuaddch(char);
void   menuclear(void);
void   menuprint(const std::string &);
void   showmenu(void);
void   mprint(const std::string &);
void   nprint1(const std::string &);
void   nprint2(const std::string &);
void   nprint3(const std::string &);
void   phaseprint(void);
void   plotchar(Symbol, int, int);
void   plotmon(struct monster *);
void   plotspot(int, int, int);
void   print1(const std::string &);
void   print2(const std::string &);
void   print3(const std::string &);
void   print_combat_stats();
void   print_inventory_menu(Symbol = NULL_ITEM);
void   print_messages();
void   putspot(int, int, Symbol);
void   queue_message(const std::string &);
void   room_name_print(const std::string &);
void   redraw(void);
void   screencheck(int, int);
void   setlastxy(int, int);
void   show_screen(void);
void   omshowcursor(int, int);
void   showflags(void);
void   showscores(void);
void   shrink_message_window();
void   spreadroomdark(int, int, int);
void   spreadroomlight(int, int, int);
void   timeprint(void);
void   title(void);
void   xredraw(void);