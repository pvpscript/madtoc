#ifndef LIST_H
#define LIST_H

struct list;
struct node;

struct list *init_list();
struct node *list_get_head(struct list *l);
struct node *list_get_tail(struct list *l);
struct node *node_get_next(struct node *n);
void *node_get_data(struct node *n);
size_t list_get_length(struct list *l);
void list_add(struct list *l, void *data);
void destroy_list(struct list *l, void (destroy_inner(void *)));

#endif /* LIST_H */
