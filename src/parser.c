#include <stdio.h>
#include <stdlib.h>

#define MAX_SECTION_LEN 6

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

struct section *add_subsection(struct section *s, struct section *subs)
{
        s->subsections = realloc(s->subsections,
                        sizeof (struct section *) * (s->total_subsections + 1));
        s->subsections[s->total_subsections++] = subs;

        return subs;
}

/* Debugging
 */
static void show_section_and_subsections(struct section *s)
{
        int i;

        printf("\nThis section: %s\n", s->name);
        for (i = 0; i < s->total_subsections; i++)
                printf("Subsection %d: %s\n", i, s->subsections[i]->name);
}

int main(void)
{
        struct section *s = new_section("Some pretty cool title, innit", 1);
        struct section *ss = add_subsection(s, new_section("Fancy a cuppa?", 2));

        show_section_and_subsections(s);
        show_section_and_subsections(ss);


        return 0;
}
