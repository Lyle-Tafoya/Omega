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

#ifndef OMEGA_FILE_H_
#define OMEGA_FILE_H_

#include <fstream>
#include <type_traits>

template<typename T>
void file_write(std::ofstream &file, T &&data)
{
  file.write(reinterpret_cast<const char *>(&data), sizeof(data));
}
template<typename T>
void file_read(std::ifstream &file, T &data)
{
  file.read(reinterpret_cast<char *>(&data), sizeof(data));
}

#endif
