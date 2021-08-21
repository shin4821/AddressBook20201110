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

	//1. 윈도우가 생성될때
	  //1.1 주소록을 만든다.
	  addressBook = (AddressBook*)malloc(sizeof(AddressBook));
	  AddressBook_Create(addressBook, 10000);
	  SetWindowLong(hWnd, GWL_USERDATA, (LONG)addressBook);


	  //1.2 적재하다.(Load)
	  Load(addressBook);


	  //1.3 리스트뷰 컨트롤의 헤더를 만든다.
	  column.mask = LVCF_WIDTH | LVCF_TEXT;
	  column.cx = 50;
	  column.pszText = "번호";
	  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)0, (LPARAM)&column);


	  column.cx = 80;
	  column.pszText = "성명";
	  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)1, (LPARAM)&column);


	  column.cx = 200;
	  column.pszText = "주소";
	  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)2, (LPARAM)&column);

	  column.cx = 150;
	  column.pszText = "전화번호";
	  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)3, (LPARAM)&column);

	  column.cx = 150;
	  column.pszText = "이메일주소";
	  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)4, (LPARAM)&column);

	  SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS),
		  LVM_SETEXTENDEDLISTVIEWSTYLE,
		  LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES,
		  LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	  //1.4 적재된 개수만큼 리스트뷰 컨트롤에 항목을 추가한다.
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

	//2. 기재하기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 성명, 주소, 전화번호, 이메일 주소를 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_GETTEXT, (WPARAM)11, (LPARAM)name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_GETTEXT, (WPARAM)64, (LPARAM)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)emailAddress);

		//2.2 주소록에 기재한다.
		addressBook=(AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Record(addressBook, name, address, telephoneNumber, emailAddress);
		personal = AddressBook_GetAt(addressBook, index);

		//2.3 리스트뷰 컨트롤에 항목을 추가한다.
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

	//3. 찾기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED){

	   //3.1 찾기 윈도우를 출력한다.
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

	//5. 고치기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1 주소, 전화번호, 이메일 주소를 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_GETTEXT, (WPARAM)64, (LPARAM)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)emailAddress);

		//5.2 리스트뷰 컨트롤의 번호를 읽는다.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, 0, 0);

		//5.3 주소록에서 고친다.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Correct(addressBook, index, address, telephoneNumber, emailAddress);
		personal = AddressBook_GetAt(addressBook, index);

		//5.4 리스트뷰 컨트롤에서 항목을 고친다.
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

	//6. 지우기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1 리스트뷰 컨트롤의 번호를 읽는다.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, 0, 0);

		//6.2 주소록에서 지운다.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		Erase(addressBook, index);
		
		//6.3 리스트뷰 컨트롤에서 항목을 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEITEM, (WPARAM)index, 0);

		//6.4  지운 리스트뷰 컨트롤 번호의 하위 번호에 대하여, 리스트뷰에 1씩 빼서 적는다.
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

	//7. 정리하기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//7.1 주소록을 정리한다.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		Arrange(addressBook);
		personal = AddressBook_GetAt(addressBook, i);

		//7.2 리스트뷰 컨트롤의 모든 항목을 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEALLITEMS, 0, 0);

		//7.3 리스트뷰 컨트롤에 주소록의 length만큼 항목을 추가한다.
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

	//4. 항목을 더블클릭 했을때
	if (((LPNMHDR)lParam) -> code == NM_DBLCLK) {
		//4.1 리스트뷰 컨트롤의 번호를 읽는다.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, 0, 0);

		//4.2 해당 항목의 성명, 주소, 전화번호, 이메일 주소를 읽는다.
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

		//4.3 개인에 성명, 주소, 전화번호, 이메일 주소를 적는다.
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

	//8. 닫기 버튼을 클릭했을때
	  //8.1 주소록을 없앤다.
	addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
	if (addressBook != NULL) {
		Save(addressBook);
		Destroy(addressBook);
		free(addressBook);
		addressBook = NULL;
	}
	//8.2 윈도우를 종료한다.
	EndDialog(hWnd, 0);

	return TRUE;
}

