#include <stdlib.h>

#include "list.h"

struct list {
        struct node *head;
        struct node *tail;
        size_t size;
};

struct node {
        void *data;
        struct node *next;
};

struct list *init_list()
{
        struct list *l = malloc(sizeof (struct list));

        l->head = NULL;
        l->tail = NULL;
        l->size = 0;

        return l;
}

struct node *list_get_head(struct list *l)
{
        return l->head;
}

struct node *list_get_tail(struct list *l)
{
        return l->tail;
}

struct node *node_get_next(struct node *n)
{
        return n->next;
}

void *node_get_data(struct node *n)
{
        return n->data;
}

size_t list_get_length(struct list *l)
{
        return l->size;
}

static struct node *new_node(void *data)
{
        struct node *n = malloc(sizeof (struct node));

        n->data = data;
        n->next = NULL;

        return n;
}

void list_add(struct list *l, void *data)
{
        struct node *n = new_node(data);

        if (l->head == NULL)
                l->head = n;
        if (l->tail == NULL) {
                l->tail = n;
        } else {
                l->tail->next = n;
                l->tail = l->tail->next;
        }
}

void destroy_list(struct list *l, void (free_inner(void *)))
{
        struct node *n, *tmp;

        for(n = l->head; n; tmp = n, n = n->next, free(tmp))
                if (free_inner)
                        free_inner(n);

        free(l);
}
