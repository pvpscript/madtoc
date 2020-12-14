#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "section.h"

void show_section_and_subsections(struct section *s)
{
        int i;
        struct section **sub;

        printf("%*sThis section: %s\n", get_section_level(s)-1, "", get_section_name(s));
        sub = get_section_subsections(s);
        for (i = 0; i < get_section_total_subsections(s); i++) {
                if (get_section_total_subsections(sub[i]) > 0)
                        show_section_and_subsections(sub[i]);

                printf("%*sSubsection %d: %s\n", get_section_level(sub[i])-1, "", i, get_section_name(sub[i]));
        }
        puts("");
}

void parse_section(FILE *fp)
{
        int level = 0;
        char c;
        char prev;
        struct section *sections[20] = { 0 };
        struct section *s;
        int sections_index = 0;
        char buf[500];
        int buf_index = 0;

        for (prev = 0; (c = fgetc(fp)) != EOF; prev = c) {
                if (c == '#') {
                        level++;
                } else if (c != '#' && c != ' ' || c == ' ' && prev != '#') {
                        while (c = fgetc(fp), c != '\n' && c != EOF)
                                ;
                        level = 0;
                } else if (c == ' ' && level > 0 && level <= MAX_SECTION_LEN) {
                        while ((c = fgetc(fp)) != '\n')
                                buf[buf_index++] = c;
                        buf[buf_index] = 0;

                        s = sections[sections_index - 1];
                        if (sections_index > 0 && level > get_section_level(s)) {
                                add_subsection(s, new_section(strdup(buf), level));
                                printf("Added subsection: %s\n", buf);
                        } else {
                                sections[sections_index++] = new_section(
                                                strdup(buf), level);
                                printf("Added section: %s\n", buf);
                        }
                        level = 0;
                        buf_index = 0;
                }
        }

        for(int i = 0; i < sections_index; i++)
                show_section_and_subsections(sections[i]);
}

int main(void)
{
        struct section *s = new_section("Some pretty cool title, innit", 1);
        struct section *ss = add_subsection(s, new_section("Fancy a cuppa?", 2));
        struct section *kk = add_subsection(ss, new_section("So english m8", 3));
        add_subsection(kk, new_section("Du", 4));
        add_subsection(kk, new_section("Du Hast", 4));
        add_subsection(kk, new_section("Du Hast Mich", 4));
        add_subsection(kk, new_section("Du Hast Mich Gefragt", 4));
        add_subsection(ss, new_section("Bri'ish", 3));
        struct section *ss2 = add_subsection(s, new_section("Hello, World", 2));
        struct section *ss3 = add_subsection(s, new_section("Hi there", 2));

        show_section_and_subsections(s);
        show_section_and_subsections(ss);

        destroy_section(s);

        FILE *f = fopen("../test.md", "r");
        parse_section(f);


        return 0;
}
