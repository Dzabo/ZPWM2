#include <Windows.h>
#include "res.h"
#include <stdio.h>
#include <math.h>
#pragma comment(lib,"winmm.lib")

#define PI 3.141592
float f=880;
float f1=1000;
float f2=2000;

float fC=261.626;
float fD=277.183;
float fE=329.628;
float fF=349.228;
float fG=391.995;
float fA=444.000;
float fB=493.883;

int Note(BYTE* pBufferForAudio, int iStart, int iDuration, float fNote, float fDiv);

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)//Funkcja obs³ugi komunikatów
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		/*case EN_CHANGE:
			switch (LOWORD(wParam))
			{
			case IDC_EDIT1:
				
					HWND hwndEditBox = GetDlgItem(hwndDlg, IDC_EDIT1);
					int iTextLength = GetWindowTextLength(hwndEditBox);//Pobranie d³ugoœci tekstu
					CHAR szText[500];
					GetWindowText(hwndEditBox, szText, iTextLength + 1);//Pobranie Tekstu
					HWND hwndButton = GetDlgItem(hwndDlg, IDC_BUTTON1);
					SetWindowText(hwndButton, szText);
					return TRUE;
				
			}*/
		return FALSE;

		case BN_CLICKED://Zdarzenie klikniêcia 
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON1://Klikniêcie na nasz przycisk Button1
				/*MessageBox(hwndDlg, TEXT("Klikniecie tego magicznego przyciusku powoduje zmiane jego opisu"), TEXT("Magiczny Przycisk"), MB_OK);
				//Zmiana pola text na przycisku na tekst z EditBox
				HWND hwndEditBox = GetDlgItem(hwndDlg, IDC_EDIT1); 
				int iTextLength = GetWindowTextLength(hwndEditBox);//Pobranie d³ugoœci tekstu
				CHAR szText[500];
				GetWindowText(hwndEditBox, szText, iTextLength + 1);//Pobranie Tekstu
				SetWindowText((HWND)lParam, szText);//Przypisanei tekstu do przycisku*/

				return TRUE;
			}
		}
		return FALSE;
	
	case WM_LBUTTONDOWN :
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

  WAVEFORMATEX pcmWaveFormat;
  pcmWaveFormat.wFormatTag = WAVE_FORMAT_PCM; // bez straty
  pcmWaveFormat.nChannels=1;//ile kanalow
  pcmWaveFormat.nSamplesPerSec=44100L;//czestotliwosc probkowania, L daje wartoœæ typu long
  pcmWaveFormat.wBitsPerSample=8;//ile bitow na probke
  pcmWaveFormat.nAvgBytesPerSec=pcmWaveFormat.nSamplesPerSec*pcmWaveFormat.wBitsPerSample/8;
  pcmWaveFormat.nBlockAlign=1;//
  pcmWaveFormat.cbSize=0;//

  MMRESULT mmResult=0;
  HWAVEOUT hwo=0;
  UINT devId=0;

  for (devId = 0;devId < waveOutGetNumDevs();++devId)
  {
    mmResult=waveOutOpen(&hwo,devId,&pcmWaveFormat,0,0,CALLBACK_NULL);
    if(mmResult==MMSYSERR_NOERROR)break;
  }
  if (mmResult != MMSYSERR_NOERROR) //gdy nie uda siê otworzyæ karty dŸwiêkowej
  {
    MessageBox(hwndMainWindow,TEXT("Nie znaleziono karty"),TEXT("Error"),MB_OK);
    return mmResult;
  }

  BYTE* pBufferForAudio = new BYTE [10* pcmWaveFormat.nAvgBytesPerSec];

 
  int i = 0;
  /*for (int i = 0;i < 4 * pcmWaveFormat.nAvgBytesPerSec;++i) // 4 to dlugosc bufora
  {
    //f=f+0.1;
    float t=i/ (float) pcmWaveFormat.nSamplesPerSec;
    f = f1 + (f2 - f1)*t; // rosnacy dzwiek
    pBufferForAudio[i]=128*sin(2*PI*f*t)+128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }*/
  {
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
    float t = i / (float)pcmWaveFormat.nSamplesPerSec;
    f = fF;// 
    pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }
  for (;i < 3.5 * pcmWaveFormat.nAvgBytesPerSec;++i) // 4 to dlugosc bufora
  {
    //f=f+0.1;
    float t = i / (float)pcmWaveFormat.nSamplesPerSec;
    f = fG;// 
    pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }
  for (;i < 4 * pcmWaveFormat.nAvgBytesPerSec;++i) // 4 to dlugosc bufora
  {
    //f=f+0.1;
    float t = i / (float)pcmWaveFormat.nSamplesPerSec;
    f = fA;// 
    pBufferForAudio[i] = 128 * sin(2 * PI*f*t) + 128; //128 * ¿eby przeskalowaæ zakres od -1 do 1 a nie 0 255
  }
  }
  WAVEHDR whdr;
  ZeroMemory(&whdr,sizeof(WAVEHDR));
  whdr.lpData = (LPSTR) pBufferForAudio;
  whdr.dwBufferLength=1 * pcmWaveFormat.nAvgBytesPerSec; // 4 to dlugosc bufora

  mmResult=waveOutPrepareHeader(hwo,&whdr,sizeof(WAVEHDR));
  mmResult=waveOutWrite(hwo,&whdr,sizeof(WAVEHDR));

  while((whdr.dwFlags&WHDR_DONE)!=WHDR_DONE)Sleep(100);

  mmResult=waveOutUnprepareHeader(hwo, &whdr, sizeof(WAVEHDR));

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
  whdr.dwBufferLength = 10 * pcmWaveFormat.nAvgBytesPerSec;
  {

   float S=pcmWaveFormat.nAvgBytesPerSec;
   int i=0;
   i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.25*S, fC, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.125*S, fG, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.25*S, fC, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.125*S, fG, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.5*S, fB, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.5*S, fB, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.5*S, fB, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.25*S, fC, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.125*S, fG, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.25*S, fC, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.125*S, fG, pcmWaveFormat.nSamplesPerSec);
   i = Note(pBufferForAudio, i, 0.5*S, fE, pcmWaveFormat.nSamplesPerSec);

  }
  mmResult = waveOutPrepareHeader(hwo, &whdr, sizeof(WAVEHDR));
  mmResult = waveOutWrite(hwo, &whdr, sizeof(WAVEHDR));
  while ((whdr.dwFlags&WHDR_DONE) != WHDR_DONE)Sleep(100);
  mmResult = waveOutUnprepareHeader(hwo, &whdr, sizeof(WAVEHDR));
  mmResult=waveOutClose(hwo);
  delete [] pBufferForAudio;
}

int Note(BYTE* pBufferForAudio, int iStart, int iDuration, float fNote, float fDiv)
{
int i;
  for (i = iStart; i < iStart + iDuration-200;i++)
  {
  float t=i/fDiv;
  pBufferForAudio[i]=128*sin(2*PI*fNote*t)+128;
  }
  for (;i < iStart + iDuration;++i)
  {
    float t=i/fDiv;
    pBufferForAudio[i]=128;
  }
  return i;
}
// DO DOMU SKOMPONOWAC UTWOR TRWAJACY MINIMUM 35 SEKUND 
// char sNotes = "E E E C G ";
// char sTime = "16 16 16 8 4";
//w taki sposób ma byæ zrealizowane 

/*
MessageBox(0 , "Treœæ wyœwietlana" , "Tytu³" , Rodzaj); // zwracany typ int
*/
/*
Wyœwietlanie zmiennej int
char tekst=sprintf_s(text, "Are you looking for %d", mid); // wpisujemy zmienn¹ typu int do tablicy char, %d pokazuje gdzie bêdzie, mid to nazwa zmiennej
MessageBox(0,(LPCSTR)text  , "Question", MB_YESNO); // rzutowanie na LPCSTR bo visual ma downa


*/

	/*
	MessageBox(0 , "Treœæ wyœwietlana" , "Tytu³" , Rodzaj); // zwracany typ int
	*/
	/*
	Wyœwietlanie zmiennej int
	char tekst=sprintf_s(text, "Are you looking for %d", mid); // wpisujemy zmienn¹ typu int do tablicy char, %d pokazuje gdzie bêdzie, mid to nazwa zmiennej
	MessageBox(0,(LPCSTR)text  , "Question", MB_YESNO); // rzutowanie na LPCSTR bo visual ma downa


	*/
