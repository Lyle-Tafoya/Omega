#ifndef OMEGA_INTERACTIVE_MENU_HPP_
#define OMEGA_INTERACTIVE_MENU_HPP_

#include <string>
#include <vector>
#include <curses.h>

class interactive_menu
{
public:
  interactive_menu(WINDOW *menu_window, uint16_t width, uint16_t height);
  int get_player_input();
  void print();
  void load(const std::vector<std::string> &lines, const std::vector<std::string> &header);
  void load(const std::vector<std::string> &lines);
  void resize(uint16_t width, uint16_t height);
  void move(uint16_t x, uint16_t y);
  void reset_position();

private:
  WINDOW *menu_window;
  std::vector<std::string> lines;
  std::vector<std::string> header;
  uint16_t position = 0;
  uint16_t width;
  uint16_t height;
};

#endif
