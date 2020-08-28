#include <stdlib.h>
#include <check.h>
#include <stdio.h>
#include <time.h>

#include "heap.h"


int intlessthan(void *a, void *b)
{
        return *((int*) a) < *((int*) b);
}


int checkheaporder(HEAP *heap)
{
        HEAPLT lt = heap->lessthan;
        int i;
        for (i = 0; i < heap->size; i++) {
                if (HEAPLEFT(i) < heap->size)
                        if (lt(heap->array[HEAPLEFT(i)], heap->array[i]))
                                return 0;

                if (HEAPRIGHT(i) < heap->size)
                        if (lt(heap->array[HEAPRIGHT(i)], heap->array[i]))
                                return 0;
        }
        return 1;
}


/* simply check nothing goes wrong making and freeing the heap */
START_TEST(check_make_then_free)
{
        HEAP *empty = makeheap(&intlessthan);
        ck_assert_int_eq(empty->size, 0);
        ck_assert_int_eq(empty->cap, HEAPINITSIZE);
        freeheap(empty);
}
END_TEST


/* check that inserting into the heap increases its size */
START_TEST(check_insert_increases_size)
{
        HEAP *heap = makeheap(&intlessthan);
        int i;

        for (i = 1; i < 65; i++) {
                int *element = malloc(sizeof(int));
                *element = i;
                heapinsert(heap, element);
                ck_assert_int_eq(heap->size, i);
        }

        for (i = 0; i < 64; i++)
                free(heap->array[i]);
        freeheap(heap);
}
END_TEST


/* check that inserting into the heap increases its size, even beyond its init size */
START_TEST(check_insert_increases_size_with_expansion)
{
        HEAP *heap = makeheap(&intlessthan);
        int i;

        for (i = 1; i < 1025; i++) {
                int *element = malloc(sizeof(int));
                *element = i;
                heapinsert(heap, element);
                ck_assert_int_eq(heap->size, i);
        }

        for (i = 0; i < 1024; i++)
                free(heap->array[i]);
        freeheap(heap);
}
END_TEST


/* check that inserting maintains the heap order */
START_TEST(check_insert_maintains_heaporder)
{
        HEAP *heap = makeheap(&intlessthan);
        int i;
        srand(time(NULL));
        for (i = 0; i < (1 << 16); i++) {
                int *element = malloc(sizeof(int));
                *element = rand();
                heapinsert(heap, element);
        }

        ck_assert(checkheaporder(heap));
        
        for (i = 0; i < (1 << 16); i++)
                free(heap->array[i]);
        freeheap(heap);
}
END_TEST


/* check that deletion decreases the heap size */
START_TEST(check_delete_decreases_size)
{
        HEAP *heap = makeheap(&intlessthan);
        int i;

        for (i = 0; i < 1024; i++) {
                int *element = malloc(sizeof(int));
                *element = i;
                heapinsert(heap, element);
                ck_assert_int_eq(heap->size, i + 1);
        }

        for (i = 1023; i >= 0; i--) {
                void *element = heapdelete(heap, i);
                ck_assert_int_eq(heap->size, i);
                if (element)
                        free(element);
                ck_assert(checkheaporder(heap));
        }

        freeheap(heap);
}
END_TEST


/* check that deleting first element maintains heaporder and decreases size */
START_TEST(check_delete_min_decreases_size)
{
        HEAP *heap = makeheap(&intlessthan);
        int i;

        for (i = 0; i < (1 << 14); i++) {
                int *element = malloc(sizeof(int));
                *element = i;
                heapinsert(heap, element);
                ck_assert_int_eq(heap->size, i + 1);
        }

        for (i = heap->size; i > 0; i--) {
                void *element = heapdelete(heap, 0);
                ck_assert_int_eq(heap->size, i - 1);
                ck_assert(checkheaporder(heap));
                if (element)
                        free(element);
        }

        freeheap(heap);
}
END_TEST


/* check that random deletion preserves the heaporder */
START_TEST(check_delete_maintains_heaporder)
{
        HEAP *heap = makeheap(&intlessthan);
        int i;
        srand(time(NULL));

        for (i = 0; i < 64; i++) {
                int *element = malloc(sizeof(int));
                *element = rand() % 64;
                heapinsert(heap, element);
                ck_assert_int_eq(heap->size, i + 1);
        }

        for (i = 0; i < 64; i++) {
                unsigned int victim = rand() % heap->size;
                void *element = heapdelete(heap, victim);
                ck_assert(checkheaporder(heap));
                free(element);
        }

        freeheap(heap);
}
END_TEST


/* check that parent macro works as expected */
START_TEST(check_heapparent)
{
        ck_assert_int_eq(HEAPPARENT((unsigned int) 0), 0);
        ck_assert_int_eq(HEAPPARENT((unsigned int) 1), 0);
        ck_assert_int_eq(HEAPPARENT((unsigned int) 2), 0);
        ck_assert_int_eq(HEAPPARENT(0), 0);
        ck_assert_int_eq(HEAPPARENT(1), 0);
        ck_assert_int_eq(HEAPPARENT(2), 0);
}
END_TEST


Suite *heap_suite()
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("heap");
	tc_core = tcase_create("core");

        tcase_add_test(tc_core, check_heapparent);
	tcase_add_test(tc_core, check_make_then_free);
        tcase_add_test(tc_core, check_insert_increases_size);
        tcase_add_test(tc_core, check_insert_increases_size_with_expansion);
        tcase_add_test(tc_core, check_insert_maintains_heaporder);
        tcase_add_test(tc_core, check_delete_decreases_size);
        tcase_add_test(tc_core, check_delete_min_decreases_size);
        tcase_add_test(tc_core, check_delete_maintains_heaporder);

	suite_add_tcase(s, tc_core);

	return s;
}


int main(void)
{
	int number_failed;  
	Suite *s;  
	SRunner *sr;  

	s = heap_suite();  
	sr = srunner_create(s);  

	srunner_run_all(sr, CK_NORMAL);  
	number_failed = srunner_ntests_failed(sr);  
	srunner_free(sr);
	return (number_failed == 0) ? 0 : 1;
}
