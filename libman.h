/**
 * File: libman.h
*/
#ifndef _LIBMAN_H
#define _LIBMAN_H

#include "aes256.h"

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
#define EALL "-all"
#define ARG "<arg>"
#define ARG_OPTIONAL "[<arg>]"

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

/* A Book_t in file takes 512B */
#define BOOK_SIZE 512

/* Max length of password */
#define PASSWORD_LEN 20

/**
 * Anonymous struct: cmd
 * ---------------------
 * Desciption:
 *  Used for saving command inputted from keyboard
 * and saving arguments splited from the command
*/
struct
{
    char str_cmd[CMD_LEN];
    int argc;
    char *argv[MAX_ARGS];
} cmd;

/**
 * Struct: Book_t
 * --------------
 * Description:
 *  Used for store the info about the book
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
 * Struct: Node_t
 * --------------
 * Description:
 *  Used only in linked list.
 *  Each node is associated with a book and it stores the
 * previous node and next node. And index is the sequence 
 * number of the book in data file.
*/
typedef struct Node
{
    int index;
    Book_t *book;
    struct Node *previous;
    struct Node *next;
} Node_t;

/**
 * Struct: BookData_t
 * ------------------
 * Description:
 *  Used as the temporary database in internal memory
 *  It stores the name of the data file in hard disk,
 * the hash list table which stores the nodes defined
 * before, the number of deletions and records in file
 * which are used to sync the data file before quiting
 * the programme. And aes is used in encryption and 
 * decryption of the data file
*/
typedef struct BookData {
    char filename[FNAME_LEN];
    Node_t* hash_list[BUCKETS_SIZE];
    int nrecords_in_file;
    int ndeletion;
    aes256_context_t* aes;
} BookData_t;


/**
 * Function: initialize
 * --------------------
 * Description:
 *  The function initialize the dictionary and data
 *  from the file indicated by filename
 * Return value:
 *  It returns the pointer of the type BookData_t*
 *  It returns NULL if it failed to create the pointer
*/
BookData_t* initialize(char* filename);

/**
 * Fucntion: libman_interactive
 * -----------------
 * Description:
 *  Interact with stdio
 * Return value:
 *  It returns 0 when user decided to exit
*/
int libman_interactive(BookData_t* data);

/**
 * Function: free_memory
 * ---------------------
 * Description:
 *  The function free the memory allocated for linked list and
 * rewrite the file if some deletion has been made
 * Return value:
 *  It returns 0 if everything is all fine
 * 	It returns -1 if there is something wrong with the file
 *  It returns -2 if there is something wrong with the file, but it can
 * be fixed manually.
*/
int free_memory(BookData_t* data);

#endif
