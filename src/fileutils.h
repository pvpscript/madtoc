#ifndef FILEUTILS_H
#define FILEUTILS_H

/*
 * Useful functions that handles files
 */

#include "file.h"

/* 
 * Increase the size of a file and adds a string in an arbitrary point of it
 */
void advance_file(struct file *f, char *str_add, long offset);

/*
 * Removes a given amount of bytes from a file in an arbitrary point of it
 */
void shrink_file(struct file *f, long bytes_remove, long offset);

#endif /* FILEUTILS_H */
