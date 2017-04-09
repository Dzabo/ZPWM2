#include <Windows.h>
#include "res.h"
#include <stdio.h>

#define x_min 150
#define x_max 300
#define y_min 100
#define y_max 250
#define x_width 50
#define y_width 50

int result_tab[9] = { 0 };
int licznik = 0;
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
int GameResult(HWND hwndDlg, HDC x);


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
          for (int i = 0; i < 3; i++)
          {
            for (int j = 0; j < 3; j++)
            {
              is_field_ocupied_by_first_player[i][j] = false;
              is_field_ocupied_by_second_player[i][j] = false;
              result_tab[3 * j + i] = 0;
            }
          }
          wsprintf(sz_text, "Gra siê rozpoczê³¹");
          SetWindowText(hwndStatic5, sz_text);
          wsprintf(sz_text, "STOP");
          SetWindowText(hwndButton10, sz_text);
          is_game_on = true;
        }
        else
        {
          wsprintf(sz_text, "START");
          SetWindowText(hwndButton10, sz_text);
          wsprintf(sz_text, "Rozpocznij grê");
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
      if ((x > x_min && x < x_max) && ((y > y_min) && (y < y_max)))
      {
        //x=((x-iMinBoardX)/iWidthBoardX)*iWidthBoardX+iMaxBoardX/2);
        //y=((y-iMinBoardY)/iWidthBoardY)*iWidthBoardY+iMaxBoardY/2);
        int filed_x = ((x - x_min) / x_width);
        int filed_y = ((y - y_min) / y_width);
        if (is_field_ocupied_by_first_player[filed_x][filed_y] == false
          && is_field_ocupied_by_second_player[filed_x][filed_y] == false)
        {
          x = (filed_x * x_width) + (x_min+(x_width/2));
          y = (filed_y * y_width) + (y_min + (x_width / 2));
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
    return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
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

void ClearBoard(HDC x)
{
  COLORREF color = GetPixel(x, 80, 80);
  COLORREF BgColorR = GetRValue(color);
  COLORREF BgColorG = GetGValue(color);
  COLORREF BgColorB = GetBValue(color);
  HPEN h_my_pen = CreatePen(PS_SOLID, 2, RGB(BgColorR, BgColorG, BgColorB));
  SelectObject(x, h_my_pen);
  for (int i = 0; i<300; i++)
  {
    MoveToEx(x, i, 100, NULL);
    LineTo(x, i, 300);
  }
  DeleteObject(h_my_pen);
}
void DrawBoard(HDC x)
{
  HPEN h_my_pen = CreatePen(PS_SOLID, 2, RGB(255, 125, 125));
  SelectObject(x, h_my_pen);
  MoveToEx(x, x_min+50, y_min, NULL);
  LineTo(x, x_min + 50, y_max);
  MoveToEx(x, x_max-50, y_min, NULL);
  LineTo(x, x_max - 50, y_max);
  MoveToEx(x, x_min, y_min+50, NULL);
  LineTo(x, x_max, y_min + 50);
  MoveToEx(x, x_min, y_max-50, NULL);
  LineTo(x, x_max, y_max - 50);
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
        DrawX(hdc, (i_field_x * x_width + (x_min + (x_width / 2))), (i_field_y * y_width + (y_min + (y_width / 2))));
      }
      if (is_field_ocupied_by_second_player[i_field_x][i_field_y] == true)
      {
        DrawO(hdc, (i_field_x * x_width + (x_min + (x_width / 2))), (i_field_y * y_width + (y_min + (y_width / 2))));
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
        wsprintf(sz_text, "Krzy¿yk wygrywa");
        TextOut(x, 10, 230, sz_text, strlen(sz_text));
        is_game_on = false;
        wsprintf(sz_text, "START");
        SetWindowText(GetDlgItem(hwndDlg, IDC_BUTTON10), sz_text);
        return TRUE;
      }
      else if (result_tab[3 * i] == 2)
      {
        wsprintf(sz_text, "Kó³ko wygrywa");
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
        wsprintf(sz_text, "Krzy¿yk wygrywa");
        TextOut(x, 10, 230, sz_text, strlen(sz_text));
        wsprintf(sz_text, "START");
        SetWindowText(GetDlgItem(hwndDlg, IDC_BUTTON10), sz_text);
        is_game_on = false;
        return TRUE;
      }
      else if (result_tab[i] == 2)
      {
        wsprintf(sz_text, "Kó³ko wygrywa");
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
      wsprintf(sz_text, "Krzy¿yk wygrywa");
      TextOut(x, 10, 230, sz_text, strlen(sz_text));
      is_game_on = false;
      wsprintf(sz_text, "START");
      SetWindowText(GetDlgItem(hwndDlg, IDC_BUTTON10), sz_text);
      return TRUE;
    }
    else if (result_tab[4] == 2)
    {
      wsprintf(sz_text, "Kó³ko wygrywa");
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
    return TRUE;
  }
  DeleteObject(h_my_pen);
  return TRUE;
}
//Rysowanie
//LineTo
//MoveToEx
//Rectangle
//TextOut
//GetPixel
//SetPixel
// TextOut(hdc, 0, 0, sz_text, strlen(sz_text)); //napsize start w lewym gornym, ogarn¹c