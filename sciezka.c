#include <stdio.h>
#include <stdlib.h>
#include"numeracja.h"
int streak=1;
int poprzedni=0;
	int w_y;
	int px,py;
	int kierunek_poczatkowy_x_y;
	void znajdz_koniec(const char* nazwa_pliku,int n,int m) //n-liczba wierszy
	{
		for(int i=0;i<n;i++)
		{
			
			
			if(zwroc_wartosc_x_y("plik_programu.txt",0,i,m)==-3)
			{
				
				px=0;
				py=i;
				kierunek_poczatkowy_x_y=4;
				return;
			}
		}
		for(int i=0;i<n;i++)
		{
			if(zwroc_wartosc_x_y("plik_programu.txt",m-1,i,m)==-3)
			{
				px=n-1;
				py=i;
				kierunek_poczatkowy_x_y=2;
				return;
			}
		}
		for(int i=0;i<m;i++)
		{
			if(zwroc_wartosc_x_y("plik_programu.txt",i,0,m)==-3)
			{
				px=i;
				py=0;
				kierunek_poczatkowy_x_y=1;
				return;
			}
		}
		for(int i=0;i<m;i++)
		{
			if(zwroc_wartosc_x_y("plik_programu.txt",i,n-1,m)==-3)
			{
				px=0;
				py=i;
				kierunek_poczatkowy_x_y=3;
				return;
			}
		}
	}	
	void wypisz(int co) //0-przod , 1 - left, 2 - right
	{
		if(co==0)
		{
			streak++;
			return;
		}
		else
		{
		
			printf("FORWARD %d\n", streak);
			streak=0;
			if(co==3) return;
		}
		if(co==1) printf("LEFT\n");
		else if(co==2) printf("RIGHT\n");
	}
	void dfs(const char* nazwa_pliku,int x, int y,int w_teraz,int kierunek,int w_x) //kierunek od 1 zgodnie ze wskazowkami zegara
	{
		if(x>0)
		{
			
			if(zwroc_wartosc_x_y(nazwa_pliku,x-1,y,w_x)<=(w_teraz-1) && zwroc_wartosc_x_y(nazwa_pliku,x-1,y,w_x)>=0)
			{
				if(w_teraz!=0) dfs(nazwa_pliku,x-1,y,zwroc_wartosc_x_y(nazwa_pliku,x-1,y,w_x),2,w_x);
				if(kierunek==1)
				{
					wypisz(1);
				}
				if(kierunek==3)
				{
					wypisz(2);
				}
				if(kierunek==2)
				{
					wypisz(0);
				}
				else wypisz(0);
				return;
			}
		}
		if(y>0)
		{
			if(zwroc_wartosc_x_y(nazwa_pliku,x,y-1,w_x)<=(w_teraz-1) && zwroc_wartosc_x_y(nazwa_pliku,x,y-1,w_x)>=0)
			{
				if(w_teraz!=0)dfs(nazwa_pliku,x,y-1,zwroc_wartosc_x_y(nazwa_pliku,x,y-1,w_x),3,w_x);
				if(kierunek==4)
				{
					wypisz(2);
				}
				if(kierunek==2)
				{
					wypisz(1);
				}
				if(kierunek==3)
				{
					wypisz(0);
				}
				else wypisz(0);
				return;
			}
		}
		if(y<(w_y-1))
		{
			if(zwroc_wartosc_x_y(nazwa_pliku,x,y+1,w_x)<=(w_teraz-1) &&  zwroc_wartosc_x_y(nazwa_pliku,x,y+1,w_x)>=0)
			{
				if(w_teraz!=0)dfs(nazwa_pliku,x,y+1,zwroc_wartosc_x_y(nazwa_pliku,x,y+1,w_x),1,w_x);
				if(kierunek==4)
				{
					wypisz(1);
				}
				if(kierunek==2)
				{
					wypisz(2);
				}
				if(kierunek==1)
				{
					wypisz(0);
				}
				else wypisz(0);
				return;
			}
		}
		if(x<(w_x-1))
		{
			if(zwroc_wartosc_x_y(nazwa_pliku,x+1,y,w_x)<=(w_teraz-1) && zwroc_wartosc_x_y(nazwa_pliku,x+1,y,w_x)>=0)
			{
				if(w_teraz!=0)dfs(nazwa_pliku,x+1,y,zwroc_wartosc_x_y(nazwa_pliku,x+1,y,w_x),4,w_x);

				if(kierunek==3)
				{
					wypisz(1);
				}
				if(kierunek==1)
				{
					wypisz(2);
				}
				if(kierunek==4)
				{
					wypisz(0);
				}
				else wypisz(0);
				return;
			}
		}
	}

int main(int argc, char** argv)
{
	
	w_y;
	int m=wczytaj(argv[1],&w_y);
    znajdz_koniec(argv[1],w_y,m);
   
    dfs("plik_programu.txt",px,py,999999999,kierunek_poczatkowy_x_y,m);
    streak--;
    wypisz(3);
}
//wyifowac przypadek z zerowym, zmienic rozmiary z 512


