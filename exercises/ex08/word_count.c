/*
This function generates and prints the frequencies of all of the words in a text.
*/

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

FILE * fp;
char * line = NULL;
size_t len = 0;
ssize_t read;
char *token;
GHashTable* hash; 

//takes keys and values in the hash table and prints them. For use in g_hash_table_foreach function.
void foreach_print(gchar* key, int value, gpointer user_data) {
	printf("%s : %i, ", key, value);
}

//For a token, increment the value by one if it exists, otherwise add it to the hashtable.
void increment_table(char* token) {
	gpointer* count = g_hash_table_lookup(hash, token);
	gpointer* increment = GINT_TO_POINTER(1);
		if (count == NULL) {
			g_hash_table_insert(hash, token, increment);
		} else {
			int newCountInt = GPOINTER_TO_INT(count) + 1;
			gpointer* newCountPointer = GINT_TO_POINTER(newCountInt);
			g_hash_table_insert(hash, token, newCountPointer);
		}
	printf("%s: %i, ", token, GPOINTER_TO_INT(g_hash_table_lookup(hash, token)));
}

//generates the hash table from a file that it reads.
int main() {
	hash = g_hash_table_new(g_str_hash, g_str_equal);
	fp = fopen("short_text.txt", "r");
	char delimit[]=" \t\r\n\v\f";

	while ((read = getline(&line, &len, fp)) != -1) {
		// printf("\n");
		token = strtok(line, delimit);
		if ((token != NULL)){
			// printf("%s, ", token);
			increment_table(token);
		}
		while ( token != NULL ) {
			token = strtok(NULL, delimit);
			if (token != NULL) {
				// printf("%s, ", token);
				increment_table(token);
			}
		}
	}	
	printf("\n-------\n");
	g_hash_table_foreach(hash, (GHFunc) foreach_print, NULL);
	return 0;
}