/**
 * File: libman.c
 */
#include "libman.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/**************************************
 * Declarations of private functions *
 **************************************/
static int hash(char* name);
static int set_book(Book_t* book);
static int append_linkedlist(BookData_t* data, Book_t* book, int i);
static int print_book(Node_t* node);

/*********************
 * Public functions *
 *********************/

int libman_interactive(BookData_t* data) {
    printf(
        "+--------------------+\n"
        "| LibMan Interactive |\n"
        "+--------------------+\n");
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
        } else if (strcmp(cmd.str_cmd, "help") == 0) {
            printf(
                "These commands are defined internally:\n"
                "\tadd: Add new books to the database\n"
                "\tdel: Delete books from the database\n"
                "\t\t" DID
                "\n"
                "\t\t" DALL
                "\n"
                "\thelp: Print the help doc\n"
                "\tmodify: Modify books in the database\n"
                "\t\t" MID
                "\n"
                "\tsearch: Search books in the database\n"
                "\t\t" SALL
                "\n"
                "\t\t" SAUTHOR
                "\n"
                "\t\t" SKEY
                "\n"
                "\t\t" SNAME "\n");
        } else if (strcmp(cmd.str_cmd, "modify") == 0) {
            modify_book(data);
        } else if (strcmp(cmd.str_cmd, "search") == 0) {
            i = search_book(data);
            if (i >= 0) {
                printf("%d result(s) found.\n", i);
            }
        } else {
            printf("Invalid command.\n");
        }
    }
}

int get_cmd(void) {
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
                        return -1;
                    }
                    cmd.str_cmd[i] = '\0';
                }
            }
        }
    }

    cmd.argc = count;
    return count;
}

BookData_t* initialize(char* filename) {
    printf("LibMan 1.0 | %s, %s\n", __DATE__, __TIME__);
    BookData_t* data = (BookData_t*)malloc(sizeof(BookData_t));
    FILE* fp = fopen(filename, "rb");
    Book_t* tmp;
    Node_t* tmp_node;

    int i = 0;
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
        printf("A new file has been created to save the books.\n");
    } else {
        while (1) {
            tmp = (Book_t*)malloc(sizeof(Book_t));
            fread(tmp, sizeof(Book_t), 1, fp);
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
    printf("%d book(s) initialized.\n", data->nrecords_in_file);
    return data;
}

int add_book(BookData_t* data) {
    FILE* fp = fopen(data->filename, "rb+");
    Book_t* tmp;
    int hashcode;
    int count = 0;
    int i = 0;

    while (1) {
        tmp = (Book_t*)malloc(sizeof(Book_t));
        if (set_book(tmp) == 1) {
            printf("Adding terminated: %d %s added.\n", count,
                   count > 1 ? "books" : "book");
            break;
        }

        if (data->ndeletion > 0) {
            /* If some deletion has been made, rewrite the trash record */
            fseek(fp, -data->ndeletion * (long)sizeof(Book_t), SEEK_END);
            --data->ndeletion;
        } else {
            /* Once a book is added, save it to the end of the file */
            fseek(fp, 0, SEEK_END);
            ++data->nrecords_in_file;
        }
        fwrite(tmp, sizeof(Book_t), 1, fp);
        append_linkedlist(data, tmp, ftell(fp) / sizeof(Book_t) - 1);
        ++count;
    }
    free(tmp);

    fclose(fp);
    return count;
}

int modify_book(BookData_t* data) {
    Node_t* node;
    char* key_id;
    int i, flag;
    FILE* fp = NULL;

    if (cmd.argc <= 1) {
        printf("Insufficient argument.\n");
        return -1;
    } else if (strcmp(cmd.argv[1], MID) != 0) {
        printf("Invalid argument.\n");
        return -1;
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
                fp = fopen(data->filename, "rb+");
                fseek(fp, sizeof(Book_t) * node->index, SEEK_SET);
                fwrite(node->book, sizeof(Book_t), 1, fp);
                fclose(fp);
                flag = 1;
                break;
            }
            node = node->next;
        }
        if (flag == 1) break;
    }

    if (flag == 0) {
        printf("No such id.\n");
        return 1;
    } else {
        return 0;
    }
}

int delete_book(BookData_t* data) {
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
        return -1;
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
        return -1;
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
    }

    fp = fopen(data->filename, "rb+");
    fseek(fp, (data->nrecords_in_file - data->ndeletion - 1) * sizeof(Book_t),
          SEEK_SET);
    fread(&tmp, sizeof(Book_t), 1, fp);
    fseek(fp, index * sizeof(Book_t), SEEK_SET);
    fwrite(&tmp, sizeof(Book_t), 1, fp);
    fclose(fp);
    ++data->ndeletion;
    return 0;
}

int search_book(BookData_t* data) {
    if (cmd.argc <= 1 || (cmd.argc <= 2 && strcmp(cmd.argv[1], SALL) != 0)) {
        printf("Insufficient argument.\n");
        return -1;
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
        return -1;
    }

    return count;
}

int free_memory(BookData_t* data) {
    /* Remember to free the memory allocated in append_linkedlist */
    Node_t* tmp;
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
        FILE* ftmp = fopen(tmp_name, "wb");
        for (i = 0; i < data->nrecords_in_file - data->ndeletion; ++i) {
            fread(&tmp, sizeof(Book_t), 1, fp);
            fwrite(&tmp, sizeof(Book_t), 1, ftmp);
        }
        fclose(fp);
        fclose(ftmp);
        remove(data->filename);
        rename(tmp_name, data->filename);
    }

    return 0;
}

/*************************************
 * Definitions of private functions *
 *************************************/
int hash(char* name) {
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

int set_book(Book_t* book) {
    int i = 0;
    char ch;

    printf("Enter the name of the book: ");
    gets(book->name);
    if (strlen(book->name) == 0) return 1;

    printf("Enter the id of the book: ");
    gets(book->id);

    printf("Enter the authors of the book (not exceeding %d authors):\n",
           MAX_AUTHOR);
    for (i = 0; i < MAX_AUTHOR; ++i) {
        gets(book->authors[i]);
        if (strlen(book->authors[i]) == 0) break;
    }

    printf("Enter the keys of the book (not exceeding %d keys):\n", MAX_KEY);
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

int append_linkedlist(BookData_t* data, Book_t* book, int i) {
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

int print_book(Node_t* node) {
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
