#include <stdio.h>
#include <stdlib.h>

void usage() {
    puts("./anal folder_name");
}


int main(int argc, const char *argv[])
{
    if(argc < 2) {
        usage();
        return -1;
    }

    return 0;
}
