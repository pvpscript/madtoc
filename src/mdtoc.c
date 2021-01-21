#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "fileutils.h"
#include "section.h"
#include "parser.h"
#include "list.h"

#define START_LINE      "<!-- mdtoc-start -->"
#define END_LINE        "<!-- mdtoc-end -->" 

enum types {
        NUMBERED = 0,
        BULLET,
};

static void show_numbered_sections(FILE *f, struct section *s, int *indexes);
static void show_bullet_sections(FILE *f, struct section *s, int *indexes);

struct config {
        int indexes[MAX_SECTION_LEN];
        void (*show_sections)(FILE *, struct section *, int *);
};

static struct config confs[] = {
        [NUMBERED]      = { { 0 },      &show_numbered_sections },
        [BULLET]        = { { '-' },    &show_bullet_sections }
};

struct file_info {
        long offset_start;
        long offset_end;
};

static void show_section_and_subsections(struct section *s)
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

static void inc_index(int *indexes, int index)
{
        int i;

        indexes[index]++;
        for (i = index + 1; i < MAX_SECTION_LEN; i++)
                indexes[i] = 0;
}

static void print_numbered_section(FILE *stream, int *indexes,  int level,
                                   char *name)
{
        int i;

        fprintf(stream, "%d", indexes[0]);
        for (i = 1; i < level; i++) 
                fprintf(stream, ".%d", indexes[i]);
        fprintf(stream, "\t%s\n", name);
}

static void show_numbered_sections(FILE *stream, struct section *s,
                                   int *indexes)
{
        struct section **subs = get_section_subsections(s);
        size_t total_subs = get_section_total_subsections(s); 
        int level = get_section_level(s);
        char *name = get_section_name(s);
        size_t i;

        inc_index(indexes, level - 1);
        print_numbered_section(stream, indexes, level, name);

	/* Print after a list item */
	puts("<br>");

	/* Print sections recursivelly */
        for (i = 0; i < total_subs; i++)
                show_numbered_sections(stream, subs[i], indexes);

}

static void print_bullet_section(FILE *stream, int *indexes, int level,
                                 char *name)
{
        int i;

        for (i = 0; i < level; i++)
                fprintf(stream, "\t");
        fprintf(stream, "%c %s\n", indexes[0], name);
}

static void show_bullet_sections(FILE *stream, struct section *s, int *indexes)
{
        struct section **subs = get_section_subsections(s);
        size_t total_subs = get_section_total_subsections(s); 
        int level = get_section_level(s);
        char *name = get_section_name(s);
        size_t i;

        print_bullet_section(stream, indexes, level - 1, name);

        for (i = 0; i < total_subs; i++)
                show_bullet_sections(stream, subs[i], indexes);
}

static void show_toc(FILE *stream, struct list *sections, enum types t)
{
        int *indexes = confs[t].indexes;
        struct node *n;

        for (n = list_get_head(sections); n; n = node_get_next(n))
                confs[t].show_sections(stream, node_get_data(n), indexes);
}

void parse_file(struct file *f)
{
        FILE *fp = get_file_pointer(f);
	struct offset start_offsets = parse_toc(fp, START_LINE);
	struct offset end_offsets = parse_toc(fp, END_LINE);

        printf("Bytes offset for the beginning: %ld -> %ld\n", start_offsets.start, start_offsets.end);
        printf("Bytes offset for the end: %ld -> %ld\n", end_offsets.start, end_offsets.end);

        fseek(fp, 0, SEEK_SET);
        struct list *sections = parse_section(fp);
        show_toc(stdout, sections, NUMBERED);
        /*
        struct node *n;
        for (n = list_get_head(sections); n; n = node_get_next(n)) {
                show_section_and_subsections(node_get_data(n));
                destroy_section(node_get_data(n), free);
        }
        */

        destroy_list(sections, NULL);


}


int main(int argc, char **argv)
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
        struct list *l = init_list();
        list_add(l, s);
        show_toc(stdout, l, BULLET);

        show_section_and_subsections(s);
        show_section_and_subsections(ss);

        destroy_section(s, NULL);

        struct file *f = open_file("./test.md");

        parse_file(f);


        exit(0);
        char *message = "This is a very cool message!\n";
        /*char *loren = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce ullamcorper quam mauris, at posuere augue posuere non. Sed iaculis elit et placerat dictum. Nam faucibus venenatis ex. Nullam tempor eros vel lectus gravida tincidunt. Phasellus tincidunt, nisl eget porttitor rhoncus, neque tellus aliquam massa, in ultrices est metus ac sapien. Nam dignissim ipsum diam, vel efficitur nisi placerat vitae. Morbi auctor tellus non ipsum efficitur fringilla id nec ex. Ut lectus magna, laoreet cursus erat at, iaculis egestas nisi. In euismod nisl eu tortor porttitor, vel euismod eros faucibus. Nullam enim quam, iaculis sed nunc euismod, euismod interdum metus. Mauris tristique odio dui, sit amet aliquam mauris ultricies vel. Aliquam condimentum ornare metus, ac blandit nibh elementum at. Curabitur maximus velit rutrum, vestibulum felis sed, dignissim enim. Nam laoreet, nunc eget condimentum viverra, ipsum tortor ullamcorper lorem, a iaculis metus sapien congue risus. Suspendisse sapien diam, rutrum a nulla convallis, sagittis porta justo. Quisque id viverra dolor. Aenean eu convallis mauris, quis rhoncus leo. Nunc tincidunt nibh sed faucibus tempor. Nulla vitae dictum augue, vel ultrices purus. Aliquam efficitur magna nec metus rhoncus molestie. Fusce eget dolor est. Aenean accumsan condimentum ipsum, ac molestie nibh euismod ac. Proin iaculis volutpat diam, at varius libero pretium duis.\n"; */
        char *fonky_chonky = "Do you want the fonky chonky?\n";

/*        advance_file(f, fonky_chonky, 37);*/
        shrink_file(f, strlen(fonky_chonky), 37);

        close_file(f);

        return 0;
}
