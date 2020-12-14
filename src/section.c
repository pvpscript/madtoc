#include <stdio.h>
#include <stdlib.h>

struct section {
        char *name;
        int level;
        struct section **subsections;
        size_t total_subsections;
};

struct section *new_section(char *name, int level)
{
        struct section *s = malloc(sizeof (struct section));

        s->name = name;
        s->level = level;
        s->subsections = NULL;
        s->total_subsections = 0;

        return s;
}

char *get_section_name(struct section *s)
{
        return s->name;
}

int get_section_level(struct section *s)
{
        return s->level;
}

struct section **get_section_subsections(struct section *s)
{
        return s->subsections;
}

size_t get_section_total_subsections(struct section *s)
{
        return s->total_subsections;
}

struct section *add_subsection(struct section *s, struct section *subs)
{
        s->subsections = realloc(s->subsections,
                        sizeof (struct section *) * (s->total_subsections + 1));
        s->subsections[s->total_subsections++] = subs;

        return subs;
}

void destroy_section(struct section *s)
{
        int i;

        if (s->total_subsections == 0) {
                free(s);
        } else {
                for (i = 0; i < s->total_subsections; i++)
                        destroy_section(s->subsections[i]);

                free(s->subsections);
                free(s);
        }
}
