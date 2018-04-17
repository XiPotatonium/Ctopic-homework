#include <stdio.h>
#include <string.h>
#include "libman.h"

int main(void)
{
    char *filename = "libman.db";
    BookData_t data = initialize(filename);
    printf("%d %s initialized.\n", data.nrecords_in_file,
         data.nrecords_in_file > 1 ? "books" : "book");
    int i = 0;
    while (1)
    {
        printf(">>>");
        if (get_cmd() == -1)
        {
            printf("Invalid input.\n");
        }
        else if (strcmp(cmd.str_cmd, "add") == 0)
        {
            add_book(&data);
        }
        else if (strcmp(cmd.str_cmd, "del") == 0)
        {
            delete_book(&data);
        }
        else if (strcmp(cmd.str_cmd, "exit") == 0)
        {
            break;
        }
        else if (strcmp(cmd.str_cmd, "help") == 0)
        {
            printf("These commands are defined internally:\n"
                   "\tadd: Add new books to the database\n"
                   "\tdel: Delete books from the database\n"
                   "\t\t" DID "\n"
                   "\t\t" DALL "\n"
                   "\thelp: Print the help doc\n"
                   "\tmodify: Modify books in the database\n"
                   "\t\t" MID "\n"
                   "\tsearch: Search books in the database\n"
                   "\t\t" SALL "\n"
                   "\t\t" SAUTHOR "\n"
                   "\t\t" SKEY "\n"
                   "\t\t" SNAME "\n");
        }
        else if (strcmp(cmd.str_cmd, "modify") == 0)
        {
            modify_book(&data);
        }
        else if (strcmp(cmd.str_cmd, "search") == 0)
        {
            i = search_book(&data);
            if (i >= 0)
            {
                printf("%d %s found.\n", i, i > 1 ? "results" : "result");
            }
        }
        else
        {
            printf("Invalid command.\n");
        }
    }

    free_memory(&data);
    printf("Thanks for use.\n");
    return 0;
}
