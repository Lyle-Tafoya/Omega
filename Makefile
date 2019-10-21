# This makefile works on a Sun, but may not work on some other machines

# If you have gcc and don't intend to hack around with the game,
# I recommend replacing cc with gcc and using -O instead of -g
# in the flags.

objects = o.o oabyss.o oaux1.o oaux2.o oaux3.o ochar.o ocity.o\
	ocom1.o ocom2.o ocom3.o\
	ocountry.o oeffect1.o oeffect2.o oeffect3.o\
	oetc.o oenv.o ofile.o ogen1.o ogen2.o oguild1.o oguild2.o ohouse.o\
	oinv.o oitem.o oitemf1.o oitemf2.o oitemf3.o olev.o\
	ommelee.o ommove.o omon.o omove.o omovef.o omspec.o\
	omstrike.o omtalk.o opriest.o\
	osave.o oscr.o osite1.o osite2.o\
	ospell.o otime.o otrap.o outil.o ovillage.o

sourceflags = -O -c

objectflags = #-g

libraries =  -lcurses -ltermlib

omega: $(objects)
	cc -o $@ $(objectflags) $(objects) $(libraries)

clean:
	rm -rf *.o omega

$(objects): odefs.h oglob.h $(@:.o=.c)
	cc $(sourceflags) $(@:.o=.c)
