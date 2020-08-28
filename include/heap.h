#ifndef HEAPINCLUDE
#define HEAPINCLUDE 1

/* 
array-based implementation of heap with 

insert: O(log n)
delete: O(log n)
getmin: O(1)
popmin: O(log n)
 */

#define HEAPINITSIZE 128
#define HEAPLEFT(I) (I * 2 + 1)
#define HEAPRIGHT(I) (I * 2 + 2)
#define HEAPPARENT(I) (I == 0 ? 0 : (I - 1) / 2)

typedef int (*HEAPLT)(void*, void*);
typedef struct heap {
        void **array;
        unsigned int cap;
        unsigned int size;
        HEAPLT lessthan;
} HEAP;

/* construct empty heap with order specified by given function */
HEAP *makeheap(HEAPLT);
/* free a heap (but not the memory allocated for its elements) */
void freeheap(HEAP*);
/* get the minimum element of the heap */
void *heappeek(HEAP*);
/* insert an element into the heap */
void heapinsert(HEAP*, void*);
/* remove the element at given position from the heap */
void *heapdelete(HEAP*, unsigned int);
/* remove the minimum element from the heap */
void *heappop(HEAP*);


#endif
