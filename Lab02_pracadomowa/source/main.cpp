#include <Windows.h>
#include "res.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctime>


int numberRand;
int counter = 0;
CHAR text[100];
CHAR szText[500];

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//Funkcja obs�ugi komunikat�w
{

	
	HWND hwndStatic1 = GetDlgItem(hwndDlg, IDC_STATIC1);
	HWND hwndStatic2 = GetDlgItem(hwndDlg, IDC_STATIC2);
	HWND hwndStatic3 = GetDlgItem(hwndDlg, IDC_STATIC3);
	HWND hwndStatic4 = GetDlgItem(hwndDlg, IDC_STATIC4);
	HWND hwndEditBox = GetDlgItem(hwndDlg, IDC_EDIT1);
	
	switch (uMsg)
	{
	

	case WM_COMMAND:
	{
		switch (HIWORD(wParam))
		{

		case BN_CLICKED://Zdarzenie klikni�cia 
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON1://Klikni�cie na nsz przycisk Button1

				//Zmiana pola text na przycisku na tekst z EditBox
				//HWND hwndEditBox = GetDlgItem(hwndDlg, IDC_EDIT1);
				int iTextLength = GetWindowTextLength(hwndEditBox);//Pobranie d�ugo�ci tekstu				
				GetWindowText(hwndEditBox, szText, iTextLength + 1);//Pobranie Tekstu
				int number = atoi(szText);
				wsprintf(text, "%d", number);
				SetWindowText(hwndStatic3, text);
				if ((number <= 40) && (number > 0))
				{
					
					if (number > numberRand)
					{
						counter++;
						
						wsprintf(text, "Wybra�e� liczb� %d, jest ona wi�ksza od liczby wybranej przeze mnie", number);
						MessageBox(0, text, "Wynik gry", MB_OK);
					//	HWND hwndStatic2 = GetDlgItem(hwndDlg, IDC_STATIC2);
						SetWindowText(hwndStatic2, "Nie trafi�e�");
						SetWindowText(hwndStatic4, "Za du�a");
					}
					else if (number < numberRand)
					{
						counter++;
						wsprintf(text, "Wybra�e� liczb� %d, jest ona mniejsza od liczby wybranej przeze mnie", number);
						MessageBox(0, text, "Wynik gry", MB_OK);
				//		HWND hwndStatic2 = GetDlgItem(hwndDlg, IDC_STATIC2);
						SetWindowText(hwndStatic2, "Nie trafi�e�");
						SetWindowText(hwndStatic4, "Za ma�a");
					}
					else
					{
						counter++;
						wsprintf(text, "Wybra�e� liczb� %d, wygra�e�", number);
						MessageBox(0, text, "Wynik gry", MB_OK);
				//		HWND hwndStatic2 = GetDlgItem(hwndDlg, IDC_STATIC2);
						SetWindowText(hwndStatic2, "Trafi�e�");
						SetWindowText(hwndStatic4, " ");
						
					}


				}
				else
				{
					wsprintf(text, "Wybra�e� liczb� %d, nie mie�ci si� ona w przedziale", number);
					MessageBox(0, text, "Wynik gry", MB_OK);
					DestroyWindow(hwndDlg); // zniszczenie okna
					PostQuitMessage(0); //Komunikat polecenia zako�czenia aplikacji
					return TRUE;
				}

				//HWND hwndStatic1 = GetDlgItem(hwndDlg, IDC_STATIC1);
				wsprintf(text, "Ilo�� klikni��: %d", counter);
				SetWindowText(hwndStatic1, text);
				return TRUE;
			}		
		}	
	}
		return FALSE;
	

	case WM_CLOSE:
		DestroyWindow(hwndDlg); // zniszczenie okna
		PostQuitMessage(0); //Komunikat polecenia zako�czenia aplikacji
		return TRUE;
	}
	return FALSE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	
	HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
	ShowWindow(hwndMainWindow, iCmdShow);
	
	MSG msg = {};
	srand(time(NULL));
	numberRand = rand() % 39 + 1;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	
}
