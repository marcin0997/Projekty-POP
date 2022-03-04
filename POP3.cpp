#include <iostream>
#include <ctime>
#include <cstdlib>
#include <time.h>

const int MAX_K = 9; // maksymalna liczba nauczycieli
const int MAX_S = 9; // maksymalna liczba sal

using namespace std;

//-----------------------------------------------------
// Wyswietlanie relacji nauczycieli
//-----------------------------------------------------
void pokazRelacje(bool relacje[MAX_K][MAX_K], int K) {
	// wypisywanie nr kolumny
	cout << "    |";
	for (int i = 0; i < K; i++) {
		cout << " n" << i + 1 << " |";
	}
	cout << endl;
	for (int i = 0; i < K + 1; i++) {
		cout << "-----";
	}
	cout << endl;

	for (int i = 0; i < K; i++) {
		cout << " n" << i + 1 << " |"; // wypisywanie nr wiersza
		for (int j = 0; j < K; j++) {
			cout << " " << (relacje[i][j] ? 'x' : ' ') << "  |"; // wypisywanie x jesli nauczyciele nie moga byc w tej samej sali
		}
		cout << endl;
		for (int i = 0; i < K + 1; i++) {
			cout << "-----";
		}
		cout << endl;
	}
}

//-----------------------------------------------------
// Wyswietlanie rozmieszczenia nauczycieli w salach
//-----------------------------------------------------
void pokazSale(bool sale[MAX_S][MAX_K], int S, int K) {
	for (int i = 0; i < K; i++) {
		cout << " n" << i + 1 << " | "; // wypisywanie nr nauczyciela
		for (int s = 0; s < S; s++) {
			if (sale[s][i]) {
				cout << "s" << s + 1; // wypisywanie nr sali
				break;
			}
		}
		cout << endl;
	}
}

//-----------------------------------------------------
// Sprawdzanie czy nauczyciel moze byc w danej sali
//-----------------------------------------------------
bool czyMozeBycWSali(bool relacje[MAX_K][MAX_K], bool sale[MAX_S][MAX_K], int K, int k, int s) {
	for (int i = 0; i < K; i++) {
		if (sale[s][i] && relacje[k][i]) { // jesli nauczyciel jest w sali i sprawdzany nauczyciel nie moze byc z nim w sali
			return false;				 // to sprawdzany nauczyciel nie moze byc juz w tej sali
		}
	}
	return true; // jesli w sali nie pojawil sie nauczyciel, z ktorym sprawdzany nauczyciel nie moze byc, to sprawdzany nauczyciel moze tam byc
}

//-----------------------------------------------------
// Czyszczenie rozmieszczenia po salach
//-----------------------------------------------------
void wyczyscSale(bool sale[MAX_S][MAX_K], int S, int K) {
	for (int i = 0; i < S; i++) {
		for (int j = 0; j < K; j++) {
			sale[i][j] = 0; // usuwanie nauczyciela z sali
		}
	}
}

//-----------------------------------------------------
// Umieszczanie nauczycieli w salach
//-----------------------------------------------------
bool umiescWSalach(bool relacje[MAX_K][MAX_K], bool sale[MAX_S][MAX_K], int S, int K, bool losowo) {
	wyczyscSale(sale, S, K); // przed proba umieszczenia w salach, musimy wyczyscic rozmieszczenie

	if (losowo) { // jesli rozmieszczamy losowo
		sale[rand() % S][rand() % K] = 1; // umieszczamy losowego nauczyciela w losowej sali
	}

	for (int k = 0; k < K; k++) {
		for (int s = 0; s < S; s++) {
			if (czyMozeBycWSali(relacje, sale, K, k, s)) { // jesli nauczyciel moze byc w sali
				sale[s][k] = 1; // to jest tam umieszczany
				break;
			}
			if (s == S - 1) { // jesli dla ostatniej dostepnej sali nauczyciel nie mogl zostac umieszczony, to proba sie nie powiodla
				return false;
			}
		}
	}

	return true; // jesli wszyscy nauczyciele zostali umieszczeni to proba sie powiodla
}

//-----------------------------------------------------
// Wprowadzanie wartosci bool z konsoli
//-----------------------------------------------------
bool cinTN() {
	char c;
	cout << " (T/N) ";
	cin >> c;
	if (c == 'T' || c == 't') {
		return true;
	}
	return false;
}

//-----------------------------------------------------
// Glowna funkcja wykonywania programu
//-----------------------------------------------------
int main(){
	srand(time(NULL));
	bool relacje[MAX_K][MAX_K]; // tablica relacji
	bool sale[MAX_S][MAX_K]; // tablica rozmieszczenia nauczycieli po salach
	int K,S,P; // liczba nauczycieli, sal, prob
	bool losowo; // czy relacje maja byc wprowadzane losowo

	// wprowadzanie liczby nauczycieli
	cout << "Liczba nauczycieli: ";
	cin >> K;
	// ustawianie liczby nauczycieli w przedziale
	if (K < 1) {
		K = 1;
	}
	if (K > MAX_K) {
		K = MAX_K;
	}
	// wprowadzanie liczby sal
	cout << "Liczba dostepnych sal: ";
	cin >> S;
	if (S < 1) {
		S = 1;
	}
	if (K > MAX_S) {
		K = MAX_S;
	}
	// wprowadzanie liczby prob
	cout << "Maksymalna liczba prob: ";
	cin >> P;
	if (P < 0) {
		P = 0;
	}
	cout << endl;

	cout << "Czy relacje maja byc wprowadzone losowo?";
	losowo = cinTN();
	//  wprowadzanie relacji pomiedzy nauczycielami
	for (int i = 0; i < K; i++) {
		for (int j = i; j < K; j++) {
			if (i == j) {
				relacje[i][j] = 0;
			}
			else {
				bool b;
				if (losowo) { // wprowadzanie losowo relacji
					b = rand() % 2;
				}
				else { // wprowadzanie recznie relacji
					cout << "Czy n" << i + 1 << " i n" << j + 1 << " moga byc w jednej sali?";
					b = !cinTN();
				}
				relacje[i][j] = b;
				relacje[j][i] = b;
			}
		}
	}
	system("cls"); // czyszczenie konsoli
	pokazRelacje(relacje, K);

	bool sukces = 0; // ustalanie czy proba sie udala
	sukces = umiescWSalach(relacje, sale, S, K, 0); // pierwsza proba, bez losowej kolejnosci

	int p = 0; // liczba prob
	if (!sukces) {
		for (; p < P; p++) {
			sukces = umiescWSalach(relacje, sale, S, K, 1); // losowa proba
			if (sukces) {
				break;
			}
		}
	}
	cout << endl << "Proba nr: " << p << endl;

	// okreslanie czy udala sie jakakolwiek proba
	if (sukces) {
		pokazSale(sale, S, K);
	}
	else {
		cout << "Nie udalo sie rozmiescic nauczycieli w salach" << endl;
	}
	cin >> K;
}
