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

void foreach_print(gpointer key, gpointer value, gpointer userdata) {
	printf("key = '%s', count = %i\n", (char *) key, GPOINTER_TO_INT(value));
}

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
		return;
}

int main() {
	hash = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);
	fp = fopen("shakespeare.txt", "r");
	while ((read = getline(&line, &len, fp)) != -1) {
		token = strtok(line, " ");
		if (token != NULL) {
			increment_table(token);
		}
		while ( token != NULL ) {
			token = strtok(NULL, " ");
			if (token != NULL) {
				increment_table(token);
			}
		}
	}	
	g_hash_table_foreach(hash, foreach_print, NULL);
	return 0;
}