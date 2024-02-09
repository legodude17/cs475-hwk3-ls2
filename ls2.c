#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include "ls2.h"

int explore(char *path, int indent, char *filter, stack_t *stack)
{
    DIR *dirp = opendir(path);
    // If we ever find a file that matches the filter, set this to 1
    // We return if we found a file or not so it knows whether or not to print the directory
    // We also set this to 1 if a recursive call returned 1, so it prints all the parents of found files
    int result = 0;
    if (dirp == NULL)
        return result; // If opening the directory fails, just skip it

    // Construct the indent string by concatenating INDENT indent times
    char *indentStr = (char *)malloc(indent * 4 + 1);
    *indentStr = '\0';
    for (int i = 0; i < indent; i++)
    {
        strcat(indentStr, INDENT);
    }

    // readdir allocates the memory itself
    struct dirent *dp;
    // lstat does not
    struct stat *info = (struct stat *)malloc(sizeof(struct stat));
    while ((dp = readdir(dirp)) != NULL)
    {
        // strcmp returns 0 (false) if they match, and something else otherwise
        // So this if is the same as if (d_name != "." && d_name != "..") in JS
        if (strcmp(dp->d_name, ".") && strcmp(dp->d_name, ".."))
        {
            // Construct the full path to the file by concatenating the current path, a slash, and the name
            char *fullPath = (char *)malloc(strlen(path) + strlen(dp->d_name) + 2);
            strcpy(fullPath, path);
            strcat(fullPath, "/");
            strcat(fullPath, dp->d_name);
            if (!lstat(fullPath, info)) // We use the same info for all the entries, since lstat will just keep overwriting it
            {
                if (S_ISREG(info->st_mode))
                {
                    // This is a normal file
                    if (filter == NULL || !strcmp(dp->d_name, filter)) // If the filter is NULL, we want to include everything, otherwise only files with name matching filter
                    {
                        result = 1; // We found a matching file
                        int numLen = snprintf(NULL, 0, "%ld", info->st_size); // snprintf(NULL, 0) measures the length rather than writing anything
                        int size = strlen(indentStr) + strlen(dp->d_name) + numLen + 10;
                        char *out = (char *)malloc(size);
                        // Print the file to the stack
                        sprintf(out, "%s%s (%ld bytes)", indentStr, dp->d_name, info->st_size);
                        push(stack, out);
                    }
                }
                else if (S_ISDIR(info->st_mode))
                {
                    // This is a directory, explore it
                    if (explore(fullPath, indent + 1, filter, stack)) // If explore returns 0, it didn't find anything worth printing in the directory, so don't print anything
                    {
                        result = 1;
                        int size = strlen(indentStr) + strlen(dp->d_name) + 14;
                        char *out = (char *)malloc(size);
                        // Print the directory to the stack
                        sprintf(out, "%s%s/ (directory)", indentStr, dp->d_name);
                        push(stack, out);
                    }
                }
            }
            free(fullPath); // free the fullPath
        }
    }

    free(indentStr); // Free the indentStr
    free(info); // Free the info struct
    closedir(dirp); // Close the directory stream
    // It seems we don't have to free dp, since whenever I do it gives me errors
    // readdir will reuse the same memory block on successive calls with the same stream
    // So I'd imagine closing the directory stream also frees the entry data, since they are associated

    return result;
}
