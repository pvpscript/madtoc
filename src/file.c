#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

struct file {
        FILE *ptr;
        char *path;
};

struct file *open_file(char *path)
{
        struct file *f = malloc(sizeof (struct file));

        if (!(f->ptr = fopen(path, "r+")))
                return NULL;
        f->path = path;

        return f;
}

void close_file(struct file *f)
{
        fclose(f->ptr);
}

FILE *get_file_pointer(struct file *f)
{
        return f->ptr;
}

char *get_file_path(struct file *f)
{
        return f->path;
}

static off_t get_file_size_helper(const char *path)
{
        struct stat buf;

        if (stat(path, &buf) != 0)
                return -1;

        return buf.st_size;
}

off_t get_file_size(struct file *f)
{
        return get_file_size_helper(f->path);
}
