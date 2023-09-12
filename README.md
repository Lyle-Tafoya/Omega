# Omega

My C++ fork of the 1987 roguelike "Omega", featuring numerous bug-fixes as well as balance and UI improvements.
![Screenshot of Omega](Omega.png)

## Build Instructions

### Linux

```
git clone https://github.com/Lyle-Tafoya/Omega.git
mkdir Omega/build
cd Omega/build
cmake ..
make
```

To build this project, you will need g++ version 13 or greater.

### Windows

I have been building Omega for Windows using NMake from the "x64 Native Tools Command Prompt for VS 2022"

```
git clone https://github.com/Lyle-Tafoya/Omega.git
mkdir Omega\build
cd Omega\build
cmake .. -G "NMake Makefiles"
nmake
```
