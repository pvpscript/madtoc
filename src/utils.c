#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"


char *str_to_markdown_anchor(char *heading)
{
	size_t heading_len = strlen(heading);
	char *anchor = malloc(sizeof(char) * heading_len + 1);
	int i;

	for (i = 0; i < heading_len; i++)
		anchor[i] = isalpha(heading[i]) ? tolower(heading[i]) : '-';

	return anchor;
}

