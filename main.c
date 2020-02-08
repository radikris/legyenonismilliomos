#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <locale.h>
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif
typedef struct Kerdesek{
    int nehezseg;
    char kerdes[200];
    char Avalasz[50];
    char Bvalasz[50];
    char Cvalasz[50];
    char Dvalasz[50];
    char megfejtes;
    char kategoria[20];
    int azonosito;
    struct Kerdesek * next;
}Kerdesek;

typedef struct Voltkerdes{
    int kerdesszama;
    struct Voltkerdes * next;
}Voltkerdes;

typedef struct Segitsegek{
    bool elhasznalt;
    char tipus[30];
}Segitsegek;

//typedef enum ABCD{ A, B, C, D}ABCD;

void delay(double masodperc)
{
	/**
	A kesleltetes eleresehez a gep szamolja az idot, csak ezt masodpercbe kell valtani
	*/
	// A secundumot milli_secondumba konvertalni
	double millimasodperc = 1000 * masodperc;

	clock_t start_time = clock();

	// addig noveljuk az idot, amig el nem erjuk a konvertalt millisecet
	while (clock() < start_time + millimasodperc);
}

Kerdesek * listabeolvas(char * filename){
    FILE * fp;
    fp=fopen(filename, "r");
    Kerdesek * start=NULL;

    int tempnehez;
    char tempkerdes[200];
    char tempAvalasz[50];
    char tempBvalasz[50];
    char tempCvalasz[50];
    char tempDvalasz[50];
    char tempmegfejtes;
    char tempkategoria[20];
    int db=1;
    while(!feof(fp)){
        fscanf(fp, "%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%c\t%s", &tempnehez, tempkerdes, tempAvalasz, tempBvalasz, tempCvalasz, tempDvalasz, &tempmegfejtes, tempkategoria);
        Kerdesek * uj=(Kerdesek*)malloc(sizeof(Kerdesek));
        uj->nehezseg=tempnehez;
        //printf("%d | %s? --- A,%s | B,%s | C,%s | D,%s = Megfejtes: %c -== %s\n", tempnehez, tempkerdes, tempAvalasz, tempBvalasz, tempCvalasz, tempDvalasz, tempmegfejtes, tempkategoria);
        strcpy(uj->kerdes, tempkerdes);
        strcpy(uj->Avalasz, tempAvalasz);
        strcpy(uj->Bvalasz, tempBvalasz);
        strcpy(uj->Cvalasz, tempCvalasz);
        strcpy(uj->Dvalasz, tempDvalasz);
        uj->megfejtes=tempmegfejtes;
        strcpy(uj->kategoria, tempkategoria);
        uj->azonosito=db;
        uj->next=NULL;
        //printf("%d | %s? --- A,%s | B,%s | C,%s | D,%s = Megfejtes: %c -== %s\n", uj->nehezseg, uj->kerdes, uj->Avalasz, uj->Bvalasz, uj->Cvalasz, uj->Dvalasz, uj->megfejtes, uj->kategoria);

        if(start==NULL){
            start=uj;
        }else{
            Kerdesek * mozgo=start;
            while(mozgo->next!=NULL){
                mozgo=mozgo->next;
            }
            mozgo->next=uj;
        }
        db++;
    }
    return start;
}

Voltkerdes * hozzad(Voltkerdes * eredeti, int szama){
    Voltkerdes * uj=(Voltkerdes*)malloc(sizeof(Voltkerdes));
    uj->kerdesszama=szama;
    uj->next=NULL;
    if(eredeti==NULL)
        eredeti=uj;
    else{
        Voltkerdes * mozgo=eredeti;
        while(mozgo->next!=NULL)
            mozgo=mozgo->next;
        mozgo->next=uj;
    }
    return eredeti;
}

int kerdesekszama(Kerdesek * start){
    Kerdesek * mozgo=start;
    int db=0;
    while(mozgo!=NULL){
        mozgo=mozgo->next;
        db++;
    }
    return db;
}

Kerdesek * kivalasztottkerdes(Kerdesek * start){
    int meddig=kerdesekszama(start);
    int randomkerdes=rand()%meddig+1;
    Kerdesek * mozgo=start;
    while(mozgo->azonosito!=randomkerdes){
        mozgo=mozgo->next;
    }
    return mozgo;
}

void kiir(Kerdesek * start){
    int db=0;
    while(db<10){
        Kerdesek * mozgo=kivalasztottkerdes(start);
        printf("%d. Kerdes:\n%d | %s --- A,%s | B,%s | C,%s | D,%s\nMegfejtes: %c\nKategoria: %s\n\n", mozgo->azonosito, mozgo->nehezseg, mozgo->kerdes, mozgo->Avalasz, mozgo->Bvalasz, mozgo->Cvalasz, mozgo->Dvalasz, mozgo->megfejtes, mozgo->kategoria);
        db++;
        mozgo=mozgo->next;
    }
}

bool szerepeltmar(Voltkerdes * eleje, int sorszam){
    Voltkerdes * mozgo=eleje;
    if(eleje==NULL)
        return false;
    else{
        while(mozgo!=NULL && mozgo->kerdesszama!=sorszam){
            mozgo=mozgo->next;
        }
    }
    if(mozgo==NULL)
        return false;
    return true;
}

int milyennehezjon(int * tomb, int meret){
    int i=0;
    for(i; i<meret; i++){
        if(tomb[i]<2)
            break;
    }
    return i+1;
}

void clrscr()
{
    system("@cls||clear");
}

void megfelez(Kerdesek ** jelenlegi){
    int elso, masik;
    ujra:
    elso=rand()%4;
    masik=rand()%4;
    char tomb[4]={'A', 'B', 'C', 'D'};
    if(tomb[elso]==(*jelenlegi)->megfejtes || tomb[masik]==(*jelenlegi)->megfejtes || elso==masik)
        goto ujra;

    switch(elso){
    case 0:
        strcpy((*jelenlegi)->Avalasz, "");
        break;
    case 1:
        strcpy((*jelenlegi)->Bvalasz, "");
        break;
    case 2:
        strcpy((*jelenlegi)->Cvalasz, "");
        break;
    case 3:
        strcpy((*jelenlegi)->Dvalasz, "");
        break;
    }

    switch(masik){
    case 0:
        strcpy((*jelenlegi)->Avalasz, "");
        break;
    case 1:
        strcpy((*jelenlegi)->Bvalasz, "");
        break;
    case 2:
        strcpy((*jelenlegi)->Cvalasz, "");
        break;
    case 3:
        strcpy((*jelenlegi)->Dvalasz, "");
        break;
    }

}

int main()
{
    setlocale(LC_ALL, "");
    #if defined(WIN32) || defined(_WIN32)
    SetConsoleCP(1250); SetConsoleOutputCP(1250);
    #endif
    srand(time(0));
    Kerdesek * start=listabeolvas("loimnote.txt");
    int number_of_questions=kerdesekszama(start);
    //kiir(start);
    Voltkerdes * eleje=NULL;
    int nehezsegekdarab[15]={0};
    char valasz;
    int sum=0;
    int db=0;
    while(db<30){
        ujra:
        ;
        Kerdesek * mozgo=kivalasztottkerdes(start);
        if(szerepeltmar(eleje, mozgo->azonosito)){
            //printf("Ez mar volt: %s!\n", mozgo->kerdes);
            goto ujra;
        }
        int kovi=milyennehezjon(nehezsegekdarab, 15);
        if(kovi!=mozgo->nehezseg)
            goto ujra;
        else{
            nehezsegekdarab[kovi-1]++;
        }
        //printf("%d. Kerdes:\n%d | %s\nA,%s | B,%s | C,%s | D,%s\nMegfejtes: %c\nKategoria: %s\n\n", mozgo->azonosito, mozgo->nehezseg, mozgo->kerdes, mozgo->Avalasz, mozgo->Bvalasz, mozgo->Cvalasz, mozgo->Dvalasz, mozgo->megfejtes, mozgo->kategoria);
        printf("%d. Kerdes:\n%d | %s\nA,%s | B,%s | C,%s | D,%s\nKategoria: %s\n\n", mozgo->azonosito, mozgo->nehezseg, mozgo->kerdes, mozgo->Avalasz, mozgo->Bvalasz, mozgo->Cvalasz, mozgo->Dvalasz, mozgo->kategoria);
        printf("\n\tSzeretnel segitseget hasznalni?");
        char dontes;
        scanf(" %c", &dontes);
        if(dontes=='i' || dontes=='I'){
            clrscr();
            megfelez(&mozgo);
            printf("%d. Kerdes:\n%d | %s\nA,%s | B,%s | C,%s | D,%s\nKategoria: %s\n\n", mozgo->azonosito, mozgo->nehezseg, mozgo->kerdes, mozgo->Avalasz, mozgo->Bvalasz, mozgo->Cvalasz, mozgo->Dvalasz, mozgo->kategoria);
        }else{
            printf("Rendben, akkor mit tippelsz?\n");
        }
        scanf(" %c", &valasz);
        valasz=tolower(valasz);
        printf("Megjeloltuk! Vajon helyes-e?\n");
        for(int i=0; i<3; i++){
            delay(0.5);
            printf(".");
        }
        puts("\n");
        mozgo->megfejtes=tolower(mozgo->megfejtes);
        if(valasz==mozgo->megfejtes){
            printf("Helyes valasz!\nNyertel %d Ft-ot\n", 50000*(kovi));
            sum+=50000*kovi;
            printf("Jelenlegi nyeremeny: %d\n", sum);
        }else{
            printf("Rossz valasz, a helyes megfejtes: %c volt\n", mozgo->megfejtes);
            break;
        }
        db++;
        eleje=hozzad(eleje, mozgo->azonosito);
        mozgo=mozgo->next;
        getchar();
        getchar();
        clrscr();
    }
    if(db==30)
        printf("Gratulalok, megnyerted az osszes dellat!\nNyeremenyed: %d", sum);
    else{
        printf("tuntuntuutntuuuuuuuu.\n Vesztettel, te luzer!\n");
    }
    return 0;
}
