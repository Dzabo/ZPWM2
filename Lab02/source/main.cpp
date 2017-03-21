#include <Windows.h>
#include "res.h"
#include <stdio.h>

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//Funkcja obs³ugi komunikatów
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
			switch (LOWORD(wParam))
			{
			case IDC_EDIT1:
				
					HWND hwndEditBox = GetDlgItem(hwndDlg, IDC_EDIT1);
					int iTextLength = GetWindowTextLength(hwndEditBox);//Pobranie d³ugoœci tekstu
					CHAR szText[500];
					GetWindowText(hwndEditBox, szText, iTextLength + 1);//Pobranie Tekstu
					HWND hwndButton = GetDlgItem(hwndDlg, IDC_BUTTON1);
					SetWindowText(hwndButton, szText);
					return TRUE;
				
			}
		return FALSE;

		case BN_CLICKED://Zdarzenie klikniêcia 
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON1://Klikniêcie na nasz przycisk Button1
				MessageBox(hwndDlg, TEXT("Klikniecie tego magicznego przyciusku powoduje zmiane jego opisu"), TEXT("Magiczny Przycisk"), MB_OK);
				//Zmiana pola text na przycisku na tekst z EditBox
				HWND hwndEditBox = GetDlgItem(hwndDlg, IDC_EDIT1); 
				int iTextLength = GetWindowTextLength(hwndEditBox);//Pobranie d³ugoœci tekstu
				CHAR szText[500];
				GetWindowText(hwndEditBox, szText, iTextLength + 1);//Pobranie Tekstu
				SetWindowText((HWND)lParam, szText);//Przypisanei tekstu do przycisku

				return TRUE;
			}
		}
		return FALSE;
	
	case WM_LBUTTONDOWN :
		CHAR szText[200];
		wsprintf(szText, "Kliknales myszka x=%d, y=%d", LOWORD(lParam), HIWORD(lParam));
		MessageBox(hwndDlg, szText, TEXT("Klikniecie"), MB_OK);
		return TRUE;
	
	case WM_CLOSE:
		DestroyWindow(hwndDlg); // zniszczenie okna
		PostQuitMessage(0); //Komunikat polecenia zakoñczenia aplikacji
		return TRUE;
	}
	return FALSE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{



	HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
	ShowWindow(hwndMainWindow, iCmdShow);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	
}




/*
MessageBox(0 , "Treœæ wyœwietlana" , "Tytu³" , Rodzaj); // zwracany typ int
*/
/*
Wyœwietlanie zmiennej int
char tekst=sprintf_s(text, "Are you looking for %d", mid); // wpisujemy zmienn¹ typu int do tablicy char, %d pokazuje gdzie bêdzie, mid to nazwa zmiennej
MessageBox(0,(LPCSTR)text  , "Question", MB_YESNO); // rzutowanie na LPCSTR bo visual ma downa


*/

	/*
	MessageBox(0 , "Treœæ wyœwietlana" , "Tytu³" , Rodzaj); // zwracany typ int
	*/
	/*
	Wyœwietlanie zmiennej int
	char tekst=sprintf_s(text, "Are you looking for %d", mid); // wpisujemy zmienn¹ typu int do tablicy char, %d pokazuje gdzie bêdzie, mid to nazwa zmiennej
	MessageBox(0,(LPCSTR)text  , "Question", MB_YESNO); // rzutowanie na LPCSTR bo visual ma downa


	*/
