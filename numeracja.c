#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//IZABELA BARGIEŁ


int zwroc_wartosc_x_y(const char* nazwa_pliku, int x, int y, int liczba_kolumn) { //zwraca wartość liczbową pola o konkretnych współrzędnych (x, y) z labiryntu zapisanego w formie pliku binarnego

    FILE* plik_roboczy = fopen(nazwa_pliku, "rb");
    
    if (plik_roboczy == NULL) {
        fprintf(stderr, "Nie udało się wczytać pliku! ZWROC\n");
        exit(-10);
    }
     
    // pozycja kursora
    long pozycja = (long)(y * liczba_kolumn + x) * sizeof(int);
    fseek(plik_roboczy, pozycja, SEEK_SET);

    int wartosc;
    fread(&wartosc, sizeof(int), 1, plik_roboczy);

    fclose(plik_roboczy);
    
    return wartosc;
}


void wydrukuj_plik_binarny(const char* nazwa_pliku, int ile_lin, int dl_lin){ //funkcja testowa, drukuje zdekodowane wartości znajdujące się w pliku binarnym

    printf("\n\n");
    for(int i=0; i<ile_lin; i++){
        for(int j=0; j<dl_lin; j++){
            printf("%d ", zwroc_wartosc_x_y(nazwa_pliku, j, i, dl_lin));
        }
        printf("\n");
    }
    printf("\n\n");
    
}




int stos_pop(const char* nazwa_pliku) { //zwraca wartość liczbową pola o konkretnych współrzędnych (x, y) z labiryntu zapisanego w formie pliku binarnego
    FILE* plik = fopen(nazwa_pliku, "rb+");

    if (plik == NULL) {
        fprintf(stderr, "Nie udało się otworzyć pliku %s! STOS_POP\n", nazwa_pliku);
        exit(-1);
    }

    int ostatnia = 0;
    fseek(plik, -sizeof(int), SEEK_END);
    fread(&ostatnia, sizeof(int), 1, plik);
    ftruncate(fileno(plik), ftell(plik) - sizeof(int));
    fclose(plik);

    return ostatnia;
}

void dodaj_wartosc_do_pliku(const char* nazwa_pliku, int wartosc) {
    FILE* plik = fopen(nazwa_pliku, "ab"); // Otwórz plik do dopisywania na koniec w trybie binarnym
    
    if (plik == NULL) {
        fprintf(stderr, "Nie udało się otworzyć pliku! DODAJ WART\n");
        exit(-1);
    }
    
    // Ustaw kursor na koniec pliku
    fseek(plik, 0, SEEK_END);
    
    // Zapisz wartość na koniec pliku
    fwrite(&wartosc, sizeof(int), 1, plik);
    
    fclose(plik);
}



void zmien_wartosc_x_y(const char* nazwa_pliku, int x, int y, int nw_wart, int liczba_kolumn) { //modyfikuje pole o podanych współrzędnych (x, y) w pliku binarnym nadając mu nową wartość (nw_wart)

    FILE* plik_roboczy = fopen(nazwa_pliku, "rb+");
    
    if (plik_roboczy == NULL) {
        fprintf(stderr, "Nie udało się wczytać pliku! ZMIEN WART\n");
        exit(420);
    }
    
    int wartosc;
    long pozycja;

    pozycja = (long)(y * liczba_kolumn + x) * sizeof(int);

    fseek(plik_roboczy, pozycja, SEEK_SET);
    fread(&wartosc, sizeof(int), 1, plik_roboczy);
    
    wartosc = nw_wart;

    fseek(plik_roboczy, pozycja, SEEK_SET);
    fwrite(&wartosc, sizeof(int), 1, plik_roboczy);

    fclose(plik_roboczy);
}

 
int czy_plik_pusty(const char* nazwa_pliku) {
    FILE* plik = fopen(nazwa_pliku, "rb");
    if (plik == NULL) {
        fprintf(stderr, "Nie udało się otworzyć pliku %s!\n", nazwa_pliku);
        return -1; // Zwracamy -1, jeśli nie udało się otworzyć pliku
    }

    fseek(plik, 0, SEEK_END); // Ustawiamy kursor na koniec pliku
    long rozmiar = ftell(plik); // Pobieramy pozycję kursora, która odpowiada rozmiarowi pliku
    fclose(plik);

    if (rozmiar == 0)
        return 1; // Zwracamy 1, jeśli plik jest pusty
    else
        return 0; // Zwracamy 0, jeśli plik nie jest pusty
}



void odwiedz(const char* nazwa_pliku, int x, int y, int liczba_kolumn, int liczba_wierszy) {
    FILE* plik = fopen("stos.txt", "rb+");
    FILE* plik2 = fopen("stos_odl.txt", "rb+");
    dodaj_wartosc_do_pliku("stos.txt", x+y*liczba_kolumn);
    dodaj_wartosc_do_pliku("stos_odl.txt", 0); // Zmienione na 0
    zmien_wartosc_x_y(nazwa_pliku, x, y, 1, liczba_kolumn);

    int akt_pole, pop_odl;
    while (!czy_plik_pusty("stos.txt")) {
        if(czy_plik_pusty("stos.txt") == 1){
            fclose(plik);
            fclose(plik2);
            //printf("DZIEKUJE I DO ZOBA!!\n");
            return;
        }
        akt_pole = stos_pop("stos.txt");
        pop_odl = stos_pop("stos_odl.txt");
        

        x = akt_pole % liczba_kolumn;
        y = akt_pole / liczba_kolumn;


        // Sprawdzamy czy pole jest poza granicami labiryntu
        if (x < 0 || x >= liczba_kolumn || y < 0 || y >= liczba_wierszy) {
            //printf("DUPA 1\n");
            continue;
        }

        // Pobieramy wartość pola
        int aktualna_wartosc = zwroc_wartosc_x_y(nazwa_pliku, x, y, liczba_kolumn);

        // Jeśli pole jest już odwiedzone lub jest ścianą, to kontynuujemy
        if ((aktualna_wartosc != -2 && aktualna_wartosc < pop_odl + 1) || aktualna_wartosc==-1) {
            //printf("DUPA AW: %d, PO: %d \n", aktualna_wartosc, pop_odl);
            continue;
        }

        // Nadajemy nową wartość polu
        zmien_wartosc_x_y(nazwa_pliku, x, y, pop_odl, liczba_kolumn);

        // Dodajemy sąsiadów na stos
        dodaj_wartosc_do_pliku("stos.txt", (x + 1) + y * liczba_kolumn);
        dodaj_wartosc_do_pliku("stos_odl.txt", pop_odl + 1);
        dodaj_wartosc_do_pliku("stos.txt", (x - 1) + y * liczba_kolumn);
        dodaj_wartosc_do_pliku("stos_odl.txt", pop_odl + 1);
        dodaj_wartosc_do_pliku("stos.txt", x + (y + 1) * liczba_kolumn);
        dodaj_wartosc_do_pliku("stos_odl.txt", pop_odl + 1);
        dodaj_wartosc_do_pliku("stos.txt", x + (y - 1) * liczba_kolumn);
        dodaj_wartosc_do_pliku("stos_odl.txt", pop_odl + 1);
        
        if (czy_plik_pusty("stos.txt") == 1) {
            fclose(plik);
            fclose(plik2);
            //printf("DZIEKUJE I DO ZOBA!!\n");
            return;
        }
    }
    fclose(plik);
    fclose(plik2);
    //printf("DZIEKUJE I DO ZOBA!!\n");
    return;
}

 
 
 
int wczytaj(char *nazwa_pliku, int *lini) //poddaje plik wejściowy pierwszej obróbce - zamienia znaki na liczby i zapisuje w pliku binarnym o podanej nazwie (nazwa_pliku)

 {
     FILE* plik = fopen(nazwa_pliku, "r");
    
    if(plik == NULL){
        fprintf(stderr, "Nie udało się wczytać pliku! WCZYTAJ\n");
        exit(-1);
    }
    
    FILE* plik_roboczy = fopen("plik_programu.txt", "rb+");
    
    int x0, y0;
    int licz =0;
    int dl_lin=-1;
    
    char a;
    while((a=fgetc(plik)) != EOF){
        
        // sciana
        if(a=='X'){
            int wartosc = -1;
            fwrite(&wartosc, sizeof(int), 1, plik_roboczy);
            licz++;
        }
        
        //poczatek
        else if(a=='P'){
            x0 = licz;
            y0 = licz;
            int wartosc = 0;
            fwrite(&wartosc, sizeof(int), 1, plik_roboczy);
            licz++;
        }
        
        //puste pole
        else if(a==' '){
            int wartosc = -2;
            fwrite(&wartosc, sizeof(int), 1, plik_roboczy);
            licz++;
        }
        
        //koniec
        else if(a=='K'){
            int wartosc = -3;
            fwrite(&wartosc, sizeof(int), 1, plik_roboczy);
            licz++;
        }
        
        else {
            if(dl_lin==-1){
                dl_lin = licz;
            }
        }
    }
    
    fclose(plik_roboczy);
    fclose(plik);
    
    
    
    x0%=dl_lin;
    y0=y0/dl_lin;
    int ile_lin = licz/dl_lin;

    *lini=ile_lin;
    plik_roboczy = fopen("plik_programu.txt", "rb+");
    odwiedz("plik_programu.txt", x0, y0, dl_lin, ile_lin);
    //wydrukuj_plik_binarny("plik_programu.txt", ile_lin, dl_lin);
    fclose(plik_roboczy);
        return dl_lin;
 }
