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

	index = Record(&addressBook, "��浿", "����� ���ʱ�", "01011112222", "ko@");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s %s %s %s\n", personal.name, personal.address, personal.telephoneNumber,
		personal.emailAddress);

	index = Record(&addressBook, "�ڱ浿", "����� ������", "01022223333", "park@");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s %s %s %s\n", personal.name, personal.address, personal.telephoneNumber,
		personal.emailAddress);

	index = Record(&addressBook, "���浿", "��õ�� ������", "01033334444", "Na@");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s %s %s %s\n", personal.name, personal.address, personal.telephoneNumber,
		personal.emailAddress);

	index = Record(&addressBook, "�ڱ浿", "�λ�� �ؿ�뱸", "01044445555", "PARK2@");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s %s %s %s\n", personal.name, personal.address, personal.telephoneNumber,
		personal.emailAddress);

	Find(&addressBook, "�ڱ浿", &indexes, &count);
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

	index = Correct(&addressBook, 1, "����� ���α�", "01077778888", "park@");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s %s %s %s\n", personal.name, personal.address, personal.telephoneNumber,
		personal.emailAddress);

	index = Erase(&addressBook, 3);
	if (index == -1) {
		printf("���������ϴ�.\n");
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
		//1.1 ����, �ּ�, ��ȭ��ȣ, �̸����ּҸ� �Է¹޴´�.
		flag = fread(&personal, sizeof(Personal), 1, file);
		//1. ������ ���� �ƴѵ��� �ݺ��Ѵ�,
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

	//1. ����, �ּ�, ��ȭ��ȣ, �̸����ּҸ� �Է¹޴´�.
	//2. �Է¹��� ����, �ּ�, ��ȭ��ȣ, �̸����ּҸ� personal�� ���´�.
	strcpy(personal.name, name);
	strcpy(personal.address, address);
	strcpy(personal.telephoneNumber, telephoneNumber);
	strcpy(personal.emailAddress, emailAddress);

	//3. capacity�� length���� ũ�� store�Լ��� ȣ���Ѵ�.
	if (addressBook->capacity > addressBook->length) {
		index = addressBook->length;
		index = Store(&addressBook->personals, index, &personal, sizeof(Personal));
	}
	//4. capacity�� length�� ���ų� ������ AppendForRear�Լ��� ȣ���Ѵ�.
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

	//1. �ּ�, ��ȭ��ȣ, �̸����ּ�, ��ȣ�� �Է¹޴´�.
	//2. index��°�� ������ ���ο� �Ű����´�.
	GetAt(&addressBook->personals, index, &personal, sizeof(Personal));
	//3. �Է¹��� �ּ�, ��ȭ��ȣ, �̸����ּҸ� ���ο� �Ű����´�.
	strcpy(personal.address, address);
	strcpy(personal.telephoneNumber, telephoneNumber);
	strcpy(personal.emailAddress, emailAddress);

	//4. Modify�Լ��� ȣ���Ѵ�.
	index = Modify(&addressBook->personals, index, &personal, sizeof(Personal));
	//5. ��ȣ�� ����Ѵ�.
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





