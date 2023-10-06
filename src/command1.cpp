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

/* command1.c */

/* This file has the two toplevel command scanners, p_process,
which works everywhere but the countryside, and p_couyntry_process,
which works.... */

#include "glob.h"
#include "scr.h"

// deal with a new player command in dungeon or city mode
void p_process()
{
  static int searchval = 0;

  if(Player.status[BERSERK])
  {
    if(goberserk())
    {
      setgamestatus(SKIP_PLAYER, GameStatus);
      drawvision(Player.x, Player.y);
    }
  }
  if(!gamestatusp(SKIP_PLAYER, GameStatus))
  {
    if(searchval > 0)
    {
      searchval--;
      if(searchval == 0)
      {
        resetgamestatus(FAST_MOVE, GameStatus);
      }
    }
    drawvision(Player.x, Player.y);
    if(!gamestatusp(FAST_MOVE, GameStatus))
    {
      searchval = 0;
      print_messages();
      Cmd = get_level_input();
    }
    Command_Duration = 0;
    switch(Cmd)
    {
      case ' ':
      case 13:
        setgamestatus(SKIP_MONSTERS, GameStatus);
        break; // no op on space or return
      case 6:
        abortshadowform();
        break; // ^f
      case 7:
        wizard();
        break; // ^g
      case 9:
        display_pack();
        xredraw();
        break; // ^i
      case 11:
        if(gamestatusp(CHEATED, GameStatus))
        {
          frobgamestatus();
        }
        [[fallthrough]];
      case 12:
        xredraw();
        setgamestatus(SKIP_MONSTERS, GameStatus);
        break; // ^l
      case 16:
        bufferprint();
        setgamestatus(SKIP_MONSTERS, GameStatus);
        break; // ^p
      case 18:
        xredraw();
        setgamestatus(SKIP_MONSTERS, GameStatus);
        break; // ^r
      case 23:
        if(gamestatusp(CHEATED, GameStatus))
        {
          drawscreen();
        }
        break; // ^w
      case 24: // ^x
        if(gamestatusp(CHEATED, GameStatus) || Player.rank[ADEPT])
        {
          wish(1);
        }
        Command_Duration = 5;
        break;
      case 'a':
        zapwand();
        Command_Duration = Player.speed * 8 / 5;
        break;
      case 'c':
        closedoor();
        Command_Duration = Player.speed * 2 / 5;
        break;
      case 'd':
        drop_pack_item();
        Command_Duration = Player.speed;
        break;
      case 'e':
        eat();
        Command_Duration = 30;
        break;
      case 'f':
        fire();
        Command_Duration = Player.speed;
        break;
      case 'g':
        pickup();
        Command_Duration = Player.speed * 2;
        break;
      case 'i':
        do_inventory_control();
        break;
      case 'm':
        magic();
        Command_Duration = 12;
        break;
      case 'o':
        opendoor();
        Command_Duration = Player.speed;
        break;
      case 'p':
        pickpocket();
        Command_Duration = Player.speed * 4;
        break;
      case 'q':
        quaff();
        Command_Duration = 10;
        break;
      case 'r':
        peruse();
        Command_Duration = 20;
        break;
      case 's':
        search(&searchval);
        Command_Duration = 20;
        break;
      case 't':
        talk();
        Command_Duration = 10;
        break;
      case 'v':
        vault();
        Command_Duration = Player.speed * 2;
        break;
      case 'x':
        examine();
        Command_Duration = 1;
        break;
      case 'z':
        bash_location();
        Command_Duration = Player.speed * 2;
        break;
      case 'A':
        activate();
        Command_Duration = 10;
        break;
      case 'C':
        callitem();
        break;
      case 'D':
        disarm();
        Command_Duration = 30;
        break;
      case 'E':
        dismount_steed();
        Command_Duration = Player.speed * 2;
        break;
      case 'F':
        tacoptions();
        break;
      case 'G':
        give();
        Command_Duration = 10;
        break;
      case 'M':
        city_move();
        Command_Duration = 10;
        break;
      case 'O':
        setoptions();
        break;
      case 'P':
        show_license();
        break; // actually show_license is in file.c
      case 'Q':
        quit();
        break;
#ifndef MULTI_USER_SYSTEM
      case 'R':
        rename_player();
        break;
#endif
      case 'S':
        save(false);
        break;
      case 'T':
        tunnel();
        Command_Duration = Player.speed * 6;
        break;
      case 'V':
        version();
        break;
      case 'Z':
        bash_item();
        Command_Duration = Player.speed * 2;
        break;
      case '.':
        rest();
        Command_Duration = Player.speed;
        break;
      case ',':
        Command_Duration = 10;
        nap();
        break;
      case '>':
        downstairs();
        break;
      case '<':
        upstairs();
        break;
      case '@':
        p_movefunction(Level->site[Player.x][Player.y].p_locf);
        Command_Duration = 5;
        break;
      case '/':
        charid();
        setgamestatus(SKIP_MONSTERS, GameStatus);
        break;
      case '?':
        help();
        setgamestatus(SKIP_MONSTERS, GameStatus);
        break;
      case '4':
      case 'h':
        moveplayer(-1, 0);
        Command_Duration = Player.speed;
        break;
      case '2':
      case 'j':
        moveplayer(0, 1);
        Command_Duration = Player.speed;
        break;
      case '8':
      case 'k':
        moveplayer(0, -1);
        Command_Duration = Player.speed;
        break;
      case '6':
      case 'l':
        moveplayer(1, 0);
        Command_Duration = Player.speed;
        break;
      case '1':
      case 'b':
        moveplayer(-1, 1);
        Command_Duration = Player.speed;
        break;
      case '3':
      case 'n':
        moveplayer(1, 1);
        Command_Duration = Player.speed;
        break;
      case '7':
      case 'y':
        moveplayer(-1, -1);
        Command_Duration = Player.speed;
        break;
      case '9':
      case 'u':
        moveplayer(1, -1);
        Command_Duration = Player.speed;
        break;
      case '5':
        setgamestatus(SKIP_MONSTERS, GameStatus); // don't do anything; a dummy turn
        Cmd = get_message_input();
        while((Cmd != ESCAPE) && ((Cmd < '1') || (Cmd > '9') || (Cmd == '5')))
        {
          queue_message("Run in keypad direction [ESCAPE to abort]: ");
          Cmd = get_message_input();
        }
        if(Cmd != ESCAPE)
        {
          setgamestatus(FAST_MOVE, GameStatus);
        }
        break;
      case 'H':
        setgamestatus(FAST_MOVE, GameStatus);
        Cmd = 'h';
        moveplayer(-1, 0);
        Command_Duration = Player.speed * 4 / 5;
        break;
      case 'J':
        setgamestatus(FAST_MOVE, GameStatus);
        Cmd = 'j';
        moveplayer(0, 1);
        Command_Duration = Player.speed * 4 / 5;
        break;
      case 'K':
        setgamestatus(FAST_MOVE, GameStatus);
        Cmd = 'k';
        moveplayer(0, -1);
        Command_Duration = Player.speed * 4 / 5;
        break;
      case 'L':
        setgamestatus(FAST_MOVE, GameStatus);
        Cmd = 'l';
        moveplayer(1, 0);
        Command_Duration = Player.speed * 4 / 5;
        break;
      case 'B':
        setgamestatus(FAST_MOVE, GameStatus);
        Cmd = 'b';
        moveplayer(-1, 1);
        Command_Duration = Player.speed * 4 / 5;
        break;
      case 'N':
        setgamestatus(FAST_MOVE, GameStatus);
        Cmd = 'n';
        moveplayer(1, 1);
        Command_Duration = Player.speed * 4 / 5;
        break;
      case 'Y':
        setgamestatus(FAST_MOVE, GameStatus);
        Cmd = 'y';
        moveplayer(-1, -1);
        Command_Duration = Player.speed * 4 / 5;
        break;
      case 'U':
        setgamestatus(FAST_MOVE, GameStatus);
        Cmd = 'u';
        moveplayer(1, -1);
        Command_Duration = Player.speed * 4 / 5;
        break;
      case ESCAPE:
        break;
      default:
        commanderror();
        setgamestatus(SKIP_MONSTERS, GameStatus);
        break;
    }
  }
  if(Current_Environment != E_COUNTRYSIDE)
  {
    roomcheck();
  }
  screencheck(Player.x, Player.y);
}

/* deal with a new player command in countryside mode */
void p_country_process()
{
  int no_op;

  drawvision(Player.x, Player.y);
  do
  {
    no_op = false;
    print_messages();
    Cmd = get_message_input();
    switch(Cmd)
    {
      case ' ':
      case 13:
        no_op = true;
        break;
      case 7:
        wizard();
        break; /* ^g */
      case 12:
        xredraw();
        no_op = true;
        break; /* ^l */
      case 16:
        bufferprint();
        no_op = true;
        break; /* ^p */
      case 18:
        xredraw();
        no_op = true;
        break; /* ^r */
      case 23:
        if(gamestatusp(CHEATED, GameStatus))
        {
          drawscreen();
        }
        break; /* ^w */
      case 24:
        if(gamestatusp(CHEATED, GameStatus) || Player.rank[ADEPT])
        {
          wish(1);
        }
        break; /* ^x */
      case 'd':
        drop_pack_item();
        break;
      case 'e':
        eat();
        break;
      case 'i':
        do_inventory_control();
        break;
      case 's':
        countrysearch();
        break;
      case 'x':
        examine();
        break;
      case 'E':
        dismount_steed();
        break;
      case 'H':
        hunt(Country[Player.x][Player.y].current_terrain_type);
        break;
      case 'O':
        setoptions();
        break;
      case 'P':
        show_license();
        break; /* actually show_license is in file.c */
      case 'Q':
        quit();
        break;
#ifndef MULTI_USER_SYSTEM
      case 'R':
        rename_player();
        break;
#endif
      case 'S':
        save(false);
        break;
      case 'V':
        version();
        break;
      case '>':
        enter_site(Country[Player.x][Player.y].base_terrain_type);
        break;
      case '/':
        charid();
        no_op = true;
        break;
      case '?':
        help();
        no_op = true;
        break;
      case '4':
      case 'h':
        movepincountry(-1, 0);
        break;
      case '2':
      case 'j':
        movepincountry(0, 1);
        break;
      case '8':
      case 'k':
        movepincountry(0, -1);
        break;
      case '6':
      case 'l':
        movepincountry(1, 0);
        break;
      case '1':
      case 'b':
        movepincountry(-1, 1);
        break;
      case '3':
      case 'n':
        movepincountry(1, 1);
        break;
      case '7':
      case 'y':
        movepincountry(-1, -1);
        break;
      case '9':
      case 'u':
        movepincountry(1, -1);
        break;
      default:
        commanderror();
        no_op = true;
        break;
    }
  } while(no_op);
  screencheck(Player.x, Player.y);
}
