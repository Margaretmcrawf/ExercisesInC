/* 
*  Reads from stdin and takes a file as an argument, then prints to standard out and to the file.
*  by Margo Crawford
* 
*  Things that worked well and not so well, process wise:
*  - HFC was the most useful thing to me for this assignment. It had most of the information that I needed.
*  - Most of the Stack Overflow links I found were hard to follow, because there were too many things that
*   I was unfamiliar with that weren't necessary for the barebones function to work.
*
*  Interesting differences between the actual thing and mine:
*  - Mine is A LOT shorter. This is partially because I didn't do command line options and partially because I didn't handle my own errors.
*  - Mine initially wasn't able to take more than one file as an argument, which I changed.
*  - An interesting thing I found was that that the source actually specifies "DO NOT warn if tee is given no
*   file arguments. POSIX requires that it work when given no arguments."
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	FILE *fp[argc-1];

	if (argc < 2) {
		printf("You should specify at least one file.\n");
	}

	for (int i = 1; i < argc; i++) {
		fp[i] = fopen(argv[i], "w");
	}

 	char input[80];
 	while (fgets(input, 80, stdin) != NULL) {
 		for (int i = 1; i < argc; i++) {
 			fprintf(fp[i], "%s", input);
 		}
 		printf("%s", input);
 	}
 	
 	return 0;
}
