/* omega (c) 1987 by Laurence Raphael Brothers */
/* ofile.c */
/* functions with file access in them. Also some direct calls to
   curses functions */

#include <curses.h>
#include <strings.h>
#include <sys/file.h>
#include "oglob.h"

/* from ocom */
extern void save();

/* from  outil */
extern int calc_points();

/* from oscr */
extern void endgraf(),xredraw(),printm(),mprint(),morewait();
extern char ynq(), mgetc();

/* ofile functions */
void commandlist(),helpfile(),showfile(),showscores();
void extendlog(),filescanstring(),checkhigh();
void theologyfile(),cityguidefile();
int filecheck();

FILE *checkfopen();

FILE *checkfopen(filestring,optionstring)
char *filestring,*optionstring;
{
  FILE *fd;
  char response;
  fd = fopen(filestring,optionstring);
  while (fd == NULL) {
    printm("\nWarning! Error opening file:");
    mprint(filestring);
    mprint(" Abort or Retry? [ar] ");
    do response = mgetc(); while ((response != 'a') && (response != 'r'));
    if (response == 'r') fd = fopen(filestring,optionstring);
    else {
      endgraf();
      printf("\n\nSorry 'bout that.... Bye!");
      exit(0);
    }
  }
  return(fd);
}

void commandlist()
{
  FILE *fd; 
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ocommands.txt");
  fd = checkfopen(Str1,"r");
  showfile(fd);
  fclose(fd);
  clear();
  refresh();
  xredraw();
}

void helpfile()
{
  FILE *fd;
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp.txt");
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
}




/* display a file page at a time */
void showfile(fd)
FILE *fd;
{
  char c,d=' ';
  int x,y;
  clear();
  refresh();
  c = fgetc(fd);
  while ((c != EOF)&&(d != 'q')&&(d!=ESCAPE)) {
    getyx(stdscr,y,x);
    if (y > 20) {
      printw("\n-More-");
      refresh();
      d = wgetch(stdscr);
      clear();
    }
    printw("%c",c);
    c = fgetc(fd);
  }
  if ((d != 'q')&&(d!=ESCAPE)) {
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
  strcat(Str1,"omega.hiscore");
  fd = checkfopen(Str1,"r");
  filescanstring(fd,Hiscorer);
  filescanstring(fd,Hidescrip);
  fscanf(fd,"%d\n%d\n%d\n",&Hiscore,&Hilevel,&Hibehavior);
  filescanstring(fd,Duke);
  fscanf(fd,"%d\n%d\n",&Dukelevel,&Dukebehavior);
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
  printw(", by %s, a level %d adventurer",Hiscorer,Hilevel);
  printw("\n%s",Hidescrip);
  printw("\n\nDuke of Rampart:              ");
  printw("%s",Duke);
  printw("\nCommandant:                   ");
  printw("%s",Commandant);
  printw("\nChampion:                     ");
  printw("%s",Champion);
  printw("\nArchmage:                     ");
  printw("%s",Archmage);
  printw("\nPrime Sorceror:               ");
  printw("%s",Prime);
  printw("\nShadowlord:                   ");
  printw("%s",Shadowlord);
  printw("\n\nHigh Priests:");
  printw("\n of Odin:                     ");
  printw("%s",Priest[ODIN]);
  printw("\n of Set:                      ");
  printw("%s",Priest[SET]);
  printw("\n of Athena:                   ");
  printw("%s",Priest[ATHENA]);
  printw("\n of Hecate:                   ");
  printw("%s",Priest[HECATE]);
  printw("\n of the Lords of Destiny:     ");
  printw("%s",Priest[DESTINY]);
  printw("\nThe ArchDruid:                ");
  printw("%s",Priest[DRUID]);
  printw("\n\n\n\nHit any key to continue.");
  refresh();
  wgetch(stdscr);
}

void checkhigh(descrip,behavior)
char *descrip;
int behavior;
{
  int i;
  FILE *fd;
  if (! Cheated) {
    strcpy(Str1,OMEGALIB);
    strcat(Str1,"omega.hiscore");
    fd = checkfopen(Str1,"w");
    
    if (Hiscore < calc_points()) {
      morewait();
      mprint("Yow! A new high score!");
      fprintf(fd,"%s\n",Player.name);
      fprintf(fd,"%s\n",descrip);
      fprintf(fd,"%d\n",calc_points());
      fprintf(fd,"%d\n",Player.level);
      fprintf(fd,"%d",behavior);
    }
    else {
      fprintf(fd,"%s\n",Hiscorer);
      fprintf(fd,"%s\n",Hidescrip);
      fprintf(fd,"%d\n",Hiscore);
      fprintf(fd,"%d\n",Hilevel);
      fprintf(fd,"%d",Hibehavior);
    }
    fprintf(fd,"\n%s",Duke);
    fprintf(fd,"\n%d",Dukelevel);
    if (Player.rank[NOBILITY] == DUKE)
      fprintf(fd,"\n%d",behavior);
    else fprintf(fd,"\n%d",Dukebehavior);
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
  if ((Player.level > 0) && (! Cheated)) {
    npcbehavior=fixnpc(lifestatus);
    checkhigh(descrip,npcbehavior);
    Logsize++;
    strcpy(Str1,OMEGALIB);
    strcat(Str1,"omega.lognum");
    fd = checkfopen(Str1,"w");
    fprintf(fd,"%d",Logsize);
    fclose(fd);
    strcpy(Str1,OMEGALIB);
    strcat(Str1,"omega.log");
    fd = checkfopen(Str1,"a");
    fprintf(fd,"%s\n",username);
    fprintf(fd,"%s\n",Player.name);
    fprintf(fd,"%d %d %d\n",lifestatus,Player.level,npcbehavior);
    fclose(fd);
  }
}






void filescanstring(fd,fstr)
FILE *fd;
char *fstr;
{
  int i= -1;
  char byte='x';
  while ((i<80) && (byte != '\n')) {
    i++;
    fscanf(fd,"%c",&byte);
    fstr[i] = byte;
  } 
  if (byte != '\n')
    while(byte!='\n')
      fscanf(fd,"%c",&byte);
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
  strcat(Str1,"odepths.dat");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omega.hiscore");
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
  strcat(Str1,"omega.lognum");
  result = access(Str1,F_OK|R_OK|W_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not appendable or accessible:");
    printf(Str1);
  }
  
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omega.motd");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    impossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ocommands.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  

  strcpy(Str1,OMEGALIB);
  strcat(Str1,"ohelp.txt");
  result = access(Str1,F_OK|R_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not accessible:");
    printf(Str1);
  }
  
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omega.saves");
  result = access(Str1,F_OK|R_OK|W_OK);
  if (result == -1) {
    badbutpossible = TRUE;
    printf("\nWarning! File not appendable or accessible:");
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
  
  if (impossible) {
    printf("\nFurther execution is impossible. Sorry.");
    printf("\nOMEGALIB may be badly #defined in odefs.h\n");
    return(0);
  }
  else if (badbutpossible) {
    printf("\nFurther execution may cause anomalous behavior.");
    printf("\nContinue anyhow? [yn]");
    if (getchar()=='y') return(-1);
    else return(0);
  }
  else return(1);
}
