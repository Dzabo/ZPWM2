#include <Windows.h>
#include "res.h"
#include <stdio.h>

int result_tab[9] = { 0 };
int continue_game;
CHAR sz_text[500];

bool is_game_on=false;
bool is_first_player_turn =true;
bool is_field_ocupied_by_first_player[3][3];
bool is_field_ocupied_by_second_player[3][3];
void DrawBoard(HDC x);
void DrawX(HDC hdc, int x, int y);
void DrawO(HDC hdc, int x, int y);
void RedrawBoard(HDC hdc);
void ClearBoard(HDC hdc);
void GameResult();





INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//Funkcja obs³ugi komunikatów
{

	HWND hwndStatic5 = GetDlgItem(hwndDlg, IDC_STATIC5);
  HWND hwndButton10 = GetDlgItem(hwndDlg, IDC_BUTTON10);

	
	switch (uMsg)
	{
	case WM_COMMAND:
  {
    switch (HIWORD(wParam))
    {
    case BN_CLICKED://Zdarzenie klikniêcia 
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON10://Klikniêcie na nsz przycisk Button1
      {
        if (is_game_on == false)
        {
          if (is_first_player_turn == true)
          {
            CheckRadioButton(hwndDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
          }
          else
          {
            CheckRadioButton(hwndDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
          }
          HDC hdc = GetDC(hwndDlg);
          ClearBoard(hdc);
          is_game_on = true;
          DrawBoard(hdc);
          ReleaseDC(hwndDlg, hdc);
          for (int i = 0; i < 3; i++)
          {
            for (int j = 0; j < 3; j++)
            {
              is_field_ocupied_by_first_player[i][j] = false;
              is_field_ocupied_by_second_player[i][j] = false;
            }
          }
          wsprintf(sz_text, "Gra siê rozpoczê³¹");
          SetWindowText(hwndStatic5, sz_text);
          wsprintf(sz_text, "STOP");
          SetWindowText(hwndButton10, sz_text);
        }
        else
        {
          wsprintf(sz_text, "START");
          SetWindowText(hwndButton10, sz_text);
          is_game_on = false;
        }
        return TRUE;
      }
      default: ;
      }
      return TRUE;
    default: ;
    }
    return TRUE;
  case WM_LBUTTONDOWN:
  {
    GameResult();
    if (is_game_on == true)
    {
      int x = LOWORD(lParam);
      int y = HIWORD(lParam);
      if ((x > 100 && x < 250) && ((y > 100) && (y < 250)))
      {
        //x=((x-iMinBoardX)/iWidthBoardX)*iWidthBoardX+iMaxBoardX/2);
        //y=((y-iMinBoardY)/iWidthBoardY)*iWidthBoardY+iMaxBoardY/2);
        int filed_x = ((x - 100) / 50);
        int filed_y = ((y - 100) / 50);
        if (is_field_ocupied_by_first_player[filed_x][filed_y] == false
          && is_field_ocupied_by_second_player[filed_x][filed_y] == false)
        {
          x = (filed_x * 50) + 125;
          y = (filed_y * 50) + 125;
          HDC hdc = GetDC(hwndDlg);
          if (is_first_player_turn == true)
          {
            DrawX(hdc, x, y);
            is_field_ocupied_by_first_player[filed_x][filed_y] = true;
            result_tab[3 * (filed_y)+(filed_x)] = 1;
          }
          else
          {
            DrawO(hdc, x, y);
            is_field_ocupied_by_second_player[filed_x][filed_y] = true;
            result_tab[3 * (filed_y)+(filed_x)] = 2;
          }
          ReleaseDC(hwndDlg, hdc);
          is_first_player_turn = !is_first_player_turn;
        }
      }
    }
  }
    return TRUE;
  case WM_PAINT:
  {
    HDC hdc=GetDC(hwndDlg);
    DrawBoard(hdc);
    RedrawBoard(hdc);   
    ReleaseDC(hwndDlg,hdc);
    }
    return TRUE;
	case WM_CLOSE:
		DestroyWindow(hwndDlg); // zniszczenie okna
		PostQuitMessage(0); //Komunikat polecenia zakoñczenia aplikacji
		return TRUE;
	}
	default: ;
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

enum WhoWin
{
  Nought,
  Cross,
  None
};
void ClearBoard(HDC x)
{
  COLORREF color = GetPixel(x, 80, 80);
  COLORREF BgColorR = GetRValue(color);
  COLORREF BgColorG = GetGValue(color);
  COLORREF BgColorB = GetBValue(color);
  HPEN h_my_pen = CreatePen(PS_SOLID, 2, RGB(BgColorR, BgColorG, BgColorB));
  SelectObject(x, h_my_pen);
  for (int i = 100; i<250; i++)
  {
    MoveToEx(x, i, 100, NULL);
    LineTo(x, i, 250);
  }
  DeleteObject(h_my_pen);
}
void DrawBoard(HDC x)
{
  HPEN h_my_pen = CreatePen(PS_SOLID, 2, RGB(255, 125, 125));
  SelectObject(x, h_my_pen);
  MoveToEx(x, 150, 100, NULL);
  LineTo(x, 150, 250);
  MoveToEx(x, 200, 100, NULL);
  LineTo(x, 200, 250);
  MoveToEx(x, 100, 150, NULL);
  LineTo(x, 250, 150);
  MoveToEx(x, 100, 200, NULL);
  LineTo(x, 250, 200);
  DeleteObject(h_my_pen);

}
void DrawX(HDC hdc,int x, int y)
{
  HPEN h_my_pen = CreatePen(PS_SOLID, 2, RGB(200, 150, 100));
  SelectObject(hdc, h_my_pen);
  MoveToEx(hdc, x - 10, y - 10, NULL);
  LineTo(hdc, x + 10, y + 10);
  MoveToEx(hdc, x - 10, y + 10, NULL);
  LineTo(hdc, x + 10, y - 10);
  DeleteObject(h_my_pen);
}
void DrawO(HDC hdc, int x, int y)
{
  HPEN h_my_pen = CreatePen(PS_SOLID, 2, RGB(100,150, 200));
  SelectObject(hdc, h_my_pen);
  MoveToEx(hdc, x - 10, y - 10, NULL);
  /*LineTo(hdc, x - 10, y + 10);
  LineTo(hdc, x - 10, y - 10);
  LineTo(hdc, x + 10, y - 10);
  LineTo(hdc, x + 10, y + 10);
  LineTo(hdc, x - 10, y + 10);*/
  Ellipse(hdc, x-15, y-15, x+15, y+15);
  DeleteObject(h_my_pen);

  
}
void RedrawBoard(HDC hdc)
{
  for (int i_field_x = 0; i_field_x < 3; i_field_x++)
  {
    for (int i_field_y = 0; i_field_y < 3; i_field_y++)
    {
      if (is_field_ocupied_by_first_player[i_field_x][i_field_y] == true)
      {
        DrawX(hdc, (i_field_x * 50 + 125), (i_field_y * 50 + 125));
      }
      if (is_field_ocupied_by_second_player[i_field_x][i_field_y] == true)
      {
        DrawO(hdc, (i_field_x * 50 + 125), (i_field_y * 50 + 125));
      }
    }
  }
}
void GameResult()
{
  for (int i = 0; i<3; i++)
  {
    if ((result_tab[3 * i] == result_tab[3 * i + 1]) && (result_tab[3 * i] == result_tab[3 * i + 2]))
    {
      if (result_tab[3 * i] == 1)
      {
        MessageBox(NULL, TEXT("Krzy¿yk wygrywa"), TEXT("Wynik Gry"), MB_OK);
        is_game_on = false;
        result_tab[9] = { 0 };
      }
      else if (result_tab[3 * i] == 2)
      {
        MessageBox(NULL, TEXT("Kó³ko wygrywa"), TEXT("Wynik Gry"), MB_OK);
        is_game_on = false;
        result_tab[9] = { 0 };
      }
    }
    if ((result_tab[i] == result_tab[i + 3]) && (result_tab[i] == result_tab[i + 6]))
    {
      if (result_tab[i] == 1)
      {
        MessageBox(NULL, TEXT("Krzy¿yk wygrywa"), TEXT("Wynik Gry"), MB_OK);
        is_game_on = false;
        result_tab[9] = { 0 };
      }
      else if (result_tab[i] == 2)
      {
        MessageBox(NULL, TEXT("Kó³ko wygrywa"), TEXT("Wynik Gry"), MB_OK);
        is_game_on = false;
        result_tab[9] = { 0 };
      }

    }
  }
  if ((result_tab[0] == result_tab[4] && result_tab[0] == result_tab[8]) || (result_tab[4] == result_tab[6] && result_tab[4] == result_tab[2]))
  {
    if (result_tab[4] == 1)
    {
      MessageBox(NULL, TEXT("Krzy¿yk wygrywa"), TEXT("Wynik Gry"), MB_OK);
      is_game_on = false;
      result_tab[9] = { 0 };
    }
    else if (result_tab[4] == 2)
    {
      MessageBox(NULL, TEXT("Kó³ko wygrywa"), TEXT("Wynik Gry"), MB_OK);
      is_game_on = false;
      result_tab[9] = { 0 };
    }
  }
}
//Rysowanie
//LineTo
//MoveToEx
//Rectangle
//TextOut
//GetPixel
//SetPixel
// TextOut(hdc, 0, 0, sz_text, strlen(sz_text)); //napsize start w lewym gornym, ogarn¹c