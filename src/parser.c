#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "section.h"
#include "buffer.h"
#include "list.h"

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

long parse_toc(FILE *fp, char *section)
{

        size_t section_size = strlen(section);
        char c;
        int i;

        for (i = 0; (c = fgetc(fp)) != EOF; ) {
                if (c == section[i]) {
                        i++;
                } else if (c == ' ') {
                        continue;
                } else if (c == '\n' && i == section_size) {
                        return ftell(fp);
                } else {
                        while (c = fgetc(fp), c != '\n' && c != EOF)
                                ;
                        i = 0;
                        continue;
                }
        }

        return -1;
}

struct list *parse_section(FILE *fp)
{
        struct list *sections = init_list();
        struct buffer *buf = new_buffer(500);
        struct node *n;
        char c;
        char prev;
        int level;

        for (level = 0, prev = 0; (c = fgetc(fp)) != EOF; prev = c) {
                if (c == '#') {
                        level++;
                } else if (c != '#' && c != ' ' || c == ' ' && prev != '#') {
                        while (c = fgetc(fp), c != '\n' && c != EOF)
                                ;
                        level = 0;
                } else if (c == ' ' && level > 0 && level <= MAX_SECTION_LEN) {
                        while ((c = fgetc(fp)) != '\n')
                                add_buffer_char(buf, c);

                        n = list_get_tail(sections);
                        if (n && level > get_section_level(node_get_data(n)))
                                add_subsection(node_get_data(n),
                                        new_section(strdup(buf->str), level));
                        else
                                list_add(sections, 
                                        new_section(strdup(buf->str), level));
                        level = 0;
                        reset_buffer(buf);
                }
        }

        destroy_buffer(buf);

        return sections;
}

/*
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

        destroy_section(s, NULL);

        FILE *f = fopen("../test.md", "r");
        struct list *sections = parse_section(f);
        struct node *n;
        for (n = list_get_head(sections); n; n = node_get_next(n)) {
                show_section_and_subsections(node_get_data(n));
                destroy_section(node_get_data(n), free);
        }

        destroy_list(sections, NULL);

        fclose(f);

        return 0;
}
*/
