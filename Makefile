# The value of LIBDIR should be the same as OMEGALIB in defs.h
BINDIR = /home/sdossey/omega/
LIBDIR = /home/sdossey/omega/lib/

CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g
#CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -DSAVE_LEVELS
#CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -DPLATFORM_WINDOWS
#CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

CXX = g++
#CXX = clang++

LDFLAGS = -lstdc++
CPP = $(CXX) -E

# POSIX systems use ncurses while Windows uses pdcurses
LIBS = -lncurses
#LIBS = -lpdcurses

#################### that's it for changing the Makefile ####################

OBJ = omega.o abyss.o aux1.o aux2.o aux3.o char.o city.o\
      clrgen.o command1.o command2.o command3.o\
      country.o effect1.o effect2.o effect3.o\
      etc.o env.o file.o gen1.o gen2.o guild1.o guild2.o house.o\
      init.o inv.o item.o itemf1.o itemf2.o itemf3.o lev.o\
      mmelee.o mmove.o mon.o move.o movef.o mspec.o\
      mstrike.o mtalk.o priest.o\
      save.o scr.o site1.o site2.o\
      spell.o time.o trap.o util.o village.o

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
	rm -f $(OBJ) genclr genclr.o
	rm -i omega

$(OBJ): clrgen.h defs.h extern.h glob.h iinit.h minit.h

clrgen.h clrgen.cpp: genclr.cpp minit.h defs.h
	$(MAKE) genclr
	$(CPP) -DOMEGA_CLRGEN *.{cpp,h} | ./genclr clrgen.cpp clrgen.h

genclr: genclr.o
	$(CXX) $(LDFLAGS) genclr.o -o genclr
