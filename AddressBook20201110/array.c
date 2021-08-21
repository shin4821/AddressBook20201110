//array.c
#include"array.h"
#include<memory.h>
#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>

void Create(Array* array, Long capacity, size_t size) {

	//1. capacity를 입력받는다.
	//2. array에 capacity만큼 배열을 할당한다.
	array->front = calloc(capacity, size);
	//3. array.capacity에 capacity를 적는다.
	array->capacity = capacity;
	//4. length를 초기화한다.
	array->length = 0;
	//5. 끝낸다.
}

Long Store(Array* array, Long index, void* object, size_t size) {
	//1. index, object를 입력받는다.
	//2. array.front배열의 index번째에 object를 적는다.
	memcpy(((char*)array->front) + (index * size), object, size);
	//3. length를 1씩 증가시킨다.
	array->length++;
	//4. index를 출력한다.
	return index;
	//5. 끝내다.
}

Long Insert(Array* array, Long index, void* object, size_t size) {
	void(*temps);
	Long i = 0;
	Long j = 0;

	//1. index, object를 입력받는다.
	//2. temps에 capacity+1만큼 배열을 할당한다.
	temps = calloc(array->capacity + 1, size);
	//3. array.front의 값을 index번째를 제외하고 temps에 옮겨적는다.
	while (i < index) {
		memcpy(((char*)temps) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
		i++;
	}
	j++;
	while (i < array->length) {
		memcpy(((char*)temps) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
		i++;
	}
	//4. array.front 배열을 할당해제한다.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	(char(*))array->front = (char(*))temps;
	//5. capacity를 1씩 증가시킨다.
	array->capacity++;
	//6. index번째에 object를 끼운다.
	memcpy(((char*)array->front) + (index * size), object, size);
	array->length++;
	//7. index를 출력한다.
	return index;
}

Long AppendForFront(Array* array, void* object, size_t size) {
	Long index = 0;
	void(*temps);
	Long i = 0;
	Long j = 1;

	//1. object를 입력받는다.
	//2. temps에 capacity+1만큼 배열을 할당한다.
	temps = calloc(array->capacity + 1, size);
	//3. array.front를 temps의 두번째부터 옮겨적는다.
	while (i < array->length) {
		memcpy(((char*)temps) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
		i++;
	}
	//4. array.front배열을 할당해제한다.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	(char(*))array->front = (char(*))temps;
	//5. capacity를 1씩 증가시킨다.
	array->capacity++;
	//6. array.front의 index번째에 object를 적는다.
	memcpy(((char*)array->front) + (index * size), object, size);
	//7. length를 1씩 증가시킨다.
	array->length++;
	//8. index를 출력한다.
	return index;
	//9. 끝낸다.
}

Long AppendForRear(Array* array, void* object, size_t size) {
	Long index;
	void(*temps);
	Long i = 0;

	//1. object를 입력받는다.
	//2. temps에 capacity+1만큼 배열을 할당한다.
	temps = calloc(array->capacity + 1, size);
	//3. array.front를 temps의 첫번째부터 옮겨 적는다.
	while (i < array->length) {
		memcpy(((char*)temps) + (i * size), ((char*)array->front) + (i * size), size);
		i++;
	}
	//4. array.front배열을 할당해제한다.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	(char(*))array->front = (char(*))temps;
	//5. capacity를 1씩 증가시킨다.
	array->capacity++;
	//6. array.front의 index번째에 object를 적는다.
	index = array->capacity - 1;
	memcpy(((char*)array->front) + (index * size), object, size);
	//7. length를 1씩 증가시킨다.
	array->length++;
	//8. index를 출력한다.
	return index;
	//9. 끝내다.
}

Long Delete(Array* array, Long index, size_t size) {
	void(*temps) = NULL;
	Long i = 0;
	Long j = 0;

	//1. index를 읽는다.
	//2. capacity가 1보다 클 경우,
	if (array->capacity > 1) {
		//2.1 temps에 capacity-1만큼 배열을 할당한다.
		temps = calloc(array->capacity - 1, size);
	}
	//3. array.front의 값을 index값을 제외하고 temps에 옮겨 적는다.
	while (i < index) {
		memcpy(((char*)temps) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
		i++;
	}
	i = index + 1;
	while (i < array->length) {
		memcpy(((char*)temps) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
		i++;
	}
	//4. array.front를 할당해제한다.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	if (array->capacity > 1) {
		(char(*))array->front = (char(*))temps;
	}
	array->capacity--;
	array->length--;
	index = -1;
	return index;
}

Long DeleteForFront(Array* array, size_t size) {
	Long index;
	void(*temps)=NULL;
	Long i = 1;
	Long j = 0;

	//1. capacity가 1보다 클 경우,
	if (array->capacity > 1) {
		//1.1 temps에 capacity-1만큼 배열을 할당한다.
		temps = calloc(array->capacity - 1, size);
	}
	//2. array.front의 두번째부터 temps에 옮겨적는다.
	while (i < array->length) {
		memcpy(((char*)temps) + (j * size), ((char*)array->front) + (i * size), size);
		j++;
		i++;
	}
	//3. array.front를 할당해제한다.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	if (array->capacity > 1) {
		(char(*))array->front = (char(*))temps;
	}
	//4. capacity를 1씩 뺀다.
	array->capacity--;
	//5. length를 1씩 뺀다.
	array->length--;
	//6. index에 0을 적는다.
	index = -1;
	return index;
	//7. 끝내다.
}

Long DeleteForRear(Array* array, size_t size) {
	Long index;
	void(*temps) = NULL;
	Long i = 0;

	//1. capacity가 1보다 클 경우,
	if (array->capacity > 1) {
		//1.1 temps에 capacity-1만큼 배열을 할당한다.
		temps = calloc(array->capacity - 1, size);
	}
	//2. length-1만큼 array.front를 temps에 옮겨적는다.
	while (i < array->capacity - 1) {
		memcpy(((char*)temps) + (i * size), ((char*)array->front) + (i * size), size);
		i++;
	}
	//3. array.front배열을 할당해제한다.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	if (array->capacity > 1) {
		(char(*))array->front = (char(*))temps;
	}
	//4. capacity를 1씩 감소시킨다.
	array->capacity--;
	//5. length를 1씩 감소시킨다.
	array->length--;
	//6. index에 0을 적는다,
	index = -1;
	//7. index를 출력한다.
	return index;
	//8. 끝내다.
}

void Clear(Array* array) {
	//1. array.front를 할당해제한다.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	//2. capacity를 0으로 원위치한다.
	array->capacity = 0;
	//3. length를 0으로 원위치한다.
	array->length = 0;

}

Long Modify(Array* array, Long index, void* object, size_t size) {
	//1. index, object를 입력받는다.
	//2. array.front의 index번째에 object를 적는다.
	memcpy(((char*)array->front) + (index * size), object, size);
	//3. index를 출력한다.
	return index;
}

Long LinearSearchUnique(Array* array, void* key, size_t size, int (*compare)(void*, void*)) {
	Long index = -1;
	Long i = 0;

	//1. key값을 입력받는다.
	//2. array.length보다 작거나 같고 array.front가 key가 아닌동안 반복한다.
	while (i < array->length && compare(((char*)array->front) + (i * size), key) != 0) {
		i++;
	}
	//3. array.length보다 작은 경우,
	if (i < array->length) {
		//3.1 index에 i를 적는다.
		index = i;
	}
	return index;
}

void LinearSearchDuplicate(Array* array, void* key, Long* (*indexes), Long* count, size_t size,
	int(*compare)(void*, void*)) {

	Long i = 0;
	Long j = 0;
	*count = 0;

	//1. key값을 입력받는다.
	//2. indexes에 length만큼 배열을 할당한다.
	*indexes = (Long(*))calloc(array->length, size);
	//3. length만큼 반복한다.
	while (i < array->length) {
		if (compare(((char*)array->front) + (i * size), key) == 0) {
			(*indexes)[j] = i;
			j++;
			(*count)++;
		}
		i++;
	}
	//4. indexes와 count를 출력한다.
	//5. 끝낸다.
}

Long BinarySearchUnique(Array* array, void* key, size_t size, int(*compare)(void*, void*)) {
	Long index = -1;
	Long high;
	Long low = 0;
	Long middle;

	//1. key값을 입력받는다.
	//2. low가 high보다 같거나 적고, array.front값이 key가 아닌동안 반복한다.
	high = array->length - 1;
	middle = (low + high) / 2;
	while (low <= high && compare(((char*)array->front)+(middle*size),key)!= 0) {
		if (compare(((char*)array->front)+(middle*size),key) < 0) {
			low = middle + 1;
		}
		else {
			high = middle - 1;
		}
		middle = (low + high) / 2;
	}
	//3. low가 high보다 작거나 같은 경우,
	if (low <= high) {
		//3.1 middle이 index이다.
		index = middle;
	}
	//4. index를 출력한다.
	return index;
}

void BinarySearchDuplicate(Array* array, void* key, Long* count, Long* (*indexes), size_t size,
	int(*compare)(void*, void*)) {

	Long low = 0;
	Long high;
	Long middle;
	Long i;
	Long j = 0;
	*count = 0;

	//1. key값을 입력받는다.
	high = array->length - 1;
	//2. indexes에 length만큼 배열을 할당한다.
	*indexes = (Long(*))calloc(array->length, size);
	middle = (low + high) / 2;
	//3. low가 high보다 작거나 같고 array.front값이 key가 아닐동안 반복한다.
	while (low <= high && compare(((char*)array->front) + (middle * size), key) != 0) {
		if (compare(((char*)array->front) + (middle * size), key) < 0) {
			low = middle + 1;
		}
		else {
			high = middle - 1;
		}
		middle = (low + high) / 2;
	}
	i = middle - 1;
	//4. middle이하에 key값이 있는지 찾는다.
	while (i >= 0 && compare(((char*)array->front) + (i * size), key) == 0) {
		i--;
	}
	i++;
	//5. length보다 같거나 적고 array.front에 key값이 있는지 확인한다.
	while (i < array->length && compare(((char*)array->front)+(i*size),key ) == 0) {
		(*indexes)[j] = i;
		j++;
		(*count)++;
		i++;
	}
	//6. indexes와 count를 출력한다.
	//7. 끝내다.
}

void SelectionSort(Array* array, size_t size, int(*compare)(void*, void*)) {
	void* temp;
	Long i = 0;
	Long j;
	Long index;
	
	//1. length-1만큼 반복한다.
	temp= malloc(size);
	while (i < array->length - 1) {
		index = i;
		j = i + 1;
		//1.1 length만큼 반복한다.
		while (j < array->length) {
			if (compare(((char*)array->front) + (j * size), ((char*)array->front) + (index * size)) < 0) {
				index = j;
			}
			j++;
		}
		//1.1.1.1 첫번째 수가 더 클 경우, temp에 적는다.
		memcpy(temp, ((char*)array->front) + (index * size), size);
		//1.1.1.2 두번째 수를 첫번째 위치에 적는다.
		memcpy(((char*)array->front) + (index * size), ((char*)array->front) + (i * size), size);
		//1.1.1.3 temp를 두번째 위치에 적는다.
		memcpy(((char*)array->front) + (i * size), temp, size);
		i++;
	}
	if (temp != NULL) {
		free(temp);
		temp = NULL;
	}
	//2. 끝낸다.
}

void BubbleSort(Array* array, size_t size, int(*compare)(void*, void*)) {
	Boolean temp = FALSE_;
	void* tempFront;
	Long i = 0;
	Long j;

	//1. length-1보다 작거나 temp가 TRUE가 아닌동안 반복한다.
	tempFront = malloc(size);
	while (i < array->length - 1 && temp != TRUE_) {
		temp = TRUE_;
		j = 0;
		//1.1 array.length-i만큼 반복한다.
		while (j < array->length - i-1) {
			if (compare(((char*)array->front) + (j * size), ((char*)array->front) + ((j + 1) * size)) > 0) {
				memcpy(tempFront, ((char*)array->front) + (j * size), size);
				memcpy(((char*)array->front)+(j*size), ((char*)array->front)+((j+1)*size), size);
				memcpy(((char*)array->front) + ((j + 1) * size), tempFront, size);
				temp = FALSE_;
			}
			j++;
		}
		i++;
	}
	if (tempFront != NULL) {
		free(tempFront);
		tempFront = NULL;
	}
	//2. 끝낸다.
}

void InsertSort(Array* array, size_t size, int(*compare)(void*, void*)) {
	void* temp;
	Long i = 1;
	Long j;

	//1. length만큼 반복한다.
	temp = malloc(size);
	while (i < array->length) {
		memcpy(temp, ((char*)array->front) + (i * size), size);
		j = i - 1;
		//1.1 j보다 크거나 같고 temp가 array.front보다 작을동안 반복한다.
		while (j >= 0 && compare(temp, ((char*)array->front) + (j * size)) < 0) {
			memcpy(((char*)array->front) + ((j + 1) * size), ((char*)array->front) + (j * size), size);
			j--;
		}
		memcpy(((char*)array->front) + ((j + 1) * size), temp, size);
		i++;
	}
	if (temp != NULL) {
		free(temp);
		temp = NULL;
	}
	//2. 끝낸다.
}

void Merge(Array* array, Array* one, Array* other, size_t size, int(*compare)(void*, void*)) {
	Long i = 0;
	Long j = 0;
	Long k = 0;

	//1. one배열, other배열을 입력받는다.
	//2. array.front를 할당해제한다.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	//3. one.length와 other.length의 합만큼 array에 배열을 할당한다.
	array->front = calloc(one->length + other->length, size);
	array->capacity = one->length + other->length;
	array->length = 0;
	//4. one.length보다 같거나 작고 other.length보다 같거나 작은 동안 반복한다.
	while (i < one->length && j < other->length) {
		if (compare(((char*)one->front)+(i*size), ((char*)other->front)+(j*size)) < 0) {
			memcpy(((char*)array->front) + (k * size), ((char*)one->front) + (i * size), size);
			array->length++;
			k++;
			i++;
		}
		else {
			memcpy(((char*)array->front) + (k * size), ((char*)other->front) + (j * size), size);
			array->length++;
			k++;
			j++;
		}
	}
	//5. one.length보다 같거나 작을때 front에쓴다.
	while (i < one->length) {
		memcpy(((char*)array->front) + (k * size), ((char*)one->front) + (i * size), size);
		array->length++;
		k++;
		i++;
	}
	//6. other.length보다 같거나 작을때 front에 쓴다.
	while (j < other->length) {
		memcpy(((char*)array->front) + (k * size), ((char*)other->front) + (j * size), size);
		array->length++;
		k++;
		j++;
	}
	//7. 끝낸다.
}

void GetAt(Array* array, Long index, void *object, size_t size) {

	//1. index를 입력받는다.
	//2. array.front의 index번째 값을 object에 적는다.
	memcpy(object, ((char*)array->front) + (index * size), size);
	//3. object를 출력한다.
	//4. 끝내다.
}

void Destroy(Array* array) {
	//1. array, front를 할당해제한다.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	//2. 끝내다.
}
