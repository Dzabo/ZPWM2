#include <Windows.h>
#include <stdio.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{

	/*
	MessageBox(0 , "Treœæ wyœwietlana" , "Tytu³" , Rodzaj); // zwracany typ int
	*/
	/*
	Wyœwietlanie zmiennej int
	char tekst=sprintf_s(text, "Are you looking for %d", mid); // wpisujemy zmienn¹ typu int do tablicy char, %d pokazuje gdzie bêdzie, mid to nazwa zmiennej
	MessageBox(0,(LPCSTR)text  , "Question", MB_YESNO); // rzutowanie na LPCSTR bo visual ma downa


	*/

	int left = 1;
	int right = 40;
	int mid = (left + right) / 2;
	char text[100];
	MessageBox(0, "Wybierz liczbê z zakresu 0-40", "Super gra", MB_OK);
	while (left <= right)
	{
		sprintf_s(text, "Czy Twoja liczba jest wiêksza lub równa %d", mid);
		int decision = MessageBox(0, text, "Pytanie", MB_YESNO);
		if (decision == IDYES)
		{
			left = mid + 1;
			mid = (left + right) / 2;
		}
		else
		{
			right = mid - 1;
			mid = (left + right) / 2;
		}

	}
	sprintf_s(text, "Wybra³eœ liczbê %d", mid);
	MessageBox(0, text, "Wynik gry", MB_OK);
}