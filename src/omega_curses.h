/*
Copyright (C) by Ken Paulson, 2026

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

#ifndef OMEGA_CURSES_H_
#define OMEGA_CURSES_H_

// Single point of indirection for the curses API. Alternative front-ends
// (e.g. a graphical port) define OMEGA_CURSES_STUB and supply curses_stub.h,
// which provides the curses types, constants, and function signatures while
// routing output to the replacement renderer. The standard build includes
// the real curses header.
#ifdef OMEGA_CURSES_STUB
#  include "curses_stub.h"
#else
#  include <curses.h>
#endif

#endif // OMEGA_CURSES_H_
