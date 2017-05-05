#include <Windows.h>
#include "res.h"
#include <stdio.h>
#include <gl\gl.h>
#include <gl\glu.h>

#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"glu32.lib")


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
HDC hDc = NULL;
HGLRC hRc = NULL;

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
GLint DrawGLScene();
void DrawCube1(int a, int b, int c);
void DrawCube(int xmin,int xmax, int ymin, int ymax, int zmin, int zmax, int r, int g, int b);
void DrawSide(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax, int r, int g, int b);
void glDrawBoard();
void glDrawX(int a, int b, int c);
void glDrawO(int a, int b, int c);

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
    //zmiana stylu okna
    SetWindowLong(hwndDlg, GWL_STYLE, CS_HREDRAW | CS_VREDRAW | CS_OWNDC | WS_OVERLAPPEDWINDOW);
    //SetClassLong(hwndDlg,GCL_STYLE,CS_HREDRAW|CS_VREDRAW|CS_OWNDC); //Silniejsza funkcja SetWindowLong

    //Stworzyæ maszynê stanu
    static PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR), //rozmiar
      1,                            //wersja
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //flagi
      PFD_TYPE_RGBA,//typ pixeli
      16,//
      0,0,0,0,0,0,
      0,
      0,
      0,
      0,0,0,0,
      16,
      0,
      0,
      PFD_MAIN_PLANE,
      0,
      0,0,0
    };
    //zmienne pomocnicze
    GLuint PixelFormat;

    hDc = GetDC(hwndDlg);
    PixelFormat = ChoosePixelFormat(hDc, &pfd); //sprawdzamy czy nasz format graficzny jest wspierany
                                                //ustawiamy format
    SetPixelFormat(hDc, PixelFormat, &pfd);
    //tworzymy maszynê stanu
    hRc = wglCreateContext(hDc);
    wglMakeCurrent(hDc, hRc);
    //Podpi¹æ j¹ pod okno
    return TRUE;
  }
  case WM_SIZE:
  {
    GLint iWidth = LOWORD(lParam);
    GLint iHeight = HIWORD(lParam);

    if (iWidth == 0)iWidth = 1;
    if (iHeight == 0)iHeight = 1;


    glViewport(0, 0, iWidth, iHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glLoadMatrix();

    gluPerspective(45.0f, iWidth / iHeight, 0.1f, 1000.0f); //k¹t widzenia, Rozmiar okna, Z min, Z max

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return TRUE;
  }
  /*case WM_PAINT:
  {
    CreateStart(hwndDlg, hInst);
    HDC hdc = GetDC(hwndDlg);
    DrawBoard(hdc);
    //    RedrawBoard(hdc);
    ReleaseDC(hwndDlg, hdc);
    return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
  }*/
  case WM_CLOSE:
  {
    DestroyWindow(hwndDlg); // zniszczenie okna
    PostQuitMessage(0); //Komunikat polecenia zakoñczenia aplikacji
  }
  return TRUE;
  default:;
  }
  return FALSE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  hInst = hInstance;
  HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);

  MSG msg = {};
  /* while (GetMessage(&msg, nullptr, 0, 0))
   {
     TranslateMessage(&msg);
     DispatchMessage(&msg);

   }*/
  BOOL bDone = false;
  while (!bDone)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //sprawdza czy jest cos do obs³u¿enia, jak nie to pozwala programowi przejœæ dalej
    {
      if (msg.message == WM_QUIT)bDone = true;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      DrawGLScene();
      //DrawCube1(1,1,1);
      glDrawBoard();
      glDrawX(-25,-25,0);
      glDrawO(35, 35, 0);

      SwapBuffers(hDc);
    }
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

GLint DrawGLScene()
{
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  static GLfloat fAngle = 0.0;
  fAngle += 0.01f;
  glTranslatef(0, 0, -300);
  glRotatef(fAngle, 0, 1, 0);



  /*glBegin(GL_LINES);
    glColor3d(1,0,0);
    glVertex3d(10,0,0);
    glVertex3d(-10,0,0);

    glColor3d(0, 1, 0);
    glVertex3d(0, 10, 0);
    glVertex3d(0, -10, 0);

    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 10);
    glVertex3d(0, 0, -10);

    glEnd();*/
  return 1;
}

void DrawCube1(int a, int b, int c)
{

  glBegin(GL_QUADS);
  DrawCube(0,50*a,0,50*b,0,50*c,1,0,0);
  /*glColor3d(0, -1, 0);
  glNormal3d(0, -1, 0);
  glVertex3d(0, 0, 0);
  glVertex3d(0, 0, c);
  glVertex3d(a, 0, c);
  glVertex3d(a, 0, 0);

  glColor3d(0, 1, 0);
  glNormal3d(0, -1, 0);
  glVertex3d(a, 0, 0);
  glVertex3d(a, 0, c);
  glVertex3d(a, b, c);
  glVertex3d(0, b, 0);

  glColor3d(1, 0, 0);
  glNormal3d(0, 1, 0);
  glVertex3d(a, b, 0);
  glVertex3d(a, b, c);
  glVertex3d(0, b, c);
  glVertex3d(0, b, 0);

  glColor3d(0, 1, 1);
  glNormal3d(-1, 0, 0);
  glVertex3d(0, b, 0);
  glVertex3d(0, b, c);
  glVertex3d(0, 0, c);
  glVertex3d(0, 0, 0);

  glColor3d(1, 0, 1);
  glNormal3d(0, 0, 1);
  glVertex3d(0, 0, c);
  glVertex3d(0, b, c);
  glVertex3d(a, b, c);
  glVertex3d(a, 0, c);

  glColor3d(1, 1, 0);
  glNormal3d(0, 0, -1);
  glVertex3d(0, 0, 0);
  glVertex3d(a, 0, 0);
  glVertex3d(a, b, 0);
  glVertex3d(0, b, 0);*/
  glEnd();

}

void glDrawBoard()
{
  glBegin(GL_QUADS);
  DrawCube(-85,85,-35,-25,-10,0,1,1,1);
  DrawCube(-85,85,25,35,-10,0, 1, 1, 1);
  DrawCube(-35,-25,-85,85,-10,0, 1, 1, 1);
  DrawCube(25,35,-85,85,-10,0, 1, 1, 1);
  glEnd();

}
void glDrawX(int a, int b, int c)
{
  glBegin(GL_TRIANGLES);
  /* STARE
  //dó³ 
  glColor3d(1, 1, 0);

  glVertex3d(a + 5, b + 5, c - 0);
  glVertex3d(a + 15, b + 5, c - 0);
  glVertex3d(a + 11, b + 8, c - 10);//

  glVertex3d(a + 11, b + 8, c - 10);
  glVertex3d(a + 21, b + 8, c - 10);
  glVertex3d(a + 15, b + 5, c - 0);//

  glVertex3d(a + 38, b + 5, c - 0);
  glVertex3d(a + 28, b + 5, c - 0);
  glVertex3d(a + 34, b + 8, c - 10);//

  glVertex3d(a + 34, b + 8, c - 10);
  glVertex3d(a + 44, b + 8, c - 10);
  glVertex3d(a + 38, b + 5, c - 0);//

                                   //góra
  glColor3d(1, 1, 0);

  glVertex3d(a + 5, b + 41, c - 0);
  glVertex3d(a + 15, b + 41, c - 0);
  glVertex3d(a + 11, b + 44, c - 10);//

  glVertex3d(a + 11, b + 44, c - 10);
  glVertex3d(a + 21, b + 44, c - 10);
  glVertex3d(a + 15, b + 41, c - 0);//

  glVertex3d(a + 38, b + 41, c - 0);
  glVertex3d(a + 28, b + 41, c - 0);
  glVertex3d(a + 34, b + 44, c - 10);//

  glVertex3d(a + 34, b + 44, c - 10);
  glVertex3d(a + 44, b + 44, c - 10);
  glVertex3d(a + 38, b + 41, c - 0);//

                                    //front
  glColor3d(1, 0, 1);

  glVertex3d(a + 5, b + 5, c - 0);
  glVertex3d(a + 15, b + 5, c - 0);
  glVertex3d(a + 38, b + 41, c - 0);//

  glVertex3d(a + 38, b + 41, c - 0);
  glVertex3d(a + 28, b + 41, c - 0);
  glVertex3d(a + 5, b + 5, c - 0);//

  glVertex3d(a + 5, b + 41, c - 0);
  glVertex3d(a + 15, b + 41, c - 0);
  glVertex3d(a + 38, b + 5, c - 0);//

  glVertex3d(a + 38, b + 5, c - 0);
  glVertex3d(a + 28, b + 5, c - 0);
  glVertex3d(a + 5, b + 41, c - 0);//

                                   //ty³
  glColor3d(0, 1, 0);
  glVertex3d(a + 11, b + 8, c - 10);
  glVertex3d(a + 21, b + 8, c - 10);
  glVertex3d(a + 44, b + 44, c - 10);//

  glVertex3d(a + 44, b + 44, c - 10);
  glVertex3d(a + 34, b + 44, c - 10);
  glVertex3d(a + 11, b + 8, c - 10);//

  glVertex3d(a + 11, b + 44, c - 10);
  glVertex3d(a + 21, b + 44, c - 10);
  glVertex3d(a + 44, b + 8, c - 10);//

  glVertex3d(a + 44, b + 8, c - 10);
  glVertex3d(a + 34, b + 8, c - 10);
  glVertex3d(a + 11, b + 44, c - 10);//
  
  //BOKI DOROBIC
  */

  //front
  {
    glColor3d(1, 0, 1);
    glNormal3d(1, 0, 0);
    glVertex3d(a + 5, b + 5, c - 0);
    glVertex3d(a + 15, b + 5, c - 0);
    glVertex3d(a + 38, b + 41, c - 0);//

    glVertex3d(a + 38, b + 41, c - 0);
    glVertex3d(a + 28, b + 41, c - 0);
    glVertex3d(a + 5, b + 5, c - 0);//

    glVertex3d(a + 5, b + 41, c - 0);
    glVertex3d(a + 15, b + 41, c - 0);
    glVertex3d(a + 38, b + 5, c - 0);//

    glVertex3d(a + 38, b + 5, c - 0);
    glVertex3d(a + 28, b + 5, c - 0);
    glVertex3d(a + 5, b + 41, c - 0);
  }

  //ty³
  {
    glColor3d(1, 1, 0);
    glNormal3d(-1, 0, 0);
    glVertex3d(a + 5, b + 5, c - 10);
    glVertex3d(a + 15, b + 5, c - 10);
    glVertex3d(a + 38, b + 41, c - 10);//

    glVertex3d(a + 38, b + 41, c - 10);
    glVertex3d(a + 28, b + 41, c - 10);
    glVertex3d(a + 5, b + 5, c - 10);//

    glVertex3d(a + 5, b + 41, c - 10);
    glVertex3d(a + 15, b + 41, c - 10);
    glVertex3d(a + 38, b + 5, c - 10);//

    glVertex3d(a + 38, b + 5, c - 10);
    glVertex3d(a + 28, b + 5, c - 10);
    glVertex3d(a + 5, b + 41, c - 10);
  }

  //dó³
  {
    glColor3d(0, 0, 1);

    //lewy dol
    glNormal3d(0, -1, 0);
    glVertex3d(a + 5, b + 5, c - 0);
    glVertex3d(a + 15, b + 5, c - 0);
    glVertex3d(a + 5, b + 5, c - 10);

    glVertex3d(a + 15, b + 5, c - 10);
    glVertex3d(a + 5, b + 5, c - 10);
    glVertex3d(a + 15, b + 5, c - 0);

    //lewy gora
    glNormal3d(0, 1, 0);
    glVertex3d(a + 5, b + 41, c - 0);
    glVertex3d(a + 15, b + 41, c - 0);
    glVertex3d(a + 5, b + 41, c - 10);


    glVertex3d(a + 15, b + 41, c - 10);
    glVertex3d(a + 5, b + 41, c - 10);
    glVertex3d(a + 15, b + 41, c - 0);
  }
  //góra
  {
    glColor3d(0, 0, 1);

    //prawy dol
    glNormal3d(0, -1, 0);
    glVertex3d(a + 28, b + 5, c - 0);
    glVertex3d(a + 38, b + 5, c - 0);
    glVertex3d(a + 28, b + 5, c - 10);

    glVertex3d(a + 38, b + 5, c - 10);
    glVertex3d(a + 28, b + 5, c - 10);
    glVertex3d(a + 38, b + 5, c - 0);

    //prawy gora
    glNormal3d(0, 1, 0);
    glVertex3d(a + 28, b + 41, c - 0);
    glVertex3d(a + 38, b + 41, c - 0);
    glVertex3d(a + 28, b + 41, c - 10);


    glVertex3d(a + 38, b + 41, c - 10);
    glVertex3d(a + 28, b + 41, c - 10);
    glVertex3d(a + 38, b + 41, c - 0);
  }
  //boki
  {
    glColor3d(0, 1, 1);

    //1
    glNormal3d(-1, 1, 0);
    glVertex3d(a + 5, b + 5, c - 0);
    glVertex3d(a + 28, b + 41, c - 0);
    glVertex3d(a + 5, b + 5, c - 10);//

    glVertex3d(a + 28, b + 41, c - 0);
    glVertex3d(a + 28, b + 41, c - 10);
    glVertex3d(a + 5, b + 5, c - 10);//

    //2
    glNormal3d(1, -1, 0);
    glVertex3d(a + 15, b + 5, c - 0);
    glVertex3d(a + 38, b + 41, c - 0);
    glVertex3d(a + 15, b + 5, c - 10);//

    glVertex3d(a + 38, b + 41, c - 0);
    glVertex3d(a + 38, b + 41, c - 10);
    glVertex3d(a + 15, b + 5, c - 10);//

    //3
    glVertex3d(a + 5, b + 41, c - 0);
    glVertex3d(a + 28, b + 5, c - 0);
    glVertex3d(a + 5, b + 41, c - 10);//

    glVertex3d(a + 28, b + 5, c - 0);
    glVertex3d(a + 28, b + 5, c - 10);
    glVertex3d(a + 5, b + 41, c - 10);//

    //4
    glVertex3d(a + 15, b + 41, c - 0);
    glVertex3d(a + 38, b + 5, c - 0);
    glVertex3d(a + 15, b + 41, c - 10);//

    glVertex3d(a + 38, b + 5, c - 0);
    glVertex3d(a + 38, b + 5, c - 10);
    glVertex3d(a + 15, b + 41, c - 10);//


  }
  glEnd();
}

void glDrawO(int a, int b, int c)
{
  glBegin(GL_TRIANGLES);
  //przód
  {
    glColor3d(1, 1, 0);

    //lewy
    glVertex3d(a + 5, b + 16, c + 0);
    glVertex3d(a + 16, b + 16, c + 0);
    glVertex3d(a + 5, b + 27, c + 0);//

    glVertex3d(a + 16, b + 27, c + 0);
    glVertex3d(a + 16, b + 16, c + 0);
    glVertex3d(a + 5, b + 27, c + 0);//

    //skos
    glVertex3d(a + 5, b + 27, c + 0);
    glVertex3d(a + 16, b + 27, c + 0);
    glVertex3d(a + 16, b + 38, c + 0);//

    //gora
    glVertex3d(a + 16, b + 27, c + 0);
    glVertex3d(a + 16, b + 38, c + 0);
    glVertex3d(a + 27, b + 27, c + 0);//

    glVertex3d(a + 16, b + 38, c + 0);
    glVertex3d(a + 27, b + 38, c + 0);
    glVertex3d(a + 27, b + 27, c + 0);//

    //skos
    glVertex3d(a + 27, b + 27, c + 0);
    glVertex3d(a + 27, b + 38, c + 0);
    glVertex3d(a + 38, b + 27, c + 0);//

    //prawy
    glVertex3d(a + 27, b + 16, c + 0);
    glVertex3d(a + 38, b + 16, c + 0);
    glVertex3d(a + 27, b + 27, c + 0);//

    glVertex3d(a + 38, b + 27, c + 0);
    glVertex3d(a + 38, b + 16, c + 0);
    glVertex3d(a + 27, b + 27, c + 0);//

    //skos
    glVertex3d(a + 27, b + 5, c + 0);
    glVertex3d(a + 27, b + 16, c + 0);
    glVertex3d(a + 38, b + 16, c + 0);//

    //dol
    glVertex3d(a + 16, b + 5, c + 0);
    glVertex3d(a + 16, b + 16, c + 0);
    glVertex3d(a + 27, b + 5, c + 0);//

    glVertex3d(a + 16, b + 16, c + 0);
    glVertex3d(a + 27, b + 16, c + 0);
    glVertex3d(a + 27, b + 5, c + 0);//

    //skos
    glVertex3d(a + 5, b + 16, c + 0);
    glVertex3d(a + 16, b + 16, c + 0);
    glVertex3d(a + 16, b + 5, c + 0);//
  }
  //ty³
  {
    glColor3d(0, 1, 1);

    //lewy
    glVertex3d(a + 5, b + 16, c - 10);
    glVertex3d(a + 16, b + 16, c - 10);
    glVertex3d(a + 5, b + 27, c - 10);//

    glVertex3d(a + 16, b + 27, c - 10);
    glVertex3d(a + 16, b + 16, c - 10);
    glVertex3d(a + 5, b + 27, c - 10);//

    //skos
    glVertex3d(a + 5, b + 27, c - 10);
    glVertex3d(a + 16, b + 27, c - 10);
    glVertex3d(a + 16, b + 38, c - 10);//

    //gora
    glVertex3d(a + 16, b + 27, c - 10);
    glVertex3d(a + 16, b + 38, c - 10);
    glVertex3d(a + 27, b + 27, c - 10);//

    glVertex3d(a + 16, b + 38, c - 10);
    glVertex3d(a + 27, b + 38, c - 10);
    glVertex3d(a + 27, b + 27, c - 10);//

    //skos
    glVertex3d(a + 27, b + 27, c - 10);
    glVertex3d(a + 27, b + 38, c - 10);
    glVertex3d(a + 38, b + 27, c - 10);//

    //prawy
    glVertex3d(a + 27, b + 16, c - 10);
    glVertex3d(a + 38, b + 16, c - 10);
    glVertex3d(a + 27, b + 27, c - 10);//

    glVertex3d(a + 38, b + 27, c - 10);
    glVertex3d(a + 38, b + 16, c - 10);
    glVertex3d(a + 27, b + 27, c - 10);//

    //skos
    glVertex3d(a + 27, b + 5, c - 10);
    glVertex3d(a + 27, b + 16, c - 10);
    glVertex3d(a + 38, b + 16, c - 10);//

    //dol
    glVertex3d(a + 16, b + 5, c - 10);
    glVertex3d(a + 16, b + 16, c - 10);
    glVertex3d(a + 27, b + 5, c - 10);//

    glVertex3d(a + 16, b + 16, c - 10);
    glVertex3d(a + 27, b + 16, c - 10);
    glVertex3d(a + 27, b + 5, c - 10);//

    //skos
    glVertex3d(a + 5, b + 16, c - 10);
    glVertex3d(a + 16, b + 16, c - 10);
    glVertex3d(a + 16, b + 5, c - 10);//
  }
  //boki
  {
    //lewy
    DrawSide(a + 5, a + 5, b + 16, b + 27, c - 10, c, 0, 1, 0);
    //skos
    DrawSide(a + 5, a + 16, b + 27, b + 38, c - 10, c, 0, 1, 0);
    //gora
    DrawSide(a + 16, a + 27, b + 38, b + 38, c - 10, c, 0, 1, 0);
    //skos
    DrawSide(a + 38, a + 27, b + 27, b + 38, c - 10, c, 0, 1, 0);
    //prawy
    DrawSide(a + 38, a + 38, b + 16, b + 27, c - 10, c, 0, 1, 0);
    //skos
    DrawSide(a + 27, a + 38, b + 5, b + 16, c - 10, c, 0, 1, 0);
    //dol
    DrawSide(a + 16, a + 27, b + 5, b + 5, c - 10, c, 0, 1, 0);
    //skos
    DrawSide(a + 5, a + 16, b + 16, b + 5, c - 10, c, 0, 1, 0);
    //srodek lewy
    DrawSide(a + 16, a + 16, b + 16, b + 27, c - 10, c, 0, 1, 0);
    //srodek góra
    DrawSide(a + 16, a + 27, b + 27, b + 27, c - 10, c, 0, 1, 0);
    //srodek prawy
    DrawSide(a + 27, a + 27, b + 16, b + 27, c - 10, c, 0, 1, 0);
    //srodek dol
    DrawSide(a + 16, a + 27, b + 16, b + 16, c - 10, c, 0, 1, 0);
  }
  glEnd();

}

void DrawSide(int xmin, int xmax, int ymin, int ymax,int zmin,int zmax, int r, int g, int b)
{
  glColor3d(r,g,b);

  glVertex3d(xmin,ymin,zmax);
  glVertex3d(xmax,ymax,zmax);
  glVertex3d(xmax,ymax,zmin);

  glVertex3d(xmax, ymax, zmin);
  glVertex3d(xmin,ymin, zmin);
  glVertex3d(xmin, ymin, zmax);

}

void DrawCube(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax, int r, int g, int b)
{

  glColor3d(r,g,b);
  glNormal3d(0, -1, 0);
  glVertex3d(xmin, ymin, zmin);
  glVertex3d(xmin, ymin, zmax);
  glVertex3d(xmax, ymin, zmax);
  glVertex3d(xmax, ymin, zmax);

  glNormal3d(0, -1, 0);
  glVertex3d(xmax, ymin, zmin);
  glVertex3d(xmax, ymin, zmax);
  glVertex3d(xmax, ymax, zmax);
  glVertex3d(xmin, ymax, zmin);

  glNormal3d(0, 1, 0);
  glVertex3d(xmax, ymax, zmin);
  glVertex3d(xmax, ymax, zmax);
  glVertex3d(xmin, ymax, zmax);
  glVertex3d(xmin, ymax, zmin);

  glNormal3d(-1, 0, 0);
  glVertex3d(xmin, ymax, zmin);
  glVertex3d(xmin, ymax,zmax);
  glVertex3d(xmin, ymin, zmax);
  glVertex3d(xmin, ymin, zmin);

  glNormal3d(0, 0, 1);
  glVertex3d(xmin, ymin, zmax);
  glVertex3d(xmin, ymax, zmax);
  glVertex3d(xmax, ymax, zmax);
  glVertex3d(xmax, ymin, zmax);

  glNormal3d(0, 0, -1);
  glVertex3d(xmin, ymin, zmin);
  glVertex3d(xmax, ymin, zmin);
  glVertex3d(xmax, ymax, zmin);
  glVertex3d(xmin, ymax, zmin);


}
//Rysowanie pola, kó³ek i krzy¿yków w taki sposób zrobiæ