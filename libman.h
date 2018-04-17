/**
 * File: libman.h
*/
#ifndef _LIBMAN_H
#define _LIBMAN_H

/* Definitions for commands */
#define MAX_ARGS 5
#define CMD_LEN 50
#define SALL "-all"
#define SNAME "-name"
#define SAUTHOR "-author"
#define SKEY "-key"
#define MID "-id"
#define DID "-id"
#define DALL "-all"

/* Some definitions for struct Book */
#define NAME_LEN 40
#define MAX_KEY 5
#define KEY_LEN 20
#define MAX_AUTHOR 5
#define AUTHOR_LEN 20
#define PRESS_LEN 20
#define ID_LEN 10
#define DATE_LEN 10
/**
 * Definitions for hash_list
 * -------------------------
 *  0 - 25 are for characters, 26 is for digits and 27 is for others.
*/
#define BUCKETS_SIZE 28
#define FNAME_LEN 50

/**
 * 
*/
struct
{
    char str_cmd[CMD_LEN];
    int argc;
    char *argv[MAX_ARGS];
} cmd;

/**
 * 
*/
typedef struct Book
{
    char id[ID_LEN];
    char name[NAME_LEN];
    char authors[MAX_AUTHOR][AUTHOR_LEN];
    char keys[MAX_KEY][KEY_LEN];
    char press[PRESS_LEN];
    char publish_date[DATE_LEN];
} Book_t;

/**
 * 
*/
typedef struct Node
{
    int index;
    Book_t *book;
    struct Node *previous;
    struct Node *next;
} Node_t;

/**
 * 
*/
typedef struct BookData {
    char filename[FNAME_LEN];
    Node_t* hash_list[BUCKETS_SIZE];
    int nrecords_in_file;
    int ndeletion;
} BookData_t;


/**
 * Fucntion: get_cmd
 * -----------------
 * Description:
 *  The function get a string from keyboard and resolve it
 * into several arguments
 * Return value:
 *  It returns the number of arguments it received
*/
int get_cmd(void);

/**
 * Function: initialize
 * --------------------
 * Description:
 *  The function initialize the dictionary and linked
 * list from the file indicated by filename
 * Return value:
 *  It returns the number of books in the database
*/
BookData_t initialize(char* filename);

/**
 * Function: add_book
 * ------------------
 * Description:
 *  The function adds new books into the linked list and
 * save them to the file indicated by filename
 * Return value:
 *  It returns the number of books that has been added.
*/
int add_book(BookData_t* data);

/**
 * Function: modify_book
 * ---------------------
 * Description:
 *  The function modifies a certain book indicated in the
 * command save the changes to the file indicated by filename
 * Return value:
 *  It returns -1 if there are something wrong in the command
 *  It returns 0 if everything is all fine
 *  It returns 1 if the target book doesn't exist
*/
int modify_book(BookData_t* data);

/**
 * Function: delete_book
*/
int delete_book(BookData_t* data);

/**
 * Function: search_book
 * ---------------------
 * Description:
 *  The function search the linked list with key word indicated
 * in the command and print the result in the screen
 * Return value:
 *  It returns 0 if everything is all fine
 *  It returns -1 if there are something wrong in the command
*/
int search_book(BookData_t* data);

/**
 * Function: free_memory
 * ---------------------
 * Description:
 *  The function free the memory allocated for linked list and
 * rewrite the file if some deletion has been made
 * Return value:
 *  It returns 0 if everything is all fine
*/
int free_memory(BookData_t* data);

/**
 * Function: import_book
*/
int import_book(BookData_t* data, char* src_name);

/**
 * Function: export_book
*/
int export_book(BookData_t* data);

#endif