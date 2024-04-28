#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include"numeracja.h"

int streak=0;
	int poprzedni=0;

//IB
//AZ

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


	int w_y;
	int px,py;
	int kierunek_poczatkowy_x_y;
	FILE *file_pom;
	void znajdz_koniec(int n,int m){	//n-liczba wierszy; iteracja po brzegach labiryntu szukajca pola koncowego
		for(int i=0;i<n;i++){		// pierwsza kolumna
			
			
			if(zwroc_wartosc_x_y("plik_programu.txt",0,i,m)==-3){
				
				px=0;
				py=i;
				kierunek_poczatkowy_x_y=2;
				return;
			}
		}
		for(int i=0;i<n;i++){		// ostatnia kolumna
			if(zwroc_wartosc_x_y("plik_programu.txt",m-1,i,m)==-3){
				px=m-1;
				py=i;
				
				kierunek_poczatkowy_x_y=4;
				return;
			}
		}
		for(int i=0;i<m;i++){		// pierwszy wiersz
			if(zwroc_wartosc_x_y("plik_programu.txt",i,0,m)==-3){
				px=i;
				py=0;
				kierunek_poczatkowy_x_y=3;
				return;
			}
		}
		for(int i=0;i<m;i++){		// ostatni wiersz
			if(zwroc_wartosc_x_y("plik_programu.txt",i,n-1,m)==-3){
				px=i;
				py=n-1;
				kierunek_poczatkowy_x_y=1;
				return;
			}
		}
	}	
	void wypisz(int co){ //0-przod , 1 - lewo, 2 - prawo
		
		if(co==0){
			streak++;
			return;
		}
		else{
			fprintf(file_pom,"FORWARD %d\n", streak);
			streak=0;
			if(co==3) return;
		}
		if(co==1){
			fprintf(file_pom,"TURNLEFT\n");
		}
		else if(co==2){
			fprintf(file_pom,"TURNRIGHT\n");
		}
	}
	
	int sprawdz_dostepnosc(int x, int y, int w_teraz, int w_x, const char* nazwa_pliku){ //sprawdza czy pole ma mniejsza odleglosc od startu niz aktualne pole
		if(w_teraz>=zwroc_wartosc_x_y(nazwa_pliku,x,y,w_x) && 			   zwroc_wartosc_x_y(nazwa_pliku,x,y,w_x)>=0){
			return zwroc_wartosc_x_y(nazwa_pliku,x,y,w_x);
		}
		return INT_MAX;
	}
	
	void porusz(int *x, int *y, int *kierunek, int *w_teraz,  char gdzie,int w_x, const char* nazwa_pliku){ // przejscie na poprzednie pole: zmiana wspolrzednych oraz kierunku aktualnego pola
		
		if(gdzie=='L'){
			(*w_teraz)=zwroc_wartosc_x_y(nazwa_pliku,*x-1,*y,w_x);
			(*x)--;
			(*kierunek)=2;
		}
		else if(gdzie=='G'){
			(*w_teraz)=zwroc_wartosc_x_y(nazwa_pliku,*x,*y-1,w_x);
			(*y)--;
			(*kierunek)=3;
		}
		else if(gdzie=='D'){
			(*w_teraz)=zwroc_wartosc_x_y(nazwa_pliku,*x,*y+1,w_x);
			(*y)++;
			(*kierunek)=1;
		}
		else{
			(*w_teraz)=zwroc_wartosc_x_y(nazwa_pliku,*x+1,*y,w_x);
			(*x)++;
			(*kierunek)=4;
		}
		
	}
	void dfs(const char* nazwa_pliku,int x, int y,int w_teraz,int kierunek,int w_x){ //kierunek od 1 zgodnie ze wskazowkami zegara
		file_pom=fopen("pomocniczy.txt","w");
		while(w_teraz>0){
			// ify sprawdzajace, czy nie doszlo sie do brzegu labiryntu
			if(x>0){
				if(sprawdz_dostepnosc(x-1,y,w_teraz,w_x,nazwa_pliku)<=(w_teraz-1)){
					if(kierunek==1) wypisz(1);
					if(kierunek==3) wypisz(2);
					if(kierunek==2) wypisz(0);
					else wypisz(0);
					porusz(&x,&y,&kierunek,&w_teraz,'L',w_x,nazwa_pliku);
					continue;
				}
				
			}
			if(y>0){
				if(sprawdz_dostepnosc(x,y-1,w_teraz,w_x,nazwa_pliku)<=(w_teraz-1)){
					if(kierunek==4) wypisz(2);
					if(kierunek==2) wypisz(1);
					if(kierunek==3) wypisz(0);
					else wypisz(0);
					porusz(&x,&y,&kierunek,&w_teraz,'G',w_x,nazwa_pliku);
					continue;
				}
			}
			if(y<(w_y-1)){
				if(sprawdz_dostepnosc(x,y+1,w_teraz,w_x,nazwa_pliku)<=(w_teraz-1)){
					if(kierunek==4) wypisz(1);
					if(kierunek==2) wypisz(2);
					if(kierunek==1) wypisz(0);
					else wypisz(0);
					porusz(&x,&y,&kierunek,&w_teraz,'D',w_x,nazwa_pliku);
					continue;
				}
			}
			if(x<(w_x-1)){
				if(sprawdz_dostepnosc(x+1,y,w_teraz,w_x,nazwa_pliku)<=(w_teraz-1)){

					if(kierunek==3) wypisz(1);
					if(kierunek==1) wypisz(2);
					if(kierunek==4) wypisz(0);
					else wypisz(0);
					porusz(&x,&y,&kierunek,&w_teraz,'P',w_x,nazwa_pliku);
					continue;
				}
			}
		}
		wypisz(3);
		fclose(file_pom);
	}
void obrot(char *fn){ // przepisanie wyjscia z pliku pomocniczego (odwroconego) do wyjsciowego

    FILE *plik = fopen("pomocniczy.txt","r");
    FILE *plik_wyj=fopen(fn,"w");
    
    int rozm;
    int poz;
    char znak;
    char buf[2100];
    int maks = sizeof(buf) - 1;
    int dlglinii = 0;

    fseek(plik, 0, SEEK_END);
    rozm = ftell(plik);
    
    fprintf(plik_wyj,"START\n");

    for (poz = rozm - 2; poz >= 0; poz--){
        fseek(plik, poz, SEEK_SET);
        znak = fgetc(plik);

        if (znak == '\n' || poz == 0){
        	if (poz == 0){
                	buf[dlglinii++] = znak;
		}
            for (int i = dlglinii - 1; i >= 0; i--){
                fprintf(plik_wyj,"%c",buf[i]);
            }
            fprintf(plik_wyj,"\n");
            dlglinii= 0;
        }
        else{
            if (dlglinii < maks){
                buf[dlglinii++] = znak;
            }
        }
    }
    fprintf(plik_wyj,"STOP\n");
    
    fclose(plik);
    fclose(plik_wyj);
}
    

