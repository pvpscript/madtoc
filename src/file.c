#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SIZE 500

#define MIN(x, y) ((x) < (y)) ? (x) : (y)

static void advance_file_chunk(FILE *f, long n, long offset)
{
        char buf[BUF_SIZE];

        for (fseek(f, 0, SEEK_END); ftell(f) > offset && ftell(f);
                        fseek(f, -(2*n), SEEK_CUR)) {
            printf("START FTELL: %ld\n", ftell(f));
            fseek(f, -n, SEEK_CUR);
            fread(buf, sizeof (char), n, f);
            fwrite(buf, sizeof (char), n, f);

            printf("END FTELL: %ld\n", ftell(f));
        }
}

void advance_file(FILE *f, long advance_size, long offset)
{
        long m;

        if (advance_size < BUF_SIZE)
            advance_file_chunk(f, advance_size, offset);
//        else
//            for (m = buffer; m < advance_size; m += buffer)
//                advance_file_chunk(f, m, offset);
}

int main(void)
{
        FILE *f = fopen("./test.md", "r+");
        char *message = "This is a very cool message!\n";

        advance_file(f, strlen(message), 37);
        fseek(f, 37, SEEK_SET);
        fwrite(message, sizeof (char), strlen(message), f);

        fclose(f);
}
