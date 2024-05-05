#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//IZABELA BARGIEŁ


#define MAX_SIZE ((512000/4)-10200)

typedef struct {
    int data[MAX_SIZE];
    int front;
    int rear;
} Queue;

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) {
        fprintf(stderr, "Błąd alokacji pamięci!\n");
        exit(EXIT_FAILURE);
    }
    q->front = -1;
    q->rear = -1;
    return q;
}

// sprawdza czy kolejka jest pusta
int isEmpty(Queue* q) {
    return (q->front == -1 && q->rear == -1);
}

// dodaje element do kolejki
void q_dodaj(Queue* q, int value) {
    if ((q->rear + 1) % MAX_SIZE == q->front) {
        fprintf(stderr, "Kolejka pełna!\n");
        exit(EXIT_FAILURE);
    } else if (isEmpty(q)) {
        q->front = q->rear = 0;
    } else {
        q->rear = (q->rear + 1) % MAX_SIZE;
    }
    q->data[q->rear] = value;
}

// zwraca i usuwa pierwszy element kolejki
int q_pop(Queue* q) {
    if (isEmpty(q)) {
        fprintf(stderr, "Kolejka pusta!\n");
        exit(EXIT_FAILURE);
    } else if (q->front == q->rear) {
        int value = q->data[q->front];
        q->front = q->rear = -1;
        return value;
    } else {
        int value = q->data[q->front];
        q->front = (q->front + 1) % MAX_SIZE;
        return value;
    }
}

// zwalnia pamiec uzywana przez kolejke
void freeQueue(Queue* q) {
    free(q);
}

// zwraca wartość liczbową pola o konkretnych współrzędnych (x, y) z pliku binarnego
int zwroc_wartosc_x_y(const char* nazwa_pliku, int x, int y, int liczba_kolumn) { 
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

// modyfikuje pole o podanych współrzędnych (x, y) w pliku binarnym nadając mu nową wartość (nw_wart)
void zmien_wartosc_x_y(const char* nazwa_pliku, int x, int y, int nw_wart, int liczba_kolumn) { 

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

// zapisuje labirynt w postaci pliku binarnego z odleglosciami od startu
void odwiedz(const char* nazwa_pliku, int x, int y, int liczba_kolumn, int liczba_wierszy) {
    Queue* q = createQueue();

    zmien_wartosc_x_y(nazwa_pliku, x, y, 0, liczba_kolumn);
    q_dodaj(q, x + y * liczba_kolumn);

    while (!isEmpty(q)) {
        int akt_pole = q_pop(q);
        x = akt_pole % liczba_kolumn;
        y = akt_pole / liczba_kolumn;

        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};

        for (int i = 0; i < 4; ++i) {
            int new_x = x + dx[i];
            int new_y = y + dy[i];

            if (new_x >= 0 && new_x < liczba_kolumn && new_y >= 0 && new_y < liczba_wierszy) {
                int wartosc = zwroc_wartosc_x_y(nazwa_pliku, new_x, new_y, liczba_kolumn);
                // sprawdzenie czy sąsiad nie został odwiedzony
                if (wartosc == -2) {
                    q_dodaj(q, new_x + new_y * liczba_kolumn); 
                    zmien_wartosc_x_y(nazwa_pliku, new_x, new_y, zwroc_wartosc_x_y(nazwa_pliku, x, y, liczba_kolumn) + 1, liczba_kolumn); 
                }
            }
        }
    }
}



 
 
// zamienia znaki na liczby i zapisuje w pliku binarnym o podanej nazwie (nazwa_pliku) 
int wczytaj(char *nazwa_pliku, int *lini) {
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
    odwiedz("plik_programu.txt", x0, y0, dl_lin, ile_lin);
    fclose(plik_roboczy);
        return dl_lin;
 }
