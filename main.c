#include<stdio.h>
#include<stdlib.h>
#include"numeracja.h"
#include"sciezka.h"
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
int main(int argc, char** argv)
{
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
    rev_file(argv[2]);
    return 0;
}