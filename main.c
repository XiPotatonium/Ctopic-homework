#include <stdio.h>
#include <string.h>
#include "libman.h"

int main(void) {
    char* filename = "libman.db";
    BookData_t* data = initialize(filename);
    libman_interactive(data);
    free_memory(data);
    printf("Thanks for using.\n");
    return 0;
}
