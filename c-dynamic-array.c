
/** c-dynamic-array.c

	C version: C89

	Goal: implement a dynamic integer array "ArrInt"

	The interface must have the following functionality:
	- IntArray init(n)           | Allocate the array with space for n elements
	- void push(x)               | Push the value x and reallocate if needed
	- void pop()                 | Remove the last value and reallocate
	- void remove(i)             | Remove the element at index i
	- void reserve(n)            | Allocate space for n elemn¡ents
	- int get(i)                 | Get the element at index i
	- int get_count()            | Get the count of the array
	- int get_capacity()         | Get the capacity of the array

	Notes:
	- Count = number of elements
	- Capacity = amount of space allocated (in elements, not byte size)

	All functions must ensure there is no overflow or access of elements outside the size.
 */


#include <stdio.h>
#include <stdlib.h>


typedef struct {
	int *_data;
	int _capacity;
	int _count;
} ArrInt;

ArrInt ArrInt_init (size_t n) {
	ArrInt arr;

	if (n < 0) {
		fprintf (stderr, "ArrInt_init() : cannot initialize array with negative size\n");
	}

	arr._capacity = n;
	arr._count = 0;
	arr._data = (int *) malloc (arr._capacity * sizeof (int));

	if (!arr._data) {
		fprintf (stderr, "ArrInt_init() : malloc() returned NULL\n");
	}

	return arr;
}

void ArrInt_push (ArrInt *arr, int value) {

	/* If needed, try reallocating the data */
	if ((arr->_count + 1) > arr->_capacity) {
		int *new_data = realloc (arr->_data, (arr->_capacity + 1) * sizeof (int));

		if (!new_data) {
			fprintf (stderr, "ArrInt_push() : realloc() returned NULL\n");
			return;
		}

		arr->_capacity++;
		arr->_data = new_data;
	}

	arr->_count++;

	arr->_data [arr->_count - 1] = value;
}

int main (int argc, char *argv[]) {
	int i;
	ArrInt array_0 = ArrInt_init (0);

	for (i = 0; i < 9; i++) {
		ArrInt_push (&array_0, i * i);
		printf ("%d, ", array_0._data[i]);
	}

	return 0;
}
