#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include"numeracja.h"
<<<<<<< HEAD
int streak=0;
	int poprzedni=0;
=======

//IB

struct MazeHeader {
    uint32_t fileId;
    uint8_t escape;
    uint16_t columns;
    uint16_t lines;
    uint16_t entryX;
    uint16_t entryY;
    uint16_t exitX;
    uint16_t exitY;
    uint8_t reserved[12];
    uint32_t counter;
    uint32_t solutionOffset;
    uint8_t separator;
    uint8_t wall;
    uint8_t path;
};


int streak=1;
int poprzedni=0;
>>>>>>> 4080f94bc3de499d9a493c901f7c8e5a35166bc5
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
<<<<<<< HEAD
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
=======
    
    FILE *file = fopen("maze.bin", "rb");
    FILE *plik = fopen("zdekodowany.txt", "w");
    
    
    if (file == NULL) {
        printf("Błąd podczas otwierania pliku.\n");
        return 0;
    }

    struct MazeHeader header;
    fread(&header.fileId, sizeof(header.fileId), 1, file);
    fread(&header.escape, sizeof(header.escape), 1, file);
    fread(&header.columns, sizeof(header.columns), 1, file);
    fread(&header.lines, sizeof(header.lines), 1, file);
    fread(&header.entryX, sizeof(header.entryX), 1, file);
    fread(&header.entryY, sizeof(header.entryY), 1, file);
    fread(&header.exitX, sizeof(header.exitX), 1, file);
    fread(&header.exitY, sizeof(header.exitY), 1, file);
    fread(&header.reserved, sizeof(header.reserved), 1, file);
    fread(&header.counter, sizeof(header.counter), 1, file);
    fread(&header.solutionOffset, sizeof(header.solutionOffset), 1, file);
    fread(&header.separator, sizeof(header.separator), 1, file);
    fread(&header.wall, sizeof(header.wall), 1, file);
    fread(&header.path, sizeof(header.path), 1, file);

    
    // TESTY DO PLIKU BIN
   // printf("File ID: %u\n", header.fileId);
   // printf("Escape: %u\n", header.escape);
   // printf("Columns: %d\n", header.columns);
   // printf("Lines: %d\n", header.lines);
   // printf("Entry X: %u\n", header.entryX);
   // printf("Entry Y: %u\n", header.entryY);
   // printf("Exit X: %u\n", header.exitX);
   // printf("Exit Y: %u\n", header.exitY);
   // printf("Counter: %u\n", header.counter);
   // printf("Solution Offset: %u\n", header.solutionOffset);
   // printf("Separator: %u\n", header.separator);
   // printf("Wall: %u\n", header.wall);
   // printf("Path: %u\n", header.path);
     
    uint8_t values[3];
    for(int i=0; i<=header.columns*header.lines; i+=values[2]+1){
        fread(values, sizeof(uint8_t), 3, file); // Odczytaj 3 wartości z pliku binarnego

        if(values[1]==header.wall){
            for(int ii=0; ii<=values[2]; ii++){
                fprintf(plik, "X");
            }
        }
        
        else if (values[1]==header.path){
            for(int ii=0; ii<=values[2]; ii++){
                fprintf(plik, " ");
            }
        }
        else {
            break;
        }
    }
    
    fclose(file);
    fclose(plik);
    
    plik = fopen("zdekodowany.txt", "r");
    if (plik == NULL) {
        printf("Błąd podczas otwierania pliku.\n");
        return 1;
    }
    
    FILE *plik2 = fopen("zupelnie_rozszyfrowany.txt", "w");
    
    //UWAGA!! ZAKLADAM, ZE PODANE WSPOLRZEDNE POCZATKU I KONCA SA INDEKSOWANE OD 1 (mozna latwo zmienic jesli nie)
    char a;
    for(int i=0; i<header.lines; i++){
        for(int ii=0; ii<header.columns; ii++){
            a = getc(plik);
            if(i==header.entryY-1 && ii==header.entryX-1){
                fprintf(plik2, "P");
            } else if (i==header.exitY-1 && ii==header.exitX-1) {
                fprintf(plik2, "K");
            } else {
                fprintf(plik2, "%c", a);
            }
        }
        fprintf(plik2, "\n");
    }
    
    fclose(plik2);
    
	
	w_y;
	int m=wczytaj(argv[1],&w_y);
    znajdz_koniec(argv[1],w_y,m);
   
    dfs("plik_programu.txt",px,py,999999999,kierunek_poczatkowy_x_y,m);
    streak--;
    wypisz(3);
    
    return 1;
>>>>>>> 4080f94bc3de499d9a493c901f7c8e5a35166bc5
}

//wyifowac przypadek z zerowym, zmienic rozmiary z 512


