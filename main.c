#include <stdio.h>
#include <string.h>
#include "libman.h"

int main(void) {
    char* filename = "libman.db";
    BookData_t* data = libman_initialize(filename);
    libman_interactive(data);
    libman_free(data);
    printf("Thanks for using.\n");
    return 0;
}
