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

#include "scrolling_buffer.hpp"

scrolling_buffer::scrolling_buffer(uint16_t width, uint16_t length) : width(width), length(length) {}

void scrolling_buffer::resize(uint16_t width, uint16_t length)
{
  this->width  = width;
  this->length = length;
}

void scrolling_buffer::receive(const std::string &message, bool force_break)
{
  queued_message m = {message, force_break};
  message_queue.emplace(m);
}

void scrolling_buffer::append(const std::string &message, bool pad, bool force_break)
{
  process_queue();
  if(!message_history.empty() && !force_break && message_history.back().size() + message.size() + pad < width)
  {
    std::string &previous_line = message_history.back();
    previous_line += (pad ? " " : "") + message;
  }
  else
  {
    message_history.emplace_back(message);
    if(message_history.size() > length)
    {
      message_history.pop_front();
    }
  }
}

void scrolling_buffer::replace_last(const std::string &message)
{
  message_history.pop_back();
  message_history.emplace_back(message);
}

void scrolling_buffer::pop_back()
{
  message_history.pop_back();
}

void scrolling_buffer::process_queue()
{
  std::string message;
  std::string buffer_row   = "_";
  uint16_t duplicate_count = 1;
  while(!message_queue.empty())
  {
    message          = message_queue.front().message;
    bool force_break = message_queue.front().force_break;
    message_queue.pop();
    while(!message_queue.empty() && message_queue.front().message == message)
    {
      ++duplicate_count;
      message_queue.pop();
    }
    if(duplicate_count > 1)
    {
      message += " x" + std::to_string(duplicate_count);
      duplicate_count = 1;
    }

    if((force_break && buffer_row != "_") || buffer_row.length() + message.length() + 1 > width)
    {
      message_history.emplace_back(buffer_row);
      if(message_history.size() > length)
      {
        message_history.pop_front();
      }
      buffer_row.clear();
    }
    buffer_row += (buffer_row.empty() ? "" : " ") + message;
  }
  if(buffer_row != "_")
  {
    message_history.emplace_back(buffer_row);
    if(message_history.size() > length)
    {
      message_history.pop_front();
    }
  }
}

const std::deque<std::string> &scrolling_buffer::get_message_history(bool update)
{
  if(update)
  {
    process_queue();
  }
  return message_history;
}

uint16_t scrolling_buffer::get_width() const
{
  return width;
}

uint16_t scrolling_buffer::get_length() const
{
  return length;
}

void scrolling_buffer::clear()
{
  message_history.clear();
}
