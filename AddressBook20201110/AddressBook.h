//AddressBook.h
#ifndef _ADDRESSBOOK_H
#define _ADDRESSBOOK_H
#include"array.h"

typedef struct _personal {
	char name[11];
	char address[64];
	char telephoneNumber[12];
	char emailAddress[32];
}Personal;

typedef struct _addressBook {
	Array personals;
	Long capacity;
	Long length;
}AddressBook;

void AddressBook_Create(AddressBook* addressBook, Long capacity);
Long Load(AddressBook* addressBook);
Long Record(AddressBook* addressBook, char(*name), char(*address), char(*telephoneNumber),
	char(*emailAddress));
void Find(AddressBook* addressBook, char(*name), Long* (*indexes), Long* count);
Personal AddressBook_GetAt(AddressBook* addressBook, Long index);
Long Correct(AddressBook* addressBook, Long index, char(*address), char(*telephoneNumber),
	char(*emailAddress));
Long Erase(AddressBook* addressBook, Long index);
Long Arrange(AddressBook* addressBook);
Long Save(AddressBook* addressBook);
void AddressBook_Destroy(AddressBook* addressBook);
int CompareNames(void* one, void* other);
int ComparePersonals(void* one, void* other);
#endif//_ADDRESSBOOK_H
