#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#define container_of(ptr, type, member)                         \
             (type *)( (char *)(ptr) - offsetof(type,member) );


#define GROW_BUF_BY 100

struct buffer {
    char *str;
    size_t size;
};

struct buffer *new_buffer(size_t buf_size);
void add_buffer_char(struct buffer *b, char c);
void reset_buffer(struct buffer *b);
void destroy_buffer(struct buffer *b);

#endif /* UTILS_H */
