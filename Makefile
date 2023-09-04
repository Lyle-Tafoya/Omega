# The value of LIBDIR should be the same as OMEGALIB in defs.h
BINDIR = /home/sdossey/omega/
LIBDIR = /home/sdossey/omega/lib/

# Add -DPLATFORM_WINDOWS when compiling for Windows
# -DSAVE_LEVELS is broken
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g

CXX = g++

LDFLAGS = -lstdc++
CPP = $(CXX) -E

LIBS = -lncurses

#################### that's it for changing the Makefile ####################

OBJ = src/omega.o src/abyss.o src/aux1.o src/aux2.o src/aux3.o src/char.o src/city.o\
      src/clrgen.o src/command1.o src/command2.o src/command3.o\
      src/country.o src/effect1.o src/effect2.o src/effect3.o\
      src/etc.o src/env.o src/file.o src/gen1.o src/gen2.o src/guild1.o src/guild2.o src/house.o\
      src/iinit.o src/interactive_menu.o src/inv.o src/item.o src/itemf1.o src/itemf2.o src/itemf3.o \
      src/lev.o src/minit.o src/mmelee.o src/mmove.o src/mon.o src/move.o src/movef.o src/mspec.o\
      src/mstrike.o src/mtalk.o src/priest.o\
      src/save.o src/scr.o src/scrolling_buffer.o src/site1.o src/site2.o\
      src/spell.o src/time.o src/trap.o src/util.o src/village.o

omega: $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) $(LIBS) -o omega

install: omega $(BINDIR) $(LIBDIR)
	cp omega $(BINDIR)
	chmod 4711 $(BINDIR)/omega
	- cp lib/* $(LIBDIR)
	chmod 0644 $(LIBDIR)/help*.txt $(LIBDIR)/license.txt $(LIBDIR)/motd.txt $(LIBDIR)/thanks.txt $(LIBDIR)/update.txt
	chmod 0600 $(LIBDIR)/abyss.txt $(LIBDIR)/scroll[1234].txt $(LIBDIR)/*.dat
	chmod 0600 $(LIBDIR)/omega.hi $(LIBDIR)/omega.log $(LIBDIR)/omegahi.bak

install_not_suid: omega $(BINDIR) $(LIBDIR)
	cp omega $(BINDIR)
	chmod 0711 $(BINDIR)/omega
	- cp lib/* $(LIBDIR)
	chmod 0644 $(LIBDIR)/help*.txt $(LIBDIR)/license.txt $(LIBDIR)/motd.txt $(LIBDIR)/thanks.txt $(LIBDIR)/update.txt
	chmod 0644 $(LIBDIR)/abyss.txt $(LIBDIR)/scroll[1234].txt $(LIBDIR)/*.dat
	chmod 0666 $(LIBDIR)/omega.hi $(LIBDIR)/omega.log
	chmod 0600 $(LIBDIR)/omegahi.bak

clean:
	rm -f $(OBJ) src/genclr src/genclr.o
	rm -i omega

$(OBJ): src/clrgen.h src/defs.h src/extern.h src/glob.h src/interactive_menu.hpp src/scrolling_buffer.hpp

src/clrgen.h src/clrgen.cpp: src/genclr.cpp src/minit.cpp src/defs.h
	$(MAKE) src/genclr
	$(CPP) -DOMEGA_CLRGEN src/*.cpp src/*.h | ./src/genclr

genclr: src/genclr.o
	$(CXX) $(LDFLAGS) src/genclr.o -o src/genclr
