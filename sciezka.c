#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include"numeracja.h"
int streak=0;
	int poprzedni=0;
	int w_y;
	int px,py;
	int kierunek_poczatkowy_x_y;
	FILE *file_pom;
	void znajdz_koniec(const char* nazwa_pliku,int n,int m) //n-liczba wierszy
	{
		for(int i=0;i<n;i++)
		{
			
			
			if(zwroc_wartosc_x_y("plik_programu.txt",0,i,m)==-3)
			{
				
				px=0;
				py=i;
				kierunek_poczatkowy_x_y=2;
				return;
			}
		}
		for(int i=0;i<n;i++)
		{
			if(zwroc_wartosc_x_y("plik_programu.txt",m-1,i,m)==-3)
			{
				px=m-1;
				py=i;
				
				kierunek_poczatkowy_x_y=4;
				return;
			}
		}
		for(int i=0;i<m;i++)
		{
			if(zwroc_wartosc_x_y("plik_programu.txt",i,0,m)==-3)
			{
				px=i;
				py=0;
				kierunek_poczatkowy_x_y=3;
				return;
			}
		}
		for(int i=0;i<m;i++)
		{
			if(zwroc_wartosc_x_y("plik_programu.txt",i,n-1,m)==-3)
			{
				px=i;
				py=n-1;
				kierunek_poczatkowy_x_y=1;
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
		
			fprintf(file_pom,"FORWARD %d\n", streak);
			streak=0;
			if(co==3) return;
		}
		if(co==1) fprintf(file_pom,"LEFT\n");
		else if(co==2) fprintf(file_pom,"RIGHT\n");
	}
	void dfs(const char* nazwa_pliku,int x, int y,int w_teraz,int kierunek,int w_x) //kierunek od 1 zgodnie ze wskazowkami zegara
	{
			file_pom=fopen("pomocniczy.txt","w");
			if(x>0)
			{
				if(w_teraz>zwroc_wartosc_x_y(nazwa_pliku,x-1,y,w_x) && zwroc_wartosc_x_y(nazwa_pliku,x-1,y,w_x)>=0)  w_teraz=zwroc_wartosc_x_y(nazwa_pliku,x-1,y,w_x);
			}
			if(y>0)
			{
				if(w_teraz>zwroc_wartosc_x_y(nazwa_pliku,x,y-1,w_x) && zwroc_wartosc_x_y(nazwa_pliku,x,y-1,w_x)>=0) w_teraz=zwroc_wartosc_x_y(nazwa_pliku,x,y-1,w_x);
			}
			if(y<(w_y-1))
			{
				if(w_teraz>zwroc_wartosc_x_y(nazwa_pliku,x,y+1,w_x) && zwroc_wartosc_x_y(nazwa_pliku,x,y+1,w_x)>=0) w_teraz=zwroc_wartosc_x_y(nazwa_pliku,x,y+1,w_x);
			}
			if(x<(w_x-1))
			{
				if(w_teraz>zwroc_wartosc_x_y(nazwa_pliku,x+1,y,w_x) && zwroc_wartosc_x_y(nazwa_pliku,x+1,y,w_x)>=0) w_teraz=zwroc_wartosc_x_y(nazwa_pliku,x+1,y,w_x);
			}
			w_teraz++;
			
		while(w_teraz>0)
		{
			if(x>0)
			{
				
				if(zwroc_wartosc_x_y(nazwa_pliku,x-1,y,w_x)<=(w_teraz-1) && zwroc_wartosc_x_y(nazwa_pliku,x-1,y,w_x)>=0)
				{
					
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
					x--;
					kierunek=2;
					w_teraz--;
					continue;
				}
			}
			if(y>0)
			{
				if(zwroc_wartosc_x_y(nazwa_pliku,x,y-1,w_x)<=(w_teraz-1) && zwroc_wartosc_x_y(nazwa_pliku,x,y-1,w_x)>=0)
				{
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
					y--;
					kierunek=3;
					w_teraz--;
					continue;
				}
			}
			if(y<(w_y-1))
			{
				if(zwroc_wartosc_x_y(nazwa_pliku,x,y+1,w_x)<=(w_teraz-1) &&  zwroc_wartosc_x_y(nazwa_pliku,x,y+1,w_x)>=0)
				{
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
					y++;
					kierunek=1;
					w_teraz--;
					continue;
				}
			}
			if(x<(w_x-1))
			{
				if(zwroc_wartosc_x_y(nazwa_pliku,x+1,y,w_x)<=(w_teraz-1) && zwroc_wartosc_x_y(nazwa_pliku,x+1,y,w_x)>=0)
				{

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
					x++;
					kierunek=4;
					w_teraz--;
					continue;
				}
			}
		}
		wypisz(3);
		fclose(file_pom);
	}
void rev_file(char *fn) 
{
    FILE *plik = fopen("pomocniczy.txt","r");
    FILE *plik_wyj=fopen(fn,"w");
    long rozmiar, pozycja;
    char znak;
    char buf[2056];
    int max_dl_linii = sizeof(buf) - 1;
    int dlugosc_linii = 0;

    fseek(plik, 0, SEEK_END);
    rozmiar = ftell(plik);

    for (pozycja = rozmiar - 2; pozycja >= 0; pozycja--) {
        fseek(plik, pozycja, SEEK_SET);
        znak = fgetc(plik);

        if (znak == '\n' || pozycja == 0) {
            if (pozycja == 0) {
                buf[dlugosc_linii++] = znak;
            }
            for (int i = dlugosc_linii - 1; i >= 0; i--) {
                fprintf(plik_wyj,"%c",buf[i]);
            }
            fprintf(plik_wyj,"\n");
            dlugosc_linii = 0;
        } else {
            if (dlugosc_linii < max_dl_linii) {
                buf[dlugosc_linii++] = znak;
            }
        }
    }

    fclose(plik);
}

//wyifowac przypadek z zerowym, zmienic rozmiary z 512


