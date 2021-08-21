//array.h
#ifndef _ARRAY_H
#define _ARRAY_H

#include<stddef.h> //size_t
typedef enum _boolean{
	FALSE_ = 0, TRUE_ = 1
}Boolean;
typedef signed long Long;
typedef struct _array {
	void(*front);
	Long length;
	Long capacity;
}Array;

void Create(Array* array, Long capacity, size_t size);
Long Store(Array* array, Long index, void* object, size_t size);
Long Insert(Array* array, Long index, void* object, size_t size);
Long AppendForFront(Array* array, void* object, size_t size);
Long AppendForRear(Array* array, void* object, size_t size);
Long Delete(Array* array, Long index, size_t size);
Long DeleteForFront(Array* array, size_t size);
Long DeleteForRear(Array* array, size_t size);
void Clear(Array* array);
Long Modify(Array* array, Long index, void* object, size_t size);
Long LinearSearchUnique(Array* array, void* key, size_t size, int(*compare)(void*, void*));
void LinearSearchDuplicate(Array* array, void* key, Long*(*indexes), Long* count, size_t size,
	int(*compare)(void*, void*));
Long BinarySearchUnique(Array* array, void* key, size_t size, int(*compare)(void*, void*));
void BinarySearchDuplicate(Array* array, void* key,  Long* count, Long*(*indexes), size_t size,
	int(*compare)(void*, void*));
void SelectionSort(Array* array, size_t size, int(*compare)(void*, void*));
void BubbleSort(Array* array, size_t size, int(*compare)(void*, void*));
void InsertSort(Array* array, size_t size, int(*compare)(void*, void*));
void Merge(Array* array, Array* one, Array* other, size_t size, int(*compare)(void*, void*));
void GetAt(Array* array, Long index, void *object, size_t size);
void Destroy(Array* array);

#endif//_ARRAY_H