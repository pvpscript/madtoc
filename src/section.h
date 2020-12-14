#ifndef SECTION_H
#define SECTION_H

#define MAX_SECTION_LEN 6

struct section;

struct section *new_section(char *name, int level);
struct section *add_subsection(struct section *s, struct section *subs);
void destroy_section(struct section *s);

#endif /* SECTION_H */
