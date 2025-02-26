#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <utility>
#include <iomanip>
#include <sstream>

using namespace std;

// Funkcja do znajdowania największej liczby przez połączenie elementów
string znajdzNajwiekszaLiczbe(vector<int>& liczby) {
    // Cache'owanie wyników to_string
    vector<pair<string, int>> liczby_str;
    for (int liczba : liczby) {
        liczby_str.emplace_back(to_string(liczba), liczba);
    }

    // Sortowanie z wykorzystaniem cache'u
    sort(liczby_str.begin(), liczby_str.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.first + b.first > b.first + a.first;
    });

    // Konstrukcja wyniku
    string wynik;
    for (const auto& p : liczby_str) {
        wynik += p.first;
    }

    return wynik;
}

// Funkcja do wczytywania danych z pliku
vector<int> wczytajDane(const string& nazwaPliku) {
    ifstream plik(nazwaPliku);
    vector<int> liczby;
    int liczba;

    if (plik.is_open()) {
        while (plik >> liczba) {
            liczby.push_back(liczba);
        }
        plik.close();
    } else {
        cerr << "Nie udało się otworzyć pliku: " << nazwaPliku << endl;
    }

    return liczby;
}

// Funkcja do formatowania czasu z dynamiczną precyzją
string sformatujCzas(double czasWykonania) {
    ostringstream oss;
    if (czasWykonania < 0.001) {
        oss << fixed << setprecision(9);
    } else {
        oss << fixed << setprecision(6);
    }
    oss << czasWykonania;
    return oss.str();
}

// Funkcja do zapisu wyniku do pliku
void zapiszWynik(const string& nazwaPliku, const string& wynik, double czasWykonania, const vector<int>& daneWejsciowe) {
    ofstream plik(nazwaPliku, ios::app);

    if (plik.is_open()) {
        auto teraz = chrono::system_clock::now();
        time_t czas = chrono::system_clock::to_time_t(teraz);

        plik << "Dane wejsciowe: ";
        for (int liczba : daneWejsciowe) {
            plik << liczba << " ";
        }
        plik << endl;

        plik << "Wynik: " << wynik << endl;
        plik << "Czas wykonania: " << sformatujCzas(czasWykonania) << " sekundy" << endl;
        plik << "Data i godzina: " << ctime(&czas);
        plik << "-------------------------------" << endl;

        plik.close();
    } else {
        cerr << "Nie udalo się zapisać wynikow do pliku: " << nazwaPliku << endl;
    }
}

int main() {
    // Wczytywanie danych z pliku
    string plikWejsciowy = "dane.txt";
    string plikWyjsciowy = "wynik.txt";
    vector<int> liczby = wczytajDane(plikWejsciowy);

    if (liczby.empty()) {
        cerr << "Brak danych wejsciowych" << endl;
        return 1;
    }

    // Wyświetlenie danych wejściowych
    cout << "Dane wejsciowe: ";
    for (int liczba : liczby) {
        cout << liczba << " ";
    }
    cout << endl;

    // Liczba powtórzeń algorytmu dla dokładniejszego pomiaru
    const int liczbaPowtorzen = 1000;
    chrono::nanoseconds lacznyCzas(0);
    string wynik;

    for (int i = 0; i < liczbaPowtorzen; ++i) {
        auto start = chrono::high_resolution_clock::now();
        wynik = znajdzNajwiekszaLiczbe(liczby);
        auto end = chrono::high_resolution_clock::now();
        lacznyCzas += chrono::duration_cast<chrono::nanoseconds>(end - start);
    }

    // Obliczanie średniego czasu wykonania
    double sredniCzas = static_cast<double>(lacznyCzas.count()) / liczbaPowtorzen / 1e9;

    // Wyświetlenie wyniku
    cout << "Najwieksza mozliwa liczba: " << wynik << endl;
    cout << "Sredni czas wykonania: " << sformatujCzas(sredniCzas) << " sekundy" << endl;
    cout << "Laczny czas wykonania (nanosekundy): " << lacznyCzas.count() << " ns" << endl;

    // Zapis wyniku do pliku
    zapiszWynik(plikWyjsciowy, wynik, sredniCzas, liczby);

    return 0;
}
