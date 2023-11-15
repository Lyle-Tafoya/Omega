/*
Copyright (C) by Lyle Tafoya, 2019, 2021-2023

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

#ifndef OMEGA_SCROLLING_BUFFER_HPP_
#define OMEGA_SCROLLING_BUFFER_HPP_

#include <cstdint>
#include <deque>
#include <queue>
#include <string>

class scrolling_buffer
{
public:
  struct queued_message
  {
    std::string message;
    bool force_break;
  };

  scrolling_buffer(uint16_t width = 80, uint16_t length = 64);
  void receive(const std::string &message, bool force_break = false);
  void append(const std::string &message, bool pad = true, bool force_break = false);
  void replace_last(const std::string &message);
  void pop_back();
  void resize(uint16_t width, uint16_t length);
  void clear();
  const std::deque<std::string> &get_message_history(bool update = true);
  uint16_t get_width() const;
  uint16_t get_length() const;

private:
  uint16_t width;
  uint16_t length;
  void process_queue();
  std::deque<std::string> message_history;
  std::queue<queued_message> message_queue;
};

#endif
