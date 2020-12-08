#ifndef FILE_H
#define FILE_H

#include <sys/types.h>

/*
 * A module to handle files more easily
 */

/*
 * Holds useful information about a file
 */
struct file;

struct file *open_file(char *path);

void close_file(struct file *f);

FILE *get_file_pointer(struct file *f);

char *get_file_path(struct file *f);

off_t get_file_size(struct file *f);

#endif /* FILE_H */
