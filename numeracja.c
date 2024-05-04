#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//IZABELA BARGIEŁ


#define MAX_SIZE (511900/4)

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

int isEmpty(Queue* q) {
    return (q->front == -1 && q->rear == -1);
}

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

void freeQueue(Queue* q) {
    free(q);
}

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

/*
int q_pop(const char* nazwa_pliku) { //zwraca wartość liczbową pola o konkretnych współrzędnych (x, y) z labiryntu zapisanego w formie pliku binarnego
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
*/


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


void odwiedz(const char* nazwa_pliku, int x, int y, int liczba_kolumn, int liczba_wierszy) {
    Queue* q = createQueue();

    zmien_wartosc_x_y(nazwa_pliku, x, y, 0, liczba_kolumn);
    q_dodaj(q, x + y * liczba_kolumn); // Dodanie współrzędnych startowych do kolejki

    while (!isEmpty(q)) {
        int akt_pole = q_pop(q);
        x = akt_pole % liczba_kolumn;
        y = akt_pole / liczba_kolumn;

        // Tablice przesunięć
        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};

        // Iteracja po wszystkich sąsiadach
        for (int i = 0; i < 4; ++i) {
            int new_x = x + dx[i];
            int new_y = y + dy[i];

            // Sprawdzenie czy sąsiad jest w granicach labiryntu
            if (new_x >= 0 && new_x < liczba_kolumn && new_y >= 0 && new_y < liczba_wierszy) {
                // Sprawdzenie czy sąsiad nie jest ścianą oraz czy nie został już odwiedzony
                int wartosc = zwroc_wartosc_x_y(nazwa_pliku, new_x, new_y, liczba_kolumn);
                if (wartosc != -1 && wartosc == -2) {
                    q_dodaj(q, new_x + new_y * liczba_kolumn); // Dodanie sąsiada do kolejki
                    zmien_wartosc_x_y(nazwa_pliku, new_x, new_y, zwroc_wartosc_x_y(nazwa_pliku, x, y, liczba_kolumn) + 1, liczba_kolumn); // Aktualizacja odległości
                }
            }
        }
    }
}



 
 
 
int wczytaj(char *nazwa_pliku, int *lini) {
    FILE* plik = fopen(nazwa_pliku, "r");
    
    if (plik == NULL) {
        fprintf(stderr, "Nie udało się wczytać pliku! WCZYTAJ\n");
        exit(-1);
    }
    
    FILE* plik_roboczy = fopen("plik_programu.txt", "wb+");
    
    int x0, y0;
    int licz = 0;
    int dl_lin = -1;
    char a;
    int* bufor = (int*)malloc(MAX_SIZE * sizeof(int));

    if (bufor == NULL) {
        fprintf(stderr, "Błąd alokacji pamięci dla bufora!\n");
        exit(EXIT_FAILURE);
    }

    while ((a = fgetc(plik)) != EOF) {
        int wartosc;
        if (a == 'X') wartosc = -1;
        else if (a == 'P') {
            x0 = licz;
            y0 = licz;
            wartosc = 0;
        } else if (a == ' ') wartosc = -2;
        else if (a == 'K') wartosc = -3;
        else {
            if (dl_lin == -1) dl_lin = licz;
            continue;
        }

        bufor[licz++] = wartosc;
    }

    fwrite(bufor, sizeof(int), licz, plik_roboczy);

    fclose(plik_roboczy);
    fclose(plik);

    x0 %= dl_lin;
    y0 = y0 / dl_lin;
    int ile_lin = licz / dl_lin;
    *lini = ile_lin;

    plik_roboczy = fopen("plik_programu.txt", "rb+");
    odwiedz("plik_programu.txt", x0, y0, dl_lin, ile_lin);
    fclose(plik_roboczy);

    free(bufor);
    return dl_lin;
}

