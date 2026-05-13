
/** c-dynamic-array.c

	C version: C89

	Goal: implement a dynamic integer array "ArrInt"

	The interface must have the following functionality:
	* IntArray init(n)           | Allocate the array with space for n elements
	* void push(x)               | Push the value x and reallocate if needed
	* void pop()                 | Remove the last value and reallocate
	* void remove(i)             | Remove the element at index i
	* void reserve(n)            | Allocate space for n elemn¡ents
	* void shrink_to_fit()       | Set capacity down to count
	* int get(i)                 | Get the element at index i
	* int get_count()            | Get the count of the array
	* int get_capacity()         | Get the capacity of the array

	Notes:
	- Count = number of elements
	- Capacity = amount of space allocated (in elements, not byte size)

	All functions must ensure there is no overflow or access of elements outside the size.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_ERROR(msg) fprintf (stderr, "\n%s\n", (msg))

typedef struct {
	int *_data;
	int _capacity;
	int _count;
} ArrInt;

ArrInt ArrInt_init (size_t n) {
	ArrInt arr;

	if (n < 0) {
		LOG_ERROR("ArrInt_init() : cannot initialize array with negative size");
	}

	arr._capacity = n;
	arr._count = 0;
	arr._data = (int *) malloc (arr._capacity * sizeof (int));

	if (!arr._data && n != 0) {
		LOG_ERROR("ArrInt_init() : malloc() returned NULL");
	}

	return arr;
}

void ArrInt_reserve (ArrInt *arr, size_t n) {
	if (!arr) {
		LOG_ERROR("ArrInt_reserve() : array cannot be NULL");
		return;
	}

	if (n <= arr->_capacity) {
		/* Do nothing */
		return;
	}

	int *new_data = (int *) realloc (arr->_data, n * sizeof (int));

	if (new_data == NULL) {
		LOG_ERROR("ArrInt_reserve() : realloc unsuccessful");
		return;
	}

	arr->_data = new_data;
	arr->_capacity = n;
}

void ArrInt_push (ArrInt *arr, int value) {
	/* @TODO: Check size overflow */

	if (!arr) {
		LOG_ERROR("ArrInt_push() : array cannot be NULL");
		return;
	}

	/* If needed, try to realloc() */
	if ((arr->_count + 1) > arr->_capacity) {
		int *new_data = realloc (arr->_data, (arr->_capacity + 1) * sizeof (int));

		if (!new_data) {
			LOG_ERROR("ArrInt_push() : realloc() returned NULL");
			return;
		}

		arr->_capacity++;
		arr->_data = new_data;
	}

	arr->_count++;

	arr->_data [arr->_count - 1] = value;
}

int ArrInt_remove (ArrInt *arr, size_t index) {
	if (!arr) {
		LOG_ERROR("ArrInt_remove() : array cannot be NULL");
		return;
	}

	if (index < 0 || index >= arr->_count) {
		LOG_ERROR("ArrInt_remove() : cannot remove element; index out of bounds");
		return;
	}

	int value = arr->_data [index];
	memmove (arr->_data + index,
			 arr->_data + index + 1,
			 arr->_count - index);
	arr->_count--;

	return value;
}

int ArrInt_pop (ArrInt *arr) {
	int value = 0;

	if (!arr) {
		LOG_ERROR("ArrInt_pop() : array cannot be NULL");
		return;
	}

	if (arr->_count == 0) {
		LOG_ERROR("ArrInt_pop() : cannot pop element from count-zero array");
		return;
	}

	value = arr->_data [arr->_count - 1];
	arr->_data [arr->_count - 1] = -42; /* Set a magic value, for debugging purposes */;
	arr->_count--;

	return value;
}

void ArrInt_shrink_to_fit (ArrInt *arr) {
	
	if (!arr) {
		LOG_ERROR("ArrInt_shrink_to_fit() : array cannot be NULL");
		return;
	}

	int *new_data = (int *) realloc (arr->_data, arr->_count * sizeof (int));

	if (!new_data) {
		LOG_ERROR("ArrInt_shrink_to_fit() : realloc unsuccessful");
		return;
	}

	arr->_data = new_data;
	arr->_capacity = arr->_count;
}

int ArrInt_get (ArrInt *arr, int index) {

	if (!arr) {
		LOG_ERROR("ArrInt_get() : array cannot be NULL");
		return -1;
	}

	if (index < 0 || index >= arr->_count) {
		LOG_ERROR("ArrInt_get() : index out of array bounds");
		return -1;
	}

	return arr->_data [index];
}

__inline__
int ArrInt_get_count (ArrInt *arr) {
	return arr->_count;
}

__inline__
int ArrInt_get_capacity (ArrInt *arr) {
	return arr->_capacity;
}

void ArrInt_print (ArrInt *arr) {
	int i;

	for (i = 0; i < ArrInt_get_count (arr); i++) {
		printf ("arr[%d] = %d (count %d, capacity %d)\n",
				i,
				ArrInt_get (arr, i),
				ArrInt_get_count (arr),
				ArrInt_get_capacity (arr));
	}
}

int main (int argc, char *argv[]) {
	int i;
	ArrInt array_0 = ArrInt_init (0);

	ArrInt_reserve (&array_0, 30);

	for (i = 0; i < 9; i++) {
		ArrInt_push (&array_0, i * i);
	}

	printf ("\n\nArray after push():\n");
	ArrInt_print (&array_0);

	ArrInt_shrink_to_fit (&array_0);

	ArrInt_remove (&array_0, 0);
	ArrInt_remove (&array_0, 6);

	printf ("\n\nArray after remove():\n");
	ArrInt_print (&array_0);


	return 0;
}
