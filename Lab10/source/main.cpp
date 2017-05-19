#include <Windows.h>
#include "res.h"
#include <stdio.h>
#define ID_TIMER_REDRAW 10

HANDLE hFile = 0;
HDC hDC;
HDC hdcMemDC;
BYTE* ImageBufferY;//przetrzymujemy próbki
BYTE* ImageBufferU;
BYTE* ImageBufferV;
BYTE* ImageBufferRGBA;
HBITMAP hBmp;
BITMAP bmp;
BITMAPINFOHEADER bi;
__int64 iStart;
__int64 iEnd;
// int iStart;
// int iEnd;
char text[255];

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//Funkcja obs³ugi komunikatów
{
  switch (uMsg)
  {
  case WM_TIMER:
  {
    switch (wParam)
    {
    case ID_TIMER_REDRAW:
    {
      if (hFile==NULL) break;//sprawdzamy czy plik otwarty

      DWORD ret;

      ReadFile(hFile, ImageBufferY, 352 * 288, &ret, NULL);//wczytujemy do zmiennej ImageBufferY wszystkie próbki
      ReadFile(hFile, ImageBufferU, 352 * 288 / 4, &ret, NULL);
      ReadFile(hFile, ImageBufferV, 352 * 288 / 4, &ret, NULL);
      //mierzymy czas 
      //iStart = GetTickCount();//funkcja pobiera czas systemowy
      QueryPerformanceCounter((_LARGE_INTEGER*)&iStart);

      //conversion from yuv to rgb

      for (int y = 0; y < 288; ++y)
      {
        for (int x = 0; x < 352; ++x)
        {
          //pobranie próbek
          int yy = ImageBufferY[y * 352 + x];
          int uu = ImageBufferU[y / 2 * 352 / 2 + x / 2]-128;
          int vv = ImageBufferV[y / 2 * 352 / 2 + x / 2]-128;

          int r = ((256 * 1.40200)*vv + 256*yy) / 256;
          int g = ((256 * -0.34414)*uu + (256 * -0.71414)*vv + 256 * yy) / 256;
          int b = ((256 * 1.77200)*uu + 256 * yy) / 256;

          // kolory wpisywane w windowsie s¹ od koñca wiec ³adujemy BGR a nie RGB
          //ImageBufferRGBA[3*(y * 352 + x)+0] = b;
          //ImageBufferRGBA[3*(y * 352 + x)+1] = g;
          //ImageBufferRGBA[3*(y * 352 + x)+2] = r;
          //wpisywanie do bufora tak ¿eby nie by³o do góry nogami
          ImageBufferRGBA[3 * ((287-y) * 352 + x) + 0] = b;
          ImageBufferRGBA[3 * ((287-y) * 352 + x) + 1] = g;
          ImageBufferRGBA[3 * ((287-y) * 352 + x) + 2] = r;

        }
      }

      //iEnd = GetTickCount();
      QueryPerformanceCounter((_LARGE_INTEGER*)&iEnd);
      RECT rc;
      GetClientRect(hwndDlg, &rc);//pobieramy okienko
      InvalidateRect(hwndDlg, NULL, false);//ca³e okienko wymaga odrysowania
      RedrawWindow(hwndDlg, &rc, NULL, RDW_ERASE);//odrysowujemy okienko
      return FALSE;
    }
    }
    return TRUE;
  }
  case WM_PAINT:
  {
    hDC = GetDC(hwndDlg);
    hdcMemDC = CreateCompatibleDC(hDC);

    hBmp = CreateCompatibleBitmap(hDC, 352, 288);
    SelectObject(hdcMemDC, hBmp);

    GetObject(hBmp, sizeof(BITMAP), &bmp);
    //informacje o rozmiarze i kolorach formatu
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = 352;
    bi.biHeight = 288; // metoda sprytna aby obróciæ obrazek, zmieniamy wartoœæ na ujemn¹
    bi.biPlanes = 1;
    bi.biBitCount = 24;//g³êbia 8bitów/kana³ *3kana³y
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 352*288;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrImportant = 0;
    bi.biClrUsed = 0;

    SetDIBits(hdcMemDC, hBmp, 0, 288, ImageBufferRGBA, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    BitBlt(hDC, 0, 0, 352, 288, hdcMemDC, 0, 0, SRCCOPY);
    //
    //static int x = 10;
    //static int y = 10;
    //TextOut(hDC, x, y, "O_O", 3);
    //x = x + 1;
    //y += 1;
    //
    static int x = 10;
    static int y = 10;
    wsprintf(text, "%d [us]", iEnd - iStart);
    TextOut(hDC, x, y, text,strlen(text));
    //x += 1;
    //y += 1;
    ReleaseDC(hwndDlg, hDC);
    return FALSE;
  }
  case WM_INITDIALOG:
  {
    //otwieranie plików
    hFile = CreateFile("..\\resource\\claire.cif",   //uchwyt-podajemy œcie¿kê dodaæ ../ w domu
      GENERIC_READ,     //typ dostêpu do pliku (read write itd)
      0,
      (LPSECURITY_ATTRIBUTES)0,//bez zadnych zabezpieczeñ
      OPEN_EXISTING,          //otwórz istniej¹cy, mo¿na stworzyæ albo nadpisaæ
      FILE_ATTRIBUTE_NORMAL,  //czy plik jest ukryty,systemowy,specjalny itd 
      (HANDLE)NULL);
    ImageBufferY = (BYTE*)malloc(352 * 288); //luma
    ImageBufferU = (BYTE*)malloc(352 * 288 / 4);  //chroma
    ImageBufferV = (BYTE*)malloc(352 * 288 / 4);  //chroma

    ImageBufferRGBA = (BYTE*)malloc(352 * 288 * 4);  //kolory?
    return FALSE;
  }

  case WM_CLOSE:
  {
    free(ImageBufferU);//zwalniamy buffory 
    free(ImageBufferV);
    free(ImageBufferY);
    free(ImageBufferRGBA);
    CloseHandle(hFile); // zamykamy plik t¹ funkcj¹, zamyka ona plik z uchwytu

    DestroyWindow(hwndDlg); // zniszczenie okna
    PostQuitMessage(0); //Komunikat polecenia zakoñczenia aplikacji
    return TRUE;
  }
  }
  return FALSE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{



  HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);
  SetTimer(hwndMainWindow, ID_TIMER_REDRAW, 1, NULL); // ostatni parametr do jakiejœ funckji
  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

  }

}



//zad dom przepisaæ ten fragment kodu z pêtlami w taki sposób, ¿eby liczyæ szybciej 
//(wywaliæ powtórzenia, przesuniêcia bitowe zamiast mno¿eñ i dzieleñ, przerzutowaæ floaty na inty)
//zrobiæ to http://new.owieczka.net/blog/2012/04/19/techniki-sse-i-avx/
