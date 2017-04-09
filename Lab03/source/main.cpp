#include <Windows.h>
#include "res.h"
#include <stdio.h>

CHAR sz_text[500];
bool is_game_on = false;
bool is_player_cross = true;


INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

  HWND hwnd_static1 = GetDlgItem(hwndDlg, IDC_STATIC1);
  HWND hwnd_static2 = GetDlgItem(hwndDlg, IDC_STATIC2);
  HWND hwnd_static3 = GetDlgItem(hwndDlg, IDC_STATIC3);
  
  HWND hwnd_button1 = GetDlgItem(hwndDlg, IDC_BUTTON1);
  HWND hwnd_button2 = GetDlgItem(hwndDlg, IDC_BUTTON2);
  HWND hwnd_button3 = GetDlgItem(hwndDlg, IDC_BUTTON3);
  HWND hwnd_button4 = GetDlgItem(hwndDlg, IDC_BUTTON4);
  HWND hwnd_button5 = GetDlgItem(hwndDlg, IDC_BUTTON5);
  HWND hwnd_button6 = GetDlgItem(hwndDlg, IDC_BUTTON6);
  HWND hwnd_button7 = GetDlgItem(hwndDlg, IDC_BUTTON7);
  HWND hwnd_button8 = GetDlgItem(hwndDlg, IDC_BUTTON8);
  HWND hwnd_button9 = GetDlgItem(hwndDlg, IDC_BUTTON9);
  HWND hwnd_button10 = GetDlgItem(hwndDlg, IDC_BUTTON10);




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
                      if (is_game_on==false)
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
                          wsprintf(sz_text, "Gra siê rozpoczê³a!");
                          SetWindowText(hwnd_static1, sz_text);
                          wsprintf(sz_text, "W trakcie rozgrywki");
                          SetWindowText(hwnd_static3, sz_text);
                          for (int i = 0; i < 9; i++)
                          {
                            HWND hwnd_button = GetDlgItem(hwndDlg, IDC_BUTTON1 + i);
                              wsprintf(sz_text, "");
                              SetWindowText(hwnd_button, sz_text);
                          }
              
                          wsprintf(sz_text, "Zakoñcz Grê");
                          SetWindowText(hwnd_button10, sz_text);
                      }
                      else
                      {
                          wsprintf(sz_text, "Rozpocznij Grê!");
                          SetWindowText(hwnd_button10, sz_text);
                          wsprintf(sz_text, "Rozgrywka nieaktywna");
                          SetWindowText(hwnd_static1, sz_text);
                          wsprintf(sz_text, "");
                          SetWindowText(hwnd_static3, sz_text);
                          is_game_on = !is_game_on;
                          //PostQuitMessage(0);
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
                  {
                    if (is_game_on)
                      {
                        HWND hwnd_button = (HWND)lParam;//uchwyt na przycisk
                        if (GetWindowTextLength(hwnd_button) == 0)
                        {
                          if (is_player_cross)
                          {
                            wsprintf(sz_text, "X");
                          }
                          else
                          {
                            wsprintf(sz_text, "O");
                          }
                          SetWindowText(hwnd_button, sz_text);
                          is_player_cross = !is_player_cross;
                        }
                      }
                    return TRUE;
                  }
                  return FALSE;
              }
              return FALSE;
          }
          return FALSE;
      }
      return FALSE;
    }
    return FALSE;
    case WM_CLOSE:
    {
        DestroyWindow(hwndDlg); // zniszczenie okna
        PostQuitMessage(0); //Komunikat polecenia zakoñczenia aplikacji
        return TRUE;
    }
    return FALSE;
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

