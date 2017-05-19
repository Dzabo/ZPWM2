#include <Windows.h>
#include "res.h"
#include <stdio.h>
#include <math.h>
#include "notes.h"
#include "song.h"
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dsound.lib")
#include "dsound.h"

#define PI 3.141592

/*float f = 880;
float f1 = 1000;
float f2 = 2000;*/

const int size_imp = 18;
const int max_buffor_size = 115;
const int start_buffor_size = 115;
const int stop_buffor_size = 115;

//NOTES
//C Cis D Dis E F Fis G Gis A Ais B
//C Z   D X   E F V   G N   A M   B

BYTE* pBufferForAudio = 0;
WAVEFORMATEX pcmWaveFormat;
float S = pcmWaveFormat.nAvgBytesPerSec;
void PlaySong(char *tabNote, double *tabTime, int *scale, BYTE* pBufferForAudio, int size);
int Echo(BYTE* pBufferForAudio, int iStart, int iDuration,
  int iDelay1, int iDelay2, int iDelay3,
  float fAttenuate1, float fAttenuate2, float fAttenuate3);

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

  //probki
  pcmWaveFormat.wFormatTag = WAVE_FORMAT_PCM; // bez straty
  pcmWaveFormat.nChannels = 1;//ile kanalow
  pcmWaveFormat.nSamplesPerSec = 44100L;//czestotliwosc probkowania, L daje wartoœæ typu long
  pcmWaveFormat.wBitsPerSample = 8;//ile bitow na probke
  pcmWaveFormat.nAvgBytesPerSec = pcmWaveFormat.nSamplesPerSec*pcmWaveFormat.wBitsPerSample / 8;
  pcmWaveFormat.nBlockAlign = 1;//
  pcmWaveFormat.cbSize = 0;//

  //direct sound

  IDirectSound8 *DS8=NULL;
  DirectSoundCreate8(NULL,&DS8,NULL);
  if (DS8 == NULL)
  {
    MessageBox(0,TEXT("Nie mozna1.."),TEXT("Error1."),MB_OK);
    return TRUE;
  }

  if (DS8->SetCooperativeLevel(hwndMainWindow,DSSCL_NORMAL)!=DS_OK) //ustawiamy kompatybilnosc normalna, w przypadku bledu komunikat
  {
    MessageBox(0, TEXT("Nie mozna2.."), TEXT("Error2."), MB_OK);
    return TRUE;
  }

  //

  MMRESULT mmResult = 0;
  HWAVEOUT hwo = 0;
  UINT devId;

  BYTE* pBufferForAudio = new BYTE[max_buffor_size * pcmWaveFormat.nAvgBytesPerSec];


  int i = 0;
  WAVEHDR whdr;
  ZeroMemory(&whdr, sizeof(WAVEHDR));
  whdr.lpData = reinterpret_cast<LPSTR>(pBufferForAudio);
  whdr.dwBufferLength = start_buffor_size * pcmWaveFormat.nAvgBytesPerSec; // 4 to dlugosc bufora
  PlaySong(note_smells_full, time_smells_full, scale_smells_full, pBufferForAudio, size_smells);
  //Echo(pBufferForAudio,0,whdr.dwBufferLength,22000,0.5); //ostatni parametr okreœla g³oœnoœæ echa wzglêdem orygina³u
  Echo(pBufferForAudio, 0, whdr.dwBufferLength, 22000,11000,44000,0.8,0.65,0.5);
  //Direct sound buffor

  IDirectSoundBuffer *DSB;
  DSBUFFERDESC DSBD;
  memset(&DSBD,0,sizeof(DSBUFFERDESC));
  DSBD.dwSize=sizeof(DSBUFFERDESC);
  DSBD.dwFlags=DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY; // proporcja miêdzy lewym a prawym kana³em,g³oœnoœæ, iloœæ próbek
  DSBD.dwBufferBytes=whdr.dwBufferLength;
  DSBD.lpwfxFormat=&pcmWaveFormat;
  if (DS8->CreateSoundBuffer(&DSBD, &DSB, NULL) != DS_OK)
  {
    MessageBox(0, TEXT("Nie mozna3.."), TEXT("Error3."), MB_OK);
    return TRUE;
  }

  //

  //odtwarzanie dŸwiêku przez direct sound
  DSB->SetVolume(0xFFFF);
  void *ptr1=NULL,*ptr2=NULL;
  DWORD l1,l2;//liczniki

  DSB->Lock(0,whdr.dwBufferLength,&ptr1,&l1,&ptr2,&l2,DSBLOCK_ENTIREBUFFER);//pobieramy obszar pamieci
                                                                            //od 0 do konca bufora i 
                                                                            //zwracamy adresy do wskazników
                                                                            //zajmujemy buffor za jednym zamachem
  if(ptr1)memcpy(ptr1,pBufferForAudio,l1);
  if (ptr2)memcpy(ptr2, pBufferForAudio+l1, l2);
  DSB->Unlock(ptr1,l1,ptr2,l2);

  DSB->Play(0,0,0);
  // ¿eby odtworzyæ kolejny dŸwiêk musimy powtórzyæ od DSB->LOCK do DSB->PLAY

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

  }

  DS8->Release();
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
    pBufferForAudio[i] = ((128 * sin(2 * PI*fNote*t) + 128)
                        +(128 * sin(2 * PI*fNote*2.0/3.0*t) + 128))/2.0; //2 dŸwiêki, akordy na takiej zasadzie
  }
  for (; i < iStart + iDuration; ++i)
  {
    float t = i / fDiv;
    pBufferForAudio[i] = 128;
  }
  return i;
}

int Echo(BYTE* pBufferForAudio, int iStart, int iDuration,
          int iDelay1, int iDelay2, int iDelay3, 
          float fAttenuate1,float fAttenuate2,float fAttenuate3) //echo na 3 dŸwiêki
{

  BYTE* tmp=new BYTE[iDuration];
  memcpy(tmp,pBufferForAudio,iDuration);
  for (int i = 0;i < iDuration;++i)
  {
    int iSample1 = 0, iSample2 = 0, iSample3 = 0;
    if (i - iDelay1>0)
    iSample1=tmp[i-iDelay1]-128.0;
    if (i - iDelay2>0)
    iSample2 = tmp[i - iDelay2] - 128.0;
    if (i - iDelay3>0)
    iSample3 = tmp[i - iDelay3] - 128.0;
    pBufferForAudio[i]=((tmp[i]-128.0)+iSample1*fAttenuate1
                                      + iSample2*fAttenuate2
                                      + iSample3*fAttenuate3)/ (1.0 + fAttenuate1 + fAttenuate2+ fAttenuate3) + 128.0;
  }
  delete [] tmp;
  return 0;
}
//zad domowe 
//do kó³ka i krzy¿yka w opengl dodaæ dŸwiêk
