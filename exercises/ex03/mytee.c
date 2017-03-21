/* My version of the tee function
*  inputs: :/
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

 	FILE *fp;
 	fp = fopen(argv[1], "w"); //assuming argv[1] is the filename...
 	fprintf(fp, "%s", argv[2]);

 	printf("%s", argv[2]);

}
