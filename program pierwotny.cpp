#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>  // Biblioteka do pomiaru czasu
#include <ctime>   // Biblioteka do uzyskania daty i godziny

using namespace std;  // Dodane, aby uprościć zapis

// Funkcja porównująca dwa elementy
bool porownaj(const string &a, const string &b) {
    return a + b > b + a;
}

// Funkcja tworzenia największej liczby
string tworzNajwiekszaLiczba(vector<int> &liczby) {
    vector<string> liczby_str;
    
    // Przekształcenie liczb na ciągi znaków
    for (int liczba : liczby) {
        liczby_str.push_back(to_string(liczba));
    }

    // Posortowanie liczb
    sort(liczby_str.begin(), liczby_str.end(), porownaj);

    // Połączenie liczb w jeden ciąg
    string wynik = "";
    for (const string &liczba : liczby_str) {
        wynik += liczba;
    }

    // Usunięcie wiodących zer
    if (wynik[0] == '0') {
        return "0";
    }

    return wynik;
}

// Funkcja do odczytu danych z pliku
void odczytajDane(const string &nazwa_pliku, vector<int> &liczby) {
    ifstream plik(nazwa_pliku);
    if (!plik) {
        cerr << "Blad otwarcia pliku wejsciowego!" << endl;
        return;
    }

    int liczba;
    while (plik >> liczba) {
        liczby.push_back(liczba);
    }

    plik.close();
}

// Funkcja do zapisu wyników do pliku
void zapiszWynik(const string &nazwa_pliku, const string &wynik, const string &data_i_godzina, double czas_trwania) {
    ofstream plik(nazwa_pliku, ios::app);  // Otwieramy w trybie dopisywania
    if (!plik) {
        cerr << "Blad otwarcia pliku wyjsciowego!" << endl;
        return;
    }

    // Zapisz datę, godzinę, wynik oraz czas wykonania
    plik << "Data i godzina wykonania: " << data_i_godzina;
    plik << "Najwieksza liczba: " << wynik << endl;
    plik << "Czas wykonania: " << czas_trwania << " sek." << endl;
    plik << "-----------------------------" << endl;

    plik.close();
}

int main() {
    // Zmienna do pomiaru czasu
    auto start = chrono::high_resolution_clock::now();

    // Odczyt danych z pliku
    vector<int> liczby;
    odczytajDane("dane.txt", liczby);
    if (liczby.empty()) {
        cerr << "Brak danych do przetworzenia." << endl;
        return 1;
    }

    // Wyświetlanie danych wejściowych
    cout << "Dane wejsciowe: ";
    for (int liczba : liczby) {
        cout << liczba << " ";
    }
    cout << endl;

    // Tworzenie największej liczby
    string wynik = tworzNajwiekszaLiczba(liczby);
    cout << "Najwieksza liczba: " << wynik << endl;

    // Zapis wyniku do pliku
    // Uzyskiwanie bieżącej daty i godziny
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> czas_trwania = end - start;

    // Uzyskiwanie bieżącej daty i godziny
    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    string data_i_godzina = ctime(&now); // Funkcja ctime zwraca datę i godzinę w formacie tekstowym

    // Zapisanie wyniku, daty i godziny oraz czasu wykonania do pliku czas.txt
    zapiszWynik("czas.txt", wynik, data_i_godzina, czas_trwania.count());

    return 0;
}
