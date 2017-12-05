#ifndef HEAP_H
#define HEAP_H

#define HEAP_BUFFER_SIZE (1<<10)

typedef struct
{
	void **heap;
	int (*comp)(const void *, const void *);
	int size;
} Heap;

/* construct a new heap */
Heap *heap_new(int (*compare)(const void*, const void*));

/* delete heap */
int heap_delete(Heap *);

/* insert into heap */
int heap_insert(Heap *, void *);

/* extract min element, return size of the element, 0 on failure */
void* heap_extract(Heap *);

#endif


