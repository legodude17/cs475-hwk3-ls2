#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char *argv[])
{
	if (argc != 2 && argc != 3)
	{
		printf("Usage: ./ls2 <path> [exact-match-pattern]\n");
		return 1;
	}

	// stack stores the lines to print out
	stack_t *s = initstack();

	char* filter;

	if (argc == 2) {
		filter = NULL; // If we didn't get a filter, then we want everything, so pass a NULL
	} else {
		filter = argv[2]; // argv[2] is the second argument, since argv[0] is ./ls2
	}

	// argv[1] is the first argument, aka our path, since argv[0] is ./ls2
	// We start at indent level 0, which means no indentation
	// We don't care about the result because it only means whether or not if found anything
	explore(argv[1], 0, filter, s);

	// Print the stack
	printstack(s);
	
	// free up stack
	freestack(s);

	return 0;
}
