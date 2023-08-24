/*
This file is part of Omega.

Omega is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Omega is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Omega. If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>

char cryptkey(fname)
char *fname;
{
    int pos, key = 0;

    if (!strcmp(fname + strlen(fname) - 4, ".txt"))
	return 100;
    else if (!strncmp(fname, "maze", 4))
	fname = "mazes";
    else if (!strncmp(fname, "villag", 6))
	fname = "village.dat";
    for (pos = 0; fname[pos]; pos++)
	key += 3*(fname[pos] - ' ');
    return (key&0xff);
}

main(int num_args, char *args[])
{
    char key;
    int c;

    if (num_args == 2) {
	key = cryptkey(args[1]);
	while ((c = getchar()) != EOF) {
	    putchar(key^c);
	    key = c;
	}
    }
    else
	fprintf(stderr, "Usage: %s (key) < (infile) > (outfile)\n where (key) happens to be the name of the file, without any preceding path.\n", args[0]);
}
