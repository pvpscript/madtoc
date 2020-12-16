#include <stdlib.h>
#include <stddef.h>

#include "buffer.h"

struct buffer_impl {
        struct buffer b;
        size_t pos;
};

struct buffer *new_buffer(size_t buf_size)
{
        struct buffer_impl *b_impl = malloc(sizeof (struct buffer_impl));

        b_impl->pos = 0;

        b_impl->b.str = malloc(sizeof (char) * buf_size);
        b_impl->b.size = buf_size;

        return &b_impl->b;
}

void add_buffer_char(struct buffer *b, char c)
{
        struct buffer_impl *b_impl = container_of(b, struct buffer_impl, b);

        if (b->size + 1 <= b_impl->pos) {
                b->size += GROW_BUF_BY;
                b->str = realloc(b->str, sizeof (char) * b->size);
        }
        b->str[b_impl->pos++] = c;
        b->str[b_impl->pos] = 0;
}

void reset_buffer(struct buffer *b)
{
        struct buffer_impl *b_impl = container_of(b, struct buffer_impl, b);

        b_impl->pos = 0;
}

void destroy_buffer(struct buffer *b)
{
        struct buffer_impl *b_impl = container_of(b, struct buffer_impl, b);

        free(b->str);
        free(b_impl);
}
