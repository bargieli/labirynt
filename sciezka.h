#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include"numeracja.h"
extern int streak;
extern int poprzedni;
	extern int w_y;
	extern int px,py;
	extern int kierunek_poczatkowy_x_y;

	void znajdz_koniec(int n,int m);
	void wypisz(int co);
	void dfs(const char* nazwa_pliku,int x, int y,int w_teraz,int kierunek,int w_x);
	void obrot(char *fn);
	
