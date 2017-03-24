#include <Windows.h>
#include "res.h"
#include <stdio.h>

CHAR szText[500];
bool is_game_on = false;
bool is_player_cross = true;
bool game_result = false;

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

  HWND hwndStatic1 = GetDlgItem(hwndDlg, IDC_STATIC1);
  HWND hwndStatic2 = GetDlgItem(hwndDlg, IDC_STATIC2);
  HWND hwndStatic3 = GetDlgItem(hwndDlg, IDC_STATIC3);
  HWND hwndRadio1 = GetDlgItem(hwndDlg, IDC_RADIO1);
  HWND hwndRadio2 = GetDlgItem(hwndDlg, IDC_RADIO2);
  HWND hwndButton10 = GetDlgItem(hwndDlg, IDC_BUTTON10);


  switch (uMsg)
  {
    case WM_COMMAND:
    {
      switch (HIWORD(wParam))
      {
          case BN_CLICKED:
          {
              switch (LOWORD(wParam))
              {
                  case IDC_BUTTON10:
                  {
                      if (!is_game_on)
                      {
                        if (is_player_cross == true)
                          {
                             CheckRadioButton(hwndDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
                          }
                          else
                          {
                              CheckRadioButton(hwndDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
                          }
                          is_game_on = true;
                          wsprintf(szText, "Gra siê rozpoczê³a!");
                          SetWindowText(hwndStatic1, szText);
                          wsprintf(szText, "W trakcie rozgrywki");
                          SetWindowText(hwndStatic3, szText);
                          for (int i = 0; i < 9; i++)
                          {
                              HWND hwndButton = GetDlgItem(hwndDlg, IDC_BUTTON1 + i);
                              wsprintf(szText, "");
                              SetWindowText(hwndButton, szText);
                          }
                          wsprintf(szText, "Zakoñcz Grê");
                          SetWindowText(hwndButton10, szText);
                      }
                      else
                      {
                          wsprintf(szText, "Rozpocznij Grê!");
                          SetWindowText(hwndButton10, szText);
                          wsprintf(szText, "Rozgrywka nieaktywna");
                          SetWindowText(hwndStatic1, szText);
                          is_game_on = false;
                      }
                      return TRUE;
                  }
                  case IDC_BUTTON1:
                  case IDC_BUTTON2:
                  case IDC_BUTTON3:
                  case IDC_BUTTON4:
                  case IDC_BUTTON5:
                  case IDC_BUTTON6:
                  case IDC_BUTTON7:
                  case IDC_BUTTON8:
                  case IDC_BUTTON9:
                      if (is_game_on)
                      {
                          HWND hwndButton = (HWND)lParam;//uchwyt na przycisk
                          if(GetWindowTextLength(hwndButton)==0)
                          {
                              if (is_player_cross)
                              {
                                  wsprintf(szText, "X");
                              }
                              else
                              {
                                  wsprintf(szText, "O");
                              }
                              SetWindowText(hwndButton, szText);
                              is_player_cross = !is_player_cross;
                          }
                      }
                      return TRUE;
              }
              return TRUE;
          }
          return TRUE;
      }
      return TRUE;
    }
    return TRUE;
    case WM_CLOSE:
    {
        DestroyWindow(hwndDlg); // zniszczenie okna
        PostQuitMessage(0); //Komunikat polecenia zakoñczenia aplikacji
        return TRUE;
    }
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
