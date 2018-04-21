/**
 * File: libman.c
 */
#include "libman.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>

/**************************************
 * Declarations of private functions *
 **************************************/

/**
 * Function: import_book
 * ---------------------
 * Description:
 *  Import books from a .txt file indicated by src_name
 * In princile, users should only import a .txt generated
 * by export_book
 * Return value:
 *  It returns -1 if there is something wrong in openning
 * the source file.
 *  It returns -2 if the source file is invalid.
 *  It returns 0 if everthing is fine.
*/
static int import_book(BookData_t* data, char* src_name);

/**
 * Function: export_book
 * ---------------------
 * Description:
 *  Export books to a .txt file. The filename is by default
 * "out.txt"
 * Return value:
 *  Return -1 if there is something wrong with the output file
 *  Return 0 if everthing is fine
*/
static int export_book(BookData_t* data, int allrecords);

/**
 * Function: get_password
 * ----------------------
 * Description:
 *  Get the password from keyboard input.
 * Return value:
 *  The password.
*/
static char* get_password(char* prompt, int asterisk);

/**
 * Fucntion: get_cmd
 * -----------------
 * Description:
 *  The function get a string from keyboard and resolve it
 * into several arguments
 * Return value:
 *  It returns the number of arguments it received
 *  It returns -4 if there is some error in parsing the command
*/
static int get_cmd(void);

/**
 * Function: add_book
 * ------------------
 * Description:
 *  The function adds new books into the linked list and
 * save them to the file indicated by data
 * Return value:
 *  It returns the number of books that has been added.
 *  It returns -1 if there are something wrong with teh file
*/
static int add_book(BookData_t* data);

/**
 * Function: modify_book
 * ---------------------
 * Description:
 *  The function modifies a certain book indicated in the
 * command,save the changes to the file indicated by data
 * Return value:
 *  It returns -3 if there are something wrong in the command
 *  It returns 0 if everything is all fine
 *  It returns -1 if there is something wrong with the file
 *  It returns 1 if the target book doesn't exist
*/
static int modify_book(BookData_t* data);

/**
 * Function: delete_book
 * ---------------------
 * Description:
 * 	The function delete the certain book indicated in the command 
 * 	You can also use it to delete the whole file. The programme will
 * shut down if you delete the whole file
 * Rrturn value:
 * 	It returns 0 if everthing goes well.
 * 	It returns 1 if there is something wrong with the file
 *  It returns -1 if the argument is invalid
*/
static int delete_book(BookData_t* data);

/**
 * Function: search_book
 * ---------------------
 * Description:
 *  The function search the linked list with key word indicated
 * in the command and print the result in the screen
 * Return value:
 *  It returns 0 if everything is all fine
 *  It returns -3 if there are something wrong in the command
*/
static int search_book(BookData_t* data);

static int hash(char* name);
static int set_book(Book_t* book);
static int append_linkedlist(BookData_t* data, Book_t* book, int i);
static int print_book(Node_t* node);
static int fprint_book(FILE* fp, Node_t* node);
static void book_save(BookData_t* data, Book_t* book, FILE* fp);
static void book_load(BookData_t* data, Book_t* book, FILE* fp);

#define N_COMMANDS 7
char* commands[N_COMMANDS] = {"add",  "del",    "export", "exit",
                     "help", "modify", "search"};

/*********************
 * Public functions *
 *********************/
int libman_interactive(BookData_t* data) {
    printf("\n");
    printf(
        "+----------------------------------+\n"
        "|                                  |\n"
        "|        LibMan Interactive        |\n"
        "|                                  |\n"
        "+----------------------------------+\n");
    printf("\n");
    printf("Type \"help\" for more information.\n");
    int i = 0;
    while (1) {
        printf(">>> ");
        if (get_cmd() == -1) {
            printf("Invalid input.\n");
        } else if (strcmp(cmd.str_cmd, "add") == 0) {
            add_book(data);
        } else if (strcmp(cmd.str_cmd, "del") == 0) {
            delete_book(data);
        } else if (strcmp(cmd.str_cmd, "exit") == 0) {
            return 0;
        } else if (strcmp(cmd.str_cmd, "export") == 0) {
            if (cmd.argc == 1) {
                printf("Insufficient argument.\n");
            } else if (strcmp(cmd.argv[1], "-all") == 0 && cmd.argc == 2) {
                export_book(data, 1);
            }
        } else if (strcmp(cmd.str_cmd, "help") == 0) {
            printf(
                "These commands are defined internally:\n"
                "\tadd: Add new books to the database\n"
                "\tdel: Delete books from the database\n"
                "\t\t" DID "\t" ARG
                "\n"
                "\t\t" DALL
                "\n"
                "\texit: exit the programme\n"
                "\texport: Export books to a .txt file\n"
                "\t\t" EALL
                "\n"
                "\thelp: Print the help doc\n"
                "\tmodify: Modify books in the database\n"
                "\t\t" MID "\t" ARG
                "\n"
                "\tsearch: Search books in the database\n"
                "\t\t" SALL "\t" ARG_OPTIONAL
                "\n"
                "\t\t" SAUTHOR "\t" ARG
                "\n"
                "\t\t" SKEY "\t" ARG
                "\n"
                "\t\t" SNAME "\t" ARG "\n");
        } else if (strcmp(cmd.str_cmd, "import") == 0) {
            /*
            "\timport: Import new books from .txt to the database\n"
            "\t\tUsage: import <filename>\n"
            if (cmd.argc == 1) {
                printf("Insufficient argument.\n");
            } else {
                import_book(data, cmd.argv[1]);
            }
            */
        } else if (strcmp(cmd.str_cmd, "modify") == 0) {
            modify_book(data);
        } else if (strcmp(cmd.str_cmd, "search") == 0) {
            i = search_book(data);
            if (i >= 0) {
                printf("%d result(s) found.\n", i);
            }
        } else if (strcmp(cmd.str_cmd, "") == 0) {
        } else {
            char* guess = NULL;
            int len = strlen(cmd.str_cmd);
            int ic;
            if (len == 1) {
                char ch = cmd.str_cmd[0];
                for (ic = 0; ic < N_COMMANDS; ic++) {
                    if (commands[ic][0] == ch) {
                        guess = commands[ic];
                        break;
                    }
                }
            } else {
                int max = 0;
                int i1, i2;
                for (ic = 0; ic < N_COMMANDS; ic++) {
                    int match = 0;
                    int cmdlen = strlen(commands[ic]);
                    for (i1 = 0; i1 < len - 1; i1++) {
                        int16_t cg1 = *(int16_t*)(cmd.str_cmd + i1);

                        for (i2 = 0; i2 < cmdlen - 1; i2++) {
                            int16_t cg2 = *(int16_t*)(commands[ic] + i2);
                            if (cg1 == cg2) {
                                ++match;
                                break;
                            }
                        }
                    }
                    if (match > max) {
                        max = match;
                        guess = commands[ic];
                    }
                }
            }
            if (guess == NULL) {
                printf("Invalid command.\n");
            } else {
                printf("Did you mean this?\n\t%s\n", guess);
            }
        }
    }
}

BookData_t* libman_initialize(char* filename) {
    time_t curtime;
    time(&curtime);
    printf("LibMan 1.0 | %s, %s\n", __DATE__, __TIME__);
    BookData_t* data = (BookData_t*)malloc(sizeof(BookData_t));
    FILE* fp = fopen(filename, "rb");
    Book_t* tmp;
    Node_t* tmp_node;

    int i = 0;
    int new = 0;
    int hashcode;
    /* Initialize hash list and construct the sentinel */
    for (i = 0; i < BUCKETS_SIZE; ++i) {
        data->hash_list[i] = (Node_t*)malloc(sizeof(Node_t));
        data->hash_list[i]->book = NULL;
        data->hash_list[i]->next = data->hash_list[i]->previous =
            data->hash_list[i];
    }

    i = 0;
    if (fp == NULL) {
        /* Create new database if the file doesn't exist */
        fp = fopen(filename, "wb");
        if (fp == NULL) {
            printf("ERROR: ");
            perror(NULL);
            free(data);
            return NULL;
        }
        printf("A new file has been created to save the books.\n");
        while (1) {
            char* p1 = get_password("Please create the password: ", 1);
            char* p2 = get_password("Confirm: ", 1);
            if (strcmp(p1, p2) == 0) {
                data->aes = aes256_init(aes256_get_key(p1));
                break;
            } else {
                printf("Confirm failed!\n");
            }
        }
        new = 1;
    } else {
        char* password = get_password("Enter the password: ", 1);
        char* key = aes256_get_key(password);
        data->aes = aes256_init(key);

        while (1) {
            tmp = (Book_t*)malloc(BOOK_SIZE);
            book_load(data, tmp, fp);
            if (feof(fp)) break;
            append_linkedlist(data, tmp, i);
            ++i;
        }

        free(tmp);
    }

    fclose(fp);
    data->nrecords_in_file = i;
    data->ndeletion = 0;
    strcpy(data->filename, filename);
    if (!new) {
        printf("%d book(s) initialized.\n", data->nrecords_in_file);
    }
    return data;
}

int libman_free(BookData_t* data) {
    /* Remember to free the memory allocated in append_linkedlist */
    Node_t* tmp;
    int ret;
    int i = 0;
    for (i = 0; i < BUCKETS_SIZE; ++i) {
        tmp = data->hash_list[i]->next;
        while (tmp != data->hash_list[i]) {
            tmp = tmp->next;
            free(tmp->previous->book);
            free(tmp->previous);
        }
        free(tmp->book);
        free(tmp);
    }

    /* Rewrite the file if some records have been deleted */
    if (data->ndeletion > 0) {
        Book_t tmp;
        char* tmp_name = "temp.db";
        FILE* fp = fopen(data->filename, "rb");
        if (fp == NULL) {
            printf("ERROR: ");
            perror(NULL);
            printf("Fail to open %s to save modifications\n", data->filename);
            return -1;
        }
        FILE* ftmp = fopen(tmp_name, "wb");
        if (ftmp == NULL) {
            printf("ERROR: ");
            perror(NULL);
            printf("Fail to create %s to save modifications.\n", tmp_name);
            return -1;
        }
        for (i = 0; i < data->nrecords_in_file - data->ndeletion; ++i) {
            book_load(data, &tmp, fp);
            book_save(data, &tmp, ftmp);
        }
        fclose(fp);
        fclose(ftmp);
        if (remove(data->filename) == -1) {
            printf("ERROR: ");
            perror(NULL);
            printf("Fail to remove the old file. Please remove it manually.\n");
            ret = -2;
        }
        if (rename(tmp_name, data->filename) == -1) {
            printf("ERROR: ");
            perror(NULL);
            printf(
                "Fail to rename the data file. Please rename it manually.\n");
            ret = -2;
        }
    }

    return ret;
}

/*************************************
 * Definitions of private functions *
 ************************************/
static int hash(char* name) {
    char ch = name[0];
    int i = 0;
    if (isalpha(ch)) {
        ch = tolower(ch);
        i = ch - 'a';
    } else if (isdigit(ch)) {
        i = 26;
    } else {
        i = 27;
    }

    return i;
}

static int set_book(Book_t* book) {
    int i = 0;
    char ch;

    printf("Enter the name of the book: ");
    gets(book->name);
    if (strlen(book->name) == 0) return 1;

    printf("Enter the id of the book: ");
    gets(book->id);

    printf("Enter the authors of the book (not exceeding %d authors):\n",
           MAX_AUTHOR);
    for (i = 0; i < MAX_AUTHOR; ++i) book->authors[i][0] = '\0';
    for (i = 0; i < MAX_AUTHOR; ++i) {
        gets(book->authors[i]);
        if (strlen(book->authors[i]) == 0) break;
    }

    printf("Enter the keys of the book (not exceeding %d keys):\n", MAX_KEY);
    for (i = 0; i < MAX_KEY; ++i) book->keys[i][0] = '\0';
    for (i = 0; i < MAX_KEY; ++i) {
        gets(book->keys[i]);
        if (strlen(book->keys[i]) == 0) break;
    }

    printf("Enter the name of the press of this book: ");
    gets(book->press);

    printf("Enter the pubish date of this book: ");
    gets(book->publish_date);
    printf("\n");

    return 0;
}

static int append_linkedlist(BookData_t* data, Book_t* book, int i) {
    int hashcode = hash(book->name);
    /* Add the new node to the hash list */
    Node_t* tmp_node = (Node_t*)malloc(sizeof(Node_t));
    Node_t* cur_node = data->hash_list[hashcode]->next;
    while (cur_node->book != NULL) {
        if (strcmp(cur_node->book->name, book->name) <= 0) {
            cur_node = cur_node->next;
        } else {
            break;
        }
    }
    tmp_node->previous = cur_node->previous;
    tmp_node->next = cur_node;
    cur_node->previous->next = tmp_node;
    cur_node->previous = tmp_node;
    tmp_node->book = book;
    tmp_node->index = i;
    return 0;
}

static int print_book(Node_t* node) {
    int i;
    char format[10];
    sprintf(format, "%%%ds: %%s\n", ID_LEN);
    printf(format, node->book->id, node->book->name);
    printf("\tAuthors:");
    for (i = 0; i < MAX_AUTHOR; ++i) {
        if (strlen(node->book->authors[i]) == 0) break;
        printf(" %s,", node->book->authors[i]);
    }
    printf("\b \n\tPress: %s\n", node->book->press);
    printf("\tkeys: ");
    for (i = 0; i < MAX_KEY; ++i) {
        if (strlen(node->book->keys[i]) == 0) break;
        printf(" %s,", node->book->keys[i]);
    }
    printf("\b \n\tPublish date: %s\n", node->book->publish_date);

    return 0;
}

static int fprint_book(FILE* fp, Node_t* node) {
    int i;
    char format[10];
    sprintf(format, "%%%ds: %%s\n", ID_LEN);
    fprintf(fp, format, node->book->id, node->book->name);
    fprintf(fp, "\tAuthors: ");
    for (i = 0; i < MAX_AUTHOR; ++i) {
        if (strlen(node->book->authors[i]) == 0) break;
        if (i != 0) fprintf(fp, ", ");
        fprintf(fp, "%s", node->book->authors[i]);
    }
    fprintf(fp, "\n\tPress: %s\n", node->book->press);
    fprintf(fp, "\tkeys: ");
    for (i = 0; i < MAX_KEY; ++i) {
        if (strlen(node->book->keys[i]) == 0) break;
        if (i != 0) fprintf(fp, ", ");
        fprintf(fp, "%s", node->book->keys[i]);
    }
    fprintf(fp, "\n\tPublish date: %s\n", node->book->publish_date);
    for (i = 0; i < 40; ++i) {
        fprintf(fp, "=");
    }
    fprintf(fp, "\n");

    return 0;
}

static char* get_password(char* prompt, int asterisk) {
    printf("%s", prompt);
    /* last char for /0 */
    char* password = malloc(PASSWORD_LEN + 1);
    char ch;
    int i = 0;
    while (1) {
        if (i == PASSWORD_LEN) break;
        ch = getch();

        if (ch == 13) {
            break;
        }
        if (ch == 8) /*ASCII value of BACKSPACE*/
        {
            if (i > 0) {
                putchar('\b');
                putchar(' ');
                putchar('\b');
                i--;
            }
            continue;
        }

        password[i++] = ch;
        ch = '*';
        putch(ch);
    }
    password[i] = '\0';
    printf("\n");
    return password;
}

static void book_save(BookData_t* data, Book_t* book, FILE* fp) {
    void* enc = calloc(1, BOOK_SIZE);
    memcpy(enc, book, sizeof(Book_t));
    aes256_encrypt(data->aes, enc, BOOK_SIZE);
    fwrite(enc, BOOK_SIZE, 1, fp);
    free(enc);
}

static void book_load(BookData_t* data, Book_t* book, FILE* fp) {
    void* dec = calloc(1, BOOK_SIZE);
    fread(dec, BOOK_SIZE, 1, fp);
    aes256_decrypt(data->aes, dec, BOOK_SIZE);
    memcpy(book, dec, sizeof(Book_t));
    free(dec);
}

static int export_book(BookData_t* data, int allrecords) {
    char* out_filename = "out.txt";
    FILE* fout;
    Node_t* tmp = NULL;
    int i = 0;

    fout = fopen(out_filename, "w");
    if (fout == NULL) {
        printf("ERROR: ");
        perror(NULL);
        return -1;
    }

    /* Print header */
    fprintf(fout, "+");
    for (i = 0; i < 38; ++i) {
        fprintf(fout, "=");
    }
    fprintf(fout, "+\n|      Books exported from libman      |\n+");
    for (i = 0; i < 38; ++i) {
        fprintf(fout, "=");
    }
    fprintf(fout, "+\n");

    if (allrecords) {
        /* Print all records to the .txt file */
        for (i = 0; i < BUCKETS_SIZE; ++i) {
            tmp = data->hash_list[i]->next;
            while (tmp != data->hash_list[i]) {
                fprint_book(fout, tmp);
                tmp = tmp->next;
            }
        }
    }

    fclose(fout);
    printf("Successfully export books to %s\n", out_filename);
    return 0;
}

static int import_book(BookData_t* data, char* src_name) {
    FILE* fp = fopen(src_name, "r");
    if (fp == NULL) {
        printf("ERROR: ");
        perror(NULL);
        return -1;
    }
    int max_line = AUTHOR_LEN * MAX_AUTHOR + ID_LEN + 20;
    char line[max_line];
    Book_t* book;
    char* str;
    int i, j;

    /* Verify the validity of the source file */
    fgets(line, max_line, fp);
    fgets(line, max_line, fp);
    if (strstr(line, "Books exported from libman") == NULL) {
        printf("ERROR: ");
        printf("Invalid import source, the file is not generated by export_book().\n");
        return -2;
    }
    fgets(line, max_line, fp);

    while (fgets(line, max_line, fp)) {
        book = (Book_t*)malloc(sizeof(Book_t));
        /* Parse the first line, for id and name */
        line[strlen(line) - 1] = '\0';
        i = 0;
        while (line[++i] != ' ');
        if (line[i] != ':') {
            str = &line[i];
            while (line[++i] != ':');
            line[i] = '\0';
            strcpy(book->id, str);
            str = &line[i + 2];
            strcpy(book->name, str);
        } else {
            /* For case that id is null */
            strcpy(book->id, "");
            str = &line[i + 2];
            strcpy(book->name, str);
        }
        /* Parse the second line for authors */
        fgets(line, max_line, fp);
        line[strlen(line) - 1] = '\0';
        for (i = 0; i < MAX_AUTHOR; ++i) book->authors[i][0] = '\0';
        i = 0;
        while (line[++i] != ':');
        for (j = 0; j < MAX_AUTHOR; ++j) {
            i += 2;
            str = &line[i];
            while (line[i] != ',' && line[i] != '\0') ++i;
            if (line[i] == ',') {
                line[i] = '\0';
                strcpy(book->authors[j], str);
            } else {
                strcpy(book->authors[j], str);
                break;
            }
        }
        /* Parse the third line for press */
        fgets(line, max_line, fp);
        line[strlen(line) - 1] = '\0';
        i = 0;
        while (line[i++] != ':');
        str = &line[++i];
        strcpy(book->press, str);
        /* Parse the fourth line for keys */
        fgets(line, max_line, fp);
        line[strlen(line) - 1] = '\0';
        for (i = 0; i < MAX_KEY; ++i) book->keys[i][0] = '\0';
        i = 0;
        while (line[++i] != ':');
        for (j = 0; j < MAX_KEY; ++j) {
            i += 2;
            str = &line[i];
            while (line[i] != ',' && line[i] != '\0') ++i;
            if (line[i] == ',') {
                line[i] = '\0';
                strcpy(book->keys[j], str);
            } else {
                strcpy(book->keys[j], str);
                break;
            }
        }
        /* Parse the fifth line for publish date*/
        fgets(line, max_line, fp);
        line[strlen(line) - 1] = '\0';
        i = 0;
        while (line[i++] != ':');
        str = &line[++i];
        strcpy(book->publish_date, str);
        /* Parse the delimited identifiers */
        fgets(line, max_line, fp);
        append_linkedlist(data, book, -1);
    }

    return 0;
}

static int add_book(BookData_t* data) {
    FILE* fp = fopen(data->filename, "rb+");
    if (fp == NULL) {
        printf("ERROR: ");
        perror(NULL);
        return -1;
    }
    Book_t* tmp;
    int hashcode;
    int count = 0;
    int i = 0;

    while (1) {
        tmp = (Book_t*)malloc(BOOK_SIZE);
        if (set_book(tmp) == 1) {
            printf("Adding terminated: %d %s added.\n", count,
                   count > 1 ? "books" : "book");
            break;
        }

        if (data->ndeletion > 0) {
            /* If some deletion has been made, rewrite the trash record */
            fseek(fp, -data->ndeletion * BOOK_SIZE, SEEK_END);
            --data->ndeletion;
        } else {
            /* Once a book is added, save it to the end of the file */
            fseek(fp, 0, SEEK_END);
            ++data->nrecords_in_file;
        }
        book_save(data, tmp, fp);
        append_linkedlist(data, tmp, ftell(fp) / BOOK_SIZE - 1);
        ++count;
    }
    free(tmp);

    fclose(fp);
    return count;
}

static int modify_book(BookData_t* data) {
    Node_t* node;
    char* key_id;
    int i, flag;
    FILE* fp = NULL;
    fp = fopen(data->filename, "rb+");
    if (fp == NULL) {
        printf("ERROR: ");
        perror(NULL);
        return -1;
    }

    if (cmd.argc <= 1) {
        printf("Insufficient argument.\n");
        return -3;
    } else if (strcmp(cmd.argv[1], MID) != 0) {
        printf("Invalid argument.\n");
        return -3;
    } else if (cmd.argc == 2) {
        key_id = "";
    } else {
        key_id = cmd.argv[2];
    }

    for (i = 0, flag = 0; i < BUCKETS_SIZE; ++i) {
        node = data->hash_list[i]->next;
        while (node != data->hash_list[i]) {
            if (strcmp(key_id, node->book->id) == 0) {
                set_book(node->book);
                fseek(fp, BOOK_SIZE * node->index, SEEK_SET);
                book_save(data, node->book, fp);
                flag = 1;
                break;
            }
            node = node->next;
        }
        if (flag == 1) break;
    }

    fclose(fp);
    if (flag == 0) {
        printf("No such id.\n");
        return 1;
    } else {
        return 0;
    }
}

static int delete_book(BookData_t* data) {
    /* Once a book is deleted, fill the gap with the last book in the file */
    char* key_word;
    char ch;
    Book_t tmp;
    Node_t* cur_node = NULL;
    int i = 0;
    int index = -1;
    FILE* fp = NULL;

    if (cmd.argc <= 1) {
        printf("Insufficient argument.\n");
        return -3;
    } else if (strcmp(cmd.argv[1], DALL) == 0) {
        printf("Are you sure you want to delete the whole file?\n");
        printf("This may not be undone (Y/N): ");
        ch = getchar();
        if (ch == 'y' || ch == 'Y') {
            remove(data->filename);
            data->ndeletion = 0;
            free_memory(data);
            exit(0);
        } else {
            printf("Deletion terminated.\n");
            return 0;
        }
    } else if (strcmp(cmd.argv[1], DID) != 0) {
        printf("Invalid argument\n");
        return -3;
    } else if (cmd.argc == 2) {
        key_word = "";
    } else {
        key_word = cmd.argv[2];
    }

    for (i = 0; i < BUCKETS_SIZE; ++i) {
        cur_node = data->hash_list[i]->next;
        while (cur_node != data->hash_list[i]) {
            if (strcmp(cur_node->book->id, key_word) == 0) {
                index = cur_node->index;
                cur_node->previous->next = cur_node->next;
                cur_node->next->previous = cur_node->previous;
                free(cur_node->book);
                free(cur_node);
                break;
            }
            cur_node = cur_node->next;
        }
        if (index >= 0) break;
    }
    if (i == BUCKETS_SIZE) {
        printf("No such id.\n");
    } else {
        fp = fopen(data->filename, "rb+");
        if (fp == NULL) {
            printf("ERROR: ");
            perror(NULL);
            return -1;
        }
        fseek(fp, (data->nrecords_in_file - data->ndeletion - 1) * BOOK_SIZE,
              SEEK_SET);
        fread(&tmp, sizeof(Book_t), 1, fp);
        fseek(fp, index * BOOK_SIZE, SEEK_SET);
        fwrite(&tmp, sizeof(Book_t), 1, fp);
        fclose(fp);
        ++data->ndeletion;
    }

    return 0;
}

static int search_book(BookData_t* data) {
    if (cmd.argc <= 1 || (cmd.argc <= 2 && strcmp(cmd.argv[1], SALL) != 0)) {
        printf("Insufficient argument.\n");
        return -3;
    }

    int count = 0;
    int hashcode;
    char* key_word;
    Node_t* cur_node;
    int i, j;

    if (strcmp(cmd.argv[1], SALL) == 0 && cmd.argc == 2) {
        for (i = 0; i < BUCKETS_SIZE; ++i) {
            cur_node = data->hash_list[i]->next;
            while (cur_node != data->hash_list[i]) {
                print_book(cur_node);
                ++count;
                cur_node = cur_node->next;
            }
        }
    } else if (strcmp(cmd.argv[1], SALL) == 0) {
        key_word = cmd.argv[2];
        for (i = 0; i < BUCKETS_SIZE; ++i) {
            cur_node = data->hash_list[i]->next;
            while (cur_node != data->hash_list[i]) {
                int match = 1;
                if (strstr(cur_node->book->name, key_word)) {
                    goto done;
                }
                if (strstr(cur_node->book->press, key_word)) {
                    goto done;
                }
                for (j = 0; j < MAX_AUTHOR; ++j) {
                    if (strstr(cur_node->book->authors[j], key_word)) {
                        goto done;
                    }
                }
                for (j = 0; j < MAX_KEY; ++j) {
                    if (strstr(cur_node->book->keys[j], key_word)) {
                        goto done;
                    }
                }
                match = 0;
            done:
                if (match) {
                    ++count;
                    print_book(cur_node);
                }
                cur_node = cur_node->next;
            }
        }
    } else if (strcmp(cmd.argv[1], SNAME) == 0) {
        /* Exact match between keyword and book name */
        int cmp_result;
        key_word = cmd.argv[2];
        hashcode = hash(key_word);
        cur_node = data->hash_list[hashcode]->next;
        while (cur_node != data->hash_list[hashcode]) {
            cmp_result = strcmp(cur_node->book->name, key_word);
            if (cmp_result == 0) {
                ++count;
                print_book(cur_node);
            } else if (cmp_result > 0) {
                break;
            }
            cur_node = cur_node->next;
        }
    } else if (strcmp(cmd.argv[1], SAUTHOR) == 0) {
        key_word = cmd.argv[2];
        for (i = 0; i < BUCKETS_SIZE; ++i) {
            cur_node = data->hash_list[i]->next;
            while (cur_node != data->hash_list[i]) {
                for (j = 0; j < MAX_AUTHOR; ++j) {
                    if (strlen(cur_node->book->authors[j]) == 0) {
                        break;
                    }
                    if (strstr(cur_node->book->authors[j], key_word)) {
                        ++count;
                        print_book(cur_node);
                        break;
                    }
                }
                cur_node = cur_node->next;
            }
        }
    } else if (strcmp(cmd.argv[1], SKEY) == 0) {
        key_word = cmd.argv[2];
        for (i = 0; i < BUCKETS_SIZE; ++i) {
            cur_node = data->hash_list[i]->next;
            while (cur_node != data->hash_list[i]) {
                for (j = 0; j < MAX_KEY; ++j) {
                    if (strlen(cur_node->book->keys[j]) == 0) {
                        break;
                    } else if (strstr(cur_node->book->keys[j], key_word)) {
                        ++count;
                        print_book(cur_node);
                        break;
                    }
                }
                cur_node = cur_node->next;
            }
        }
    } else {
        printf("Invalid argument.\n");
        return -3;
    }

    return count;
}

static int get_cmd(void) {
    int i = 0;
    int count = 0;
    int is_arg = 0;
    gets(cmd.str_cmd);

    /* Split the command and store the arguments */
    for (i = 0; cmd.str_cmd[i] != '\0' && i < CMD_LEN; ++i) {
        if (isspace(cmd.str_cmd[i])) {
            if (is_arg) {
                cmd.str_cmd[i] = '\0';
                is_arg = 0;
            }
        } else if (isgraph(cmd.str_cmd[i])) {
            if (!is_arg) {
                cmd.argv[count] = &cmd.str_cmd[i];
                ++count;
                is_arg = 1;
            }
            if (cmd.str_cmd[i] == '"') {
                if (cmd.str_cmd[i + 1] == '"') {
                    /* There is no character inside the quotes */
                    --count;
                    ++i;
                } else {
                    cmd.argv[count - 1] = &cmd.str_cmd[i + 1];
                    while (cmd.str_cmd[++i] != '"' && i < CMD_LEN)
                        ;
                    if (i >= CMD_LEN) {
                        return -4;
                    }
                    cmd.str_cmd[i] = '\0';
                }
            }
        }
    }

    cmd.argc = count;
    return count;
}
