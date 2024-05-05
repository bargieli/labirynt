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
    FILE* plik_roboczy = fopen(nazwa_pliku, "rb+");

    if (plik_roboczy == NULL) {
        fprintf(stderr, "Nie udało się wczytać pliku! ZMIEN WART\n");
        exit(420);
    }
    
    
    Queue* q = createQueue();
    Queue* q_odl = createQueue(); // Kolejka do przechowywania odległości

    zmien_wartosc_x_y(nazwa_pliku, x, y, 1, liczba_kolumn);

    q_dodaj(q, x+y*liczba_kolumn); // Dodanie współrzędnych startowych do kolejki
    q_dodaj(q_odl, 0); // Dodanie odległości 0 do kolejki odległości

    while (!isEmpty(q)) {
        int akt_pole = q_pop(q);
        int pop_odl = q_pop(q_odl);

        x = akt_pole % liczba_kolumn;
        y = akt_pole / liczba_kolumn;

        if (x < 0 || x >= liczba_kolumn || y < 0 || y >= liczba_wierszy) {
            continue;
        }
        
        int aktualna_wartosc = zwroc_wartosc_x_y(nazwa_pliku, x, y, liczba_kolumn);

        if (aktualna_wartosc == -1) // Jeśli to jest ściana, przejdź do następnego pola
            continue;

        // Sprawdź czy pole nie zostało odwiedzone wcześniej
        if (aktualna_wartosc != -2 && aktualna_wartosc <= pop_odl)
            continue;

        zmien_wartosc_x_y(nazwa_pliku, x, y, pop_odl, liczba_kolumn);

        // Dodaj sąsiadów do kolejki
        q_dodaj(q, (x + 1) + y * liczba_kolumn);
        q_dodaj(q_odl, pop_odl + 1);
        q_dodaj(q, (x - 1) + y * liczba_kolumn);
        q_dodaj(q_odl, pop_odl + 1);
        q_dodaj(q, x + (y + 1) * liczba_kolumn);
        q_dodaj(q_odl, pop_odl + 1);
        q_dodaj(q, x + (y - 1) * liczba_kolumn);
        q_dodaj(q_odl, pop_odl + 1);
    }

    freeQueue(q);
    freeQueue(q_odl);
    fclose(plik_roboczy);
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

