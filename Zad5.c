
/*
Autorzy kodu:
Piotr Siciński      - lider zespołu
Julia Dasiewicz     - korektor komentarzy 
Wojciech Szczytko   - koordynator ocen
Juliusz Winnicki    - administrator kodu
*/

/*
Oświadczamy, że niniejsza praca stanowiąca podstawię do uznania osiągnięcia efektów uczenia się przedmiotu POPRO została wykonana przeze nas wspólnie.
*/

//-----------------------BIBLIOTEKI--------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//-----------------------DEFINICJE---------------------------

#define MAX_NAZWA 100
#define KROK_ODST_WYSWIETLANIA 2

//-----------------------STRUKTURY---------------------------

typedef struct biblioteka //Julia
{

    int biblioteka_ID; //jest kluczem w BST
    char nazwaBiblioteki[MAX_NAZWA];
    char miasto[MAX_NAZWA];
} biblioteka;

typedef struct lisc //struktura liść BST                                              //Wojtek
{

    struct biblioteka biblioteka; // zawieraID ktore jest kluczem
    struct lisc *prawy;
    struct lisc *lewy;
} lisc;

//-----------------------FUNKCJE---------------------------

lisc *dodajKorzen(int biblioteka_ID, char *nazwaBiblioteki, char *miasto);            //Wojtek
void dodajlisc(int biblioteka_ID, char *nazwaBiblioteki, char *miasto, lisc *Korzen); //Wojtek
lisc *usunLisc(int biblioteka_ID, lisc *Korzen);                                      //Julia
lisc *Szukaj(int biblioteka_ID, lisc *Korzen);                                        //Juliusz
lisc *SzukajNajmniejszej(lisc *Korzen);                                               //Juliusz
void usunGalaz(lisc *Korzen, bool delete);                                            //Julia
lisc *printSzukaj(int biblioteka_ID, lisc *Korzen);                                   //Juliusz
void wyswietlRekordKrotko(biblioteka bibl);                                           //Piotr
void wyswietlRekord(biblioteka bibl);                                                 //Piotr
void wyswietlDrzewo(lisc *ojciec, int odstep);                                        //Piotr
lisc *wczytajPlik(char *nazwaPliku, lisc *Korzen);                                    //Piotr

//------------------------------------------------------------------
// Praca wspólna
// W funkcji main zaprezentowaliśmy działanie/test wszystkch funkcji
//------------------------------------------------------------------

int main()
{
    lisc *Korzen = dodajKorzen(35, "ALEKSANDRYJSKA", "ALEKSANDRIA");
    dodajlisc(30, "GLOWNA", "NIEZNANE", Korzen);
    dodajlisc(21, "POMARANCZOWA", "POZNAN", Korzen);
    dodajlisc(45, "PIOTRKOWSKA", "WARSZAWA", Korzen);
    dodajlisc(67, "WOJCIECHOWSKA", "WARSZAWA", Korzen);
    dodajlisc(72, "JULII", "WARSZAWA", Korzen);
    dodajlisc(53, "JULIUSZA", "SZCZECIN", Korzen);
    dodajlisc(41, "MALA", "POZNAN", Korzen);
    dodajlisc(67, "STARA", "WASZYNGTON", Korzen);

    printSzukaj(43, Korzen);
    printSzukaj(45, Korzen);

    usunLisc(45, Korzen);
    printSzukaj(45, Korzen);

    wyswietlDrzewo(Korzen, 0);
    putchar('\n');

    usunGalaz(Korzen->prawy, false);
    wyswietlDrzewo(Korzen, 0);

    usunGalaz(Korzen, true);

    Korzen = NULL;
    Korzen = wczytajPlik("testowy.txt", Korzen);
    wyswietlDrzewo(Korzen, 0);

    usunGalaz(Korzen, true); // zwalnianie pamięci

    return 0;
}

lisc *dodajKorzen(int biblioteka_ID, char *nazwaBiblioteki, char *miasto) // Funkcja dodajKorzen tworzy korzeń, będący
{                                                                         // podstawą nowego drzewa binarnego.
    lisc *nowyKorzen = (lisc *)malloc(sizeof(lisc));
    nowyKorzen->biblioteka.biblioteka_ID = biblioteka_ID;
    strcpy(nowyKorzen->biblioteka.miasto, miasto);
    strcpy(nowyKorzen->biblioteka.nazwaBiblioteki, nazwaBiblioteki);
    nowyKorzen->lewy = NULL;
    nowyKorzen->prawy = NULL;
    printf("Dodano nowy korzen\tnazwaBiblioteki: %s, miasto: %s, biblioteka_ID: %d\n\n", nazwaBiblioteki, miasto, biblioteka_ID);
    return nowyKorzen;
}

void dodajlisc(int biblioteka_ID, char *nazwaBiblioteki, char *miasto, lisc *Korzen) // Funkcja dodajlisc dodaje kolejne rekordy do
{                                                                                    // naszego drzewa binarnego, zaczynając od
    if (Szukaj(biblioteka_ID, Korzen) == NULL)                                       // rozbudowania korzenia stworzonego w dodajKorzen.
    {
        lisc *nowylisc = (lisc *)malloc(sizeof(lisc));
        lisc *KorzenKopia = Korzen;

        nowylisc->biblioteka.biblioteka_ID = biblioteka_ID;
        strcpy(nowylisc->biblioteka.nazwaBiblioteki, nazwaBiblioteki);
        strcpy(nowylisc->biblioteka.miasto, miasto);
        nowylisc->lewy = NULL;
        nowylisc->prawy = NULL;
        while (KorzenKopia != NULL)
        {
            if (biblioteka_ID < KorzenKopia->biblioteka.biblioteka_ID) // Jeżeli dodawany klucz jest mniejszy od swojego
            {                                                          // ojca, umieszczamy go jako lewy rekord drzewa.
                if (KorzenKopia->lewy != NULL)
                    KorzenKopia = KorzenKopia->lewy;
                else
                {
                    KorzenKopia->lewy = nowylisc;
                    break;
                }
            }
            else // Jeżeli dodawany klucz jest większy od swojego
            {    // ojca, umieszczamy go jako prawy rekord drzewa.
                if (KorzenKopia->prawy != NULL)
                    KorzenKopia = KorzenKopia->prawy;
                else
                {
                    KorzenKopia->prawy = nowylisc;
                    break;
                }
            }
        }
        printf("Dodano nowy rekord\tnazwaBiblioteki: %s, miasto: %s, ID: %d\n", nazwaBiblioteki, miasto, biblioteka_ID);
        return;
    }
    else // Zabezpieczyliśmy bazę przed wprowadzaniem
    {    // kolejnych rekordów o istniejącym już kluczu.
        printf("\nW bazie znajduje sie juz rekord o kluczu %d\n", biblioteka_ID);
    }
}

lisc *usunLisc(int biblioteka_ID, lisc *Korzen) // Usuwa pojedynczy rekord.
{
    if (Korzen == NULL)
        return NULL;
    if (biblioteka_ID > Korzen->biblioteka.biblioteka_ID)
        Korzen->prawy = usunLisc(biblioteka_ID, Korzen->prawy);
    else if (biblioteka_ID < Korzen->biblioteka.biblioteka_ID)
        Korzen->lewy = usunLisc(biblioteka_ID, Korzen->lewy);
    else
    {
        if (Korzen->lewy == NULL && Korzen->prawy == NULL) // Jeżeli usuwany klucz jest liściem.
        {
            free(Korzen);
            return NULL;
        }

        else if (Korzen->lewy == NULL || Korzen->prawy == NULL) // Jeżeli usuwany klucz ma jednego syna.
        {
            lisc *temp;
            if (Korzen->lewy == NULL)
                temp = Korzen->prawy;
            else
                temp = Korzen->lewy;
            free(Korzen);
            return temp;
        }

        else                                                                              // Jeżeli usuwany klucz ma dwóch synów,
        {                                                                                 // to w miejsce usuniętego klucza umieszczamy
            lisc *temp = SzukajNajmniejszej(Korzen->prawy);                               // jedo następnik, czyli najmniejszy element
            strcpy(Korzen->biblioteka.nazwaBiblioteki, temp->biblioteka.nazwaBiblioteki); // spośród elementów od niego większych.
            strcpy(Korzen->biblioteka.miasto, temp->biblioteka.miasto);
            Korzen->biblioteka.biblioteka_ID = temp->biblioteka.biblioteka_ID;
            Korzen->prawy = usunLisc(temp->biblioteka.biblioteka_ID, Korzen->prawy);
        }
    }
    return Korzen;
}

lisc *SzukajNajmniejszej(lisc *Korzen) // Funkcja SzukajNajmniejszej znajduje najmniejszy
{                                      // klucz we wskazanej gałęzi.
    if (Korzen == NULL)
        return NULL;
    else if (Korzen->lewy != NULL)
        return SzukajNajmniejszej(Korzen->lewy);
    return Korzen;
}

void usunGalaz(lisc *Korzen, bool delete) // Funkacja usunGalaz usuwa wszystkie rekordy,
{                                         // które w drzewie znajdują się poniżej klucza
    if (Korzen)                           // podanego w funkcji.
    {
        usunGalaz(Korzen->lewy, true);
        usunGalaz(Korzen->prawy, true);
        if (delete)
            free(Korzen);
        Korzen->prawy = NULL;
        Korzen->lewy = NULL;
    }
}

lisc *Szukaj(int biblioteka_ID, lisc *Korzen) // Funkcja Szukaj wyszukuje wskazany klucz.
{                                             // Jest wykorzystywana w funkcji dodajlisc.
    if (Korzen != NULL)
    {
        if (Korzen->biblioteka.biblioteka_ID == biblioteka_ID)
            return Korzen;
        else if (biblioteka_ID > Korzen->biblioteka.biblioteka_ID)
            return Szukaj(biblioteka_ID, Korzen->prawy);
        else
            return Szukaj(biblioteka_ID, Korzen->lewy);
    }
    return NULL;
}

lisc *printSzukaj(int biblioteka_ID, lisc *Korzen) // Funkcja printSzukaj służy do wyszukiwania
{                                                  // kluczy i wypisywania ich w terminalu.
    if (Korzen != NULL)
    {
        if (Korzen->biblioteka.biblioteka_ID == biblioteka_ID)
        {
            printf("\nZnaleziono rekord\n");
            wyswietlRekord(Korzen->biblioteka);
            return Korzen;
        }
        else if (biblioteka_ID > Korzen->biblioteka.biblioteka_ID)
            return printSzukaj(biblioteka_ID, Korzen->prawy);
        else
            return printSzukaj(biblioteka_ID, Korzen->lewy);
    }
    printf("\nNie znaleziono rekordu %d\n", biblioteka_ID);
    return NULL;
}

void wyswietlDrzewo(lisc *ojciec, int odstep)
{
    if (ojciec)
    {
        for (int i = 0; i < odstep; ++i)
            putchar(' ');
        wyswietlRekordKrotko(ojciec->biblioteka);
        wyswietlDrzewo(ojciec->lewy, odstep + KROK_ODST_WYSWIETLANIA);
        wyswietlDrzewo(ojciec->prawy, odstep + KROK_ODST_WYSWIETLANIA);
    }
}

void wyswietlRekordKrotko(biblioteka bibl)
{
    printf("%d %s %s\n", bibl.biblioteka_ID, bibl.nazwaBiblioteki, bibl.miasto);
}

void wyswietlRekord(biblioteka bibl)
{
    printf("NazwaBiblioteki: %s, miasto: %s, ID: %d\n", bibl.nazwaBiblioteki, bibl.miasto, bibl.biblioteka_ID);
}

lisc *wczytajPlik(char *nazwaPliku, lisc *Korzen)
{
    FILE *fp;
    biblioteka tmpBibl;

    fp = fopen(nazwaPliku, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Nie udalo sie otworzyc pliku");
        return NULL;
    }

    char bufor[BUFSIZ];

    while (fgets(bufor, sizeof(bufor), fp))
    {
        size_t dlugosc = strlen(bufor);
        if (dlugosc)
        {
            if (sscanf(bufor, "%d %99s %99s ", &tmpBibl.biblioteka_ID, tmpBibl.miasto, tmpBibl.nazwaBiblioteki) != 3)
            {
                puts("Blad!");
                return NULL;
            }
            if (!Korzen)
            {
                Korzen = dodajKorzen(tmpBibl.biblioteka_ID, tmpBibl.nazwaBiblioteki, tmpBibl.miasto);
            }
            else
            {
                dodajlisc(tmpBibl.biblioteka_ID, tmpBibl.nazwaBiblioteki, tmpBibl.miasto, Korzen);
            }
        }
    }
    fclose(fp);
    return Korzen;
}