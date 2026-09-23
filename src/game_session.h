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

#ifndef OMEGA_GAME_SESSION_H_
#define OMEGA_GAME_SESSION_H_

// Outcome of init_game_session(): Failed if data files are missing or
// initialization otherwise fails; NewGame for a fresh character;
// Continued for a restored save.
enum class InitResult
{
  Failed,
  NewGame,
  Continued
};

// Initializes the game world and rendering. Returns once the world is set up
// and the first screen has been drawn; the first turn has NOT yet run.
InitResult init_game_session();

// Runs the main game loop. Blocks until the player quits.
// Pass reset_clock = true for a new game (resets Tick/Player.click on the
// first turn). In the GDExtension build this is called on a background thread.
void run_game_loop(bool reset_clock);

#endif
