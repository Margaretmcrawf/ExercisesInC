#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

int main() {
	GSequenceIter * g_sequence_insert_sorted(GSequence *seq, gpointer data, GCompareDataFunc cmp_func, gpointer cmp_data);

	gint compare(gconstpointer a, gconstpointer b, gpointer user_data) {
		gint a_int = GPOINTER_TO_INT(a);
		gint b_int = GPOINTER_TO_INT(b);
		return a_int - b_int;
	}

return 0;
}