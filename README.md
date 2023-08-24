# Omega

My C++ fork of the 1987 roguelike "Omega", featuring numerous bug-fixes as well as balance and UI improvements.
![Screenshot of Omega](Omega.png)

## Build Instructions

### Linux

```
git clone https://github.com/Lyle-Tafoya/Omega.git
cd Omega
make
```

By default, the Makefile assumes you are using ncurses and a version of g++ which supports C++17. However, I have also successfully compiled using clang++ as well as alternative curses implementations such as PDCursesMod.

### Windows

I use MSYS2 MingW x64. Installing MSYS2 and setting up your build environment are beyond the scope of this guide. You should reference their official documentation if you need help with that.

```
git clone https://github.com/Lyle-Tafoya/Omega.git
cd Omega
make CXXFLAGS='-std=c++17 -DPLATFORM_WINDOWS' LIBS='-lpdcurses'
```

The curses implementation I use on Windows is PDCursesMod. Building PDCursesMod is beyond the scope of this guide. You should reference their official documentation if you need help with that.
