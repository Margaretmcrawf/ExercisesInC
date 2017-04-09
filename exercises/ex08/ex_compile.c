/* Simple example using Glib.

From http://www.ibm.com/developerworks/linux/tutorials/l-glib/

Where is GLib from and why does it exist? Who uses it, and for what? Would it have been better if they had given it a name less likely to be confused with glibc?

GLib is used for data structures in C, such as HashMaps, Linked Lists, Trees, etc. It seems like the type of thing that's super useful to everyone, and it's not built in with C. 

What are the most important pros and cons of using GLib, as opposed to another library, or just using core C features?

Pros: 
Standard, open source library for lots of structures that are taken for granted in other languages.
Cons:
Annoying to compile because you have to tell gcc where to find it, probably hard to find the google results you want because it is so close to glibc.

What do you have to do to compile and run a "Hello GLib" example?

gcc ex_compile.c -o ex_compile `pkg-config --cflags --libs glib-2.0`


*/

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

int main(int argc, char** argv) {
    GList* list = NULL;
    list = g_list_append(list, "Hello world!");
    printf("The first item is '%s'\n", (char *) g_list_first(list)->data);
    return 0;
}

