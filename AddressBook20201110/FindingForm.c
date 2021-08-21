//FindingForm.c
#include"FindingForm.h"
#include"resource.h"
#include"AddressBook.h"
#include<stdlib.h>
#include<stdio.h>
#include<Commctrl.h>
#pragma warning(disable:4996)

BOOL CALLBACK FindingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (message) {
	case WM_INITDIALOG:
		ret = FindingForm_OnInitDialog(hWnd, wParam, lParam);
		break;

	case WM_COMMAND:
		ret = FindingForm_OnCommand(hWnd, wParam, lParam);
		break;

	case WM_NOTIFY:
		ret = FindingForm_OnNotify(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		ret = FindingForm_OnClose(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL FindingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (LOWORD(wParam)) {
	case IDC_BUTTON_FIND:
		ret = FindingForm_OnFindButtonClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL FindingForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (wParam) {
	case IDC_LIST_PERSONALS:
		ret = FindingForm_OnListViewItemDoubleClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL FindingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	LVCOLUMN column = { 0, };


	//1. 윈도우가 생성될 때
	  //1.1 리스트뷰 컨트롤의 헤더를 만든다.
	column.mask = LVCF_TEXT | LVCF_WIDTH;
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

	column.cx = 80;
	column.pszText = "이메일주소";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)4, (LPARAM)&column);

	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS),
		LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES,
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;
}

BOOL FindingForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	TCHAR name[11];
	AddressBook* addressBook;
	HWND addressBookForm;
	Long(*indexes);
	Long count;
	LVITEM item = { 0, };
	int i;
	TCHAR number[64];
	Personal personal;

	//2. 찾기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 성명을 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_GETTEXT, (WPARAM)11, (LPARAM)name);

		//2.2 주소록 윈도우를 찾는다.
		addressBookForm = FindWindow("#32770", "주소록");

		//2.3 주소록 윈도우의 주소록에서 찾는다.
		addressBook = (AddressBook*)GetWindowLong(addressBookForm, GWL_USERDATA);
		indexes = (Long(*))GetWindowLong(hWnd, GWL_USERDATA);
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}
		Find(addressBook, name, &indexes, &count);

		SetWindowLong(hWnd, GWL_USERDATA, (LONG)indexes);

		//2.4 리스트뷰 컨트롤의 모든 항목을 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEALLITEMS, 0, 0);

		//2.5 찾은 개수만큼 리스트뷰 컨트롤에 항목을 추가한다.
		item.mask = LVIF_TEXT;
		i = 0;
		while (i < count) {
			personal = AddressBook_GetAt(addressBook, indexes[i]);

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

BOOL FindingForm_OnListViewItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	LVITEM item = { 0, };
	int index;
	HWND addressBookForm;
	TCHAR name[11];
	TCHAR address[64];
	TCHAR telephoneNumber[12];
	TCHAR emailAddress[32];
	Long(*indexes);

	//3. 항목을 더블클릭했을때
	if (((LPNMHDR)lParam)->code == NM_DBLCLK) {
		//3.1 리스트뷰 컨트롤의 번호를 읽는다.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, 0, 0);
		
		//3.2 읽은 리스트뷰 컨트롤 항목의 성명, 주소, 전화번호, 이메일주소를 읽는다.
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

		//3.3 주소록 윈도우를 찾는다.
		addressBookForm = FindWindow("#32770", "주소록");

		//3.4 주소록 윈도우의 개인에 성명, 주소, 전화번호, 이메일 주소를 적는다.
		SendMessage(GetDlgItem(addressBookForm, IDC_EDIT_NAME), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)name);
		SendMessage(GetDlgItem(addressBookForm, IDC_EDIT_ADDRESS), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)address);
		SendMessage(GetDlgItem(addressBookForm, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT, 0, 
			(LPARAM)(LPCTSTR)telephoneNumber);
		SendMessage(GetDlgItem(addressBookForm, IDC_EDIT_EMAILADDRESS), WM_SETTEXT, 0, 
			(LPARAM)(LPCTSTR)emailAddress);

		//3.5 주소록 윈도우의 리스트뷰 컨트롤에서 해당 항목을 선택한다.
		indexes = (Long(*))GetWindowLong(hWnd, GWL_USERDATA);
		SendMessage(GetDlgItem(addressBookForm, IDC_LIST_PERSONALS), LVM_SETSELECTIONMARK, 0, (LPARAM)indexes[index]);
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}

		//3.6 윈도우를 종료한다.
		EndDialog(hWnd, 0);
	}
	return TRUE;
}

BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Long(*indexes);

	//4. 닫기 버튼을 클릭했을때
	   //4.1 찾기 윈도우를 종료한다.
	indexes = (Long(*))GetWindowLong(hWnd, GWL_USERDATA);
	if (indexes != NULL) {
		free(indexes);
		indexes = NULL;
	}
	EndDialog(hWnd, 0);
	
	return TRUE;
}
