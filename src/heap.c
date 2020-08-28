#include <stdlib.h>

#include "heap.h"


HEAP *makeheap(HEAPLT lessthan)
{
        HEAP *heap = malloc(sizeof(HEAP));
        heap->array = malloc(sizeof(void*) * HEAPINITSIZE);
        heap->cap = HEAPINITSIZE;
        heap->size = 0;
        heap->lessthan = lessthan;
        return heap;
}


void freeheap(HEAP *heap)
{
        free(heap->array);
        free(heap);
}


/* double the size of the underlying array for a heap (free the old one) */
static void expand(HEAP *heap)
{
        void **newarray = malloc(sizeof(void*) * heap->cap * 2);
        unsigned int i;
        for (i = 0; i < heap->size; i++)
                newarray[i] = heap->array[i];

        free(heap->array);
        heap->array = newarray;
        heap->cap *= 2;
}


void *heappeek(HEAP *heap)
{
        void *min = NULL;
        if (heap->size > 0)
                min = heap->array[0];
        return min;
}


/* swap the elements in the heap's array at positions a and b  */
static void swap(HEAP *heap, unsigned int a, unsigned int b)
{
        void *tmp = heap->array[a];
        heap->array[a] = heap->array[b];
        heap->array[b] = tmp;
}


/* bubble up the heap from position a in the array */
static void bubbleup(HEAP *heap, unsigned int a)
{
        HEAPLT lt = heap->lessthan;
        while (a > 0 && lt(heap->array[a], heap->array[HEAPPARENT(a)])) {
                swap(heap, a, HEAPPARENT(a));
                a = HEAPPARENT(a);
        }
}


/* bubble down the heap from position a in the array */
static void bubbledown(HEAP *heap, unsigned int a)
{
        HEAPLT lt = heap->lessthan;

        while (HEAPLEFT(a) < heap->size) {
                unsigned int b = HEAPLEFT(a);
                if (HEAPRIGHT(a) < heap->size && lt(heap->array[HEAPRIGHT(a)], heap->array[HEAPLEFT(a)]))
                        b = HEAPRIGHT(a);

                if (lt(heap->array[b], heap->array[a])) {
                        swap(heap, a, b);
                        a = b;
                } else {
                        return;
                }
        }
}

void heapinsert(HEAP *heap, void *element)
{
        if (heap->size >= heap->cap)
                expand(heap);

        heap->array[heap->size] = element;
        heap->size += 1;
        bubbleup(heap, heap->size - 1);
}


void *heapdelete(HEAP *heap, unsigned int a)
{
        if (a > heap->size - 1 || heap->size == 0)
                return NULL;

        swap(heap, a, heap->size - 1);
        heap->size -= 1;
        void *victim = heap->array[heap->size];
        heap->array[heap->size] = NULL;

        if (heap->size > 1 && a < heap->size) {
                HEAPLT lt = heap->lessthan;
                if (lt(heap->array[a], heap->array[HEAPPARENT(a)]))
                        bubbleup(heap, a);
                else
                        bubbledown(heap, a);
        }
        
        return victim;
}


void *heappop(HEAP *heap)
{
        return heapdelete(heap, 0);
}
