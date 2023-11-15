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

/* file.c */
/* functions with file access in them. Also some direct calls to
   curses functions */

#include "file.h"

#include "glob.h"
#include "scr.h"

#include <array>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

extern std::string get_home_path();
extern std::string version_string(int version);

void save_omegarc()
{
  std::string omegarc_filename = std::format("{}/.omegarc", get_home_path());
  std::ofstream omegarc_file(omegarc_filename, std::ios::binary);
  if(!omegarc_file)
  {
    queue_message("Sorry, couldn't save .omegarc for some reason.");
  }
  else
  {
    file_write(omegarc_file, CONFIG_FILE_VERSION);
    file_write(omegarc_file, GAME_VERSION);
    file_write(omegarc_file, Player.maxstr);
    file_write(omegarc_file, Player.maxcon);
    file_write(omegarc_file, Player.maxdex);
    file_write(omegarc_file, Player.maxagi);
    file_write(omegarc_file, Player.maxiq);
    file_write(omegarc_file, Player.maxpow);
  }
}

bool read_omegarc()
{
  std::filesystem::path omegarc_path{std::format("{}/.omegarc", get_home_path())};
  std::ifstream omegarc_file{omegarc_path, std::ios::binary};
  if(!omegarc_file.is_open())
  {
    queue_message("Found omegarc, but could not open it!");
    return false;
  }
  int config_file_version;
  file_read(omegarc_file, config_file_version);
  int game_version;
  file_read(omegarc_file, game_version);
  if(config_file_version != CONFIG_FILE_VERSION)
  {
    omegarc_file.close();
    queue_message("Sorry, I can't load an outdated omegarc file!");
    queue_message("omegarc is from version " + version_string(game_version));
    return false;
  }
  file_read(omegarc_file, Player.maxstr);
  file_read(omegarc_file, Player.maxcon);
  file_read(omegarc_file, Player.maxdex);
  file_read(omegarc_file, Player.maxagi);
  file_read(omegarc_file, Player.maxiq);
  file_read(omegarc_file, Player.maxpow);
  Player.str = Player.maxstr;
  Player.con = Player.maxcon;
  Player.dex = Player.maxdex;
  Player.agi = Player.maxagi;
  Player.iq  = Player.maxiq;
  Player.pow = Player.maxpow;
  omegarc_file.close();
  if(omegarc_file.fail())
  {
    queue_message("Error reading omegarc file!");
    return false;
  }
  else
  {
    return true;
  }
}

std::fstream check_fstream_open(const std::string &file_path, std::ios::openmode mode)
{
  std::fstream file{file_path, mode};
  while(!file)
  {
    queue_message("Warning! Error opening file: " + file_path);
    queue_message(" Abort or Retry? [ar] ");
    char response;
    do
    {
      response = static_cast<char>(mcigetc());
    } while(response != 'a' && response != 'r');
    if(response == 'r')
    {
      file.open(file_path);
    }
    else
    {
      queue_message("Sorry 'bout that.... Saving character, then quitting.");
      save(true);
      endgraf();
      exit(0);
    }
  }
  return file;
}

FILE *checkfopen(const std::string &filestring, const std::string &optionstring)
{
  char response;

  FILE *fd = fopen(filestring.c_str(), optionstring.c_str());
  while(!fd)
  {
    queue_message("Warning! Error opening file: " + filestring);
    queue_message(" Abort or Retry? [ar] ");
    do
    {
      response = static_cast<char>(mcigetc());
    } while((response != 'a') && (response != 'r'));
    if(response == 'r')
    {
      fd = fopen(filestring.c_str(), optionstring.c_str());
    }
    else
    {
      queue_message("Sorry 'bout that.... Saving character, then quitting.");
      save(true);
      endgraf();
      exit(0);
    }
  }
  return (fd);
}

void commandlist()
{
  if(Current_Environment == E_COUNTRYSIDE)
  {
    displayfile(std::format("{}help13.txt", Omegalib));
  }
  else
  {
    displayfile(std::format("{}help12.txt", Omegalib));
  }
  xredraw();
}

// display a file given a string name of file
void displaycryptfile(const std::string &filestr)
{
  FILE *fd = checkfopen(filestr, "rb");
  int c, d = ' ';
  char key = 100;

  clear();
  refresh();
  c = fgetc(fd);
  while((c != EOF) && (static_cast<char>(d) != 'q') && (static_cast<char>(d) != ESCAPE))
  {
    if(getcury(stdscr) > ScreenLength)
    {
      standout();
      printw("\n-More-");
      standend();
      refresh();
      d = wgetch(stdscr);
      clear();
    }
    key = static_cast<unsigned char>(c) ^ key;
    printw("%c", key);
    c = fgetc(fd);
  }
  if((static_cast<char>(d) != 'q') && (static_cast<char>(d) != ESCAPE))
  {
    standout();
    printw("\n-Done-");
    standend();
    refresh();
    getch();
  }
  clear();
  refresh();
  fclose(fd);
}

void user_intro()
{
  displaycryptfile(std::format("{}intro.txt", Omegalib));
  xredraw();
}

void show_license()
{
  displayfile(std::format("{}license.txt", Omegalib));
  xredraw();
}

void abyss_file()
{
  displaycryptfile(std::format("{}abyss.txt", Omegalib));
}

void combat_help()
{
  displayfile(std::format("{}help5.txt", Omegalib));
  menuclear();
}

void cityguidefile()
{
  displaycryptfile(std::format("{}scroll2.txt", Omegalib));
  xredraw();
}

void wishfile()
{
  displaycryptfile(std::format("{}scroll3.txt", Omegalib));
  xredraw();
}

void adeptfile()
{
  displaycryptfile(std::format("{}scroll4.txt", Omegalib));
  xredraw();
}

void theologyfile()
{
  displaycryptfile(std::format("{}scroll1.txt", Omegalib));
  xredraw();
}

void showmotd()
{
  displayfile(std::format("{}motd.txt", Omegalib));
}

void lock_score_file()
{
#ifdef MULTI_USER_SYSTEM
  int attempts = 0;
  int64_t timestamp, last_timestamp = -1;

  std::string filepath = std::format("{}omega.hi.lock", Omegalib);
  std::fstream lock;
  do
  {
    std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
    std::chrono::milliseconds milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch());
    auto milliseconds_count = milliseconds.count();
    lock = std::fstream{filepath, std::ios::out | std::ios::noreplace} << milliseconds_count;
    if(!lock.is_open())
    {
      std::fstream lockfile{filepath, std::ios::in};
      if(lockfile.is_open())
      {
        lockfile >> timestamp;
        lockfile.close();
        if(timestamp != last_timestamp)
        {
          attempts = 0;
        }
        last_timestamp = timestamp;
      }
      ++attempts;
      if(attempts > 10)
      {
        // Assume that lock file has been abandoned
        std::filesystem::remove(filepath);
      }
      else
      {
        std::this_thread::sleep_for(std::chrono::seconds(2));
      }
    }
  } while(!lock.is_open());
  lock.close();
#endif
}

void unlock_score_file()
{
  std::filesystem::remove(std::format("{}omega.hi.lock", Omegalib));
}

void read_scores()
{
  lock_score_file();

  std::string high_scores_file_path = std::format("{}omega.hi", Omegalib);
  std::fstream high_score_file      = check_fstream_open(high_scores_file_path, std::ios::in);

  std::getline(high_score_file, Hiscorer);
  std::getline(high_score_file, Hidescrip);
  high_score_file >> Hiscore >> Hilevel >> Hibehavior >> std::ws;
  for(int i = 1; i < 7; ++i)
  {
    std::getline(high_score_file, Priest[i]);
    high_score_file >> Priestlevel[i] >> Priestbehavior[i] >> std::ws;
  }
  std::getline(high_score_file, Shadowlord);
  high_score_file >> Shadowlordlevel >> Shadowlordbehavior >> std::ws;
  std::getline(high_score_file, Commandant);
  high_score_file >> Commandantlevel >> Commandantbehavior >> std::ws;
  std::getline(high_score_file, Archmage);
  high_score_file >> Archmagelevel >> Archmagebehavior >> std::ws;
  std::getline(high_score_file, Prime);
  high_score_file >> Primelevel >> Primebehavior >> std::ws;
  std::getline(high_score_file, Champion);
  high_score_file >> Championlevel >> Championbehavior >> std::ws;
  std::getline(high_score_file, Duke);
  high_score_file >> Dukelevel >> Dukebehavior >> std::ws;
  std::getline(high_score_file, Chaoslord);
  high_score_file >> Chaoslordlevel >> Chaos >> Chaoslordbehavior >> std::ws;
  std::getline(high_score_file, Lawlord);
  high_score_file >> Lawlordlevel >> Law >> Lawlordbehavior >> std::ws;
  std::getline(high_score_file, Justiciar);
  high_score_file >> Justiciarlevel >> Justiciarbehavior >> std::ws;
  std::getline(high_score_file, Grandmaster);
  high_score_file >> Grandmasterlevel >> Grandmasterbehavior >> std::ws;

  high_score_file.close();

  unlock_score_file();
}

/* Writes a new high-score file, with the nominated npc as whatever it is */
/* in this particular game, but the others as they appear in the file. */
void save_hiscore_npc(int npc)
{
  if(gamestatusp(CHEATED, GameStatus))
  {
    return;
  }
  lock_score_file();
  std::string infile_name  = std::format("{}omega.hi", Omegalib);
  std::fstream infile      = check_fstream_open(infile_name, std::ios::in);
  std::string outfile_name = std::format("{}omega.hi.new", Omegalib);
  std::fstream outfile     = check_fstream_open(outfile_name, std::ios::out);
  for(int i = 0; i < 16; ++i)
  {
    if(npc == i)
    {
      switch(i)
      {
        case 0:
          outfile << std::format(
            "{}\n{}\n{} {} {}\n", Hiscorer, Hidescrip, Hiscore, Hilevel, Hibehavior
          );
          break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
          outfile << std::format("{}\n{} {}\n", Priest[i], Priestlevel[i], Priestbehavior[i]);
          break;
        case 7:
          outfile << std::format("{}\n{} {}\n", Shadowlord, Shadowlordlevel, Shadowlordbehavior);
          break;
        case 8:
          outfile << std::format("{}\n{} {}\n", Commandant, Commandantlevel, Commandantbehavior);
          break;
        case 9:
          outfile << std::format("{}\n{} {}\n", Archmage, Archmagelevel, Archmagebehavior);
          break;
        case 10:
          outfile << std::format("{}\n{} {}\n", Prime, Primelevel, Primebehavior);
          break;
        case 11:
          outfile << std::format("{}\n{} {}\n", Champion, Championlevel, Championbehavior);
          break;
        case 12:
          outfile << std::format("{}\n{} {}\n", Duke, Dukelevel, Dukebehavior);
          break;
        case 13:
          outfile << std::format("{}\n{} {} {}\n", Chaoslord, Chaoslordlevel, Chaos, Chaoslordbehavior);
          break;
        case 14:
          outfile << std::format("{}\n{} {} {}\n", Lawlord, Lawlordlevel, Law, Lawlordbehavior);
          break;
        case 15:
          outfile << std::format("{}\n{} {}\n", Justiciar, Justiciarlevel, Justiciarbehavior);
          break;
        case 16:
          outfile << std::format("{}\n{} {}", Grandmaster, Grandmasterlevel, Grandmasterbehavior);
      }
    }
    std::string buffer;
    if(i == 0)
    {
      std::getline(infile, buffer);
      if(i != npc)
      {
        outfile << buffer << '\n';
      }
    }
    std::getline(infile, buffer);
    if(i != npc)
    {
      outfile << buffer << '\n';
    }
    std::getline(infile, buffer);
    if(i != npc)
    {
      outfile << buffer << '\n';
    }
  }
  infile.close();
  outfile.close();

  std::filesystem::rename(outfile_name, infile_name);
  unlock_score_file();
}

void checkhigh(const std::string &descrip, int behavior)
{
  long points;

  if(FixedPoints > 0)
  {
    points = FixedPoints;
  }
  else
  {
    points = calc_points();
  }
  if(!gamestatusp(CHEATED, GameStatus))
  {
    if(Hiscore < points)
    {
      Hiscorer   = Player.name;
      Hidescrip  = descrip;
      Hiscore    = points;
      Hilevel    = Player.level;
      Hibehavior = behavior;
      save_hiscore_npc(0);
      queue_message("Yow! A new high score!");
    }
    if(Player.alignment < Chaos)
    {
      Chaoslord         = Player.name;
      Chaoslordlevel    = Player.level;
      Chaos             = Player.alignment;
      Chaoslordbehavior = behavior;
      save_hiscore_npc(13);
      queue_message("Criminy! A new Lord of Chaos!");
    }
    if(Player.alignment > Law)
    {
      Lawlord         = Player.name;
      Lawlordlevel    = Player.level;
      Law             = Player.alignment;
      Lawlordbehavior = behavior;
      save_hiscore_npc(14);
      queue_message("Gosh! A new Lord of Law!");
    }
  }
}

void extendlog(const std::string &descrip, int lifestatus)
{
  if((Player.level > 0) && (!gamestatusp(CHEATED, GameStatus)))
  {
    int npcbehavior = fixnpc(lifestatus);
    checkhigh(descrip, npcbehavior);
    FILE *fd = checkfopen(std::format("{}omega.log", Omegalib), "a");
    fprintf(fd, " %d %d %d %s\n", lifestatus, Player.level, npcbehavior, Player.name.c_str());
    fclose(fd);
  }
}

bool test_file_access(const std::string &file_name, char mode)
{
  std::fstream file;
  auto file_mode = (mode == 'r' ? std::ios::in : std::ios::in | std::ios::out);
  file.open(file_name, file_mode);
  if(!file)
  {
    return false;
  }
  file.close();
  return true;
}

const std::array<std::string, 27> required_file_list{
  "city.dat",     "country.dat",  "dlair.dat",    "misle.dat",    "court.dat",    "speak.dat",
  "temple.dat",   "abyss.dat",    "village1.dat", "village2.dat", "village3.dat", "village4.dat",
  "village5.dat", "village6.dat", "home1.dat",    "home2.dat",    "home3.dat",    "arena.dat",
  "maze1.dat",    "maze2.dat",    "maze3.dat",    "maze4.dat",    "omega.hi",     "omega.log",
  "motd.txt",     "license.txt",  "circle.dat"};

const std::array<std::string, 18> optional_file_list{
  "help1.txt", "help2.txt",   "help4.txt",   "help5.txt",   "help6.txt",  "help7.txt",
  "help8.txt", "help9.txt",   "help10.txt",  "help11.txt",  "help12.txt", "help13.txt",
  "abyss.txt", "scroll1.txt", "scroll2.txt", "scroll3.txt", "scroll4.txt"};

/* Checks existence of omega data files */
/* Returns 1 if OK, 0 if impossible to run, -1 if possible but not OK */
int filecheck()
{
  int impossible = false, badbutpossible = false;

  std::string omega_lib_path = Omegalib;
  for(const std::string &file_name : required_file_list)
  {
    std::string file_path = omega_lib_path + file_name;
    if(file_name == "omega.hi" || file_name == "omega.log")
    {
      if(!test_file_access(file_path, 'w'))
      {
        impossible = true;
        std::cout << "\nError! File not appendabe or accessible: " << file_path;
      }
    }
    else if(!test_file_access(file_path, 'r'))
    {
      impossible = true;
      std::cout << "\nError! File not accessible: " << file_path;
    }
  }
  for(const std::string &file_name : optional_file_list)
  {
    std::string file_path = omega_lib_path + file_name;
    if(!test_file_access(file_path, 'r'))
    {
      badbutpossible = true;
      std::cout << "\nWarning! File not accessible: " << file_path;
    }
  }
  if(impossible)
  {
    printf("\nFurther execution is impossible. Sorry.");
    if(omega_lib_path != OMEGALIB)
    {
      std::cout << "\nEnvironment variable OMEGALIB badly set\n";
    }
    else
    {
      std::cout << "\nOMEGALIB may be badly #defined in defs.h\n";
#ifdef USER_DEFINED_OMEGALIB
      std::cout << "\nYou can set the environment variable OMEGALIB to\n";
      std::cout << "the location of the omegalib directory.\n";
#endif
    }
    return 0;
  }
  else if(badbutpossible)
  {
    std::cout << "\nFurther execution may cause anomalous behavior.";
    std::cout << "\nContinue anyhow? [yn] ";
    if(getchar() == 'y')
    {
      return -1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return 1;
  }
}

/* display a file given a string name of file */
void displayfile(const std::string &filestr)
{
  FILE *fd = checkfopen(filestr, "r");
  int c, d = ' ';
  clear();
  refresh();
  c = fgetc(fd);
  while((c != EOF) && (static_cast<char>(d) != 'q') && (static_cast<char>(d) != ESCAPE))
  {
    if(getcury(stdscr) > ScreenLength)
    {
      standout();
      printw("\n-More-");
      standend();
      refresh();
      d = wgetch(stdscr);
      clear();
    }
    printw("%c", static_cast<char>(c));
    c = fgetc(fd);
  }
  if((static_cast<char>(d) != 'q') && (static_cast<char>(d) != ESCAPE))
  {
    standout();
    printw("\n-Done-");
    standend();
    refresh();
    getch();
  }
  clear();
  refresh();
  fclose(fd);
}
