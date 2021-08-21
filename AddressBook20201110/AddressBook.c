//AddressBook.c
#include"AddressBook.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)

int main(int argc, char* argv[]) {
	AddressBook addressBook;
	Long index;
	Long(*indexes);
	Personal personal;
	Long i = 0;
	Long count;

	AddressBook_Create(&addressBook, 3);

	index = Record(&addressBook, "고길동", "서울시 서초구", "01011112222", "ko@");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s %s %s %s\n", personal.name, personal.address, personal.telephoneNumber,
		personal.emailAddress);

	index = Record(&addressBook, "박길동", "서울시 마포구", "01022223333", "park@");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s %s %s %s\n", personal.name, personal.address, personal.telephoneNumber,
		personal.emailAddress);

	index = Record(&addressBook, "나길동", "인천시 연수구", "01033334444", "Na@");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s %s %s %s\n", personal.name, personal.address, personal.telephoneNumber,
		personal.emailAddress);

	index = Record(&addressBook, "박길동", "부산시 해운대구", "01044445555", "PARK2@");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s %s %s %s\n", personal.name, personal.address, personal.telephoneNumber,
		personal.emailAddress);

	Find(&addressBook, "박길동", &indexes, &count);
	while (i < count) {
		personal = AddressBook_GetAt(&addressBook, indexes[i]);
		printf("%s %s %s %s\n", personal.name, personal.address, personal.telephoneNumber,
			personal.emailAddress);
		i++;
	}

	if (indexes != NULL) {
		free(indexes);
		indexes = NULL;
	}

	index = Correct(&addressBook, 1, "서울시 종로구", "01077778888", "park@");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s %s %s %s\n", personal.name, personal.address, personal.telephoneNumber,
		personal.emailAddress);

	index = Erase(&addressBook, 3);
	if (index == -1) {
		printf("지워졌습니다.\n");
	}

	Arrange(&addressBook);
	i = 0;
	while (i < addressBook.length) {
		personal = AddressBook_GetAt(&addressBook, i);
		printf("%s %s %s %s\n", personal.name, personal.address, personal.telephoneNumber,
			personal.emailAddress);
		i++;
	}

	AddressBook_Destroy(&addressBook);

	return 0;
}

void AddressBook_Create(AddressBook* addressBook, Long capacity) {
	Create(&addressBook->personals, capacity, sizeof(Personal));
	addressBook->capacity = capacity;
	addressBook->length = 0;
}

Long Load(AddressBook* addressBook) {
	Personal personal;
	Long index;
	FILE* file;
	size_t flag;
	file = fopen("AddressBook.dat", "rb");

	if (file != NULL) {
		//1.1 성명, 주소, 전화번호, 이메일주소를 입력받는다.
		flag = fread(&personal, sizeof(Personal), 1, file);
		//1. 파일의 끝이 아닌동안 반복한다,
		while (!feof(file) && flag != 0) {
			if (addressBook->capacity > addressBook->length) {
				index = addressBook->length;
				Store(&addressBook->personals, index, &personal, sizeof(Personal));
			}
			else {
				AppendForRear(&addressBook->personals, &personal, sizeof(Personal));
				addressBook->capacity++;
			}
			addressBook->length++;
			flag = fread(&personal, sizeof(Personal), 1, file);
		}
		fclose(file);
	}
	return addressBook->length;
}

Long Record(AddressBook* addressBook, char(*name), char(*address), char(*telephoneNumber),
	char(*emailAddress)) {
	Long index;
	Personal personal;

	//1. 성명, 주소, 전화번호, 이메일주소를 입력받는다.
	//2. 입력받을 성명, 주소, 전화번호, 이메일주소를 personal에 적는다.
	strcpy(personal.name, name);
	strcpy(personal.address, address);
	strcpy(personal.telephoneNumber, telephoneNumber);
	strcpy(personal.emailAddress, emailAddress);

	//3. capacity가 length보다 크면 store함수를 호출한다.
	if (addressBook->capacity > addressBook->length) {
		index = addressBook->length;
		index = Store(&addressBook->personals, index, &personal, sizeof(Personal));
	}
	//4. capacity가 length와 같거나 작으면 AppendForRear함수를 호출한다.
	else {
		index = AppendForRear(&addressBook->personals, &personal, sizeof(Personal));
		addressBook->capacity++;
	}
	addressBook->length++;
	return index;
}

void Find(AddressBook* addressBook, char(*name), Long* (*indexes), Long* count) {
	LinearSearchDuplicate(&addressBook->personals, name, indexes, count, sizeof(Personal), CompareNames);
}

Long Correct(AddressBook* addressBook, Long index, char(*address), char(*telephoneNumber),
	char(*emailAddress)) {
	Personal personal;

	//1. 주소, 전화번호, 이메일주소, 번호를 입력받는다.
	//2. index번째의 개인을 개인에 옮겨적는다.
	GetAt(&addressBook->personals, index, &personal, sizeof(Personal));
	//3. 입력받은 주소, 전화번호, 이메일주소를 개인에 옮겨적는다.
	strcpy(personal.address, address);
	strcpy(personal.telephoneNumber, telephoneNumber);
	strcpy(personal.emailAddress, emailAddress);

	//4. Modify함수를 호출한다.
	index = Modify(&addressBook->personals, index, &personal, sizeof(Personal));
	//5. 번호를 출력한다.
	return index;
}

Personal AddressBook_GetAt(AddressBook* addressBook, Long index) {
	Personal personal;
	GetAt(&addressBook->personals, index, &personal, sizeof(Personal));
	return personal;
}

Long Erase(AddressBook* addressBook, Long index) {
	index = Delete(&addressBook->personals, index, sizeof(Personal));
	addressBook->capacity--;
	addressBook->length--;
	index = -1;
	return index;
}

Long Arrange(AddressBook* addressBook) {
	SelectionSort(&addressBook->personals, sizeof(Personal), ComparePersonals);
}

int CompareNames(void* one, void* other) {
	Personal* one_ = (Personal*)one;
	char(*other_) = (char*)other;
	int ret;

	ret = strcmp(one_->name, other_);

	return ret;
}

int ComparePersonals(void* one, void* other) {
	Personal* one_ = (Personal*)one;
	Personal* other_ = (Personal*)other;
	int ret;
	ret = strcmp(one_->name, other_->name);
	return ret;
}

Long Save(AddressBook* addressBook) {
	Personal personal;
	Long i = 0;
	FILE* file;
	file = fopen("AddressBook.dat", "wb");
	if (file != NULL) {
		while (i < addressBook->length) {
			GetAt(&addressBook->personals, i, &personal, sizeof(Personal));
			fwrite(&personal, sizeof(Personal), 1, file);
			i++;
		}
		fclose(file);
	}
	return addressBook->length;
}


void AddressBook_Destroy(AddressBook* addressBook) {
	Destroy(&addressBook->personals);

}





