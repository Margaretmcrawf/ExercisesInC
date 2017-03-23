/* My version of the tee function
*  inputs: :/
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

 	FILE *fp;
 	fp = fopen(argv[1], "w");


 	char input[80];
 	while (fgets(input, 80, stdin) != NULL) {
 		fprintf(fp, "%s", input);
 		printf("%s", input);
 	}
 	
 	return 0;

}
