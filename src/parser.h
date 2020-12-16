#ifndef PARSER_H
#define PARSER_H

#include "list.h"

long parse_toc(FILE *fp, char *section);
struct list *parse_section(FILE *fp);

#endif /* PARSER_H */
