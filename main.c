#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
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
	int opt; 
    // put ':' in the starting of the 
    // string so that program can  
    //distinguish between '?' and ':'  
    int opcja=-1; //1 oznacza binarny, 0 tekstowy
    while ((opt = getopt(argc, argv, "tb")) != -1) 
    {
        switch (opt) 
        {
            case 'b':
            	opcja=1;
            	//printf("Użycie: maze.bin out.txt [-b]\n Opcja domyslna to -t.\n");
                break;
            case 't':
            	opcja=0;
            	//printf("Użycie: maze.txt out.txt [-t]\n Opcja domyslna to -t.\n");
                break;
            default:
                //printf("Użycie: maze.txt out.txt [-t]\n Opcja domyslna to -t.\n");
        }
    }
    if (opcja==-1 && argc==3) 
    {
    	printf("Nie podano flagi\n");
    	exit(-6);
    }
	if (argc!=4)
    {
    	printf("Zla liczba argumentow\n");
    	exit(-5);
    }
    char *nazwa_pliku;
    if(opcja==1)
    {
	    FILE *file = fopen(argv[3], "rb");
	    FILE *plik = fopen("zdekodowany.txt", "w");
	    
	    
	    if (file == NULL) {
		printf("Błąd podczas otwierania pliku.\n");
		exit(-1);
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
		exit(1);
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
	    fclose(plik);
	    fclose(plik2);
	    nazwa_pliku="zupelnie_rozszyfrowany.txt";
	    
    }
    else nazwa_pliku=argv[2];

	int m=wczytaj(nazwa_pliku,&w_y);

    //printf("wczytaj: %s \n",nazwa_pliku);
	int m=wczytaj(nazwa_pliku,&w_y);
	
	//printf("znajdz_koniec\n");
    znajdz_koniec(w_y,m);

    //printf("dfs\n");
    dfs("plik_programu.txt",px,py,999999999,kierunek_poczatkowy_x_y,m);

    //printf("obrot %s\n", argv[3]);
    obrot(argv[3]);

    //printf("KONIEC\n");
    return 0;
}
