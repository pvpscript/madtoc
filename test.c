#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_LINE "[//]: #mdtoc_start"
#define END_LINE "[//]: #mdtoc_end"
#define START_LINE_LEN (sizeof(START_LINE)/sizeof(char)) + 2
#define END_LINE_LEN (sizeof(END_LINE)/sizeof(char)) + 2

#define MAX(x, y) ((x) > (y)) ? (x) : (y)

int main(int argc, char **argv)
{
        FILE *f = fopen("./test.md", "r");
        size_t lines_to_start = 0;
        size_t lines_to_end = 0;
        char buf[MAX(START_LINE_LEN, END_LINE_LEN) + 1];
        size_t buf_temp_len;
        char w;

        printf("BUF LEN -> %lu\n", sizeof buf);

        for (;;) {
                if(fgets(buf, (sizeof buf), f))
                        buf_temp_len = strlen(buf);
                else
                        break;

                if (buf[buf_temp_len - 1] == '\n') {
                        lines_to_start++;
                        if (!strncmp(buf, START_LINE, START_LINE_LEN - 3)) {
                                printf("FOUND IT <<start>>! $%s$\n", buf);
                                break;
                        }
                        printf("Buffer: $%s$\n", buf);
                        continue;
                }


                while((w = fgetc(f)) != '\n') {
                        printf("CHAR: %d -> %c\n", w, w);
                        getchar();
                }
                puts("END OF LINE");
                lines_to_start++; 
        }

        puts("SEARCH FOR THE ENDING");
        getchar();

        for (;;) {
                if(fgets(buf, (sizeof buf), f))
                        buf_temp_len = strlen(buf);
                else
                        break;

                if (buf[buf_temp_len - 1] == '\n') {
                        lines_to_start++;
                        if (!strncmp(buf, END_LINE, END_LINE_LEN - 3)) {
                                printf("FOUND IT <<end>>! $%s$\n", buf);
                                break;
                        }
                        printf("Buffer: $%s$\n", buf);
                        continue;
                }


                while((w = fgetc(f)) != '\n') {
                        printf("CHAR: %d -> %c\n", w, w);
                        getchar();
                }
                puts("END OF LINE");
                lines_to_start++; 
        }

        return 0;
}
