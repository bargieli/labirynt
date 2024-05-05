#include <stdio.h>
#include <stdlib.h>


//IZABELA BARGIEŁ


int zwroc_wartosc_x_y(const char* nazwa_pliku, int x, int y, int liczba_kolumn) { //zwraca wartość liczbową pola o konkretnych współrzędnych (x, y) z labiryntu zapisanego w formie pliku binarnego

    FILE* plik_roboczy = fopen(nazwa_pliku, "rb");
    
    if (plik_roboczy == NULL) {
        fprintf(stderr, "Nie udało się wczytać pliku! \n");
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




void zmien_wartosc_x_y(const char* nazwa_pliku, int x, int y, int nw_wart, int liczba_kolumn) { ///modyfikuje pole o podanych współrzędnych (x, y) w pliku binarnym nadając mu nową wartość (nw_wart)

    FILE* plik_roboczy = fopen(nazwa_pliku, "rb+");
    
    if (plik_roboczy == NULL) {
        fprintf(stderr, "Nie udało się wczytać pliku! \n");
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



void odwiedz(const char* nazwa_pliku, int x, int y, int wart_akt, int liczba_kolumn, int liczba_wierszy){ //nadaje polu o współrzędnych (x, y) wartość jego odległości od punktu początkowego (wart_akt) i odsyłająca do jego sąsiadów, aby im również nadać wartość (wart_akt+1)

    //fprintf(stderr,"%d %d\n",x,y);
    zmien_wartosc_x_y(nazwa_pliku, x, y, wart_akt, liczba_kolumn);
    
    
    if(x<liczba_kolumn-1){
        if(zwroc_wartosc_x_y(nazwa_pliku, x+1, y, liczba_kolumn)==(-2) ||  zwroc_wartosc_x_y(nazwa_pliku, x+1, y, liczba_kolumn)>wart_akt+1){
            odwiedz(nazwa_pliku, x+1, y, wart_akt+1, liczba_kolumn, liczba_wierszy);
        }
    }
    
    if(y<liczba_wierszy-1){
        if(zwroc_wartosc_x_y(nazwa_pliku, x, y+1, liczba_kolumn)==(-2) ||  zwroc_wartosc_x_y(nazwa_pliku, x, y+1, liczba_kolumn)>wart_akt+1){
            odwiedz(nazwa_pliku, x, y+1, wart_akt+1, liczba_kolumn, liczba_wierszy);
        }
    }
    
    if(x>0){
        if(zwroc_wartosc_x_y(nazwa_pliku, x-1, y, liczba_kolumn)==(-2) ||  zwroc_wartosc_x_y(nazwa_pliku, x-1, y, liczba_kolumn)>wart_akt+1){
            odwiedz(nazwa_pliku, x-1, y, wart_akt+1, liczba_kolumn, liczba_wierszy);
        }
    }
    
    if(y>0){
        if(zwroc_wartosc_x_y(nazwa_pliku, x, y-1, liczba_kolumn)==(-2) ||  zwroc_wartosc_x_y(nazwa_pliku, x, y-1, liczba_kolumn)>wart_akt+1){
            odwiedz(nazwa_pliku, x, y-1, wart_akt+1, liczba_kolumn, liczba_wierszy);
        }
    }
    
    return;
}

int wczytaj(char *nazwa_pliku, int *lini) //poddaje plik wejściowy pierwszej obróbce - zamienia znaki na liczby i zapisuje w pliku binarnym o podanej nazwie (nazwa_pliku)

 {
 	
     FILE* plik = fopen(nazwa_pliku, "r");
    
    if(plik == NULL){
        fprintf(stderr, "Nie udało się wczytać pliku! \n");
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
    odwiedz("plik_programu.txt", x0, y0, 0, dl_lin, ile_lin);
    fclose(plik_roboczy);
        return dl_lin;
 }
