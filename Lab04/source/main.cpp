#include <Windows.h>
#include "res.h"
#include <stdio.h>

#define X_MIN 150
#define X_MAX 300
#define Y_MIN 100
#define Y_MAX 250
#define X_WIDTH 50
#define Y_WIDTH 50
#define TAB_SIZE_X 3
#define TAB_SIZE_Y 3
#define COLOR_O RGB(100,150, 200)
#define COLOR_X RGB(200, 150, 100)
#define COLOR_BOARD RGB(255, 125, 125)

int result_tab[9] = { 0 };
int continue_game;
int licznik = 0;
CHAR sz_text[500];

bool is_game_on=false;
bool is_first_player_turn =true;
bool is_field_ocupied_by_first_player[TAB_SIZE_X][TAB_SIZE_Y];
bool is_field_ocupied_by_second_player[TAB_SIZE_X][TAB_SIZE_Y];
void DrawBoard(HDC x);
void DrawX(HDC hdc, int x, int y);
void DrawO(HDC hdc, int x, int y);
void RedrawBoard(HDC hdc);
void ClearBoard(HDC hdc);
int GameResult(HWND hwndDlg, HDC x);


INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//Funkcja obs�ugi komunikat�w
{

	HWND hwndStatic5 = GetDlgItem(hwndDlg, IDC_STATIC5);
  HWND hwndButton10 = GetDlgItem(hwndDlg, IDC_BUTTON10);

	
	switch (uMsg)
	{
	case WM_COMMAND:
  {
    switch (HIWORD(wParam))
    {
    case BN_CLICKED://Zdarzenie klikni�cia 
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON10://Klikni�cie na nsz przycisk Button1
      {
        HDC hdc = GetDC(hwndDlg);
        ClearBoard(hdc);
        DrawBoard(hdc);
        licznik = 0;
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
          ReleaseDC(hwndDlg, hdc);
          for (int i = 0; i < TAB_SIZE_X; ++i)
          {
            for (int j = 0; j < TAB_SIZE_Y; ++j)
            {
              is_field_ocupied_by_first_player[i][j] = false;
              is_field_ocupied_by_second_player[i][j] = false;
              result_tab[3 * j + i] = 0;
            }
          }
          wsprintf(sz_text, "Gra si� rozpocz곹");
          SetWindowText(hwndStatic5, sz_text);
          wsprintf(sz_text, "STOP");
          SetWindowText(hwndButton10, sz_text);
          is_game_on = true;
        }
        else
        {
          wsprintf(sz_text, "START");
          SetWindowText(hwndButton10, sz_text);
          wsprintf(sz_text, "Rozpocznij gr�");
          SetWindowText(hwndStatic5, sz_text);
          is_game_on = false;
        }
        return TRUE;
      }
      default: ;
      }
      return TRUE;
    default: ;
    }
  case WM_LBUTTONDOWN:
  {
    HDC hdc = GetDC(hwndDlg);
    GameResult(hwndDlg,hdc);
    if (is_game_on == true)
    {
      int x = LOWORD(lParam);
      int y = HIWORD(lParam);
      if ((x > X_MIN && x < X_MAX) && ((y > Y_MIN) && (y < Y_MAX)))
      {
        int filed_x = ((x - X_MIN) / X_WIDTH);
        int filed_y = ((y - Y_MIN) / Y_WIDTH);
        if (is_field_ocupied_by_first_player[filed_x][filed_y] == false
          && is_field_ocupied_by_second_player[filed_x][filed_y] == false)
        {
          x = (filed_x * X_WIDTH) + (X_MIN+(X_WIDTH/2));
          y = (filed_y * Y_WIDTH) + (Y_MIN + (Y_WIDTH / 2));
          if (is_first_player_turn == true)
          {
            DrawX(hdc, x, y);
            is_field_ocupied_by_first_player[filed_x][filed_y] = true;
            result_tab[3 * (filed_y)+(filed_x)] = 1;
            licznik++;
          }
          else
          {
            DrawO(hdc, x, y);
            is_field_ocupied_by_second_player[filed_x][filed_y] = true;
            result_tab[3 * (filed_y)+(filed_x)] = 2;
            licznik++;
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
		PostQuitMessage(0); //Komunikat polecenia zako�czenia aplikacji
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

void ClearBoard(HDC x)
{
  COLORREF color = GetPixel(x, 80, 80);
  COLORREF BgColorR = GetRValue(color);
  COLORREF BgColorG = GetGValue(color);
  COLORREF BgColorB = GetBValue(color);
  HPEN h_my_pen = CreatePen(PS_SOLID, 2, RGB(BgColorR, BgColorG, BgColorB));
  SelectObject(x, h_my_pen);
  for (int i = 0; i<X_MAX; ++i)
  {
    MoveToEx(x, i, Y_MIN, nullptr);
    LineTo(x, i, Y_MAX);
  }
  DeleteObject(h_my_pen);
}
void DrawBoard(HDC x)
{
  HPEN h_my_pen = CreatePen(PS_SOLID, 2, COLOR_BOARD);
  SelectObject(x, h_my_pen);
  MoveToEx(x, X_MIN+ X_WIDTH, Y_MIN, nullptr);
  LineTo(x, X_MIN + X_WIDTH, Y_MAX);
  MoveToEx(x, X_MAX- X_WIDTH, Y_MIN, nullptr);
  LineTo(x, X_MAX - X_WIDTH, Y_MAX);
  MoveToEx(x, X_MIN, Y_MIN+ Y_WIDTH, nullptr);
  LineTo(x, X_MAX, Y_MIN + Y_WIDTH);
  MoveToEx(x, X_MIN, Y_MAX- Y_WIDTH, nullptr);
  LineTo(x, X_MAX, Y_MAX - Y_WIDTH);
  DeleteObject(h_my_pen);

}
void DrawX(HDC hdc,int x, int y)
{
  HPEN h_my_pen = CreatePen(PS_SOLID, 2, COLOR_X);
  SelectObject(hdc, h_my_pen);
  MoveToEx(hdc, x - 10, y - 10, NULL);
  LineTo(hdc, x + 10, y + 10);
  MoveToEx(hdc, x - 10, y + 10, NULL);
  LineTo(hdc, x + 10, y - 10);
  DeleteObject(h_my_pen);
}
void DrawO(HDC hdc, int x, int y)
{
  HPEN h_my_pen = CreatePen(PS_SOLID, 2, COLOR_O);
  SelectObject(hdc, h_my_pen);
  Ellipse(hdc, x-15, y-15, x+15, y+15);
  DeleteObject(h_my_pen);

  
}
void RedrawBoard(HDC hdc)
{
  for (int i_field_x = 0; i_field_x < TAB_SIZE_X; ++i_field_x)
  {
    for (int i_field_y = 0; i_field_y < TAB_SIZE_Y; ++i_field_y)
    {
      if (is_field_ocupied_by_first_player[i_field_x][i_field_y] == true)
      {
        DrawX(hdc, (i_field_x * X_WIDTH + (X_MIN + (X_WIDTH / 2))), (i_field_y * Y_WIDTH + (Y_MIN + (Y_WIDTH / 2))));
      }
      if (is_field_ocupied_by_second_player[i_field_x][i_field_y] == true)
      {
        DrawO(hdc, (i_field_x * X_WIDTH + (X_MIN + (X_WIDTH / 2))), (i_field_y * Y_WIDTH + (Y_MIN + (Y_WIDTH / 2))));
      }
    }
  }
}
int GameResult(HWND hwndDlg, HDC x)
{
  HPEN h_my_pen = CreatePen(PS_SOLID, 2, RGB(255, 125, 125));
  SelectObject(x, h_my_pen);
  for (int i = 0; i < 3; i++)
  {
    if ((result_tab[3 * i] == result_tab[3 * i + 1]) && (result_tab[3 * i] == result_tab[3 * i + 2]))
    {
      if (result_tab[3 * i] == 1)
      {
        wsprintf(sz_text, "Krzy�yk wygrywa");
        TextOut(x, 10, 230, sz_text, strlen(sz_text));
        is_game_on = false;
        wsprintf(sz_text, "START");
        SetWindowText(GetDlgItem(hwndDlg, IDC_BUTTON10), sz_text);
        return TRUE;
      }
      else if (result_tab[3 * i] == 2)
      {
        wsprintf(sz_text, "K�ko wygrywa");
        TextOut(x, 10, 230, sz_text, strlen(sz_text));
        is_game_on = false;
        wsprintf(sz_text, "START");
        SetWindowText(GetDlgItem(hwndDlg, IDC_BUTTON10), sz_text);
        return TRUE;
      }
    }
    if ((result_tab[i] == result_tab[i + 3]) && (result_tab[i] == result_tab[i + 6]))
    {
      if (result_tab[i] == 1)
      {
        wsprintf(sz_text, "Krzy�yk wygrywa");
        TextOut(x, 10, 230, sz_text, strlen(sz_text));
        wsprintf(sz_text, "START");
        SetWindowText(GetDlgItem(hwndDlg, IDC_BUTTON10), sz_text);
        is_game_on = false;
        return TRUE;
      }
      else if (result_tab[i] == 2)
      {
        wsprintf(sz_text, "K�ko wygrywa");
        TextOut(x, 10, 230, sz_text, strlen(sz_text));
        is_game_on = false;
        wsprintf(sz_text, "START");
        SetWindowText(GetDlgItem(hwndDlg, IDC_BUTTON10), sz_text);
        return TRUE;
      }

    }
  }
  if ((result_tab[0] == result_tab[4] && result_tab[0] == result_tab[8]) || (result_tab[4] == result_tab[6] && result_tab[4] == result_tab[2]))
  {
    if (result_tab[4] == 1)
    {
      wsprintf(sz_text, "Krzy�yk wygrywa");
      TextOut(x, 10, 230, sz_text, strlen(sz_text));
      is_game_on = false;
      wsprintf(sz_text, "START");
      SetWindowText(GetDlgItem(hwndDlg, IDC_BUTTON10), sz_text);
      return TRUE;
    }
    else if (result_tab[4] == 2)
    {
      wsprintf(sz_text, "K�ko wygrywa");
      TextOut(x, 10, 230, sz_text, strlen(sz_text));
      is_game_on = false;
      wsprintf(sz_text, "START");
      SetWindowText(GetDlgItem(hwndDlg, IDC_BUTTON10), sz_text);
      return TRUE;
    }
  }
  if (licznik == 9)
  {
    wsprintf(sz_text, "Remis");
    TextOut(x, 10, 230, sz_text, strlen(sz_text));
    is_game_on = false;
    wsprintf(sz_text, "START");
    SetWindowText(GetDlgItem(hwndDlg, IDC_BUTTON10), sz_text);
  }
  DeleteObject(h_my_pen);
}
//Rysowanie
//LineTo
//MoveToEx
//Rectangle
//TextOut
//GetPixel
//SetPixel
// TextOut(hdc, 0, 0, sz_text, strlen(sz_text)); //napsize start w lewym gornym, ogarn�c