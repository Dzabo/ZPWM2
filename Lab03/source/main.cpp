#include <Windows.h>
#include "res.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctime>



CHAR szText[500];
bool is_game_on=false;
bool is_first_player_turn =true;


INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//Funkcja obs³ugi komunikatów
{

	
	HWND hwndStatic5 = GetDlgItem(hwndDlg, IDC_STATIC5);
  HWND hwndStatic = GetDlgItem(hwndDlg, IDC_STATIC6);
	HWND hwndRadio1 = GetDlgItem(hwndDlg, IDC_RADIO1);
	HWND hwndRadio2 = GetDlgItem(hwndDlg, IDC_RADIO2);
  HWND hwndButton1 = GetDlgItem(hwndDlg, IDC_BUTTON1);
  HWND hwndButton2 = GetDlgItem(hwndDlg, IDC_BUTTON2);
  HWND hwndButton3 = GetDlgItem(hwndDlg, IDC_BUTTON3);
  HWND hwndButton4 = GetDlgItem(hwndDlg, IDC_BUTTON4);
  HWND hwndButton5 = GetDlgItem(hwndDlg, IDC_BUTTON5);
  HWND hwndButton6 = GetDlgItem(hwndDlg, IDC_BUTTON6);
  HWND hwndButton7 = GetDlgItem(hwndDlg, IDC_BUTTON7);
  HWND hwndButton8 = GetDlgItem(hwndDlg, IDC_BUTTON8);
  HWND hwndButton9 = GetDlgItem(hwndDlg, IDC_BUTTON9);
  HWND hwndButton10 = GetDlgItem(hwndDlg, IDC_BUTTON10);

	
	switch (uMsg)
	{
	



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
        else
        
        { 
          CheckRadioButton(hwndDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
        }
          is_game_on =true;
        wsprintf(szText,"Gra siê rozpoczê³¹");
        SetWindowText(hwndStatic5,szText);
        for (int i = 0;i < 9;i++)
        {
          HWND hwndButton=GetDlgItem(hwndDlg,IDC_BUTTON1+i);
          wsprintf(szText,"");
          SetWindowText(hwndButton,szText);
        }
        
        
        wsprintf(szText, "STOP");
        SetWindowText(hwndButton10, szText);
        }
        else
        {
          wsprintf(szText, "START");
          SetWindowText(hwndButton10, szText);
          is_game_on =false;
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
      if(is_game_on){
        //isFirstPlayerTurn
        HWND hwndButton=(HWND)lParam;

        if(GetWindowTextLength(hwndButton)==0)
        {
         if (is_first_player_turn)
         {
          wsprintf(szText,"X");
         }
         else
         {
         wsprintf(szText,"O");
         }
         SetWindowText(hwndButton,szText);
         is_first_player_turn=!is_first_player_turn;
        }
        }
      return TRUE;
		}	
    return TRUE;
	}
		return TRUE;


	case WM_CLOSE:
		DestroyWindow(hwndDlg); // zniszczenie okna
		PostQuitMessage(0); //Komunikat polecenia zakoñczenia aplikacji
		return TRUE;
	}
	return FALSE;
}
return FALSE;
}
    //CASE WM_SIZE OGARNAC


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	
	HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
	ShowWindow(hwndMainWindow, iCmdShow);
	
	MSG msg = {};
	//srand(time(NULL));
	//numberRand = rand() % 39 + 1;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	
}
