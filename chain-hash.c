/*******************************************************************************
 * NAME:	    chain-hash.c
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    Source code for the implementation of a chained hash-table.
 *
 * CREATED:	    07/15/2017
 *
 * LAST EDITED:	    02/12/2018
 ***/

/**
 * \brief Source code for the Chained-Hash table
 * 
 * This file contains the API functions and source code for testing.
 */

/*******************************************************************************
 * INCLUDES
 ***/

#ifdef CONFIG_DEBUG_CHAIN_HASH
#include <stdio.h>
#endif

#include <stdlib.h>
#include <time.h>

#include "chain-hash.h"
#include "linkedlist.h"

/*******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 ***/

#ifdef CONFIG_DEBUG_CHAIN_HASH
static _Noreturn void error_exit(char * msg);
static int hash_func(const void *);
static int match_func(const void *, const void *);
#endif

/*******************************************************************************
 * API FUNCTIONS
 ***/

/*******************************************************************************
 * FUNCTION:	    chash_init
 *
 * DESCRIPTION:	    Initializes and returns a CHash struct with the parameters.
 *
 * ARGUMENTS:	    size: (int) -- the size of the array. The size will remain
 *			constant if not compiled with CONFIG_DYNAMIC_STRUCTURES.
 *		    hash: (int (*)(const void *)) -- user-defined hash function.
 *		    match: (int (*)(const void *, const void *) -- user defined
 *			function to compare two keys.
 *		    destroy: (void (*)(void *)) -- user-defined function to free
 *			allocated memory.
 *
 * RETURN:	    Reference to allocated CHash structure.
 *
 * NOTES:	    Unlike init() for many of my other APIs, this does not take
 *		    an already allocated struct as a param.
 ***/
CHash * chash_init(int size,
		   int (*hash)(const void *),
		   int (*match)(const void *, const void *),
		   void (*destroy)(void *))
{
  CHash * tbl = malloc(sizeof(CHash));

  if (tbl == NULL)
    return NULL;

  *tbl = (CHash){.buckets = size,
		 .hash = hash,
		 .match = match,
		 .destroy = destroy,
		 .size = 0,
		 .table = calloc(size - 1, sizeof(list *))
  };

  for (int i = 0; i < size; i++) {
    if ((tbl->table[i] = list_create(tbl->destroy)) == NULL)
      return NULL; /* TODO: chash_init - Fix in case of mem leak? */
  }

  return tbl;
}

/*******************************************************************************
 * FUNCTION:	    chash_insert
 *
 * DESCRIPTION:	    Inserts the data pointed to by data into the table, if it
 *		    is not already contained in the file.
 *
 * ARGUMENTS:	    tbl: (CHash *) -- the table to insert into.
 *		    data: (const void *) -- the data to insert into the table.
 *
 * RETURN:	    int -- 0 on success, 1 if the table already contains the
 *		    data, -1 if there is an error..
 *
 * NOTES:	    
 ***/
int chash_insert(CHash * tbl, const void * data)
{
  if (data == NULL)
    return -1;

  void * tmp = (void *)data;
  if (chash_lookup(tbl, &tmp))
    return 1;

  int bucket = tbl->hash(data) % tbl->buckets;
  
  if (tbl->table[bucket] == NULL)
    return -1;

  list * tmplist = tbl->table[bucket];
  if (!list_insnxt(tmplist, list_tail(tmplist), data))
    tbl->size++;

  return 0;
}

/*******************************************************************************
 * FUNCTION:	    chash_remove
 *
 * DESCRIPTION:	    Removes the data element specified by *data from the Hash
 *		    table, if it exists.
 *
 * ARGUMENTS:	    tbl: (CHash *) -- the hash table in question.
 *		    data: (void **) -- the data in question.
 *
 * RETURN:	    int -- 0 on success, -1 otherwise.
 *
 * NOTES:	    
 ***/
int chash_remove(CHash * tbl, void ** data)
{ 
  if (tbl->size == 0)
    return -1; /* Do not allow removal from an empty list. */

  if (*data != NULL) {
    int bucket = tbl->hash(*data) % tbl->buckets;

    listelmt * prev = NULL;
    for (listelmt * elmt = list_head(tbl->table[bucket]);
	   elmt != NULL; elmt = list_next(elmt)) {
      if (tbl->match(*data, list_data(elmt))) {

	list_remnxt(tbl->table[bucket], prev, data);
	tbl->size--;
	return 0;

      }
      prev = elmt;
    }
  } else {
    int i;
    for (i = 0; i < tbl->buckets; i++)
      if (list_size(tbl->table[i]) > 0)
	break;

    list_remnxt(tbl->table[i], NULL, data);
    tbl->size--;
    return 0;
  }
  
  /* If we got to this point, obviously something went wrong. */
  return -1;
}

/*******************************************************************************
 * FUNCTION:	    chash_lookup
 *
 * DESCRIPTION:	    Queries the hash table to determine if it already contains
 *		    the data pointed to by *data. Returns 0 or 1 depending on
 *		    the result.
 *
 * ARGUMENTS:	    tbl: (CHash *) -- the table in question
 *		    data: (void **) -- the data in question
 *
 * RETURN:	    0 if the table does not contain the data, 1 if it does.
 *
 * NOTES:	    none.
 ***/
int chash_lookup(CHash * tbl, void ** data)
{
  if (*data != NULL) {
    int bucket = tbl->hash(*data) % tbl->buckets;

    for (listelmt * elmt = list_head(tbl->table[bucket]);
	 elmt != NULL; elmt = list_next(elmt)) {
      if (tbl->match(*data, list_data(elmt))) {
	*data = list_data(elmt);
	return 1;
      }
    }
  } else {
    *data = list_data(list_head(tbl->table[0]));
    return 1;
  }
  return 0;
}

/*******************************************************************************
 * FUNCTION:	    chash_traverse
 *
 * DESCRIPTION:	    Traverses the hash table and calls callback() on every
 *		    element.
 *
 * ARGUMENTS:	    tbl: (CHash *) -- the hash table in question.
 *		    callback: (void (*)(const void *)) -- the callback function.
 *
 * RETURN:	    void.
 *
 * NOTES:	    none.
 ***/
void chash_traverse(CHash * tbl, void (*callback)(void *))
{
  for (int i = 0; i < tbl->buckets; i++) {
    list_traverse(tbl->table[i], callback);
  }
}

/*******************************************************************************
 * FUNCTION:	    chash_destroy
 *
 * DESCRIPTION:	    Removes all of the data from a chained hash table and frees
 *		    the memory associated with the table. If tbl->destroy is
 *		    set to NULL, does not free the memory associated with the
 *		    table.
 *
 * ARGUMENTS:	    tbl: (CHash *) -- the chained hash table in question.
 *
 * RETURN:	    void.
 *
 * NOTES:	    none.
 ***/
void chash_destroy(CHash * tbl)
{
  for (int i = 0; i < tbl->buckets; i++)
    list_destroy(&tbl->table[i]);

  free(tbl);
}

/*******************************************************************************
 * MAIN
 ***/

#ifdef CONFIG_DEBUG_CHAIN_HASH
int main(int argc, char * argv[]) {

  CHash * hash = chash_init(10, hash_func, match_func, NULL);
  if (hash == NULL)
    error_exit("There was a problem in chash_init");

  int * pInt;

  srand((unsigned)time(NULL));
  printf("===== INSERTING =====\n");
  for (int i = 0; i < 10; i++) {
    pInt = malloc(sizeof(int));
    *pInt = rand() % 20;
    printf("int %d @ %p\n", *pInt, pInt);
    if(chash_insert(hash, (void *)pInt) < 0)
      error_exit("There was an error inserting pInt!");
  }

  printf("===== REMOVING ======\n");
  while (!chash_isempty(hash)) {
    pInt = NULL;
    if (chash_remove(hash, (void **)&pInt) < 0)
      error_exit("There was an issue in chash_remove!");
    printf("int %d @ %p\n", *pInt, pInt);
  }

  chash_destroy(hash);

}
#endif

/*******************************************************************************
 * STATIC FUNCTIONS
 ***/

#ifdef CONFIG_DEBUG_CHAIN_HASH
/*******************************************************************************
 * FUNCTION:	    error_exit
 *
 * DESCRIPTION:	    The exit function. Called if there is an error. Prints msg
 *		    to STDERR and gtfo.
 *
 * ARGUMENTS:	    msg: (char *) -- the error string to print to STDERR.
 *
 * RETURN:	    Abandon all hope, ye who declare a function with _Noreturn.
 *
 * NOTES:	    none.
 ***/
static _Noreturn void error_exit(char * msg)
{
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

/*******************************************************************************
 * FUNCTION:	    hash_func
 *
 * DESCRIPTION:	    Static function for hashing the data in the test hashtable.
 *
 * ARGUMENTS:	    data: (const void *) -- the data to put into the table.
 *
 * RETURN:	    int -- the hash key for referring to the data in the table.
 *
 * NOTES:	    debug function -- not compiled in finished executable unless
 *		    CONFIG_DEBUG_CHAIN_HASH is defined.
 ***/
static int hash_func(const void * data)
{
  return *(int *)data;
}

/*******************************************************************************
 * FUNCTION:	    match_func
 *
 * DESCRIPTION:	    Static function for comparing two data keys. Used for the
 *		    debug main method.
 *
 * ARGUMENTS:	    data1: (const void *) -- the first key.
 *		    data2: (const void *) -- the second key.
 *
 * RETURN:	    int -- 1 if the two are the same, 0 otherwise.
 *
 * NOTES:	    debug function -- not compiled in finished executable unless
 *		    CONFIG_DEBUG_CHAIN_HASH is defined.
 ***/
static int match_func(const void * data1, const void * data2)
{
  return *(int *)data1 == *(int *)data2;
}
#endif

/******************************************************************************/
