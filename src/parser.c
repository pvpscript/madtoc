#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "section.h"
#include "buffer.h"
#include "list.h"

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
                } else if (c != '\n' && ((c != '#' && c != ' ') ||
                                (c == ' ' && prev != '#'))) {
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
