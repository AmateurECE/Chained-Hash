/******************************************************************************
 * NAME:	    chain-hash.h
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    The header file containing the interface for the
 *		    implementation of the chained-hash table included in this
 *		    repository.
 *
 * CREATED:	    07/15/2017
 *
 * LAST EDITED:	    03/14/2018
 ***/

/**
 * \file chain-hash.h
 * \author Ethan D. Twardy
 * \date 07/15/2017
 * \brief Details public interface of the CHash API.
 *
 * This file contains everything a user needs to know to use the CHash API.
 * Functions and their usage, macros, types included, etc.
 */

#ifndef __ET_CHAIN_HASH_H__
#define __ET_CHAIN_HASH_H__

/******************************************************************************
 * INCLUDES
 ***/

#include "list.h"

/******************************************************************************
 * MACRO DEFINITIONS
 ***/

/**
 * \brief Returns the size of the Hash.
 */
#define chash_size(Table) ((Table)->size)

/**
 * \brief Returns true if the hash is empty, false if it is not.
 */
#define chash_isempty(Table) ((Table)->size == 0 ? 1 : 0)

/******************************************************************************
 * TYPE DEFINITIONS
 ***/

/**
 * \brief The CHash struct definition
 *
 * The CHash structure is the main type provided in this API. This structure
 * represents the Chained Hash table and all data held within it.
 *
 * \warning The user should interface directly with the struct elements as
 * sparingly as possible.
 */
typedef struct _CHash_ {

  unsigned int size;
  unsigned int buckets;
  int (*hash)(const void *);
  int (*match)(const void *, const void *);
  void (*destroy)(void *);
  list ** table;

} CHash;

/******************************************************************************
 * API FUNCTION PROTOTYPES
 ***/

/**
 * \brief The initialization funtion.
 * \param size The number of containers to create in the hash.
 * \param hash The user-defined hash function.
 * \param match The user-defined function for comparing two data points.
 * \param destroy The user-defined function for freeing data points.
 * \return CHash* Pointer to a created and initialized CHash struct.
 */
extern CHash * chash_init(int size,
			  int (*hash)(const void *),
			  int (*match)(const void *, const void *),
			  void (*destroy)(void *)
			  );

/**
 * \brief Function to free all data associated with the hash.
 * \param table The table to destroy.
 * \return void
 * \warning If \codetable->destroy\endcode is set to \c NULL, the data will not
 * be freed, and it is the responsibility of the programmer to manage this mem.
 */
extern void chash_destroy(CHash * table);

/**
 * \brief Inserts the value specified by \c data into the hash.
 * \param table The table to insert the value into.
 * \param data The data to insert into the table.
 * \return int \c 0 on success, \c 1 if the data already exists within the 
 * table, and \c -1 if there was an error.
 */
extern int chash_insert(CHash * table, const void * data);

/**
 * \brief Removes the element specified by data from the table
 * \param table The table to operate on
 * \param data Pointer to the data to remove. If set to \c NULL, removes the 
 * first element in the hash.
 * \return int \c 0 on success, \c -1 if there was an error.
 */
extern int chash_remove(CHash * table, void ** data);

/**
 * \brief Invokes \c callback on every element in the hash
 * \param table The hash table to traverse
 * \param callback The callback function to invoke on every element.
 * \return void
 */
extern void chash_traverse(CHash * table, void (*callback)(void *));

/**
 * \brief Queries the hash for a specific data point
 * \param table The table to search
 * \param data The data to search for
 * \return int \c 1 if the data was found, \c 0 if it was not. 
 * If the data was found, data now contains the address of that data.
 */
extern int chash_lookup(CHash * table, void ** data);

#endif /* __ET_CHAIN_HASH_H__ */

/*****************************************************************************/
