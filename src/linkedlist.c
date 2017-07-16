/*******************************************************************************
 * NAME:	    linkedlist.c
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    The file defining the API functions for the types and
 *		    function prototypes in linkedlist.h, and code meant for the
 *		    testing of those functions.
 *
 * CREATED:	    06/05/17
 * 
 * LAST EDITED:	    06/05/17
 ***/

/*******************************************************************************
 * INCLUDES
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef CONFIG_DEBUG_LIST
#include <time.h>
#endif /* CONFIG_DEBUG_LIST */

#include "linkedlist.h"

/*******************************************************************************
 * LOCAL PROTOTYPES
 ***/

#ifdef CONFIG_DEBUG_LIST
static inline void error_exit(int, char *);
void destroy(void *);
#endif /* CONFIG_DEBUG_LIST */

/*******************************************************************************
 * API FUNCTIONS
 ***/

/*******************************************************************************
 * FUNCTION:        list_init
 *
 * DESCRIPTION:     Initializes a list pointer.
 *
 * ARGUMENTS:       list: (List *) -- the list to be operated on.
 *                  destroy: (void (*)(void *)) -- the user-defined function for
 *                           freeing data contained in the list.
 *
 * RETURN:          void
 *
 * NOTES:           O(1)
 ***/
void list_init(List * list, void (*destroy)(void * data))
{

  list->size = 0;
  list->destroy = destroy;
  list->head = NULL;
  list->tail = NULL;

  return;
}

/*******************************************************************************
 * FUNCTION:        list_insnxt
 *
 * DESCRIPTION:     Inserts a new ListElm after the ListElm specified.
 *
 * ARGUMENTS:       list: (List *) -- the list to be operated on.
 *                  node: (ListElm *) -- the reference ListElm; new ListElm is
 *                        inserted after this node.
 *                  data: (const void *) -- the data to be placed in the new
 *                        ListElm.
 *
 * RETURN:          int -- -1 for failure, 0 for success.
 *
 * NOTES:           O(1)
 ***/
int list_insnxt(List * list, ListElm * node, const void * data)
{
  ListElm * new;
  
  /* Perform null check on allocated memory */
  if ((new = (ListElm *)malloc(sizeof(ListElm))) == NULL)
    return -1;

  new->data = (void *)data;

  /* Handle insertion of element at Head */
  if (node == NULL) {
    if (list_size(list) == 0)
      list->tail = new;

    new->next = list->head;
    list->head = new;
  
    /* Handle insertion somewhere else */
  } else {
    if (list_next(node) == NULL)
      list->tail = new;

    new->next = node->next;
    node->next = new;
  }
  
  list->size++;
  return 0;
}

/*******************************************************************************
 * FUNCTION:        list_remnxt
 *
 * DESCRIPTION:     Removes the element after the ListElm specified.
 *
 * ARGUMENTS:       list: (List *) -- the list to be operated on.
 *                  node: (ListElm *) -- the reference ListElm; old ListElm is
 *                        removed after this node.
 *                  data: (void **) -- location for the data to be placed in
 *                        after removing the node.
 *
 * RETURN:          int -- -1 for failure, 0 for success.
 *
 * NOTES:           O(1)
 ***/
int list_remnxt(List * list, ListElm * node, void ** data)
{
  if (list_istail(list, node) || list_size(list) == 0)
    return -1;
  
  ListElm * old;

  /* Handle deletion at the head */
  if (node == NULL) {
    old = list->head;
    list->head = list->head->next;
    *data = old->data;

    /* Handle deletion somewhere else in the list */
  } else {
    if (list_istail(list, list_next(node)))
      list->tail = node;

    old = node->next;
    node->next = node->next->next;
    *data = old->data;
  }

  list->size--;
  free(old);
  return 0;
}

/*******************************************************************************
 * FUNCTION:        list_traverse
 *
 * DESCRIPTION:     Traverses the list from end to end and invokes func().
 *
 * ARGUMENTS:       list: (List *) -- the list to be operated on.
 *                  func: (void (*)(void *)) -- the function to call on
 *                        each data point.
 *
 * RETURN:          void
 *
 * NOTES:           O(1)
 ***/
void list_traverse(List * list, void (*func)(void *))
{
  ListElm * elm;
  
  for (elm = list_head(list); elm != NULL; elm = list_next(elm)) {
    func(elm->data);
  }
}

/*******************************************************************************
 * FUNCTION:        list_dest
 *
 * DESCRIPTION:     Clears the memory inhabited by a list and sets all bytes 0.
 *
 * ARGUMENTS:       list: (List *) -- the list to be operated on.
 *
 * RETURN:          void
 *
 * NOTES:           O(n)
 ***/
int list_dest(List * list)
{
  if (list->destroy == NULL)
    return -1;

  void * data;

  while (list_size(list) > 0) {
    if (list_remnxt(list, NULL, (void **)&data) == 0) {
      list->destroy(data);
    }
  }
  memset(list, 0, sizeof(List));
  return 0;
}


/*******************************************************************************
 * MAIN
 ***/

#ifdef CONFIG_DEBUG_LIST
int main(int argc, char * argv[])
{  
  List * list;
  int * pNum;

  /* Allocate a list, or fail if it cannot be done. */
  if ((list = malloc(sizeof(List))) == NULL)
    error_exit(1, "Could not allocate storage for list!\n");
  
  /* Initialize the list */
  list_init(list, destroy);

  srand((unsigned)time(NULL));

  printf("Tail test:\n");
  /* Add ten random digits to the TAIL of thelist. */
  for (int i = 0; i < 10; i++) {
    pNum = malloc(sizeof(int));
    *pNum = rand() % 20;
    list_insnxt(list, list_tail(list), pNum);
  }

  /* Remove and print each element from the list, starting at HEAD. */
  while (!list_isempty(list)) {
    list_remnxt(list, NULL, (void **)&pNum);
    printf("%d ", *pNum);
    free(pNum);
  }
  
  list_dest(list);
  list_init(list, destroy);

  printf("\nHead test:\n");
  /* Add ten random digits to the HEAD of thelist. */
  for (int i = 0; i < 10; i++) {
    pNum = malloc(sizeof(int));
    *pNum = rand() % 20;
    list_insnxt(list, NULL, pNum);
  }

  /* Remove and print each element from the list, starting at HEAD. */
  while (!list_isempty(list)) {
    list_remnxt(list, NULL, (void **)&pNum);
    printf("%d ", *pNum);
    free(pNum);
  }

  list_dest(list);
  list_init(list, destroy);

  pNum = malloc(sizeof(int));
  *pNum = rand() % 20;
  list_insnxt(list, list_head(list), pNum);

  printf("\nRandom test:\n");
  /* Add ten random digits to the HEAD of thelist. */
  for (int i = 0; i < 9; i++) {
    pNum = malloc(sizeof(int));
    *pNum = rand() % 20;
    list_insnxt(list, list_tail(list), pNum);
  }

  /* Remove and print each element from the list, starting at HEAD. */
  while (!list_isempty(list)) {
    list_remnxt(list, NULL, (void **)&pNum);
    printf("%d ", *pNum);
    free(pNum);
  }

  list_dest(list);

  printf("\n");
  free(list);
  return 0;
}
#endif /* CONFIG_DEBUG_LIST */

/*******************************************************************************
 * LOCAL FUNCTIONS
 ***/

#ifdef CONFIG_DEBUG_LIST
static inline void error_exit(int status, char * msg)
{
  fprintf(stderr, "%s", msg);
  exit(status);
  return;
}

void destroy(void * data)
{
  free(data);
}
#endif

/******************************************************************************/
