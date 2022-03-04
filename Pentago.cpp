#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;

struct historia_ruchow
{
    int wiersz;
    char kolumna, obrot, plansza_obrotu, gracz;
    char gra[6][6];
};

void nowy_zeton(char zeton[])
{
    cout<<"Witam w wyborze nowego zetonu!";
    cout<<"Podaj zeton gracza 1: ";
    cin>>zeton[0];
    cout<<"Podaj zeton gracza 2: ";
    cin>>zeton[1];
    while(zeton[0] == zeton[1])
    {
        cout<<"Zetony sa takie same, powtorz!"<<endl;
        cout<<"Podaj zeton gracza 2: ";
        cin>>zeton[1];
    }
}

void obrot(int m, char (*pole1)[3], char strona)
{
    if(strona=='P')
    {
        for(int i=0;i<m;++i)
        {
            for(int j=0;j<i;++j)
            {
                char temp = pole1[i][j];
                pole1[i][j] = pole1[j][i];
                pole1[j][i] = temp;
            }
        }

                for(int i=0;i<m;++i)
                {
                    for(int j=0;j<m/2;++j)
                    {
                        char temp = pole1[i][j];
                        pole1[i][j] = pole1[i][m-j-1];
                        pole1[i][m-j-1] = temp;
                    }
                }
            }
            else if(strona=='L')
            {
                for(int i=0;i<m;i++) reverse(pole1[i],pole1[i]+m);

                for(int i=0;i<m;i++)
                { for(int j=i;j<m;j++)
                    swap(pole1[i][j],pole1[j][i]);
                }
            }
}

bool czy_wygral(char gra[6][6], char gracz)
{
    //wygrana w poziomie
    for(int i=0;i<6;i++)
    {
        char sprawdz1 = gra[i][0], sprawdz2 = gra[i][1];
        int licznik1 = 0, licznik2=0;
        if (sprawdz2 == '0') break;
        for(int j=0;j<5;j++)
        {
            if(sprawdz1 != '0')
            {
                if(gra[i][j] == sprawdz1)
                    licznik1++;
            }

            if(gra[i][j+1] == sprawdz2)
                licznik2++;
        }
        if(licznik1 == 5 || licznik2 == 5)
            return true;
    }

    //wygrana w pionie
    for(int i=0;i<6;i++)
    {
        char sprawdz1 = gra[0][i], sprawdz2 = gra[1][i];
        int licznik1 = 0, licznik2=0;
        if (sprawdz2 == '0') break;
        for(int j=0;j<5;j++)
        {
            if(sprawdz1 != '0')
            {
                if(gra[j][i] == sprawdz1)
                    licznik1++;
            }
            if(gra[j+1][i] == sprawdz2)
                licznik2++;
        }
        if(licznik1 == 5 || licznik2 == 5)
            return true;
    }

    char sprawdz1 = gra[0][0], sprawdz2 = gra[1][1];
    int licznik1 = 0, licznik2=0, licznik3=0, licznik4=0;
    int j=5;
    //wygrana po skosie glowna przekatna (od lewej do prawej)
    for(int i=0;i<5;i++)
    {
        if (sprawdz2 == '0') break;
            //od lewej-gora do prawej-dol
            if(sprawdz1 != '0')
            {
                if(gra[i][i] == sprawdz1)
                    licznik1++;
            }
            if(gra[i+1][i] == sprawdz2)
                licznik2++;

            //od prawej-gora do lewej-dol
            if(gra[1][4] == '0') break;
            else
            {
                if(gra[0][5] != '0')
                {
                    if(gra[i][j] == gra[0][5])
                        licznik3++;
                }

                if(gra[i][j-1] == gra[1][4])
                    licznik4++;
            }
            j--;
    }
    if(licznik1==5 || licznik2 == 5 || licznik3 == 5 || licznik4 == 5)
        return true;

    licznik1=0;licznik2=0;licznik3=0;licznik4=0;
    j=1;
    int k=5;
    for(int i=0;i<5;i++)
    {
        //przekatne lewa-gora => prawa-dol
        if(gra[0][1] != '0')
        {
            if(gra[0][1] == gra[i][j])
            {
                licznik1++;
            }
        }
        if(gra[1][0] != '0')
        {
            if(gra[1][0] == gra[j][i])
            {
                licznik2++;
            }
        }

        //przekatne prawa-gora => lewa-dol
        if(gra[0][4] != '0')
        {
            if(gra[0][4] == gra[i][k-1])
            {
                licznik3++;
            }
        }
        if(gra[1][5] != '0')
        {
            if(gra[1][5] == gra[i+1][k])
            {
                licznik4++;
            }
        }

        j++;k--;
    }

    return false;
}

bool czy_remis(char gra[6][6])
{
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<6;j++)
        {
            if(gra[i][j] == '0') return false;
        }
    }
    return true;
}

void wyswietl(int n, int m, char tab[6][6], char zeton[])
{
    system("cls");  //czyszczenie okna konsoli
    char kolumna = 'A';
    char wiersz = '0';

    cout<<" ";
    //wypisanie kolumn jako liter
    for (int wypisz_kolumny=0;wypisz_kolumny<n;wypisz_kolumny++)
    {
        cout<<" "<<kolumna;
        kolumna++;
        if(wypisz_kolumny==m-1) cout<<" ";
    }
    cout<<endl;

    //generowanie planszy
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            //Wypisanie cyfr wierszy
            if(j==0)
            {
                cout<<wiersz;
                wiersz++;
            }

            if(j%3==0)
            {
                cout<<char(186);
            }

            //Zera jako spacje
            if(tab[i][j] == '0')
            {
                cout<<" "<<char(186);
            }
            else if (tab[i][j] == 'O') cout<<zeton[0]<<char(186);
            else cout<<zeton[1]<<char(186);

        }
        cout<<endl;
        if(i==2)
        {
            cout<<" ";
            for(int k=0;k<14;k++)
            {
                cout<<char(205);
            }
            cout<<endl;
        }

    }
}

bool czy_liczba_dobra(char wiersz_liczba)
{
    if(wiersz_liczba != '1' || wiersz_liczba != '2' || wiersz_liczba != '3' || wiersz_liczba != '4' || wiersz_liczba != '5' || wiersz_liczba != '0')
    {
        return true;
    }
    return false;
}

bool wykonanie_ruchu(char gra[6][6], char pole1[3][3], char pole2[3][3], char pole3[3][3], char pole4[3][3], string imie1, string imie2, int czy_pentago, double czas_gry, char zeton[], historia_ruchow historia[], int &licz_historia)
{

    //ruch gracz
    int n=6, m=3;
    int wiersz, kolumna;
    int runda=0;
    char gracz='O';
    char kolumna_litera;
    char wiersz_liczba;
    char strona, nr_planszy;

    licz_historia=0;

    wyswietl(n,m,gra, zeton);
    cout<<czas_gry;
    double czas = 0;
    double czas_gracz1 = czas_gry;
    double czas_gracz2 = czas_gry;
    clock_t start, stop;
    czas_gry = 0;


    for(;;)
    {
        if(runda%2==0)
        {
            gracz='O';
            cout<<"Gracz wykonujacy ruch: "<<imie1<<endl;
            cout<<"Pionek: "<<gracz<<endl;
            cout<<"Pozostaly czas w sekundach: "<<czas_gracz1<<endl;
        }
        else
        {
            gracz='X';
            cout<<"Gracz wykonujacy ruch: "<<imie2<<endl;
            cout<<"Pionek: "<<gracz<<endl;
            cout<<"Pozostaly czas w sekundach: "<<czas_gracz2<<endl;
        }


        char zamien_zeton;
        cout<<"Czas ruchu jeszcze nie wystartowal, szybkie pytanie: \n";
        cout<<"Czy chcesz zmienic swoj zeton? [t/n]";
        cin>>zamien_zeton;
        if(tolower(zamien_zeton) != 't') //tolower - gdyby uzytkownik podal wielka litere
        {
            cout<<"Brak zmiany zetonu w tej rundzie\n";
        }
        else
        {
            nowy_zeton(zeton);
        }



        start = clock();
        bool poprawne_dane = false;
        do
        {
            //wykonaj ruch - dolozenie zetonu
            cout<<"Podaj kolumne i wiersz np.(A5): ";
            cin>>kolumna_litera>>wiersz_liczba;
            kolumna_litera = toupper(kolumna_litera);
            while(kolumna_litera != 'A' && kolumna_litera != 'B' && kolumna_litera != 'C' && kolumna_litera != 'D' &&kolumna_litera != 'E' && kolumna_litera != 'F')
            {
                if(kolumna_litera == 'p')
                {
                    stop = clock();
                    czas_gry = (double) (stop-start) / CLOCKS_PER_SEC;
                    system("cls");
                    system("pause");
                    wyswietl(n,m,gra,zeton);
                    start = clock();
                }
                cout<<"Bledne dane!"<<endl;
                cout<<"Podaj kolumne i wiersz np.(A5): ";
                cin>>kolumna_litera>>wiersz_liczba;
            };

            while(!czy_liczba_dobra(wiersz_liczba))
            {
                cout<<"Bledne dane!"<<endl;
                cout<<"Podaj kolumne i wiersz np.(A5): ";
                cin>>kolumna_litera>>wiersz_liczba;
            };


            kolumna = (int) kolumna_litera-65;
            wiersz = (int) wiersz_liczba-48;



            if(kolumna<3 && wiersz<3)
            {
                if(pole1[wiersz][kolumna] != '0')
                {
                    cout<<"Bledne dane"<<endl;
                    poprawne_dane=false;
                }
                else
                {
                    poprawne_dane=true;
                    pole1[wiersz][kolumna] = gracz;
                }
            }
            else if (kolumna>=3 && wiersz <3)
            {
                if(pole2[wiersz][kolumna-3] != '0'){cout<<"Bledne dane"<<endl; poprawne_dane=false;}
                else
                {
                    poprawne_dane=true;
                    pole2[wiersz][kolumna-3] = gracz;
                }
            }
            else if(kolumna<3 && wiersz>=3)
            {
                if(pole3[wiersz-3][kolumna] != '0'){cout<<"Bledne dane"<<endl; poprawne_dane=false;}
                else {poprawne_dane=true; pole3[wiersz-3][kolumna] = gracz;}
            }
            else
            {
                if(pole4[wiersz-3][kolumna-3] != '0'){cout<<"Bledne dane"<<endl; poprawne_dane=false;}
                else {poprawne_dane=true; pole4[wiersz-3][kolumna-3] = gracz;}
            }
        }while(poprawne_dane == false);

        historia[licz_historia].kolumna = kolumna_litera;
        historia[licz_historia].wiersz = wiersz;
        historia[licz_historia].gracz = zeton[runda];

        //aktualizacja pola calej gry
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(i<3 && j<3) gra[i][j] = pole1[i][j];
                else if(i<3 && j>=3) gra[i][j] = pole2[i][j-3];
                else if(i>=3 && j<3) gra[i][j] = pole3[i-3][j];
                else gra[i][j] = pole4[i-3][j-3];
            }
        }

        wyswietl(n,m,gra, zeton);



        if(czy_pentago == 1)
        {
            //obrot planszy o 90*
            cout<<endl;
            cout<<"Ktora plansze chcesz obrocic? [1,2,3,4]: ";
            cin>>nr_planszy;

            cout<<"W ktora strone chcesz obrocic plansze o 90*? [L/P]";
            cin>>strona;


            if(nr_planszy == '1')
            {
                obrot(m,pole1, strona);

            }else if(nr_planszy=='2')
            {
                obrot(m,pole2,strona);
            }
            else if(nr_planszy=='3')
            {
                obrot(m, pole3, strona);
            }
            else
            {
                obrot(m, pole4, strona);
            }

            //aktualizacja pola calej gry
            for(int i=0;i<n;i++)
            {
                for(int j=0;j<n;j++)
                {
                    if(i<3 && j<3) gra[i][j] = pole1[i][j];
                    else if(i<3 && j>=3) gra[i][j] = pole2[i][j-3];
                    else if(i>=3 && j<3) gra[i][j] = pole3[i-3][j];
                    else gra[i][j] = pole4[i-3][j-3];
                    historia[licz_historia].gra[i][j] = gra[i][j];
                }
            }
        }

        historia[licz_historia].obrot = strona;
        historia[licz_historia].plansza_obrotu = nr_planszy;

        licz_historia++;



        wyswietl(n,m,gra, zeton);
        stop = clock();

        czas = (double) ((stop-start) / CLOCKS_PER_SEC ) + czas_gry;    //Dodanie czasu, ktory uplynal przed pauza

        if(runda%2==0)
        {
            czas_gracz1 -= czas;
        }
        else czas_gracz2 -= czas;


        if (czy_wygral(gra, gracz) == true)
        {
            cout<<"Wygrywa gracz: ";
            if(gracz=='O')cout<<imie1<<endl;
            else cout<<imie2<<endl;
            return true;
        }

        if(czy_remis(gra) == true)
        {
            cout<<"REMIS";
            return true;
        }

        if(czas_gracz1<0)
        {
            cout<<"Koniec czasu gracza: "<<imie1<<endl;
            cout<<"Wygrywa gracz: "<<imie2<<endl;
            return true;
        }
        if(czas_gracz2<0)
        {
            cout<<"Koniec czasu gracza: "<<imie2<<endl;
            cout<<"Wygrywa gracz: "<<imie1<<endl;
            return true;
        }

        runda++;

    }

}

bool aktualizuj_pola(char gra[6][6], char pole1[3][3], char pole2[3][3], char pole3[3][3], char pole4[3][3], string imie1, string imie2, double czas_gry, char zeton[], historia_ruchow historia[], int &licz_historia)
{
    int n=6,m=3;
    //aktualizacja pola calej gry
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(i<3 && j<3) pole1[i][j] = gra[i][j];
                else if(i<3 && j>=3) pole2[i][j-3] = gra[i][j];
                else if(i>=3 && j<3) pole3[i-3][j] = gra[i][j];
                else pole4[i-3][j-3] = gra[i][j];
            }
        }

        wyswietl(n,m,gra,zeton);

    if(wykonanie_ruchu(gra, pole1,pole2,pole3,pole4,imie1,imie2,1, czas_gry, zeton, historia, licz_historia)) return true;
    return false;
}


int main()
{
    char plansza_tytulowa[6][6] = { {'0','X','0','0','O','0'},
                                          {'O','O','X','O','X','0'},
                                          {'0','O','0','0','0','0'},
                                          {'O','O','0','0','0','0'},
                                          {'X','O','O','0','X','0'},
                                          {'0','X','0','0','X','X'}};

    string imie1, imie2;
    cout<<"Podaj imie gracza 1: ";
    cin>>imie1;
    cout<<"Podaj imie gracza 2: ";
    cin>>imie2;

    const int n=6;
    const int m=n/2;
    char gra[n][n];

    char pole1[m][m], pole2[m][m], pole3[m][m], pole4[m][m];
    int czy_pentago = 0;
    double czas_gry = 0;

    int licz_historia=0;
    historia_ruchow * historia = new historia_ruchow[36];

    for(;;)
    {
        cout<<"Podczaj czas trwania gry [w sekundach]: ";
        cin>>czas_gry;
        while(czas_gry<1)
        {
            cout<<"Bledne dane";
            cout<<"Podczaj czas trwania gry [w sekundach]: ";
            cin>>czas_gry;
        }

        char zeton[2];
        cout<<"Podaj zeton gracza 1: ";
        cin>>zeton[0];
        cout<<"Podaj zeton gracza 2: ";
        cin>>zeton[1];
        while(zeton[0] == zeton[1])
        {
            cout<<"Zetony sa takie same, powtorz!"<<endl;
            cout<<"Podaj zeton gracza 2: ";
            cin>>zeton[1];
        }




        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                gra[i][j]='0';
            }
        }

        for(int i=0;i<m;i++)
        {
            for(int j=0;j<m;j++)
            {
                pole1[i][j]='0';
                pole2[i][j]='0';
                pole3[i][j]='0';
                pole4[i][j]='0';
            }
        }

        //wyswietl(n,m,gra);
        char opcja;
        cout<<"Projekt PoP PENTAGO"<<endl;
        cout<<"[0] Rozpocznij gre w kolko i krzyzyk"<<endl;
        cout<<"[1] Rozpocznij gre w pentago"<<endl;
        cout<<"[2] Rozpocznij gre z przykladowa plansza"<<endl;
        cout<<"[3] Opis programu"<<endl;
        cout<<"[4] Opcje"<<endl;
        cout<<"[5] Wyjdz z programu"<<endl;
        cout<<"Wybierz opcje: ";
        cin>>opcja;


        switch(opcja)
        {
            case '0': if(wykonanie_ruchu(gra, pole1,pole2,pole3,pole4,imie1,imie2, 0, czas_gry, zeton, historia, licz_historia)) break;
            case '1': if(wykonanie_ruchu(gra, pole1,pole2,pole3,pole4,imie1,imie2, 1, czas_gry, zeton, historia, licz_historia)) break;
            case '2': if(aktualizuj_pola(plansza_tytulowa,pole1,pole2,pole3,pole4,imie1,imie2, czas_gry, zeton, historia, licz_historia)) break;
            case '3': cout<<"Gra w kolko i krzyzyk\n Pentago jest grš dwuosobowš na planszy. Plansza składa się z 4 ruchomych częsci, które można obracać. Każda czesc zawiera 9 miejsc na żetony.racze po kolei oddajš ruch składajšcy się z 2 częci: dołożenia swojego żetonu na niezajęte pole planszy; obrót jednej z częci planszy o 90 stopni w dowolnym kierunku. Nie można zrezygnować z żadnej częci ruchu.Wygrywa osoba, której 5 żetonów, po pełnym ruchu, sš ułożone w rzędzie/kolumnie/po skosach (podobnie do gry kółko krzyżyk)."; break;
            case '4': cout<<"Brak opcji"<<endl; break;
            case '5': return 0;
            default: cout<<"Bledne dane!! Sprobuj jeszcze raz: [1/2/3/4/5]"<<endl;
        }

        char his;
        cout<<"Czy chcesz zobaczyc historie gry? [y/n]: ";
        cin>>his;
        if(tolower(his) == 'y')
        {
            for(int i=licz_historia-1;i<=0;i--)
            {
                cout<<"Ruch nr: "<<licz_historia<<endl;
                cout<<"Wykonano ruchy: \nWstawiono gracza: "<<historia[i].gracz<<" w pole: "<<historia[i].kolumna<<" "<<historia[i].wiersz<<endl;
                cout<<"Obrocono plansze nr: "<<historia[i].plansza_obrotu<<"w strone: "<<historia[i].obrot<<endl;
                cout<<"Wynik: "<<endl;
                wyswietl(n,m,historia[i].gra, zeton);
                system("pause");
            }
        }

        char odpowiedz;
        cout<<"Czy chcesz zagrac jeszcze raz? [y/n]: ";
        cin>>odpowiedz;
        switch (odpowiedz)
        {
            case 'y': break;
            case 'n': return 0;
        }
    }


    return 0;
}
