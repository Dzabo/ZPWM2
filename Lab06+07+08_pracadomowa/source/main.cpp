#include <Windows.h>
#include "res.h"
#include <stdio.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>

#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"glu32.lib")
//
#pragma comment(lib,"winmm.lib")


const int max_buffor_size = 115;
const int start_buffor_size = 1;
const int stop_buffor_size = 115;
#define PI 3.141592



BYTE* pBufferForAudio = 0;
WAVEFORMATEX pcmWaveFormat;
float S = pcmWaveFormat.nAvgBytesPerSec;
//



#define XGL_MIN 90 //-85
#define XGL_MAX 270//85
#define YGL_MIN 40//-85
#define YGL_MAX 220//85
#define GL_WIDTH 60
#define GL_SIZE 100
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
GLuint idTexture;

float test = 0;
int xparam;
int yparam;
static GLfloat fangle = 0.0;
float fStep = 1;

HINSTANCE hInst;
HBITMAP hBitmapGameBoard;
HBITMAP hBitmapX;
HBITMAP hBitmapO;
HBITMAP hBitmapRes;
HBITMAP hBitmapStart;

MMRESULT mmResult = 0;
HWAVEOUT hwo = 0;
UINT devId;
WAVEHDR whdr;
const int size_test = 3;
char note_test[size_test] = { 'E','B','T' };
double time_test[size_test] = { 1,1,1 };
int scale_test[size_test] = { 4,4,4 };


void PlayClick();
void PlayEnd();
bool is_game_on = false;
bool is_first_player_turn = true;
bool is_field_ocupied_by_first_player[TAB_SIZE_X][TAB_SIZE_Y];
bool is_field_ocupied_by_second_player[TAB_SIZE_X][TAB_SIZE_Y];
void glGameResult();
GLint DrawGLScene();
void DrawCube1(int a, int b, int c);
void DrawCube(int xmin,int xmax, int ymin, int ymax, int zmin, int zmax, int r, int g, int b);
void DrawSide(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax, int r, int g, int b);
void glDrawBoard();
void glDrawX(int a, int b, int c);
void glDrawO(int a, int b, int c);
unsigned char* ReadBmpFromFile(char* szFileName, int &riWidth, int &riHeight);
bool IsMouseInArea(int x, int y);
void PlaySong(char *tabNote, double *tabTime, int *scale, BYTE* pBufferForAudio, int size);
int Note(BYTE* pBufferForAudio, int iStart, int iDuration, float fNote, float fDiv);

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//Funkcja obs3ugi komunikatów
{
 
  switch (uMsg)
  {
  case WM_COMMAND:
  {
    switch (HIWORD(wParam))
    {
    case BN_CLICKED://Zdarzenie klikniecia 
    {
      switch (LOWORD(wParam))
      {
      default:;
      }
    }
    return TRUE;
    default:;
    }
    return TRUE;
  }
  case WM_LBUTTONDOWN:
  {
    /*CHAR szText[200];
    wsprintf(szText, "Kliknales myszka x=%d, y=%d", LOWORD(lParam), HIWORD(lParam));
    MessageBox(hwndDlg, szText, TEXT("Klikniecie"), MB_OK);*/
    if(is_game_on==true)
    {
      int x = LOWORD(lParam);
      int y = HIWORD(lParam);
      if (IsMouseInArea(x,y) == true)
      {
        int filed_x = ((x - XGL_MIN) / GL_WIDTH);
        int filed_y = ((y - YGL_MIN) / GL_WIDTH);
        if (is_field_ocupied_by_first_player[filed_x][filed_y] == false
          && is_field_ocupied_by_second_player[filed_x][filed_y] == false)
        {
          if (is_first_player_turn == true)
          {
            PlayClick();
            is_field_ocupied_by_first_player[filed_x][filed_y] = true;
            result_tab[3 * (filed_y)+(filed_x)] = 1;
            licznik++;
            glGameResult();
          }
          else
          {
            PlayClick();
            is_field_ocupied_by_second_player[filed_x][filed_y] = true;
            result_tab[3 * (filed_y)+(filed_x)] = 2;
            licznik++;
            glGameResult();
          }
          is_first_player_turn = !is_first_player_turn;
        }
      }
    }
  }
  return TRUE;
  case WM_TIMER:
  {
    if(wParam==1)
    {
      fangle += 15*fStep;
      DrawGLScene();
      glPushMatrix();
      glTranslatef(0, 0, 0);
      glDrawBoard();
      glPopMatrix();
      for (int i = 0; i < TAB_SIZE_X; ++i)
      {
        for (int j = 0; j < TAB_SIZE_Y; ++j)
        {
          int temp_x = (GL_SIZE / 2 + 8)*(i - 1);
          int temp_y = (GL_SIZE / 2 + 8)*(-j+1);
          glPushMatrix();
          glTranslatef(temp_x, temp_y - 20, 0);
          
          if(is_field_ocupied_by_first_player[i][j]==true)
          {
            glRotatef(fangle, 0, 1, 0);
            glDrawX(-25,  0, 0);
          }
          if(is_field_ocupied_by_second_player[i][j]==true)
          {
            glRotatef(fangle, 0, 1, 0);
            glDrawO(-25, 0, 0);
          }
          glPopMatrix();
        }
      }
      glLoadIdentity();
      SwapBuffers(hDc);
    }
    return TRUE;
  }
  case WM_INITDIALOG: //done
  { 
    //zerowaie wartoœci gry
    for (int i = 0; i < TAB_SIZE_X; ++i)
    {
      for (int j = 0; j < TAB_SIZE_Y; ++j)
      {
        is_field_ocupied_by_first_player[i][j] = false;
        is_field_ocupied_by_second_player[i][j] = false;
        result_tab[3 * j + i] = 0;
      }
    }
    is_game_on = true;
    is_first_player_turn = true;
    //
    //generowanie pojedynczyj tekstury cz1 start
    int iWidth;
    int iHeight;
    unsigned char* pMyImage=ReadBmpFromFile("..\\resource\\tekstura.bmp",iWidth,iHeight);
    //generowanie pojedynczyj tekstury cz1 stop


    //zmiana stylu okna
    SetWindowLong(hwndDlg, GWL_STYLE, CS_HREDRAW | CS_VREDRAW | CS_OWNDC | WS_OVERLAPPEDWINDOW);
    //SetClassLong(hwndDlg,GCL_STYLE,CS_HREDRAW|CS_VREDRAW|CS_OWNDC); //Silniejsza funkcja SetWindowLong
    //Stworzyc maszyne stanu
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
    //tworzymy maszyne stanu
    hRc = wglCreateContext(hDc);
    wglMakeCurrent(hDc, hRc);
    //Podpiac ja pod okno

    //generowanie pojedynczyj tekstury cz2 start
    glGenTextures(1,&idTexture);
    glBindTexture(GL_TEXTURE_2D,idTexture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,iWidth,iHeight,0,GL_RGB,GL_UNSIGNED_BYTE,pMyImage);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    delete [] pMyImage;
    //generowanie pojedynczyj tekstury cz2 stop
  }
  return TRUE;
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

    gluPerspective(45.0f, iWidth / static_cast<float>(iHeight), 0.1f, 1000.0f); //k1t widzenia, Rozmiar okna, Z min, Z max

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }
  return TRUE;

  case WM_CLOSE:
  {
    DestroyWindow(hwndDlg); // zniszczenie okna
    PostQuitMessage(0); //Komunikat polecenia zakonczenia aplikacji
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



  SetTimer(hwndMainWindow, 1, 50, NULL);
  MSG msg = {};
  while (GetMessage(&msg, nullptr, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

 
}

//Rysowanie
//LineTo
//MoveToEx
//Rectangle
//TextOut
//GetPixel
//SetPixel
// TextOut(hdc, 0, 0, sz_text, strlen(sz_text)); //napsize start w lewym gornym, ogarn1c

GLint DrawGLScene()
{
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);

  glLoadIdentity();
  static GLfloat fAngle = 0.0;
  //fAngle += 1.01f;
  glTranslatef(0, 0, -300);
  //glRotatef(fAngle, 0, 1, 0);
  return 1;
}

void DrawCube1(int a, int b, int c)
{

  glBegin(GL_QUADS);
  DrawCube(0,50*a,0,50*b,0,50*c,1,0,0);
  glEnd();

}

void glDrawBoard()
{
  glBindTexture(GL_TEXTURE_2D,idTexture);
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

  //ty3
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

  //dó3
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
  //ty3
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
  //glTexCoord2f(0.0, 0.0) ogarn1a w tej funkcji wspó3rzedne tak, ?eby teksturowa3o 
  glColor3d(r,g,b);
  glNormal3d(0, -1, 0);
  glTexCoord2f(0.0, 0.0);glVertex3d(xmin, ymin, zmin);
  glTexCoord2f(0.0, 1.0); glVertex3d(xmin, ymin, zmax);
  glTexCoord2f(1.0, 1.0);glVertex3d(xmax, ymin, zmax);
  glTexCoord2f(1.0, 0.0); glVertex3d(xmax, ymin, zmax);

  glNormal3d(0, -1, 0);
  glTexCoord2f(0.0, 0.0);glVertex3d(xmax, ymin, zmin);
  glTexCoord2f(0.0, 1.0);glVertex3d(xmax, ymin, zmax);
  glTexCoord2f(1.0, 1.0);glVertex3d(xmax, ymax, zmax);
  glTexCoord2f(1.0, 0.0);glVertex3d(xmin, ymax, zmin);

  glNormal3d(0, 1, 0);
  glTexCoord2f(0.0, 0.0);glVertex3d(xmax, ymax, zmin);
  glTexCoord2f(0.0, 1.0);glVertex3d(xmax, ymax, zmax);
  glTexCoord2f(1.0, 1.0);glVertex3d(xmin, ymax, zmax);
  glTexCoord2f(1.0, 0.0);glVertex3d(xmin, ymax, zmin);

  glNormal3d(-1, 0, 0);
  glTexCoord2f(0.0, 0.0);glVertex3d(xmin, ymax, zmin);
  glTexCoord2f(0.0, 1.0);glVertex3d(xmin, ymax, zmax);
  glTexCoord2f(1.0, 1.0);glVertex3d(xmin, ymin, zmax);
  glTexCoord2f(1.0, 0.0);glVertex3d(xmin, ymin, zmin);

  glNormal3d(0, 0, 1);
  glTexCoord2f(0.0, 0.0);glVertex3d(xmin, ymin, zmax);
  glTexCoord2f(0.0, 1.0);glVertex3d(xmin, ymax, zmax);
  glTexCoord2f(1.0, 1.0);glVertex3d(xmax, ymax, zmax);
  glTexCoord2f(1.0, 0.0);glVertex3d(xmax, ymin, zmax);

  glNormal3d(0, 0, -1);
  glTexCoord2f(0.0, 0.0);glVertex3d(xmin, ymin, zmin);
  glTexCoord2f(0.0, 1.0);glVertex3d(xmax, ymin, zmin);
  glTexCoord2f(1.0, 1.0);glVertex3d(xmax, ymax, zmin);
  glTexCoord2f(1.0, 0.0);glVertex3d(xmin, ymax, zmin);


}

unsigned char* ReadBmpFromFile(char* szFileName, int &riWidth, int &riHeight)
{
  BITMAPFILEHEADER     bfh;
  BITMAPINFOHEADER     bih;

  int                i, j, h, v, lev, l, ls;
  unsigned char*     buff = NULL;

  unsigned char* p_palette = NULL;
  unsigned short n_colors = 0;

  unsigned char* pRGBBuffer=nullptr;

  FILE* hfile=nullptr;
  fopen_s(&hfile,szFileName, "rb");

  if (hfile != NULL)
  {
    fread(&bfh, sizeof(bfh), 1, hfile);
    if (!(bfh.bfType != 0x4d42 || (bfh.bfOffBits < (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)))))
    {
      fread(&bih, sizeof(bih), 1, hfile);
      v = bih.biWidth;
      h = bih.biHeight;
      lev = bih.biBitCount;

      riWidth = v;
      riHeight = h;
      pRGBBuffer = new unsigned char[riWidth*riHeight * 3]; //Zaalokowanie odpowiedniego buffora obrazu

                                                            //Za3aduj Palete barw jesli jest
      if ((lev == 1) || (lev == 4) || (lev == 8))
      {
        n_colors = 1 << lev;
        p_palette = new unsigned char[4 * n_colors];
        fread(p_palette, 4 * n_colors, 1, hfile);
      }

      fseek(hfile, bfh.bfOffBits, SEEK_SET);

      buff = new unsigned char[v * 4];

      switch (lev)
      {
      case 1:
        //Nie obs3ugiwane
        break;
      case 4:
        //nie Obs3ugiwane
        break;
      case 8: //Skala szaro?ci
        ls = (v + 3) & 0xFFFFFFFC;
        for (j = (h - 1);j >= 0;j--)
        {
          fread(buff, ls, 1, hfile);
          for (i = 0, l = 0;i<v;i++)
          {
            pRGBBuffer[((j*riWidth) + i) * 3 + 2] = p_palette[(buff[i] << 2) + 2];//R
            pRGBBuffer[((j*riWidth) + i) * 3 + 1] = p_palette[(buff[i] << 2) + 1];//G
            pRGBBuffer[((j*riWidth) + i) * 3 + 0] = p_palette[(buff[i] << 2) + 0];//B
          }
        };
        break;
      case 24:
        //bitmapa RGB
        ls = (v * 3 + 3) & 0xFFFFFFFC;
        for (j = (h - 1);j >= 0;j--)
        {
          //x_fread(hfile,buff,ls);
          fread(buff, ls, 1, hfile);
          for (i = 0, l = 0;i<v;i++, l += 3)
          {
            pRGBBuffer[((j*riWidth) + i) * 3 + 0] = buff[l + 2];
            pRGBBuffer[((j*riWidth) + i) * 3 + 1] = buff[l + 1];
            pRGBBuffer[((j*riWidth) + i) * 3 + 2] = buff[l + 0];
          };
        };
        break;
      case 32:
        // RGBA bitmap 
        for (j = (h - 1);j >= 0;j--)
        {
          fread(buff, v * 4, 1, hfile);
          for (i = 0, l = 0;i<v;i++, l += 4)
          {
            pRGBBuffer[((j*riWidth) + i) * 3 + 0] = buff[l + 2];
            pRGBBuffer[((j*riWidth) + i) * 3 + 1] = buff[l + 1];
            pRGBBuffer[((j*riWidth) + i) * 3 + 2] = buff[l + 0];
          }
        };
        break;
      };
      delete buff;
      if (p_palette) delete p_palette;

    }
  }
  return pRGBBuffer;
}
//Rysowanie pola, kó3ek i krzy?yków w taki sposób zrobia

bool IsMouseInArea(int x, int y)
{
  if ((x > XGL_MIN)&&(x<XGL_MAX)&&(y>YGL_MIN)&&(y<YGL_MAX))return true;
  //if ((x > 60) && (x < 160) && (y > 70) && (y < 170))return true;
  return false;
}
void glGameResult()
{
  {

    for (int i = 0; i < 3; i++)
    {
      if ((result_tab[3 * i] == result_tab[3 * i + 1]) && (result_tab[3 * i] == result_tab[3 * i + 2]))
      {
        if (result_tab[3 * i] == 1)
        {
          PlayEnd();
          MessageBox(NULL, TEXT("Krzyzyk wygral"),TEXT("KiK"), MB_OK);
          is_game_on = false;
          return;
        }
        if (result_tab[3 * i] == 2)
        {
          PlayEnd();
          MessageBox(NULL, TEXT("Kolko wygralo"), TEXT("KiK"), MB_OK);
          is_game_on = false;
          return;
        }
      }
      if ((result_tab[i] == result_tab[i + 3]) && (result_tab[i] == result_tab[i + 6]))
      {
        if (result_tab[i] == 1)
        {
          PlayEnd();
          MessageBox(NULL, TEXT("Krzyzyk wygral"), TEXT("KiK"), MB_OK);
          is_game_on = false;
          return;
        }
        if (result_tab[i] == 2)
        {
          PlayEnd();
          MessageBox(NULL, TEXT("Kolko wygralo"), TEXT("KiK"), MB_OK);
          is_game_on = false;
          return;
        }

      }
    }
    if ((result_tab[0] == result_tab[4] && result_tab[0] == result_tab[8]) || (result_tab[4] == result_tab[6] && result_tab[4] == result_tab[2]))
    {
      if (result_tab[4] == 1)
      {
        PlayEnd();
        MessageBox(NULL, TEXT("Krzyzyk wygral"), TEXT("KiK"), MB_OK);
        is_game_on = false;
        return;
      }
      if (result_tab[4] == 2)
      {
        PlayEnd();
        MessageBox(NULL, TEXT("Kolko wygralo"), TEXT("KiK"), MB_OK);
        is_game_on = false;
        return;
      }
    }
    if (licznik == 9)
    {
      PlayEnd();
      MessageBox(NULL, TEXT("Remis"), TEXT("KiK"), MB_OK);
      is_game_on = false;
      return;
    }
  }
  
}

int Note(BYTE* pBufferForAudio, int iStart, int iDuration, float fNote, float fDiv)
{
  int i;
  for (i = iStart; i < iStart + iDuration - 200; i++)
  {
    float t = i / fDiv;
    pBufferForAudio[i] = 128 * sin(2 * PI*fNote*t) + 128;
  }
  for (; i < iStart + iDuration; ++i)
  {
    float t = i / fDiv;
    pBufferForAudio[i] = 128;
  }
  return i;
}


void PlayClick()
{
  sndPlaySound("..//resource//Klik.wav", SND_ASYNC);
}

void PlayEnd()
{
  sndPlaySound("..//resource//End.wav", SND_ASYNC);
}