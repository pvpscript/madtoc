#include <stdio.h>
#include <stdlib.h>

#include "section.h"

void show_section_and_subsections(struct section *s)
{
        int i;
        struct section **sub;

        printf("This section: %s\n", get_section_name(s));
        sub = get_section_subsections(s);
        for (i = 0; i < get_section_total_subsections(s); i++)
                printf("Subsection %d: %s\n", i, get_section_name(sub[i]));
}

int main(void)
{
        struct section *s = new_section("Some pretty cool title, innit", 1);
        struct section *ss = add_subsection(s, new_section("Fancy a cuppa?", 2));
        struct section *ss2 = add_subsection(s, new_section("Hello, World", 2));
        struct section *ss3 = add_subsection(s, new_section("Hi there", 2));

        show_section_and_subsections(s);
        show_section_and_subsections(ss);

        destroy_section(s);

        return 0;
}
