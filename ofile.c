/* omega (c) 1987,1988 by Laurence Raphael Brothers */
/* ofile.c */
/* functions with file access in them. Also some direct calls to
   curses functions */

#include <curses.h>
#include <sys/file.h>
#include "oglob.h"

#ifndef F_OK
#define F_OK 00 
#define R_OK 04
#define W_OK 02
#endif

FILE *checkfopen(filestring,optionstring)
char *filestring,*optionstring;
{
  FILE *fd;
  char response;
  fd = fopen(filestring,optionstring);
  clearmsg();
  while (fd == NULL) {
    print3("Warning! Error opening file:");
    nprint3(filestring);
    print1(" Abort or Retry? [ar] ");
    do response = mcigetc(); while ((response != 'a') && (response != 'r'));
    if (response == 'r') fd = fopen(filestring,optionstring);
    else {
      print2("Sorry 'bout that.... Bye!");
      morewait();
      endgraf();
      exit(0);
    }
  }
  return(fd);
}

void commandlist()
{
  FILE *fd; 
  strcpy(Str1,OMEGALIB);
  if (Current_Environment == E_COUNTRYSIDE)
    strcat(Str1,"occmds.txt");
  else strcat(Str1,"ocmds.txt");
  fd = checkfopen(Str1,"r");
  showfile(fd);
  fclose(fd);
  clear();
  refresh();
  xredraw();
}


void user_intro()
{
  FILE *fd; 
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ointro.txt");
  fd = checkfopen(Str1,"r");
  showfile(fd);
  fclose(fd);
  clear();
  refresh();
  xredraw();
}

void show_license()
{
  FILE *fd; 
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"olicense.txt");
  fd = checkfopen(Str1,"r");
  showfile(fd);
  fclose(fd);
  clear();
  refresh();
  xredraw();
}



void abyss_file()
{
  FILE *fd; 
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"oabyss.txt");
  fd = checkfopen(Str1,"r");
  showfile(fd);
  fclose(fd);
  clear();
  refresh();
}




void inv_help()
{
  FILE *fd; 
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp3.txt");
  fd = checkfopen(Str1,"r");
  showfile(fd);
  fclose(fd);
  clear();
  refresh();
  xredraw();
}



void combat_help()
{
  FILE *fd; 
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp5.txt");
  fd = checkfopen(Str1,"r");
  showfile(fd);
  fclose(fd);
  clear();
  refresh();
  xredraw();
}




void cityguidefile()
{
  FILE *fd;
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"oscroll2.txt");
  fd = checkfopen(Str1,"r");
  showfile(fd);
  fclose(fd);
  clear();
  refresh();
  xredraw();
}


void wishfile()
{
  FILE *fd;
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"oscroll3.txt");
  fd = checkfopen(Str1,"r");
  showfile(fd);
  fclose(fd);
  clear();
  refresh();
  xredraw();
}

void adeptfile()
{
  FILE *fd;
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"oscroll4.txt");
  fd = checkfopen(Str1,"r");
  showfile(fd);
  fclose(fd);
  clear();
  refresh();
  xredraw();
}

void theologyfile()
{
  FILE *fd;
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"oscroll1.txt");
  fd = checkfopen(Str1,"r");
  showfile(fd);
  fclose(fd);
  clear();
  refresh();
  xredraw();
}


void showmotd()
{
  FILE *fd;
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omotd.txt");
  fd = checkfopen(Str1,"r");
  showfile(fd);
  fclose(fd);
  clear();
  refresh();
}




/* display a file page at a time */
void showfile(fd)
FILE *fd;
{
  int c,d=' ';
  int x,y;
  clear();
  refresh();
  c = fgetc(fd);
  while ((c != EOF)&&((char) d != 'q')&&((char) d!=ESCAPE)) {
    getyx(stdscr,y,x);
    if (y > ScreenLength) {
      printw("\n-More-");
      refresh();
      d = wgetch(stdscr);
      clear();
    }
    printw("%c",(char) c);
    c = fgetc(fd);
  }
  if (((char) d != 'q')&&((char) d!=ESCAPE)) {
    printw("\n-Done-");
    refresh();
    getch();
  }
}    



void showscores()
{
  FILE *fd;
  int i;
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omega.hi");
  fd = checkfopen(Str1,"r");
  filescanstring(fd,Hiscorer);
  filescanstring(fd,Hidescrip);
  fscanf(fd,"%d\n%d\n%d\n",&Hiscore,&Hilevel,&Hibehavior);
  filescanstring(fd,Chaoslord);
  fscanf(fd,"%d\n%d\n%d\n",&Chaoslordlevel,&Chaos,&Chaoslordbehavior);
  filescanstring(fd,Lawlord);
  fscanf(fd,"%d\n%d\n%d\n",&Lawlordlevel,&Law,&Lawlordbehavior);
  filescanstring(fd,Duke);
  fscanf(fd,"%d\n%d\n",&Dukelevel,&Dukebehavior);
  filescanstring(fd,Justiciar);
  fscanf(fd,"%d\n%d\n",&Justiciarlevel,&Justiciarbehavior);
  filescanstring(fd,Commandant);
  fscanf(fd,"%d\n%d\n",&Commandantlevel,&Commandantbehavior);
  filescanstring(fd,Champion);
  fscanf(fd,"%d\n%d\n",&Championlevel,&Championbehavior);
  filescanstring(fd,Archmage);
  fscanf(fd,"%d\n%d\n",&Archmagelevel,&Archmagebehavior);
  filescanstring(fd,Prime);
  fscanf(fd,"%d\n%d\n",&Primelevel,&Primebehavior);
  filescanstring(fd,Shadowlord);
  fscanf(fd,"%d\n%d\n",&Shadowlordlevel,&Shadowlordbehavior);
  for(i=1;i<7;i++) {
    filescanstring(fd,Priest[i]);
    fscanf(fd,"%d\n%d\n",&(Priestlevel[i]),&(Priestbehavior[i]));
  }
  fclose(fd);
  clear();
  printw("High Score: %d",Hiscore);
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
}


/* writes a new high score file */
void checkhigh(descrip,behavior)
char *descrip;
int behavior;
{
  int i,points;
  FILE *fd;

  if (FixedPoints > 0) points = FixedPoints;
  else points = calc_points();

  if (! gamestatusp(CHEATED)) {
    strcpy(Str1,OMEGALIB);
    strcat(Str1,"omega.hi");
    fd = checkfopen(Str1,"w");
    
    if (Hiscore < points) {
      morewait();
      mprint("Yow! A new high score!");
      fprintf(fd,"%s\n",Player.name);
      fprintf(fd,"%s\n",descrip);
      fprintf(fd,"%d\n",points);
      fprintf(fd,"%d\n",Player.level);
      fprintf(fd,"%d\n",behavior);
    }
    else {
      fprintf(fd,"%s\n",Hiscorer);
      fprintf(fd,"%s\n",Hidescrip);
      fprintf(fd,"%d\n",Hiscore);
      fprintf(fd,"%d\n",Hilevel);
      fprintf(fd,"%d\n",Hibehavior);
    }



    if (Player.alignment < Chaos) {
      morewait();
      mprint("Criminy! A new Lord of Chaos!");
      fprintf(fd,"%s\n",Player.name);
      fprintf(fd,"%d\n",Player.level);
      fprintf(fd,"%d\n",Player.alignment);
      fprintf(fd,"%d\n",behavior);
    }
    else {
      fprintf(fd,"%s\n",Chaoslord);
      fprintf(fd,"%d\n",Chaoslordlevel);
      fprintf(fd,"%d\n",Chaos);
      fprintf(fd,"%d\n",Chaoslordbehavior);
    }






    if (Player.alignment > Law) {
      morewait();
      mprint("Gosh! A new Lord of Law!");
      fprintf(fd,"%s\n",Player.name);
      fprintf(fd,"%d\n",Player.level);
      fprintf(fd,"%d\n",Player.alignment);
      fprintf(fd,"%d\n",behavior);
    }
    else {
      fprintf(fd,"%s\n",Lawlord);
      fprintf(fd,"%d\n",Lawlordlevel);
      fprintf(fd,"%d\n",Law);
      fprintf(fd,"%d\n",Lawlordbehavior);
    }




    fprintf(fd,"%s",Duke);
    fprintf(fd,"\n%d",Dukelevel);
    if (Player.rank[NOBILITY] == DUKE)
      fprintf(fd,"\n%d",behavior);
    else fprintf(fd,"\n%d",Dukebehavior);
    fprintf(fd,"\n%s",Justiciar);
    fprintf(fd,"\n%d",Justiciarlevel);
    if (Player.rank[ORDER] == JUSTICIAR)
      fprintf(fd,"\n%d",behavior);
    else fprintf(fd,"\n%d",Justiciarbehavior);
    fprintf(fd,"\n%s",Commandant);
    fprintf(fd,"\n%d",Commandantlevel);
    if (Player.rank[LEGION] == COMMANDANT)
      fprintf(fd,"\n%d",behavior);
    else fprintf(fd,"\n%d",Commandantbehavior);
    fprintf(fd,"\n%s",Champion);
    fprintf(fd,"\n%d",Championlevel);
    if (Player.rank[ARENA] == CHAMPION)
      fprintf(fd,"\n%d",behavior);
    else fprintf(fd,"\n%d",Championbehavior);
    fprintf(fd,"\n%s",Archmage);
    fprintf(fd,"\n%d",Archmagelevel);
    if (Player.rank[COLLEGE] == ARCHMAGE)
      fprintf(fd,"\n%d",behavior);
    else fprintf(fd,"\n%d",Archmagebehavior);
    fprintf(fd,"\n%s",Prime);
    fprintf(fd,"\n%d",Primelevel);
    if (Player.rank[CIRCLE] == PRIME)
      fprintf(fd,"\n%d",behavior);
    else fprintf(fd,"\n%d",Primebehavior);
    fprintf(fd,"\n%s",Shadowlord);
    fprintf(fd,"\n%d",Shadowlordlevel);
    if (Player.rank[THIEVES] == SHADOWLORD)
      fprintf(fd,"\n%d",behavior);
    else fprintf(fd,"\n%d",Shadowlordbehavior);
    for(i=1;i<7;i++) {
      fprintf(fd,"\n%s",Priest[i]);
      fprintf(fd,"\n%d",Priestlevel[i]);
      if ((Player.rank[PRIESTHOOD] == HIGHPRIEST) && (Player.patron == i))
	fprintf(fd,"\n%d",behavior);
      else fprintf(fd,"\n%d",Priestbehavior[i]);
    }
    fprintf(fd,"\n");
    fclose(fd);
  }
}

void extendlog(descrip,lifestatus)
char *descrip;
int lifestatus;
{
  FILE *fd;
  char username[60];
  int npcbehavior;
  strcpy(username,getlogin());
  if ((Player.level > 0) && (! gamestatusp(CHEATED))) {
    npcbehavior=fixnpc(lifestatus);
    checkhigh(descrip,npcbehavior);
    strcpy(Str1,OMEGALIB);
    strcat(Str1,"omega.log");
    fd = checkfopen(Str1,"a");
    fprintf(fd,
	    " %d %d %d %s\n",
	    lifestatus,
	    Player.level,
	    npcbehavior,
	    Player.name);
    fclose(fd);
  }
}





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


/* Checks existence of omega data files */
/* Returns 1 if OK, 0 if impossible to run, -1 if possible but not OK */
int filecheck()
{
  int impossible=FALSE,badbutpossible=FALSE;
  int result;

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ocity.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ocountry.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"odlair.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omisle.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ocourt.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ospeak.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"otemple.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"oabyss.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ovillage1.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ovillage2.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ovillage3.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ovillage4.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohome1.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohome2.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohome3.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"oarena.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  
  
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omaze1.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not appendable or accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omaze2.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not appendable or accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omaze3.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not appendable or accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omaze4.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not appendable or accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omega.hi");
  result = access(Str1,F_OK|R_OK|W_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not appendable or accessible:");
    printf(Str1);
  }
  
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omega.log");
  result = access(Str1,F_OK|R_OK|W_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not appendable or accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omotd.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"olicense.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ocircle.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ocmds.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"occmds.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp1.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp2.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp3.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp4.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp5.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp6.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp7.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp8.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp9.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp10.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp11.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp12.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp13.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  
  
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"oabyss.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"oscroll1.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"oscroll2.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }


  strcpy(Str1,OMEGALIB);
  strcat(Str1,"oscroll3.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"oscroll4.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  
  if (impossible) {
    printf("\nFurther execution is impossible. Sorry.");
    printf("\nOMEGALIB may be badly #defined in odefs.h\n");
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
      printw("\n-More-");
      refresh();
      d = wgetch(stdscr);
      clear();
    }
    printw("%c",(char) c);
    c = fgetc(fd);
  }
  if (((char) d != 'q')&&((char) d!=ESCAPE)) {
    printw("\n-Done-");
    refresh();
    getch();
  }
  clear();
  refresh();
}    


/* display a file given a string name of file */
void copyfile(srcstr)
char *srcstr;
{
  char deststr[80];
  char cmd[200];
  print1("Enter name of file to create: ");
  strcpy(deststr,msgscanstring());
  strcpy(cmd,"cp ");
  strcat(cmd,srcstr);
  strcat(cmd," ");
  strcat(cmd,deststr);
  print2("Copying file....");
  system(cmd);
  print3("Done.");
}    

