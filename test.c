#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_LINE "[//]: #mdtoc_start"
#define END_LINE "[//]: #mdtoc_end"
#define START_LINE_LEN (sizeof(START_LINE)/sizeof(char)) + 2
#define END_LINE_LEN (sizeof(END_LINE)/sizeof(char)) + 2

#define MAX(x, y) ((x) > (y)) ? (x) : (y)

struct file_info {
        size_t lines_to_start;
        size_t lines_to_end;
        long offset_start;
        long offset_end;
};

int main(int argc, char **argv)
{
        FILE *f = fopen("./test.md", "r+");
        size_t lines_to_start = 0;
        size_t lines_to_end = 0;
        char buf[MAX(START_LINE_LEN, END_LINE_LEN) + 1];
        size_t buf_temp_len;
        char w;

        struct file_info info = {
                .lines_to_start = 0,
                .lines_to_end = 0
        };

        printf("BUF LEN -> %lu\n", sizeof buf);

        for (;;) {
                if(fgets(buf, (sizeof buf), f))
                        buf_temp_len = strlen(buf);
                else
                        break;

                if (buf[buf_temp_len - 1] == '\n') {
                        if (!strncmp(buf, START_LINE, START_LINE_LEN - 3)) {
                                printf("FOUND IT <<start>>! $%s$\n", buf);
                                info.offset_start = ftell(f);
                                break;
                        }
                        info.lines_to_start++;
                        printf("Buffer: $%s$\n", buf);
                        continue;
                }


                while((w = fgetc(f)) != '\n') {
                        printf("CHAR: %d -> %c\n", w, w);
                        getchar();
                }
                puts("END OF LINE");
                info.lines_to_start++; 
        }

        puts("SEARCH FOR THE ENDING");
        getchar();

        for (;;) {
                if(fgets(buf, (sizeof buf), f))
                        buf_temp_len = strlen(buf);
                else
                        break;

                if (buf[buf_temp_len - 1] == '\n') {
                        if (!strncmp(buf, END_LINE, END_LINE_LEN - 3)) {
                                printf("FOUND IT <<end>>! $%s$\n", buf);
                                info.offset_end = ftell(f);
                                break;
                        }
                        info.lines_to_end++;
                        printf("Buffer: $%s$\n", buf);
                        continue;
                }


                while((w = fgetc(f)) != '\n') {
                        printf("CHAR: %d -> %c\n", w, w);
                        getchar();
                }
                puts("END OF LINE");
                info.lines_to_end++; 
        }

        fseek(f, 0, SEEK_END);
        long current_offset = ftell(f);
        printf("Went to (end): %ld\n", current_offset);
        fseek(f, info.offset_start, SEEK_SET);
        printf("Went to %ld\n", info.offset_start);
        char *temp_buf = malloc(sizeof (char) * (current_offset - info.offset_start));
        fread(temp_buf, sizeof (char), current_offset - info.offset_start, f);
        printf("Wrote stuff with length %ld\n", current_offset - info.offset_start);
        fseek(f, info.offset_start, SEEK_SET);
        printf("Went to %ld\n", info.offset_start);
        char *str = "This is a message!\n";
        fwrite(str, sizeof (char), strlen(str), f);
        printf("Wrote str with size %lu to file\n", strlen(str));
        fwrite(temp_buf, sizeof (char), current_offset - info.offset_start, f);
        printf("Wrote str with size %ld to file\n", current_offset-info.offset_start);
        fclose(f);

        printf("Lines to the 'start line': %d\n", info.lines_to_start);
        printf("Bytes offset for the beginning: %ld\n", info.offset_start);
        printf("Lines from the 'start line' to the 'end line': %d\n", info.lines_to_end);
        printf("Bytes offset for the end: %ld\n", info.offset_end);

        return 0;
}
