#include <stdio.h>
#include <string.h>

#include "file.h"
#include "fileutils.h"

int main(int argc, char **argv)
{
        struct file *f = open_file("./test.md");
        char *message = "This is a very cool message!\n";
        /*char *loren = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce ullamcorper quam mauris, at posuere augue posuere non. Sed iaculis elit et placerat dictum. Nam faucibus venenatis ex. Nullam tempor eros vel lectus gravida tincidunt. Phasellus tincidunt, nisl eget porttitor rhoncus, neque tellus aliquam massa, in ultrices est metus ac sapien. Nam dignissim ipsum diam, vel efficitur nisi placerat vitae. Morbi auctor tellus non ipsum efficitur fringilla id nec ex. Ut lectus magna, laoreet cursus erat at, iaculis egestas nisi. In euismod nisl eu tortor porttitor, vel euismod eros faucibus. Nullam enim quam, iaculis sed nunc euismod, euismod interdum metus. Mauris tristique odio dui, sit amet aliquam mauris ultricies vel. Aliquam condimentum ornare metus, ac blandit nibh elementum at. Curabitur maximus velit rutrum, vestibulum felis sed, dignissim enim. Nam laoreet, nunc eget condimentum viverra, ipsum tortor ullamcorper lorem, a iaculis metus sapien congue risus. Suspendisse sapien diam, rutrum a nulla convallis, sagittis porta justo. Quisque id viverra dolor. Aenean eu convallis mauris, quis rhoncus leo. Nunc tincidunt nibh sed faucibus tempor. Nulla vitae dictum augue, vel ultrices purus. Aliquam efficitur magna nec metus rhoncus molestie. Fusce eget dolor est. Aenean accumsan condimentum ipsum, ac molestie nibh euismod ac. Proin iaculis volutpat diam, at varius libero pretium duis.\n"; */
        char *fonky_chonky = "Do you want the fonky chonky?\n";

/*        advance_file(f, fonky_chonky, 37);*/
        shrink_file(f, strlen(fonky_chonky), 37);

        close_file(f);

        return 0;
}
