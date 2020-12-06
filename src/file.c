#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SIZE 500

#define MIN(x, y) ((x) < (y)) ? (x) : (y)

static void advance_file_chunk(FILE *f, long backstep, long bytes_add,
                long offset)
{
        char buf[BUF_SIZE];

        for (fseek(f, 0, SEEK_END); ftell(f) > offset;
                        fseek(f, -backstep - bytes_add, SEEK_CUR)) {
                if (ftell(f) < backstep)
                        backstep = ftell(f) - offset;

                fseek(f, -backstep, SEEK_CUR);
                fread(buf, sizeof (char), backstep, f);
                fseek(f, -backstep + bytes_add, SEEK_CUR);
                fwrite(buf, sizeof (char), backstep, f);
        }
}

static void goback_file_chunk(FILE *f, off_t file_size, long forwardstep,
                long bytes_remove, long offset)
{
        char buf[BUF_SIZE];

        for (fseek(f, offset + bytes_remove, SEEK_SET); ftell(f) < file_size;
                        fseek(f, bytes_remove, SEEK_CUR)) {
                if (ftell(f) + forwardstep > file_size)
                        forwardstep = file_size - ftell(f);

                fread(buf, sizeof (char), forwardstep, f);
                fseek(f, -forwardstep - bytes_remove, SEEK_CUR);
                fwrite(buf, sizeof (char), forwardstep, f);
        }
}

off_t get_file_size(const char *path)
{
        struct stat buf;

        if (stat(path, &buf) != 0)
                return -1;

        return buf.st_size;
}

static void advance_file(FILE *f, off_t file_size, long bytes_add, long offset)
{
        long backstep = MIN(file_size - offset, BUF_SIZE);

        advance_file_chunk(f, backstep, bytes_add, offset);
}

static void goback_file(FILE *f, off_t file_size, long bytes_remove, long offset)
{
        long forwardstep = MIN(file_size, BUF_SIZE);

        goback_file_chunk(f, file_size, forwardstep, bytes_remove, offset);
        ftruncate(fileno(f), file_size - bytes_remove);
}

int main(void)
{
        FILE *f = fopen("./test.md", "r+");
        char *message = "This is a very cool message!\n";
        char *loren = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce ullamcorper quam mauris, at posuere augue posuere non. Sed iaculis elit et placerat dictum. Nam faucibus venenatis ex. Nullam tempor eros vel lectus gravida tincidunt. Phasellus tincidunt, nisl eget porttitor rhoncus, neque tellus aliquam massa, in ultrices est metus ac sapien. Nam dignissim ipsum diam, vel efficitur nisi placerat vitae. Morbi auctor tellus non ipsum efficitur fringilla id nec ex. Ut lectus magna, laoreet cursus erat at, iaculis egestas nisi. In euismod nisl eu tortor porttitor, vel euismod eros faucibus. Nullam enim quam, iaculis sed nunc euismod, euismod interdum metus. Mauris tristique odio dui, sit amet aliquam mauris ultricies vel. Aliquam condimentum ornare metus, ac blandit nibh elementum at. Curabitur maximus velit rutrum, vestibulum felis sed, dignissim enim. Nam laoreet, nunc eget condimentum viverra, ipsum tortor ullamcorper lorem, a iaculis metus sapien congue risus. Suspendisse sapien diam, rutrum a nulla convallis, sagittis porta justo. Quisque id viverra dolor. Aenean eu convallis mauris, quis rhoncus leo. Nunc tincidunt nibh sed faucibus tempor. Nulla vitae dictum augue, vel ultrices purus. Aliquam efficitur magna nec metus rhoncus molestie. Fusce eget dolor est. Aenean accumsan condimentum ipsum, ac molestie nibh euismod ac. Proin iaculis volutpat diam, at varius libero pretium duis.\n";
        char *fonky_chonky = "Do you want the fonky chonky?\n";

//        advance_file(f, get_file_size("./test.md"), strlen(fonky_chonky), 37);
//        fseek(f, 37, SEEK_SET);
//        fwrite(fonky_chonky, sizeof (char), strlen(fonky_chonky), f);
        goback_file(f, get_file_size("./test.md"), strlen(message), 37);

        fclose(f);
}
