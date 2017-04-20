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

HINSTANCE hInst;
HBITMAP hBitmapGameBoard;
HBITMAP hBitmapX;
HBITMAP hBitmapO;
HBITMAP hBitmapRes;
HBITMAP hBitmapStart;


bool is_game_on = false;
bool is_first_player_turn = true;
bool is_field_ocupied_by_first_player[TAB_SIZE_X][TAB_SIZE_Y];
bool is_field_ocupied_by_second_player[TAB_SIZE_X][TAB_SIZE_Y];
void DrawBoard(HDC x);
void DrawX(HDC hdc, int x, int y);
void DrawO(HDC hdc, int x, int y);
void RedrawBoard(HDC hdc);
void ClearBoard(HDC hdc);
void GameResult(HWND hwndDlg, HDC x);
void CreateStart(HWND hwnd, HINSTANCE hInstance);

LRESULT CALLBACK ButtonWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_PAINT:
  {
    HDC hdc = GetDC(hwnd);
    HDC hDCBitmap;
    hDCBitmap = CreateCompatibleDC(hdc);
    SelectObject(hDCBitmap, hBitmapStart);
    BitBlt(hdc, 0, 0, 100, 40, hDCBitmap, 0, 0, SRCCOPY);
    DeleteDC(hDCBitmap);
    ReleaseDC(hwnd, hdc);
    return FALSE;
  }
  default:return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
}

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
    {
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON10://Klikniêcie na nsz przycisk Button1
      {
        HDC hdc = GetDC(hwndDlg);
        //ClearBoard(hdc);
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
      default:;
      }
    }
    return TRUE;
    default:;
    }
  }
  return TRUE;
  case WM_LBUTTONDOWN:
  {
    HDC hdc = GetDC(hwndDlg);
    GameResult(hwndDlg, hdc);
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
          x = (filed_x * X_WIDTH) + (X_MIN + (X_WIDTH / 2));
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
  case WM_INITDIALOG:
  {
    //WNDPROC wpOrgButtonProc = (WNDPROC)SetWindowLong(hwndButton10, -4, (LONG)ButtonWndProc);
    CreateStart(hwndDlg, hInst); // RYSOWANIE STARTU, BRAK FUNKCJONALNOSCI
    hBitmapGameBoard = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOARD));
    hBitmapO = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_O));
    hBitmapX = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_X));
    hBitmapRes = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RESET));
    hBitmapStart = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_START));

  }
  return TRUE;
  case WM_PAINT:
  {
    CreateStart(hwndDlg, hInst);
    HDC hdc = GetDC(hwndDlg);
    DrawBoard(hdc);
    //    RedrawBoard(hdc);
    ReleaseDC(hwndDlg, hdc);
    return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
  }
  case WM_CLOSE:
  {
    DestroyWindow(hwndDlg); // zniszczenie okna
    PostQuitMessage(0); //Komunikat polecenia zakoñczenia aplikacji
  }
  return TRUE;
  default: ;
  }
  return FALSE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  hInst = hInstance;
  HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);

  MSG msg = {};
  while (GetMessage(&msg, nullptr, 0, 0))
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
  for (int i = 0; i < X_MAX; ++i)
  {
    MoveToEx(x, i, Y_MIN, nullptr);
    LineTo(x, i, Y_MAX);
  }
  DeleteObject(h_my_pen);
}
void DrawBoard(HDC x)
{
  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(x);
  SelectObject(hDCBitmap, hBitmapGameBoard);
  BitBlt(x, X_MIN, Y_MIN, 150, 150, hDCBitmap, 0, 0, SRCCOPY); //szerokosc,wysokosc
  DeleteDC(hDCBitmap);

}
void DrawX(HDC hdc, int x, int y)
{
  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hdc);
  SelectObject(hDCBitmap, hBitmapX);
  BitBlt(hdc, x - 23, y - 23, 47, 47, hDCBitmap, 0, 0, SRCCOPY); //szerokosc,wysokosc
  DeleteDC(hDCBitmap);
}
void DrawO(HDC hdc, int x, int y)
{
  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hdc);
  SelectObject(hDCBitmap, hBitmapO);
  BitBlt(hdc, x - 23, y - 23, 47, 47, hDCBitmap, 0, 0, SRCCOPY); //szerokosc,wysokosc
  DeleteDC(hDCBitmap);
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
void GameResult(HWND hwndDlg, HDC x)
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
      }
      else if (result_tab[3 * i] == 2)
      {
        wsprintf(sz_text, "Kó³ko wygrywa");
        TextOut(x, 10, 230, sz_text, strlen(sz_text));
        is_game_on = false;
        wsprintf(sz_text, "START");
        SetWindowText(GetDlgItem(hwndDlg, IDC_BUTTON10), sz_text);
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
      }
      else if (result_tab[i] == 2)
      {
        wsprintf(sz_text, "Kó³ko wygrywa");
        TextOut(x, 10, 230, sz_text, strlen(sz_text));
        is_game_on = false;
        wsprintf(sz_text, "START");
        SetWindowText(GetDlgItem(hwndDlg, IDC_BUTTON10), sz_text);
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
    }
    else if (result_tab[4] == 2)
    {
      wsprintf(sz_text, "Kó³ko wygrywa");
      TextOut(x, 10, 230, sz_text, strlen(sz_text));
      is_game_on = false;
      wsprintf(sz_text, "START");
      SetWindowText(GetDlgItem(hwndDlg, IDC_BUTTON10), sz_text);
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
void CreateStart(HWND hwnd, HINSTANCE hInstance)
{
  HWND button = CreateWindow("BUTTON",
    "My Button",
    WS_VISIBLE | WS_CHILD | BS_BITMAP,
    20, 100, 100, 40,
    hwnd,
    NULL,
    hInstance,
    NULL);
  SendMessage(button, BM_SETIMAGE, /*(WPARAM)*/IMAGE_BITMAP, (LPARAM)hBitmapStart);
}
//Rysowanie
//LineTo
//MoveToEx
//Rectangle
//TextOut
//GetPixel
//SetPixel
// TextOut(hdc, 0, 0, sz_text, strlen(sz_text)); //napsize start w lewym gornym, ogarn¹c