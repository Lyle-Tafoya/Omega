# Omega

My C++ fork of the 1987 roguelike "Omega", featuring numerous bug-fixes as well as balance and UI improvements.

## Build Instructions

### Linux

```
git clone https://github.com/Lyle-Tafoya/Omega.git
cd omega
make
```

By default, the Makefile assumes you are using ncurses and a version of g++ which supports C++17. However, I have also successfully compiled using clang++ as well as alternative curses implementations such as PDCursesMod.

To compile with clang++, simply edit the Makefile from `CXX = g++` to `CXX = clang++`

To link against PDCursesMod, edit the Makefile from `LIBS = -lncurses` to `LIBS = -lpdcurses`

### Windows

I use MSYS2 MingW x64. Installing MSYS2 and setting up your build environment are beyond the scope of this guide. You should reference their official documentation if you need help with that.

```
git clone https://github.com/Lyle-Tafoya/Omega.git
cd omega
```

You will have to make a couple changes to the Makefile.

Change from `CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g` to `CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g -DPLATFORM_WINDOWS`

Change from `LIBS = -lncurses` to `LIBS = -lpdcurses`

Once these changes to the Makefile are complete, the only remaining step is:

```
make
```

The curses implementation I use on Windows is PDCursesMod. Specifically, I have been testing with the "wincon" build of PDCursesMod. Building PDCursesMod is beyond the scope of this guide and you should reference their official documentation if you need help with that.
