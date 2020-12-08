#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "file.h"

#define BUF_SIZE 500

#define MIN(x, y) ((x) < (y)) ? (x) : (y)

static void advance_file_chunk(FILE *f, long backstep, long bytes_add,
                long offset)
{
        char buf[BUF_SIZE];

        for (fseek(f, 0, SEEK_END); ftell(f) > offset;
                        fseek(f, -backstep - bytes_add, SEEK_CUR)) {
                if (ftell(f) < backstep)
                        backstep = ftell(f) - offset;

                fseek(f, -backstep, SEEK_CUR);
                fread(buf, sizeof (char), backstep, f);
                fseek(f, -backstep + bytes_add, SEEK_CUR);
                fwrite(buf, sizeof (char), backstep, f);
        }
}

static void shrink_file_chunk(FILE *f, off_t file_size, long forwardstep,
                long bytes_remove, long offset)
{
        char buf[BUF_SIZE];

        for (fseek(f, offset + bytes_remove, SEEK_SET); ftell(f) < file_size;
                        fseek(f, bytes_remove, SEEK_CUR)) {
                if (ftell(f) + forwardstep > file_size)
                        forwardstep = file_size - ftell(f);

                fread(buf, sizeof (char), forwardstep, f);
                fseek(f, -forwardstep - bytes_remove, SEEK_CUR);
                fwrite(buf, sizeof (char), forwardstep, f);
        }
}

void advance_file(struct file *f, char *str_add, long offset)
{
        off_t file_size = get_file_size(f);
        long backstep = MIN(file_size - offset, BUF_SIZE);
        long bytes_add = strlen(str_add);
        FILE *fp = get_file_pointer(f);

        advance_file_chunk(fp, backstep, bytes_add, offset);
        fseek(fp, offset, SEEK_SET);
        fwrite(str_add, sizeof (char), bytes_add, fp);
}

void shrink_file(struct file *f, long bytes_remove, long offset)
{
        off_t file_size = get_file_size(f);
        long forwardstep = MIN(file_size, BUF_SIZE);
        FILE *fp = get_file_pointer(f);

        shrink_file_chunk(fp, file_size, forwardstep, bytes_remove, offset);
        ftruncate(fileno(fp), file_size - bytes_remove);
}
