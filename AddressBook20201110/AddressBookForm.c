//AddressBookForm.c
#include"AddressBookForm.h"
#include"FindingForm.h"
#include"resource.h"
#include"AddressBook.h"
#include<stdlib.h>
#include<stdio.h>
#include<Commctrl.h>
#pragma warning(disable:4996)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevlnstance, LPSTR lpCmdLine, int nCmdShow) {
	int response;
	response = DialogBox(hInstance, MAKEINTRESOURCE(IDD_ADDRESSBOOKFORM), NULL, AddressBookFormProc);
	return response;
}

BOOL CALLBACK AddressBookFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG:
		ret = AddressBookForm_OnInitDialog(hWnd, wParam, lParam);
		break;

	case WM_COMMAND:
		ret = AddressBookForm_OnCommand(hWnd, wParam, lParam);
		break;

	case WM_NOTIFY:
		ret = AddressBookForm_OnNotify(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		ret = AddressBookForm_OnClose(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL AddressBookForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (LOWORD(wParam)) {
	case IDC_BUTTON_RECORD:
		ret = AddressBookForm_OnRecordButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_FIND:
		ret = AddressBookForm_OnFindButtonClicked(hWnd, wParam, lParam);
		break;


	case IDC_BUTTON_CORRECT:
		ret = AddressBookForm_OnCorrectButtonClicked(hWnd, wParam, lParam);
		break;
		
	case IDC_BUTTON_ERASE:
		ret = AddressBookForm_OnEraseButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_ARRANGE:
		ret = AddressBookForm_OnArrangeButtonClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL AddressBookForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (wParam) {
	case IDC_LIST_PERSONALS:
		ret = AddressBookForm_OnListViewItemButtonClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL AddressBookForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;
	LVCOLUMN column = { 0, };
	LVITEM item = { 0, };
	int i = 0;
	TCHAR number[64];
	Personal personal;

	//1. �����찡 �����ɶ�
	  //1.1 �ּҷ��� �����.
	  addressBook = (AddressBook*)malloc(sizeof(AddressBook));
	  AddressBook_Create(addressBook, 10000);
	  SetWindowLong(hWnd, GWL_USERDATA, (LONG)addressBook);


	  //1.2 �����ϴ�.(Load)
	  Load(addressBook);


	  //1.3 ����Ʈ�� ��Ʈ���� ����� �����.
	  column.mask = LVCF_WIDTH | LVCF_TEXT;
	  column.cx = 50;
	  column.pszText = "��ȣ";
	  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)0, (LPARAM)&column);


	  column.cx = 80;
	  column.pszText = "����";
	  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)1, (LPARAM)&column);


	  column.cx = 200;
	  column.pszText = "�ּ�";
	  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)2, (LPARAM)&column);

	  column.cx = 150;
	  column.pszText = "��ȭ��ȣ";
	  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)3, (LPARAM)&column);

	  column.cx = 150;
	  column.pszText = "�̸����ּ�";
	  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)4, (LPARAM)&column);

	  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS),
		  LVM_SETEXTENDEDLISTVIEWSTYLE,
		  LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES,
		  LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	  //1.4 ����� ������ŭ ����Ʈ�� ��Ʈ�ѿ� �׸��� �߰��Ѵ�.
	  item.mask = LVIF_TEXT;
	  while (i < addressBook->length) {
		  item.iItem = i;
		  item.iSubItem = 0;
		  sprintf(number, "%d", i + 1);
		  item.pszText = number;
		  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTITEM, 0, (LPARAM)&item);

		  personal = AddressBook_GetAt(addressBook, i);
		  item.iSubItem = 1;
		  item.pszText = personal.name;
		  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS),LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);

		  item.iSubItem = 2;
		  item.pszText = personal.address;
		  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);

		  item.iSubItem = 3;
		  item.pszText = personal.telephoneNumber;
		  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);


		  item.iSubItem = 4;
		  item.pszText = personal.emailAddress;
		  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);

		  i++;
	  }

	  return TRUE;
}

BOOL AddressBookForm_OnRecordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;
	TCHAR name[11];
	TCHAR address[64];
	TCHAR telephoneNumber[12];
	TCHAR emailAddress[32];
	TCHAR number[64];
	LVITEM item = { 0, };
	int index;
	Personal personal;

	//2. �����ϱ� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 ����, �ּ�, ��ȭ��ȣ, �̸��� �ּҸ� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_GETTEXT, (WPARAM)11, (LPARAM)name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_GETTEXT, (WPARAM)64, (LPARAM)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)emailAddress);

		//2.2 �ּҷϿ� �����Ѵ�.
		addressBook=(AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Record(addressBook, name, address, telephoneNumber, emailAddress);
		personal = AddressBook_GetAt(addressBook, index);

		//2.3 ����Ʈ�� ��Ʈ�ѿ� �׸��� �߰��Ѵ�.
		item.mask = LVIF_TEXT;
		item.iItem = index;
		item.iSubItem = 0;
		sprintf(number, "%d", index + 1);
		item.pszText = number;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTITEM, 0, (LPARAM)&item);

		item.iSubItem = 1;
		item.pszText = personal.name;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.iSubItem = 2;
		item.pszText = personal.address;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.iSubItem = 3;
		item.pszText = personal.telephoneNumber;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.iSubItem = 4;
		item.pszText = personal.emailAddress;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);
	}
	return TRUE;
}

BOOL AddressBookForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND FindingForm;

	//3. ã�� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED){

	   //3.1 ã�� �����츦 ����Ѵ�.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_FINDINGFORM), hWnd,
			FindingFormProc);
	}
	return TRUE;
}

BOOL AddressBookForm_OnCorrectButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;
	TCHAR address[64];
	TCHAR telephoneNumber[12];
	TCHAR emailAddress[32];
	int index;
	LVITEM item = { 0, };
	Personal personal;

	//5. ��ġ�� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1 �ּ�, ��ȭ��ȣ, �̸��� �ּҸ� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_GETTEXT, (WPARAM)64, (LPARAM)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)emailAddress);

		//5.2 ����Ʈ�� ��Ʈ���� ��ȣ�� �д´�.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, 0, 0);

		//5.3 �ּҷϿ��� ��ģ��.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Correct(addressBook, index, address, telephoneNumber, emailAddress);
		personal = AddressBook_GetAt(addressBook, index);

		//5.4 ����Ʈ�� ��Ʈ�ѿ��� �׸��� ��ģ��.
		item.mask = LVIF_TEXT;
		item.iItem = index;
		item.iSubItem = 2;
		item.pszText = personal.address;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.iSubItem = 3;
		item.pszText = personal.telephoneNumber;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.iSubItem = 4;
		item.pszText = personal.emailAddress;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);
	}
	return TRUE;
}

BOOL AddressBookForm_OnEraseButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;
	int index;
	LVITEM item = { 0, };
	TCHAR number[64];

	//6. ����� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1 ����Ʈ�� ��Ʈ���� ��ȣ�� �д´�.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, 0, 0);

		//6.2 �ּҷϿ��� �����.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		Erase(addressBook, index);
		
		//6.3 ����Ʈ�� ��Ʈ�ѿ��� �׸��� �����.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEITEM, (WPARAM)index, 0);

		//6.4  ���� ����Ʈ�� ��Ʈ�� ��ȣ�� ���� ��ȣ�� ���Ͽ�, ����Ʈ�信 1�� ���� ���´�.
		item.mask = LVIF_TEXT;
		while (index < addressBook->length) {
			item.iItem = index;
			item.iSubItem = 0;
			sprintf(number, "%d", index + 1);
			item.pszText = number;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);
			index++;
		}
	}
	return TRUE;
}

BOOL AddressBookForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;
	int i = 0;
	LVITEM item = { 0, };
	TCHAR number[64];
	Personal personal;

	//7. �����ϱ� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//7.1 �ּҷ��� �����Ѵ�.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		Arrange(addressBook);
		personal = AddressBook_GetAt(addressBook, i);

		//7.2 ����Ʈ�� ��Ʈ���� ��� �׸��� �����.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEALLITEMS, 0, 0);

		//7.3 ����Ʈ�� ��Ʈ�ѿ� �ּҷ��� length��ŭ �׸��� �߰��Ѵ�.
		item.mask = LVIF_TEXT;
		while (i < addressBook->length) {
			item.iItem = i;
			item.iSubItem = 0;
			sprintf(number, "%d", i + 1);
			item.pszText = number;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTITEM, 0, (LPARAM)&item);

			item.iSubItem = 1;
			item.pszText = personal.name;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);

			item.iSubItem = 2;
			item.pszText = personal.address;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);

			item.iSubItem = 3;
			item.pszText = personal.telephoneNumber;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);

			item.iSubItem = 4;
			item.pszText = personal.emailAddress;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);

			i++;
		}
	}
	return TRUE;
}

BOOL AddressBookForm_OnListViewItemButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	TCHAR name[11];
	TCHAR address[64];
	TCHAR telephoneNumber[12];
	TCHAR emailAddress[32];
	LVITEM item = { 0, };
	int index;

	//4. �׸��� ����Ŭ�� ������
	if (((LPNMHDR)lParam) -> code == NM_DBLCLK) {
		//4.1 ����Ʈ�� ��Ʈ���� ��ȣ�� �д´�.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, 0, 0);

		//4.2 �ش� �׸��� ����, �ּ�, ��ȭ��ȣ, �̸��� �ּҸ� �д´�.
		item.mask = LVIF_TEXT;
		item.iItem = index;
		item.iSubItem = 1;
		item.pszText = name;
		item.cchTextMax = 11;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.iSubItem = 2;
		item.pszText = address;
		item.cchTextMax = 64;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.iSubItem = 3;
		item.pszText = telephoneNumber;
		item.cchTextMax = 12;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		item.iSubItem = 4;
		item.pszText = emailAddress;
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		//4.3 ���ο� ����, �ּ�, ��ȭ��ȣ, �̸��� �ּҸ� ���´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)(LPCTSTR)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT, 0,
			(LPARAM)(LPCTSTR)emailAddress);
	}
	return TRUE;
}

BOOL AddressBookForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;

	//8. �ݱ� ��ư�� Ŭ��������
	  //8.1 �ּҷ��� ���ش�.
	addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
	if (addressBook != NULL) {
		Save(addressBook);
		Destroy(addressBook);
		free(addressBook);
		addressBook = NULL;
	}
	//8.2 �����츦 �����Ѵ�.
	EndDialog(hWnd, 0);

	return TRUE;
}

