/* omega (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* file.c */
/* functions with file access in them. Also some direct calls to
   curses functions */

#ifdef MSDOS_SUPPORTED_ANTIQUE
# include "curses.h"
#else
# ifdef AMIGA
#  include <curses210.h>
# else
#  include <curses.h>
# endif
# include <sys/types.h>
# include <unistd.h>
# include <sys/file.h>
# include <fcntl.h>
# include <errno.h>
#endif

#include "glob.h"

FILE *checkfopen(filestring,optionstring)
char *filestring,*optionstring;
{
  FILE *fd;
  char response;

  change_to_game_perms();
  fd = fopen(filestring,optionstring);
  clearmsg();
  while (fd == NULL) {
    print3("Warning! Error opening file:");
    nprint3(filestring);
    print1(" Abort or Retry? [ar] ");
    do response = (char) mcigetc();
    while ((response != 'a') && (response != 'r'));
    if (response == 'r') fd = fopen(filestring,optionstring);
    else {
      print2("Sorry 'bout that.... Saving character, then quitting.");
      morewait();
      save(optionp(COMPRESS_OPTION), TRUE);
      endgraf();
      exit(0);
    }
  }
  return(fd);
}

void commandlist()
{
  strcpy(Str1,Omegalib);
  if (Current_Environment == E_COUNTRYSIDE)
    strcat(Str1,"help13.txt");
  else strcat(Str1,"help12.txt");
  displayfile(Str1);
  xredraw();
}


void user_intro()
{
  strcpy(Str1,Omegalib);
  strcat(Str1,"intro.txt");
  displaycryptfile(Str1);
  xredraw();
}

void show_license()
{
  strcpy(Str1,Omegalib);
  strcat(Str1,"license.txt");
  displayfile(Str1);
  xredraw();
}



void abyss_file()
{
  strcpy(Str1,Omegalib);
  strcat(Str1,"abyss.txt");
  displaycryptfile(Str1);
}




void inv_help()
{
  strcpy(Str1,Omegalib);
  strcat(Str1,"help3.txt");
  displayfile(Str1);
  xredraw();
}



void combat_help()
{
  strcpy(Str1,Omegalib);
  strcat(Str1,"help5.txt");
  displayfile(Str1);
  menuclear();
}




void cityguidefile()
{
  strcpy(Str1,Omegalib);
  strcat(Str1,"scroll2.txt");
  displaycryptfile(Str1);
  xredraw();
}


void wishfile()
{
  strcpy(Str1,Omegalib);
  strcat(Str1,"scroll3.txt");
  displaycryptfile(Str1);
  xredraw();
}

void adeptfile()
{
  strcpy(Str1,Omegalib);
  strcat(Str1,"scroll4.txt");
  displaycryptfile(Str1);
  xredraw();
}

void theologyfile()
{
  strcpy(Str1,Omegalib);
  strcat(Str1,"scroll1.txt");
  displaycryptfile(Str1);
  xredraw();
}


void showmotd()
{
  strcpy(Str1,Omegalib);
  strcat(Str1,"motd.txt");
  displayfile(Str1);
}


void lock_score_file()
{
#ifndef MSDOS
    int lock, attempts, thispid, lastpid = 0;
    FILE *lockfile;

    strcpy(Str1,Omegalib);
    strcat(Str1,"omega.hi.lock");
    do
    {
	lock = open(Str1, O_CREAT|O_EXCL, 0600);	/* create lock file */
	if (lock < 0 && errno == EEXIST)
	{
	    lockfile = fopen(Str1, "rb");
	    if (lockfile)
	    {
		fscanf(lockfile, "%d", &thispid);
		fclose(lockfile);
		if (thispid != lastpid)
		    attempts = 0;
		lastpid = thispid;
	    }
	    attempts++;
	    if (attempts > 10)	/* assume that lock file has been abandoned */
		unlink(Str1);	/* so we unlink it ourselves - ugly...	*/
	    else
		sleep(2);
	}
	else if (lock < 0)	/* oops - something very wrong */
	    return;
    } while (lock < 0);
    sprintf(Str1, "%d", getpid());
    write(lock, Str1, strlen(Str1));
    close(lock);
#endif
}

void unlock_score_file()
{
#ifndef MSDOS
    strcpy(Str1,Omegalib);
    strcat(Str1,"omega.hi.lock");
    unlink(Str1);
#endif
}

void showscores()
{
  FILE *fd;
  int i;

  lock_score_file();
  strcpy(Str1,Omegalib);
  strcat(Str1,"omega.hi");
  fd = checkfopen(Str1,"rb");
  filescanstring(fd,Hiscorer);
  filescanstring(fd,Hidescrip);
  fscanf(fd,"%ld %d %d\n",&Hiscore,&Hilevel,&Hibehavior);
  for(i=1;i<7;i++) {
    filescanstring(fd,Priest[i]);
    fscanf(fd,"%d %d\n",&(Priestlevel[i]),&(Priestbehavior[i]));
  }
  filescanstring(fd,Shadowlord);
  fscanf(fd,"%d %d\n",&Shadowlordlevel,&Shadowlordbehavior);
  filescanstring(fd,Commandant);
  fscanf(fd,"%d %d\n",&Commandantlevel,&Commandantbehavior);
  filescanstring(fd,Archmage);
  fscanf(fd,"%d %d\n",&Archmagelevel,&Archmagebehavior);
  filescanstring(fd,Prime);
  fscanf(fd,"%d %d\n",&Primelevel,&Primebehavior);
  filescanstring(fd,Champion);
  fscanf(fd,"%d %d\n",&Championlevel,&Championbehavior);
  filescanstring(fd,Duke);
  fscanf(fd,"%d %d\n",&Dukelevel,&Dukebehavior);
  filescanstring(fd,Chaoslord);
  fscanf(fd,"%d %d %d\n",&Chaoslordlevel,&Chaos,&Chaoslordbehavior);
  filescanstring(fd,Lawlord);
  fscanf(fd,"%d %d %d\n",&Lawlordlevel,&Law,&Lawlordbehavior);
  filescanstring(fd,Justiciar);
  fscanf(fd,"%d %d\n",&Justiciarlevel,&Justiciarbehavior);
  fclose(fd);
  unlock_score_file();
  clear();
  printw("High Score: %ld",Hiscore);
  printw(", by %s (%s)",Hiscorer,levelname(Hilevel));
  printw("\n%s\n",Hidescrip);
  printw("\nLord of Chaos: %s (%s)",Chaoslord,levelname(Chaoslordlevel));
  printw("\nLord of Law: %s (%s)",Lawlord,levelname(Lawlordlevel));
  printw("\n\nDuke of Rampart:              ");
  printw("%s (%s)",Duke,levelname(Dukelevel));
  printw("\nJusticiar:                    ");
  printw("%s (%s)",Justiciar,levelname(Justiciarlevel));
  printw("\nCommandant:                   ");
  printw("%s (%s)",Commandant,levelname(Commandantlevel));
  printw("\nChampion:                     ");
  printw("%s (%s)",Champion,levelname(Championlevel));
  printw("\nArchmage:                     ");
  printw("%s (%s)",Archmage,levelname(Archmagelevel));
  printw("\nPrime Sorceror:               ");
  printw("%s (%s)",Prime,levelname(Primelevel));
  printw("\nShadowlord:                   ");
  printw("%s (%s)",Shadowlord,levelname(Shadowlordlevel));
  printw("\n\nHigh Priests:");
  printw("\n of Odin:                     ");
  printw("%s (%s)",Priest[ODIN],levelname(Priestlevel[ODIN]));
  printw("\n of Set:                      ");
  printw("%s (%s)",Priest[SET],levelname(Priestlevel[SET]));
  printw("\n of Athena:                   ");
  printw("%s (%s)",Priest[ATHENA],levelname(Priestlevel[ATHENA]));
  printw("\n of Hecate:                   ");
  printw("%s (%s)",Priest[HECATE],levelname(Priestlevel[HECATE]));
  printw("\n of the Lords of Destiny:     ");
  printw("%s (%s)",Priest[DESTINY],levelname(Priestlevel[DESTINY]));
  printw("\nThe ArchDruid:                ");
  printw("%s (%s)",Priest[DRUID],levelname(Priestlevel[DRUID]));
  printw("\n\nHit any key to continue.");
  refresh();
  wgetch(stdscr);
  clear_screen();
}

/* Writes a new high-score file, with the nominated npc as whatever it is */
/* in this particular game, but the others as they appear in the file. */
void save_hiscore_npc(npc)
int npc;
{
  FILE *infile, *outfile;
  char buffer[80];
  int i;

  if (gamestatusp(CHEATED))
      return;
  lock_score_file();
  strcpy(Str1,Omegalib);
  strcat(Str1,"omega.hi");
  infile = checkfopen(Str1,"rb");
  strcpy(Str2,Omegalib);
#ifdef MSDOS
  strcat(Str2,"omegahi.new");	/* stupid 8.3 msdos filename limit */
#else
  strcat(Str2,"omega.hi.new");
#endif
  outfile = checkfopen(Str2,"wb");
  for (i = 0; i < 16; i++) {
    if (npc == i) {
      switch (i) {
	case 0:
	  fprintf(outfile,"%s\n%s\n%ld %d %d\n",Hiscorer, Hidescrip, Hiscore,
	    Hilevel, Hibehavior);
	  break;
	case 1: case 2: case 3: case 4: case 5: case 6:
	  fprintf(outfile,"%s\n%d %d\n", Priest[i], Priestlevel[i],
	    Priestbehavior[i]);
	  break;
	case 7:
	  fprintf(outfile,"%s\n%d %d\n", Shadowlord, Shadowlordlevel,
	    Shadowlordbehavior);
	  break;
	case 8:
	  fprintf(outfile,"%s\n%d %d\n", Commandant, Commandantlevel,
	    Commandantbehavior);
	  break;
	case 9:
	  fprintf(outfile,"%s\n%d %d\n", Archmage, Archmagelevel,
	    Archmagebehavior);
	  break;
	case 10:
	  fprintf(outfile,"%s\n%d %d\n", Prime, Primelevel, Primebehavior);
	  break;
	case 11:
	  fprintf(outfile,"%s\n%d %d\n", Champion, Championlevel,
	    Championbehavior);
	  break;
	case 12:
	  fprintf(outfile,"%s\n%d %d\n", Duke, Dukelevel, Dukebehavior);
	  break;
	case 13:
	  fprintf(outfile,"%s\n%d %d %d\n",Chaoslord, Chaoslordlevel, Chaos,
	    Chaoslordbehavior);
	  break;
	case 14:
	  fprintf(outfile,"%s\n%d %d %d\n", Lawlord, Lawlordlevel, Law,
	    Lawlordbehavior);
	  break;
	case 15:
	  fprintf(outfile,"%s\n%d %d\n", Justiciar, Justiciarlevel,
	    Justiciarbehavior);
	  break;
      }
    }
    if (i == 0) {
      fgets(buffer, 80, infile);
      if (i != npc)
	fputs(buffer, outfile);
    }
    fgets(buffer, 80, infile);
    if (i != npc)
      fputs(buffer, outfile);
    fgets(buffer, 80, infile);
    if (i != npc)
      fputs(buffer, outfile);
  }
  fclose(infile);
  fclose(outfile);
  unlink(Str1);
#if defined(MSDOS) || defined(AMIGA)
  rename(Str2, Str1);
#else
  link(Str2, Str1);
  unlink(Str2);	/* renames, but sys-V doesn't have rename()... */
#endif
  unlock_score_file();
}

void checkhigh(descrip,behavior)
char *descrip;
int behavior;
{
  long points;

  if (FixedPoints > 0) points = FixedPoints;
  else points = calc_points();
  if (!gamestatusp(CHEATED)) {
    if (Hiscore < points) {
      strcpy(Hiscorer, Player.name);
      strcpy(Hidescrip, descrip);
      Hiscore = points;
      Hilevel = Player.level;
      Hibehavior = behavior;
      save_hiscore_npc(0);
      mprint("Yow! A new high score!");
      morewait();
    }
    if (Player.alignment < Chaos) {
      strcpy(Chaoslord, Player.name);
      Chaoslordlevel = Player.level;
      Chaos = Player.alignment;
      Chaoslordbehavior = behavior;
      save_hiscore_npc(13);
      mprint("Criminy! A new Lord of Chaos!");
      morewait();
    }
    if (Player.alignment > Law) {
      strcpy(Lawlord, Player.name);
      Lawlordlevel = Player.level;
      Law = Player.alignment;
      Lawlordbehavior = behavior;
      save_hiscore_npc(14);
      mprint("Gosh! A new Lord of Law!");
      morewait();
    }
  }
}

void extendlog(descrip,lifestatus)
char *descrip;
int lifestatus;
{
  FILE *fd;
  int npcbehavior;

  if ((Player.level > 0) && (! gamestatusp(CHEATED))) {
    change_to_game_perms();
    npcbehavior=fixnpc(lifestatus);
    checkhigh(descrip,npcbehavior);
    strcpy(Str1,Omegalib);
    strcat(Str1,"omega.log");
    fd = checkfopen(Str1,"a");
    fprintf(fd, " %d %d %d %s\n", lifestatus, Player.level, npcbehavior,
      Player.name);
    fclose(fd);
  }
}





#ifndef MSDOS
/* reads a string from a file. If it is a line with more than 80 char's,
   then remainder of line to \n is consumed */
void filescanstring(fd,fstr)
FILE *fd;
char *fstr;
{
  int i= -1;
  int byte='x';
  while ((i<80) && (byte != '\n') && (byte != EOF)) {
    i++;
    byte=fgetc(fd);
    fstr[i] = byte;
  } 
  if (byte != '\n')
    while((byte!='\n') && (byte != EOF))
      byte=fgetc(fd);
  fstr[i]=0;
}
#endif

#ifdef MSDOS
int test_file_access(file_name, mode)
char *file_name;
char mode;
{
    FILE *fd;

    if (mode == 'r')
	fd = fopen(file_name, "r");
    else
	fd = fopen(file_name, "r+");
    if (!fd)
	return 0;
    fclose(fd);
    return 1;
}
#else
int test_file_access(file_name, mode)
char *file_name;
char mode;
{
    int fd;

    if (mode == 'r')
	fd = open(file_name, O_RDONLY, 0);
    else
	fd = open(file_name, O_RDWR, 0);
    if (fd < 0)
	return 0;
    close(fd);
    return 1;
}
#endif

char *required_file_list[] =
{
  "city.dat", "country.dat", "dlair.dat", "misle.dat", "court.dat",
  "speak.dat", "temple.dat", "abyss.dat", "village1.dat", "village2.dat",
  "village3.dat", "village4.dat", "village5.dat", "village6.dat",
  "home1.dat", "home2.dat", "home3.dat", "arena.dat", "maze1.dat",
  "maze2.dat", "maze3.dat", "maze4.dat", "omega.hi", "omega.log", "motd.txt",
  "license.txt", "circle.dat", NULL
};

char *optional_file_list[] =
{
  "help1.txt", "help2.txt", "help3.txt", "help4.txt", "help5.txt",
  "help6.txt", "help7.txt", "help8.txt", "help9.txt", "help10.txt",
  "help11.txt", "help12.txt", "help13.txt", "abyss.txt", "scroll1.txt",
  "scroll2.txt", "scroll3.txt", "scroll4.txt", NULL
};

/* Checks existence of omega data files */
/* Returns 1 if OK, 0 if impossible to run, -1 if possible but not OK */
int filecheck()
{
    int impossible=FALSE,badbutpossible=FALSE;
    int endpos;
    int file;

    strcpy(Str1, Omegalib);
    endpos = strlen(Str1);
    for (file = 0; required_file_list[file]; file++)
    {
	strcpy(&(Str1[endpos]), required_file_list[file]);
	if ((strcmp(required_file_list[file], "omega.hi") == 0 ||
	    strcmp(required_file_list[file], "omega.log") == 0) &&
	    test_file_access(Str1, 'w') == 0)
	{
	    impossible = TRUE;
	    printf("\nError! File not appendable or accessible: %s", Str1);
	}
	else if (test_file_access(Str1, 'r') == 0)
	{
	    impossible = TRUE;
	    printf("\nError! File not accessible: %s", Str1);
	}
    }
    for (file = 0; optional_file_list[file]; file++)
    {
	strcpy(&(Str1[endpos]), optional_file_list[file]);
	if (test_file_access(Str1, 'r') == 0)
	{
	    badbutpossible = TRUE;
	    printf("\nWarning! File not accessible: %s", Str1);
	}
    }
    if (impossible) {
	printf("\nFurther execution is impossible. Sorry.");
	if (strcmp(Omegalib, OMEGALIB))
	  printf("\nEnvironment variable OMEGALIB badly set\n");
	else {
	  printf("\nOMEGALIB may be badly #defined in defs.h\n");
#ifndef FIXED_OMEGALIB
	  printf("\nYou can set the environment variable OMEGALIB to\n");
	  printf("the location of the omegalib directory.\n");
#endif
	}
	return(0);
    }
    else if (badbutpossible) {
	printf("\nFurther execution may cause anomalous behavior.");
	printf("\nContinue anyhow? [yn] ");
	if (getchar()=='y') return(-1);
	else return(0);
    }
    else return(1);
}

/* display a file given a string name of file */
void displayfile(filestr)
char *filestr;
{
  FILE *fd = checkfopen(filestr,"r");
  int c,d=' ';
  int x,y;
  clear();
  refresh();
  c = fgetc(fd);
  while ((c != EOF)&&((char) d != 'q')&&((char) d!=ESCAPE)) {
    getyx(stdscr,y,x);
    if (y > ScreenLength) { 
      standout();
      printw("\n-More-");
      standend();
      refresh();
      d = wgetch(stdscr);
      clear();
    }
    printw("%c",(char) c);
    c = fgetc(fd);
  }
  if (((char) d != 'q')&&((char) d!=ESCAPE)) {
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

/* display a file given a string name of file */
void displaycryptfile(filestr)
char *filestr;
{
  FILE *fd = checkfopen(filestr,"rb");
  int c,d=' ';
  int x,y;
  char key = 100;

  clear();
  refresh();
  c = fgetc(fd);
  while ((c != EOF)&&((char) d != 'q')&&((char) d!=ESCAPE)) {
    getyx(stdscr,y,x);
    if (y > ScreenLength) { 
      standout();
      printw("\n-More-");
      standend();
      refresh();
      d = wgetch(stdscr);
      clear();
    }
    key = ((unsigned char) c)^key;
    printw("%c", key);
    c = fgetc(fd);
  }
  if (((char) d != 'q')&&((char) d!=ESCAPE)) {
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

/* copy a file given a string name of file */
void copyfile(srcstr)
char *srcstr;
{
  char deststr[80];
  char buffer[STRING_LEN];
  FILE *in, *out;

  print1("Enter name of file to create: ");
  strcpy(deststr,msgscanstring());
  if (strlen(deststr) == 0) {
    print2("Aborting...");
    morewait();
    return;
  }
  in = checkfopen(srcstr, "rb");
  change_to_user_perms();
  out = fopen(deststr, "wb");
  if (!out) {
    sprintf(buffer, "Unable to write to file %s - Aborting.", deststr);
    print2(buffer);
    change_to_game_perms();
    morewait();
    fclose(in);
    return;
  }
  print2("Copying file....");
  while (fgets(buffer, STRING_LEN, in))
    fputs(buffer, out);
  fclose(in);
  fclose(out);
  change_to_game_perms();
  print3("Done.");
}    

