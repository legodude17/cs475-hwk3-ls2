#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "stack.h"

#define TRUE 1
#define FALSE 0
#define INDENT "    "

/**
 * Explore a directory, filtering the items and adding things to print to the given stack
 * 
 * @param path The path to the directory to explore
 * @param indent The indentation level for printing
 * @param filter A filter to match files against
 * @param stack The stack to add items to
 * 
 * @returns 1 if it finds a file that matches filter, 0 if not
*/
int explore(char* path, int indent, char* filter, stack_t* stack);

#endif
