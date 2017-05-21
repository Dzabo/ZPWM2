#include <Windows.h>
#include "res.h"
#include <stdio.h>
#include <math.h>
#include "notes.h"
#include "song.h"
#pragma comment(lib,"winmm.lib")

#define PI 3.141592

/*float f = 880;
float f1 = 1000;
float f2 = 2000;*/

const int size_imp = 18;
const int max_buffor_size = 115;
const int start_buffor_size = 1;
const int stop_buffor_size = 115;

//NOTES
//C Cis D Dis E F Fis G Gis A Ais B
//C Z   D X   E F V   G N   A M   B

BYTE* pBufferForAudio = 0;
WAVEFORMATEX pcmWaveFormat;
float S = pcmWaveFormat.nAvgBytesPerSec;
void PlaySong(char *tabNote, double *tabTime, int *scale, BYTE* pBufferForAudio, int size);

char note_imperial[size_imp] = { 'E','E','E','C','G','E','C','G','E','B','B','B','C','G','E','C','G','E' };
double time_imperial[size_imp] = { 8,8,8,4,2,8,4,2,8,8,8,8,4,2,8,4,2,8, };
int scale_imperial[size_imp] = { 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4 };
const int size_test = 3;
char note_test[size_test] = { 'E','B','T' };
double time_test[size_test] = { 1,1,1 };

int Note(BYTE* pBufferForAudio, int iStart, int iDuration, float fNote, float fDiv);

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//Funkcja obs³ugi komunikatów
{
  switch (uMsg)
  {
  case WM_COMMAND:
    switch (HIWORD(wParam))
    {
      return FALSE;

    case BN_CLICKED://Zdarzenie klikniêcia 
      switch (LOWORD(wParam))
      {
        return TRUE;

      }
    }
    return FALSE;

  case WM_LBUTTONDOWN:
    /*CHAR szText[200];
    wsprintf(szText, "Kliknales myszka x=%d, y=%d", LOWORD(lParam), HIWORD(lParam));
    MessageBox(hwndDlg, szText, TEXT("Klikniecie"), MB_OK);*/
    return TRUE;

  case WM_CLOSE:
    DestroyWindow(hwndDlg); // zniszczenie okna
    PostQuitMessage(0); //Komunikat polecenia zakoñczenia aplikacji
    return TRUE;
  }
  return FALSE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{



  HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);


  pcmWaveFormat.wFormatTag = WAVE_FORMAT_PCM; // bez straty
  pcmWaveFormat.nChannels = 1;//ile kanalow
  pcmWaveFormat.nSamplesPerSec = 44100L;//czestotliwosc probkowania, L daje wartoœæ typu long
  pcmWaveFormat.wBitsPerSample = 8;//ile bitow na probke
  pcmWaveFormat.nAvgBytesPerSec = pcmWaveFormat.nSamplesPerSec*pcmWaveFormat.wBitsPerSample / 8;
  pcmWaveFormat.nBlockAlign = 1;//
  pcmWaveFormat.cbSize = 0;//

  MMRESULT mmResult = 0;
  HWAVEOUT hwo = 0;
  UINT devId;

  for (devId = 0; devId < waveOutGetNumDevs(); ++devId)
  {
    mmResult = waveOutOpen(&hwo, devId, &pcmWaveFormat, 0, 0, CALLBACK_NULL);
    if (mmResult == MMSYSERR_NOERROR)break;
  }
  if (mmResult != MMSYSERR_NOERROR) //gdy nie uda siê otworzyæ karty dŸwiêkowej
  {
    MessageBox(hwndMainWindow, TEXT("Nie znaleziono karty"), TEXT("Error"), MB_OK);
    return mmResult;
  }

  BYTE* pBufferForAudio = new BYTE[max_buffor_size * pcmWaveFormat.nAvgBytesPerSec];


  int i = 0;
  /*for (int i = 0;i < 2 * pcmWaveFormat.nAvgBytesPerSec;++i) // 4 to dlugosc bufora
  {
    //f=f+0.1;
    float t=i/ (float) pcmWaveFormat.nSamplesPerSec;
    f = f1 + (f2 - f1)*t; // rosnacy dzwiek
    pBufferForAudio[i]=128*sin(2*PI*f*t)+128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }*/
  /*{
  for ( ;i < 0.5 * pcmWaveFormat.nAvgBytesPerSec;++i) // 4 to dlugosc bufora
  {
    //f=f+0.1;
    float t = i / (float)pcmWaveFormat.nSamplesPerSec;
    f = fC ;//
    pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }
  for (;i < 1 * pcmWaveFormat.nAvgBytesPerSec;++i) // 4 to dlugosc bufora
  {
    //f=f+0.1;
    float t = i / (float)pcmWaveFormat.nSamplesPerSec;
    f = fG;//
    pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }
  for (;i < 1.5 * pcmWaveFormat.nAvgBytesPerSec;++i) // 4 to dlugosc bufora
  {
    //f=f+0.1;
    float t = i / (float)pcmWaveFormat.nSamplesPerSec;
    f = fE;//
    pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }
  for (;i < 2 * pcmWaveFormat.nAvgBytesPerSec;++i) // 4 to dlugosc bufora
  {
    //f=f+0.1;
    float t = i / (float)pcmWaveFormat.nSamplesPerSec;
    f = fA;//
    pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }
  for (;i < 2.5 * pcmWaveFormat.nAvgBytesPerSec;++i) // 4 to dlugosc bufora
  {
    //f=f+0.1;
    float t = i / (float)pcmWaveFormat.nSamplesPerSec;
    f = fE;//
    pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }
  for (;i < 3 * pcmWaveFormat.nAvgBytesPerSec;++i) // 4 to dlugosc bufora
  {
    //f=f+0.1;
    float t = i / static_cast<float>(pcmWaveFormat.nSamplesPerSec);
    f = fF;//
    pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }
  for (;i < 3.5 * pcmWaveFormat.nAvgBytesPerSec;++i) // 4 to dlugosc bufora
  {
    //f=f+0.1;
    float t = i / static_cast<float>(pcmWaveFormat.nSamplesPerSec);
    f = fG;//
    pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }
  for (;i < 4 * pcmWaveFormat.nAvgBytesPerSec;++i) // 4 to dlugosc bufora
  {
    //f=f+0.1;
    float t = i / static_cast<float>(pcmWaveFormat.nSamplesPerSec);
    f = fA;//
    pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }
  }*/

  WAVEHDR whdr;
  ZeroMemory(&whdr, sizeof(WAVEHDR));
  whdr.lpData = reinterpret_cast<LPSTR>(pBufferForAudio);
  whdr.dwBufferLength = start_buffor_size * pcmWaveFormat.nAvgBytesPerSec; // 4 to dlugosc bufora
  mmResult = waveOutPrepareHeader(hwo, &whdr, sizeof(WAVEHDR));
  mmResult = waveOutWrite(hwo, &whdr, sizeof(WAVEHDR));

  while ((whdr.dwFlags&WHDR_DONE) != WHDR_DONE)Sleep(100);

  mmResult = waveOutUnprepareHeader(hwo, &whdr, sizeof(WAVEHDR));

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

  }

  /*whdr.dwBufferLength = 1 * pcmWaveFormat.nAvgBytesPerSec; // 4 to dlugosc bufora
  for (int i = 0;i < 1 * pcmWaveFormat.nAvgBytesPerSec;++i)
  {
    //f = f - 0.05;
    float t = i / (float)pcmWaveFormat.nSamplesPerSec;
    f = f2 + (f1 - f2)*t; // opadajacy dzwiek
    pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }

  mmResult = waveOutPrepareHeader(hwo, &whdr, sizeof(WAVEHDR));
  mmResult = waveOutWrite(hwo, &whdr, sizeof(WAVEHDR));
  while ((whdr.dwFlags&WHDR_DONE) != WHDR_DONE)Sleep(100);
  mmResult = waveOutUnprepareHeader(hwo, &whdr, sizeof(WAVEHDR));
  */
  //marsz imperialny
  whdr.dwBufferLength = stop_buffor_size * pcmWaveFormat.nAvgBytesPerSec;
  //PlaySong(note_imperial, time_imperial, scale_imperial, pBufferForAudio, size_imp);
  {
      PlaySong(note_smells_full, time_smells_full, scale_smells_full, pBufferForAudio, size_smells);
    mmResult = waveOutPrepareHeader(hwo, &whdr, sizeof(WAVEHDR));
    mmResult = waveOutWrite(hwo, &whdr, sizeof(WAVEHDR));
    while ((whdr.dwFlags&WHDR_DONE) != WHDR_DONE)Sleep(100);
    mmResult = waveOutUnprepareHeader(hwo, &whdr, sizeof(WAVEHDR));
  }
/*  {
    PlaySong(note_smells_prechorus, time_smells_prechorus, scale_smells_prechorus, pBufferForAudio, size_smells);
    mmResult = waveOutPrepareHeader(hwo, &whdr, sizeof(WAVEHDR));
    mmResult = waveOutWrite(hwo, &whdr, sizeof(WAVEHDR));
    while ((whdr.dwFlags&WHDR_DONE) != WHDR_DONE)Sleep(100);
    mmResult = waveOutUnprepareHeader(hwo, &whdr, sizeof(WAVEHDR));
  }*/
  mmResult = waveOutClose(hwo);
  delete[] pBufferForAudio;
}

void PlaySong(char *tabNote, double *tabTime, int *scale, BYTE* pBufferForAudio, int size)
{
  int i = 0;
  int zm = 0;
  for (int j = 0; j < size; ++j)
  {
    switch (scale[j])
    {
    case 1:
      switch (tabNote[j])
      {
      case 'C':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fC1, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'Z':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fCis1, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'D':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fD1, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'X':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fDis1, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'E':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fE1, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'F':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fF1, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'V':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fFis1, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'G':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fG1, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'N':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fGis1, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'A':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fA1, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'M':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fAis1, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'B':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fB1, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'P':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fP, pcmWaveFormat.nSamplesPerSec);
        break;
      default:
        break;
      }
      break;
    case 2:
      switch (tabNote[j])
      {
      case 'C':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fC2, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'Z':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fCis2, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'D':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fD2, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'X':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fDis2, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'E':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fE2, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'F':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fF2, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'V':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fFis2, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'G':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fG2, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'N':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fGis2, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'A':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fA2, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'M':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fAis2, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'B':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fB2, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'P':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fP, pcmWaveFormat.nSamplesPerSec);
        break;
      default:
        break;
      }
      break;
    case 3:
      switch (tabNote[j])
      {
      case 'C':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fC3, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'Z':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fCis3, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'D':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fD3, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'X':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fDis3, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'E':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fE3, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'F':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fF3, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'V':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fFis3, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'G':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fG3, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'N':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fGis3, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'A':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fA3, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'M':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fAis3, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'B':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fB3, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'P':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fP, pcmWaveFormat.nSamplesPerSec);
        break;
      default:
        break;
      }
      break;
    case 4:
      switch (tabNote[j])
      {
      case 'C':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fC4, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'Z':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fCis4, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'D':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fD4, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'X':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fDis4, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'E':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fE4, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'F':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fF4, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'V':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fFis4, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'G':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fG4, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'N':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fGis4, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'A':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fA4, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'M':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fAis4, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'B':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fB4, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'P':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fP, pcmWaveFormat.nSamplesPerSec);
        break;
      default:
        break;
      }
      break;
    case 5:
      switch (tabNote[j])
      {
      case 'C':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fC5, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'Z':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fCis5, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'D':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fD5, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'X':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fDis5, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'E':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fE5, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'F':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fF5, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'V':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fFis5, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'G':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fG5, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'N':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fGis5, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'A':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fA5, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'M':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fAis5, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'B':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fB5, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'P':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fP, pcmWaveFormat.nSamplesPerSec);
        break;
      default:
        break;
      }
      break;
    case 6:
      switch (tabNote[j])
      {
      case 'C':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fC6, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'Z':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fCis6, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'D':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fD6, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'X':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fDis6, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'E':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fE6, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'F':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fF6, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'V':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fFis6, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'G':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fG6, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'N':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fGis6, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'A':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fA6, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'M':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fAis6, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'B':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fB6, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'P':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fP, pcmWaveFormat.nSamplesPerSec);
        break;
      default:
        break;
      }
      break;
    case 7:
      switch (tabNote[j])
      {
      case 'C':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fC7, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'Z':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fCis7, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'D':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fD7, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'X':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fDis7, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'E':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fE7, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'F':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fF7, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'V':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fFis7, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'G':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fG7, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'N':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fGis7, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'A':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fA7, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'M':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fAis7, pcmWaveFormat.nSamplesPerSec);
        break;
      case 'B':
        i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fB7, pcmWaveFormat.nSamplesPerSec);
        break; 
      case 'P':
          i = Note(pBufferForAudio, i, pcmWaveFormat.nAvgBytesPerSec*(tabTime[j] / 16.0), fP, pcmWaveFormat.nSamplesPerSec);
          break;
        default:
          break;
      }
      break;
    default:
      break;
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
// DO DOMU SKOMPONOWAC UTWOR TRWAJACY MINIMUM 35 SEKUND 
// char sNotes = "E E E C G ";
// char sTime = "16 16 16 8 4";
//w taki sposób ma byæ zrealizowane  


