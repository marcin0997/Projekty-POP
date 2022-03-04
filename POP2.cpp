#include <iostream>
#include <time.h>
#include <cstdlib>
#include <cmath>

#define SZEROKOSC 20 // szerokosc planszy
#define WYSOKOSC 20 // wysokosc planszy
#define LICZBA_WROGOW 5

using namespace std;

// numeracja pionkow
const int	PUSTY = 0,
			GRACZ = 1,
			PRZESZKODA = 2,
			SKARB = 3,
			WROG = 4;
// zamiana numeru pionka na znak
const char PIONEK[] = { 'o', '@', '#', 'x', '&' };

// numeracja poziomow
const int	LATWY = 0,
			SREDNI = 1,
			TRUDNY = 2;

// zamiana poziomu trudnosci na liczbe skarbow
const int SKARBY[] = { 3, 5, 8 };

// numeracja kierunkow ruchu
const int	GORA = 0,
			DOL = 1,
			PRAWO = 2,
			LEWO = 3;
// zamiana kierunku na inkrementacje/dekrementacje skladowej x/y
const int X[] = { 0, 0, 1, -1 }; // wybranie kierunku GORA z tabel bedzie skutkowac brakie zmiany x
const int Y[] = { -1, 1, 0, 0 }; // natomiast y bedzie zmniejszone o 1

// struktura punktu
struct Punkt {
	int x, y; // wspolrzedne x i y
};

// definicje funkcji
int liczbaPionkow(int[WYSOKOSC][SZEROKOSC], int);
void generujPlansze(int[WYSOKOSC][SZEROKOSC], int);
void pokazPlansze(int[WYSOKOSC][SZEROKOSC], bool);
int odlegloscOdNajblizszegoSkarbu(int[WYSOKOSC][SZEROKOSC]);
void wykonajRuch(int[WYSOKOSC][SZEROKOSC], int,int&);
void ruszPionek(int[WYSOKOSC][SZEROKOSC], int, int, int,int&);
Punkt znajdzPionek(int[WYSOKOSC][SZEROKOSC], int, int, int);
bool czyMozeWykonacJakikolwiekRuch(int[WYSOKOSC][SZEROKOSC], int, int, int);
void ustawLosowePole(int[WYSOKOSC][SZEROKOSC], int);

// glowna funkcja programu
int main() {
	srand(time(NULL)); // ustawienie losowania
	int plansza[WYSOKOSC][SZEROKOSC]; // tablica zawierajaca pionki
	bool pokazSkarby = false; // zmienna odpowiedzialna za pokazywanie skarbow, wpisanie x podczas gry pokaze/ukryje skarbow
	int umiejetnosc = 0; // pkt umiejetnosci gracza usuwania przeszkod
	int runda = 0; // licznik rund, potrzebny do naliczania pkt umiejetnosci
	int poziom; // poziom trudnosci
	int poz; // poziom trudnosci wpisany przez gracza

	// wybor poziomu trudnosci
	cout << "Wybierz poziom trudnosci:\n1 - Latwy\n2 - Sredni\n3 - Trudny\n";
	cin >> poz;
	poziom = poz - 1; // zamiana numeracji poziomu trudnosci
	poziom = poziom < LATWY ? LATWY : (poziom > TRUDNY ? TRUDNY : poziom); // upewnienie sie ze zmianna jest z przedzialu poziomow

	generujPlansze(plansza, poziom); // poczatkowe generowanie planszy
	char kierunek; // kierunek ruchu wpisywany przez gracza

	// petla z rozgrywka
	while (1) {
		// zliczanie rund
		if (runda++ == 5) {
			runda = 0; // licznik rund jest resetowany co 5 rund
			umiejetnosc++; // umiejetnosc jest dodawana co 5 rund
		}
		// wyswietlanie planszy i informacji o grze
		pokazPlansze(plansza, pokazSkarby);
		cout << "Zebrane skarby: " << SKARBY[poziom] - liczbaPionkow(plansza, SKARB) << endl;
		cout << "Liczba umiejetnosci: " << umiejetnosc << endl;
		cout << "Odleglosc od skarbu: " << odlegloscOdNajblizszegoSkarbu(plansza) << endl;

		// sprawdzenie czy wszystkie skarby zostaly zebrane
		if (liczbaPionkow(plansza, SKARB) == 0) {
			cout << "\nWYGRANA!!! Zebrano wszystkie skarby\n";
			break;
		}
		// sprawdzenie czy gracz jest na mapie, jesli nie to musial zostac zbity
		if (liczbaPionkow(plansza, GRACZ) == 0) {
			cout << "\nPRZEGRANA!!! Pionek gracza zostal zbity\n";
			break;
		}
		// sprawdzenie czy gracz zostal zablokowany
		Punkt g = znajdzPionek(plansza, GRACZ, 0, 0);
		if (!czyMozeWykonacJakikolwiekRuch(plansza, g.x, g.y, umiejetnosc)) {
			cout << "\nPRZEGRANA!!! Pionek gracza zostal zablokowany\n";
			break;
		}
		// wprowadzanie kierunku ruchu
		cout << "Wybierz kierunek ruchu (WSAD): ";
		cin >> kierunek;
		// wykonywanie ruchu
		switch (kierunek)
		{
			case 'W':
			case 'w': wykonajRuch(plansza, GORA, umiejetnosc); break;
			case 'S':
			case 's': wykonajRuch(plansza, DOL, umiejetnosc); break;
			case 'D':
			case 'd': wykonajRuch(plansza, PRAWO, umiejetnosc); break;
			case 'A':
			case 'a': wykonajRuch(plansza, LEWO, umiejetnosc); break;
			case 'X':
			case 'x': pokazSkarby = !pokazSkarby; continue; // wyswietlanie skarbow
		}
		ustawLosowePole(plansza, PRZESZKODA); // ustawianie przeszkody w losowym polu
		Punkt w[LICZBA_WROGOW]; // tablica zawierajaca pozycje wrogow
		w[0].x = 0; w[0].y = 0; // punkt rozpoczecia poszukiwan wrogow
		// petla poszukujaca wrogow
		for (int i = 0; i < LICZBA_WROGOW; i++) {
			w[i] = znajdzPionek(plansza, WROG, w[i].x, w[i].y); // szukanie pionka od podanych wczesniej wspolrzednych
			if (i + 1 < LICZBA_WROGOW)
				w[i+1] = znajdzPionek(plansza, WROG, w[i].x+1, w[i].y); // ustawianie wspolrzednych, od ktorych szukac kolejnego pionka
		}
		// wykonywanie losowych ruchow wroga
		for (int i = 0; i < LICZBA_WROGOW; i++) {
			int _; // zmienna, ktora jest potrzebna jako argument 'umiejetnosc' dla wroga, wrog nie posiada umiejetnosci usuwania przeszkod, ale argument byl wymagany
			ruszPionek(plansza, w[i].x, w[i].y, (rand() % 4),_); // ruch wroga, z losowym kierunkiem
		}
	}
}
// ustawianie 'pionka' w losowym polu na 'planszy'
void ustawLosowePole(int plansza[WYSOKOSC][SZEROKOSC], int pionek) {
	int x, y;
	// losowanie wspolrzednych, dopoki nie sa puste
	do {
		x = rand() % SZEROKOSC;
		y = rand() % WYSOKOSC;
	} while (plansza[y][x] != PUSTY);

	plansza[y][x] = pionek; // ustawianie 'pionka' na planszy
}

// funkcja zwracajaca liczbe 'pionkow' na 'planszy'
int liczbaPionkow(int plansza[WYSOKOSC][SZEROKOSC], int pionek) {
	int licznik = 0;
	for (int y = 0; y < WYSOKOSC; y++)
		for (int x = 0; x < SZEROKOSC; x++)
			if (plansza[y][x] == pionek) licznik++;
	return licznik;
}

// generowanie losowej 'planszy' o zadanym 'poziomie' trudnosci
void generujPlansze(int plansza[WYSOKOSC][SZEROKOSC], int poziom) {
	// czyszcenie planszy
	for (int y = 0; y < WYSOKOSC; y++) {
		for (int x = 0; x < SZEROKOSC; x++) {
			plansza[y][x] = PUSTY;
		}
	}
	// losowanie pol ze skarbami
	for (int i = 0; i < SKARBY[poziom]; i++)
		ustawLosowePole(plansza, SKARB);

	//losowanie pol z wrogami
	for (int i = 0; i < LICZBA_WROGOW; i++)
		ustawLosowePole(plansza, WROG);

	// losowania pola z graczem
	ustawLosowePole(plansza, GRACZ);
}

// sprawdzenie czy pionek na wspolrzednych 'x' i 'y' na 'planszy' moze wykonac ruch w kierunku 'k', uwzgledniajac 'umiejetnosc'
bool czyMozeWykonacRuch(int plansza[WYSOKOSC][SZEROKOSC], int x, int y, int k, int umiejetnosc) {
	int nx = x + X[k], ny = y + Y[k]; // pole, na ktore chce przesunac sie pionek
	// sprawdzenie czy te pole jest w granicach planszy
	if (nx < 0 || nx >= SZEROKOSC || ny < 0 || ny >= WYSOKOSC)
		return false;
	// sprawdzenie czy jest to wrog i moze poruszyc sie na puste pole lub zbic gracza
	if (plansza[y][x] == WROG && plansza[ny][nx] != PUSTY && plansza[ny][nx] != GRACZ)
		return false;
	// sprawdzenie czy jest to gracz i ma mozliwosc usuniecia przeszkody
	if (plansza[y][x] == GRACZ && plansza[ny][nx] == PRZESZKODA && umiejetnosc > 0)
		return true;
	// sprawdzenie czy to gracz i moze przejsc na puste pole lub zebrac skarb
	if (plansza[y][x] == GRACZ && plansza[ny][nx] != PUSTY && plansza[ny][nx] != SKARB)
		return false;
	return true;
}

// sprawdzenie czy pionek na wspolrzednych 'x' i 'y' na 'planszy' moze wykonac jakikolwiek ruch, uwzgledniajac 'umiejetnosc'
bool czyMozeWykonacJakikolwiekRuch(int plansza[WYSOKOSC][SZEROKOSC], int x, int y, int umiejetnosc) {
	for (int i = 0; i < 4; i++)
		if (czyMozeWykonacRuch(plansza, x, y, i, umiejetnosc))
			return true;
	return false;
}

// szukanie 'pionka' na 'planszy' od wspolrzednych 'sx' i 'sy'
Punkt znajdzPionek(int plansza[WYSOKOSC][SZEROKOSC], int pionek, int sx, int sy) {
	Punkt p; // zwracany punkt
	p.x = -1; p.y = -1; // jesli nie odnaleziono pionka zostanie zwrocony punkt (-1,-1)
	for (int y = sy; y < WYSOKOSC; y++) {
		for (int x = sx; x < SZEROKOSC; x++) {
			if (plansza[y][x] == pionek) { // jesli pionek zostaje odnaleziony
				p.x = x; p.y = y; // zwracane sa jego wspolrzedne
				return p;
			}
			sx = 0; // resetowanie pozycji startowej, tak aby kolejne linie zaczynaly sie od 0
		}
	}
	return p;
}

// funkcja wykonywania ruchu gracza na 'planszy' w kierunku 'k', uwzgedniajac 'umiejetnosc'
void wykonajRuch(int plansza[WYSOKOSC][SZEROKOSC], int k, int &umiejetnosc) {
	Punkt p = znajdzPionek(plansza, GRACZ, 0, 0);
	ruszPionek(plansza, p.x, p.y, k, umiejetnosc);
}

// funkcja wykonywania ruchu pionka o wspolrzednych 'x' i 'y' na 'planszy' w kierunku 'k', uwzgedniajac 'umiejetnosc'
void ruszPionek(int plansza[WYSOKOSC][SZEROKOSC], int x, int y, int k, int &umiejetnosc) {
	if (x == -1 || y == -1) return;
	if (!czyMozeWykonacRuch(plansza, x, y, k, umiejetnosc)) return;
	if (plansza[y + Y[k]][x + X[k]] == PRZESZKODA) umiejetnosc--;
	plansza[y + Y[k]][x + X[k]] = plansza[y][x];
	plansza[y][x] = PUSTY;
}

// wyswietlanie 'planszy', z uwzglednieniem 'pokazania skarbow'
void pokazPlansze(int plansza[WYSOKOSC][SZEROKOSC], bool pokazSkarby) {
	system("cls"); // czyszczenie konsoli
	cout << endl;
	for (int y = 0; y < WYSOKOSC; y++) {
		for (int x = 0; x < SZEROKOSC; x++) {
			if (plansza[y][x] == SKARB) // wyswietlanie skarbu, zaleznie od argumentu 'pokazSkarby'
				cout << (pokazSkarby ? PIONEK[plansza[y][x]] : PIONEK[PUSTY]) << " ";
			else // wyswietlanie reszty pionkow
				cout << PIONEK[plansza[y][x]] << " ";
		}
		cout << endl;
	}
}

// funkcja zwracajaca odleglosc do najblizszego skarbu na 'planszy'
int odlegloscOdNajblizszegoSkarbu(int plansza[WYSOKOSC][SZEROKOSC]) {
	double min = 9999; // poczatkowa wartosc minimalna
	Punkt g = znajdzPionek(plansza, GRACZ, 0, 0); // punkt gracza
	Punkt p; // punkt skarbu
	p.x = -1; p.y = 0; // punkt od ktorego zaczynamy poszukiwania
	while(1) {
		p = znajdzPionek(plansza, SKARB, p.x + 1, p.y); // szukanie pionka skarbu od kolejnego pola
		if (p.x == -1 || p.y == -1) break; // jesli nie znaleziono skarbu konczymy petle
		double odleglosc = sqrt(pow(p.x - g.x, 2) + pow(p.y - g.y, 2)); // wyliczanie odleglosci ze wzoru na dlugosc wektora
		if (odleglosc < min) // jesli odleglosc jest mniejsza niz poprzednia
			min = odleglosc; // to ja ustawiamy jaka najmniejsza
	}
	return min; // odleglosc jest zwracana jako liczba calkowita
}
