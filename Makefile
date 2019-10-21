# This makefile works on a Sun, but may not work on some other machines
# It is pretty obviou what it does, though, just runs cc.
# I use the -g flag for debugging info; if you don't intend to
# hack omega you might change the -g's to -O's for optimization
# Modified Makefile (add install section) by Bill Randle, 1/18/88

BINDIR = /usr/games
# Note: LIBDIR should refer to the directory specified in "odefs.h"
LIBDIR = /usr/games/lib/omegalib

objects = o.o oaux1.o oaux2.o ochar.o ocity.o ocom.o odepths.o\
	oeffect1.o oeffect2.o\
	oetc.o ofile.o ogen.o oguild.o oinititem1.o oinititem2.o\
	oinitmon0to3.o oinitmon4to7.o oinitmon8to10.o\
	oinv.o oitem.o oitemf.o olev.o omon.o omonf.o omove.o\
	oscr.o osite.o ospell.o otime.o outil.o

AUXFILES = ocity.dat ocommands.txt odepths.dat ohelp.txt omega.hiscore\
	omega.intro omega.log omega.lognum omega.motd\
	oscroll1.txt oscroll2.txt

WRITEABLEFILES = $(LIBDIR)/omega.saves $(LIBDIR)/omega.hiscore\
	$(LIBDIR)/omega.log $(LIBDIR)/omega.lognum

sourceflags = -c -O # -g

objectflags = # -g

libraries =  -lcurses -ltermlib

omega: $(objects)
	cc -o $@ $(objectflags) $(objects) $(libraries)

$(objects): odefs.h oglob.h $(@:.o=.c)
	cc $(sourceflags) $(@:.o=.c)

install: omega $(AUXFILES)
	-mkdir $(LIBDIR)
	chmod 777 $(LIBDIR)
	install -s omega $(BINDIR)
	cp $(AUXFILES) $(LIBDIR)
	cp /dev/null $(LIBDIR)/omega.saves
	chmod 666 $(WRITEABLEFILES)

