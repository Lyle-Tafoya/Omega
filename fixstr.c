/* This utility program is not part of the sources to Omega.
   It was written by Nathan Glasser  nathan@brokaw.lcs.mit.edu	(internet)
	   			     nathan@mit-eddie.uucp	(usenet).
   As such, it is copyright by Nathan Glasser, 1987,1988,1989.
   Please don't attempt to sell this program or take credit for it
   yourself, and please don't remove this notice.
*/


/* This program reads in a C source file and replaces all static strings
with variable names defined above in the file, which are all declared
to be far arrays. You don't need to run this for the unix version. */

/* This program does not know about comments, or \'s in front of "'s.
Thus it can be broken. */

#include <stdio.h>
#include <malloc.h>

#define TMPFILE "fixtmp.c"

int num_strings;
char **string_list;

#define REALLOC_INCR 500


main(argc,argv)
int argc;
char **argv;
{
    FILE *sourcefp,*destfp;

    if (argc != 2)
    {
    	printf("Usage: %s <cfile>\n",argv[0]);
	exit(1);
    }

    if ((sourcefp = fopen(argv[1],"r")) == NULL ||
      (destfp = fopen(TMPFILE,"w")) == NULL)
    {
    	perror("Can't open a file (pass1)");
	exit(1);
    }

    printf("Scanning %s...",argv[1]);
    fflush(stdout);
    do_scan(sourcefp,destfp);
    printf("Done\n");

    fclose(sourcefp);
    fclose(destfp);

    if ((sourcefp = fopen(TMPFILE,"r")) == NULL ||
      (destfp = fopen(argv[1],"w")) == NULL)
    {
    	perror("Can't open a file (pass2)");
	exit(1);
    }

    printf("Writing new %s...",argv[1]);
    fflush(stdout);
    do_output(sourcefp,destfp);
    printf("Done\n");
    remove(TMPFILE);
    exit(0);
}

char include[] = "#include";
#define include_size (sizeof(include) - 1)

do_scan(sourcefp,destfp)
FILE *sourcefp,*destfp;
{
    int max_strings;
    int ch,last_ch = EOF;
    char temp_string[128],*temp;

    /* Vars for figuring out about #include's */
    int pos = 0,include_flag = 0,include_tmp = 1;

    string_list = (char **)malloc((max_strings = REALLOC_INCR) *
      sizeof(char *));

    while ((ch = getc(sourcefp)) != EOF)
    {
    	switch (ch)
	{
	    case '\n':
	    	putc(ch,destfp);
		pos = include_flag = 0;
		include_tmp = 1;
		break;
	    case '"':
	    	if (!include_flag && last_ch != '\'')
		{
		    /* Start of a string */
		    for (temp = temp_string; (*temp = getc(sourcefp)) != '"';
		      temp++);
		    *temp = '\0';
		    strcpy((string_list[num_strings] =
		      (char *)malloc(temp - temp_string + 1)),temp_string);
		    fprintf(destfp,"_str_%d",num_strings);
		    if (++num_strings == max_strings)
		    	string_list = (char **)realloc(string_list,
			  (max_strings += REALLOC_INCR) * sizeof(char *));
		    include_tmp = 0;
		    break;
		}
	    default:
	    	if (include_tmp)
		{
		    if ((include_tmp = ch == include[pos++]) &&
		      pos == include_size)
		    {
		        include_flag = 1;
			include_tmp = 0;
		    }
		}
		putc(ch,destfp);
		break;
	}
	last_ch = ch;
    }
}

do_output(sourcefp,destfp)
FILE *sourcefp,*destfp;
{
    char buf[1024];
    int i;

    fprintf(destfp,"/* These static strings have been moved here to */\n");
    fprintf(destfp,"/* declare them as far and avoid having too much */\n");
    fprintf(destfp,"/* initialized memory in the CONST segment. */\n\n");

    for (i = 0; i < num_strings; i++)
    	fprintf(destfp,"static char far _str_%d[] = \"%s\";\n",
	  i,string_list[i]);

    putc('\n',destfp);

    while (i = fread(buf,1,sizeof(buf),sourcefp))
    	fwrite(buf,i,1,destfp);
}
