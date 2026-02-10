// RysowaczWykresow.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

void menu();
double sprawdzczytoFLOAT(std::string);
int sprawdzczytoINT(std::string);

struct Danefunkcja {
    int elementscount = 0;
    std::string functionString = "";
    int** tokenlist = nullptr;
};

Danefunkcja FunctionData;

enum class typzmiennej :int {
    znakM = 0, liczba = 1, zmienna = 2
};
enum class Smat :int {
    dodawanie = 0, odejmowanie = 1, mnozenie = 2, dzielenie = 3, potegowanie = 4
};

double ArraytoFunction(double x)
{

    int** lista = nullptr;
    lista = FunctionData.tokenlist;
    int rozmiar = FunctionData.elementscount;
    double wynik = 0.0;
    double aktualny = 0.0;

    int znak = +1; // +1 lub -1
    int i = 0;

    while (i < rozmiar) {
        if (lista[i][0] == static_cast<int>(typzmiennej::liczba)) {
            aktualny = lista[i][1];
            // liczba * x lub liczba * x^n
            if (i + 1 < rozmiar && lista[i + 1][0] == static_cast<int>(typzmiennej::zmienna)) {
                aktualny *= x;
                // x^n
                if (i + 2 < rozmiar && lista[i + 2][0] == static_cast<int>(typzmiennej::znakM) && lista[i + 2][1] == static_cast<int>(Smat::potegowanie)) {
                    aktualny = lista[i][1] * pow(x, lista[i + 3][1]);
                    i += 2;
                }

                i++; // przeskok nad x
            }
        }
        //sama zmienna
        else if (lista[i][0] == static_cast<int>(typzmiennej::zmienna)) {
            aktualny = x;

            // x^n
            if (i + 1 < rozmiar && lista[i + 1][0] == static_cast<int>(typzmiennej::znakM) && lista[i + 1][1] == static_cast<int>(Smat::potegowanie)) {
                aktualny = pow(x, lista[i + 2][1]);
                i += 2;
            }
        }
        wynik += znak * aktualny;
        aktualny = 0.0;

        // ---------------- ZNAK + / - ----------------
        if (i + 1 < rozmiar && lista[i + 1][0] == static_cast<int>(typzmiennej::znakM)) {
            if (lista[i + 1][1] == static_cast<int>(Smat::dodawanie)) {
                znak = +1;
            }
            else if (lista[i + 1][1] == static_cast<int>(Smat::odejmowanie)) {
                znak = -1;
            }
            i += 2;
        }
        else
        {
            break;
        }
    }

    return wynik;
}



void RysujMacierz(int iloscwierszy, int ilosckolumn, std::string *lista, std::string znakspecjalny) { // co robi *lista taki wskaznik ? naprawdze sprawdzic co to robi xD
    int LiczbaWierszy = 0;
    for (int i = 0; i < iloscwierszy;) {
        for (int xi = 0; xi < ilosckolumn; ) {
            std::cout << *(lista + i * ilosckolumn + xi);
            xi++;
            if (xi == ilosckolumn) {
                LiczbaWierszy++;
            }
        }
        std::cout << "\n" << znakspecjalny;
        i++;
    }
}


void rysujfunkcje(double skalaX,double skalaY,int liczbaX,int liczbaY) {

    //podawac narazie tylko wartosci parzyste nie chce mi sie myslec nad innymi

    // zamiana na parametry funkcji
    /*
    skalaX = 5;
    skalaY = 5;
    liczbaX = 10;
    liczbaY = 10;
    */
    int rozmiarX = 2 * (liczbaX * std::floor(skalaX)) + 1;
    int rozmiarY = 2 * (liczbaY * std::floor(skalaY)) + 1;

    std::vector<std::string> osX;
    std::vector<std::vector<std::string>> osY;
    std::vector<int> wartosci;

    //przypisywanie do jednego wiersza x wartosci #
    for (int i = 0; i < rozmiarX;) {
        //osX.push_back("x"+std::to_string(i));
        osX.push_back("#");
        i++;
    }
    //przypisywanie do KAZDEGO wiersza z rozmiarY wartosci # macierz 
    for (int i = 0; i < rozmiarY;) {
        osY.push_back(osX);
        i++;
    }
    //rysowanie srodka wykresu tzw OSI X i Y da się uprościc :)
    for (int i = 0; i < rozmiarX; i++) {
        osY[std::floor(rozmiarY / 2.0)][i] = "\033[33mx\033[0m";
        int intX = std::floor(skalaX);
        // podzial na dyszki
        if (std::floor(skalaX) > 1 && i % intX == 0) {
            osY[std::floor(rozmiarY / 2.0) + 1][i] = "\033[33mx\033[0m";
            osY[std::floor(rozmiarY / 2.0) + -1][i] = "\033[33mx\033[0m";
        }
    }
    for (int i = 0; i < rozmiarY; i++) {
        osY[i][std::floor(rozmiarX / 2.0)] = "\033[33my\033[0m";
        int intY = std::floor(skalaY);
        if (std::floor(skalaY) > 1 && i % intY == 0) {
            osY[i][std::floor(rozmiarX / 2.0) + -1] = "\033[33my\033[0m";
            osY[i][std::floor(rozmiarX / 2.0) + 1] = "\033[33my\033[0m";
        }
    }
    /////////////////////////////////////////////////
    //przypisawnie wartosci do listy orangutan instrukcja
    double inkrementacjaR = 1.0 / skalaX;
    for (float wx = -liczbaX; wx <= liczbaX; ) {
        double funkcjaW = skalaY * ArraytoFunction(wx);
        wartosci.push_back(std::round(funkcjaW));
        wx = wx + inkrementacjaR;

    }
    //////////////////////////////////////////////////



    //SZpont GPT
    for (int i = 0; i < wartosci.size(); i++) {
        int xPos = (liczbaX * std::floor(skalaX)) + i - wartosci.size() / 2;
        int yPos = (liczbaY * std::floor(skalaY)) - wartosci[i];
        if (yPos >= 0 && yPos < rozmiarY && xPos >= 0 && xPos < rozmiarX) { // opis krawędzi góra dół lewo prawo super.
            osY[yPos][xPos] = "\033[32m-\033[0m";
        }
    }


    //generowanie wykresu i funkcji do funkcji to zrobić
    int LiczbaWierszy = 0;
    for (int i = 0; i < rozmiarY;) {
        for (int xi = 0; xi < rozmiarX; ) {
            std::cout << osY[LiczbaWierszy][xi] << " ";
            xi++;
            if (xi == rozmiarX) {
                LiczbaWierszy++;
            }
        }
        std::cout << "\n";
        i++;
    }
    return;
}

/*
dobra a co myslisz by zapicas wzor np w tablicy dwuwymiarowej nawet int ?
działao by to w ten sposób 13x^2+6x^3+6 gdzie bylaby to tablica zawierajaca
w ten sposób:
{{0,0,13},{1,1,0},{2,2,0},{3,0,6},{4,1,0},{5,2,4}{6,0,3} i dalej
gdzie pierwsze pole to miejsce w ktorym znak sie znajduje,
drugie pole to typ gdzie 0-liczba,1-zmienna,2-znak matematyczny,
trzecie pole to wartość dla liczb, to liczba czyli np 13, w drugim 0 czyli x, a w trzecim 0- +, 1--,2-*,3-/,4-^(potęgowanie) ?
zrobić na enumach by zrozumiec ich
*/

int zliczator(const std::string ciag) {
    int i = 0;
    int elementy = 0;
    while(true) {
        //badanie czy jest liczba i dodanie jej do sumy elementow TO DO: dodac do warunku "." jesli bede robil na double
        // liczby dwucyfrowe i wiecej liczy jako pojedynczy s
        if ((ciag[i] >= '0' && ciag[i] <= '9') && (ciag[i + 1] >= '0' && ciag[i + 1] <= '9')) {
            int j = i;
            while (true){
                if (ciag[j] == '+' || ciag[j] == '^' || ciag[j] == '-' || ciag[j] == 'x' || ciag[j] == '*' || ciag[j] == '/' || ciag[j] == '\0') {
                    elementy++;
                    i = j;
                    break;
                }
                j++;
            }
        }
        else if(ciag[i] >= '0' && ciag[i] <= '9') {
            elementy++;
        }
        if (ciag[i] == 'x') {
            elementy++;
        }

        if (ciag[i] == '+' || ciag[i] == '^' || ciag[i] == '-' || ciag[i] == '*' || ciag[i] == '/') {
            elementy++;
        }
        if (ciag[i] == '\0') {
            break;
        }
        i++;
    }
    return elementy;
}
 
// znakM: 0-dodawanie 1- odejmowanie,2-mnozenie,3-dzielenie,4-potegowanie
//szpont gpt

int** parsermath(  std::string wpis) {
    
    //definicja macierzy/vectora ax3)
    const int rozmiarelementu = 2;
    int iloscelementow = zliczator(wpis);
    //iloscelementow = 15;
    int** ptrlistaPyke = new int*[iloscelementow];
    for (int i = 0; i < iloscelementow; i++ ) {
        ptrlistaPyke[i] = new int[rozmiarelementu];
    }
    //ptrlistaPyke[19][1] = (int)2; //czemu nie wypierdala poza zakres wtf ? odp: Bo moze XDD, cpp szponci >:) 
    // przypisanie tablicy samych 0
    for (int i = 0; i < iloscelementow; i++) {
        for (int j = 0; j < rozmiarelementu; j++ ) {
            ptrlistaPyke[i][j] = 0;
        }
    }
    //tłumaczanie funkcji
    int ciagmemory = 0;
    int dlugoscelementu = 0;
    int  nrel = 0;
    std::string tempstring = "";
    int i = 0;
    while(true) {
        if (wpis[i] >= '0' && wpis[i] <= '9') {
            // dla pojedynczej cyfry
            if (!(wpis[i + 1] >= '0' && wpis[i + 1] <= '9')) {
                char poprzednwpis[2] = { wpis[i],'\0'}; //szpontGPT
                int cyfra = std::atoi(poprzednwpis);
                ptrlistaPyke[nrel][0] = static_cast<int>(typzmiennej::liczba);
                ptrlistaPyke[nrel][1] = cyfra;
            }
            // dla liczby wielocyfrowej
            else if ((wpis[i + 1] >= '0' && wpis[i + 1] <= '9') || wpis[i + 1] == '.') {
                ciagmemory = i;
                int j = i;
                ptrlistaPyke[nrel][0] = static_cast<int>(typzmiennej::liczba);
                while (true) {
                    if (!(wpis[j + 1] >= '0' && wpis[j + 1] <= '9')) {
                        dlugoscelementu = (j - ciagmemory) +1;
                        tempstring = wpis.substr(ciagmemory, dlugoscelementu);
                        int liczba = std::stoi(tempstring); // ERROR blad ciagu 
                        ptrlistaPyke[nrel][1] = liczba;
                        ciagmemory = j;
                        i = j;
                        break;
                    }
                    j++;
                  }
            }

        }
        
        if (wpis[i] == 'x') {
            ptrlistaPyke[nrel][0] = static_cast<int>(typzmiennej::zmienna);
            ptrlistaPyke[nrel][1] = 0;
        }

        if (wpis[i] == '+' || wpis[i] == '^' || wpis[i] == '-'|| wpis[i] == '*'|| wpis[i] == '/') {
            ptrlistaPyke[nrel][0] = static_cast<int>(typzmiennej::znakM);
            switch (wpis[i]) {
            case'+':
                ptrlistaPyke[nrel][1] = static_cast<int>(Smat::dodawanie);
                break;
            case'-':
                ptrlistaPyke[nrel][1] = static_cast<int>(Smat::odejmowanie);
                break;
            case'*':
                ptrlistaPyke[nrel][1] = static_cast<int>(Smat::mnozenie);
                break;
            case'/':
                ptrlistaPyke[nrel][1] = static_cast<int>(Smat::dzielenie);
                break;
            case'^':
                ptrlistaPyke[nrel][1] = static_cast<int>(Smat::potegowanie);
                break;
            }
            
        }
        if (wpis[i] == '\0') {
            break;
        }
        nrel++;
        i++;
        
    }

    return ptrlistaPyke;
}

bool IsOneVariableFunction(std::string wpis) {
    int i = 0;
    while (wpis[i] != '\0') {
        if (!(wpis[i] >= '0' && wpis[i] <= '9') && wpis[i] != 'x' && wpis[i] != '^' && wpis[i] != '+' && wpis[i] != '-' && wpis[i] != '*' && wpis[i] != '/') {
            return 0;
        }        
        i++;
    }
    return 1;
}
std::string WprowadzFunkcje() {
    std::cout << "Wprowadz funkcje w postaci y=ax^q+bx+c WAZNE POTEGA W NAJWYZSZA na poczatek lub w  inny sposob ale musi zawierac y= i zmienne przyjete ogolnie jak x czy a reszta wedlug zyczenia\n";
    std::cout << "^ - znak potegi np a^2, ^1/2 - pierwiastek  \n";
    std::string  funkcja = "";
    std::cin >> funkcja;
    return funkcja;

    //
                      
    
    
}

// znakM:0 liczba:1 zmienna:2
// znakM: 0-dodawanie 1- odejmowanie,2-mnozenie,3-dzielenie,4-potegowanie
// kolejnosc dzialan potegowanie -> mnozenie/dzielnie -> dodawanie/odejmowanie
// symbole "+" i "-" rozdzielaja wyrazenia do sumowania poniewaz sa najnizsza kolejnascia dzialania
//szukaj dopoki nie znajdziesz symbolu "+" lub "-" i rozpoczni sprawdzanie jakie sa znaki po drodze OD TYLU
//2 szpont GPT

void Miejscazerowe() {
    system("cls");
    int zakresX = sprawdzczytoINT("Wprowadz zakres lokalny funkcji w postaci zakresu jednej strony osi czyli 10 bedzie oznaczalo zakres -10 do 10 czyli wsm 20 \n");
    double dokladnoscX = sprawdzczytoFLOAT("Wprowadz z jaka dokladnoscia ma skanowac funkcje np 0.001: \t");
    int CountZeros = 0;
    for (double x = -(static_cast<double>(zakresX)); x <= static_cast<double>(zakresX); x += dokladnoscX) {
        if (ArraytoFunction(x) >-0.0001 && ArraytoFunction(x) < 0.0001) { // pochodna zmienia znak
            std::cout << "\n Jedno z miejs zerowych wynosi : " << x << '\n';
            CountZeros++;
        }
    }
    if (CountZeros == 0) {
        std::cout << "\n Brak miejsc zerowych";
    }
    system("pause");
    system("cls");
}
void zwolnijpamiecFunctionData() {
    for (int i = 0; i < FunctionData.elementscount; i++)
        delete[] FunctionData.tokenlist[i];

    delete[] FunctionData.tokenlist;
    FunctionData.tokenlist = nullptr;
    FunctionData.elementscount = 0;
}

void WprowadzFunkcjeMenu() {
    system("cls");
    zwolnijpamiecFunctionData();
    std::string funkcjaCiag ="";
    while (true) {
        funkcjaCiag = WprowadzFunkcje();
        if (IsOneVariableFunction(funkcjaCiag)) {
            std::cout << "Wprowadziles dobra funkcje \n";
            break;
        }
        else {
            std::cout << "Wprowadziles niepoprawnie funkcje \n";
        }
        
    }
    

    FunctionData.functionString = funkcjaCiag;
    FunctionData.elementscount = zliczator(FunctionData.functionString);
    FunctionData.tokenlist = parsermath(funkcjaCiag);
    //przerwanie = false;
    system("pause");
    system("cls");
}

int sprawdzczytoINT( std::string wejscie) {
    int x = 0;
    while (true) {
        std::cout << wejscie;
        if (std::cin >> x && x > 0) {
            return x;
        }
        std::cout << "Blad! Wpisales liczbe mniejsza od 0 badz nie jest to cyfra.\n";
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }
}

double sprawdzczytoFLOAT(std::string wejscie) {
    double x = 0;
    while (true) {
        std::cout << wejscie;
        if (std::cin >> x && x > static_cast<double>(0)) {
            return x;
        }
        std::cout << "Blad! Wpisales liczbe mniejsza od 0 badz nie jest to cyfra.\n";
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }
}

void rysujfunkcjemenu() {
    system("cls");
    //dodac parametry
    double skalarX = 0;
    double skalarY = 0;
    int zakresX = 0;
    int zakresY = 0;
    skalarX = sprawdzczytoFLOAT("Wprowadz skale osi X:\t");
    skalarY = sprawdzczytoFLOAT("Wprowadz skale osi Y:\t");
    zakresX = sprawdzczytoINT("Wprowadz zakres osi X:\t");
    zakresY = sprawdzczytoINT("Wprowadz zakres osi Y:\t");
    rysujfunkcje(skalarX,skalarY,zakresX,zakresY);
    system("pause");
    system("cls");
}

double pochodna(double x) {
    double h = 1e-5;
    double pochodna = (ArraytoFunction(x + h) - ArraytoFunction(x)) / h;
    return pochodna;
}
void Extremafunkcji() {
    system("cls");
    int zakresX = sprawdzczytoINT("Wprowadz zakres lokalny funkcji w postaci zakresu jednej strony osi czyli 10 bedzie oznaczalo zakres -10 do 10 czyli wsm 20 \n");
    double dokladnoscX = sprawdzczytoFLOAT("Wprowadz z jaka dokladnoscia ma skanowac funkcje np 0.001: \t");

    for (double x = -(static_cast<double>(zakresX)); x <= static_cast<double>(zakresX); x += dokladnoscX) {
        double d1 = pochodna(x);
        double d2 = pochodna(x + dokladnoscX);
        if (d1 * d2 <= 0) { // pochodna zmienia znak
            std::cout << "\n Jedno z ektremow wynosi :" << x <<" a jego wartosc to: "<< ArraytoFunction(x) << '\n';
        }
        else {
            std::cout << "\n Funkcja nie posiada ekstremow" << '\n';
        }
        }
    system("pause");
    system("cls");
}

void menu() {
    std::string opcje[] = { "MENU","WPROWADZ wzor funkcji","SPRAWDZ MIEJSCA ZEROWE","SPRAWDZ MAKSIMUM I MIN F","WYKRES","WYJDZ"};
    int odleglosc = 10;
    const int iloscwierszy = 8;
    const int ilosckolumn = 40;
    //int sumatabeli = iloscwierszy * ilosckolumn;
    std::string listamenu[iloscwierszy][ilosckolumn] = { " " };
    for (int i = 0, yi = 0, xi = 0; i <= iloscwierszy * ilosckolumn;) {
        if (xi != 0) {
            listamenu[yi][xi] = "-";
        }
        listamenu[yi][0] = "|";
        listamenu[yi][ilosckolumn - 1] = "|";

        xi++;
        if (i % ilosckolumn == 0 && i != 0) {
            yi++;
            xi = 0;
        }
        i++;
    }
    
    // przydzielanie elementow menu do listy wyswietlania
    int opcjeSize = sizeof(opcje) / sizeof(opcje[0]);
    for (int li = 0; li < opcjeSize;) {
        listamenu[li][(ilosckolumn / 2) - (opcje[li].length() / 2)] = opcje[li];

        for (int ki = 0; ki < opcje[li].length() - 1; ki++) {
            listamenu[li][((ilosckolumn / 2) - (opcje[li].length() / 2)) + ki + 1] = "";
        }
        li++;

    }
    
    RysujMacierz(iloscwierszy, ilosckolumn, (std::string*)listamenu, "");

    #define KEY_UP 72
    #define KEY_DOWN 80
    #define KEY_LEFT 75
    #define KEY_RIGHT 77  
    #define ENTER_KEY 13
    #define SPACEBAR 32

    int keyclick = 0;
    int wybor = 0;
    bool przerwanie = true;
    using MenuFunc = void(*)();
    MenuFunc menufunkcji[] = { nullptr, WprowadzFunkcjeMenu,Miejscazerowe,Extremafunkcji,rysujfunkcjemenu
    };
    while (przerwanie) {
        switch((keyclick = _getch())) {   
        case KEY_DOWN:
            wybor++;
            break;
        case KEY_UP:
            wybor--;
            break;
        }

        if (wybor > iloscwierszy-3) {
            wybor = iloscwierszy-3;
        }
        if (wybor <= 0) {
            wybor = 1;
        }
        if (keyclick == ENTER_KEY && wybor != 0) {
            //std::cout << "enter"<<'\n';
            if (wybor == 5) {
                przerwanie = false;
                return;
            }
            menufunkcji[wybor]();
            RysujMacierz(iloscwierszy, ilosckolumn, (std::string*)listamenu, ""); //rzyg ale dziala
        }

        if ((keyclick == KEY_DOWN || keyclick == KEY_UP) && wybor != 0) {
            system("cls");
            if (wybor != 0) {
                listamenu[wybor - 1][(ilosckolumn / 2) - (opcje[wybor - 1].length() / 2)] = "\033[0m" + opcje[wybor - 1] + "\033[0m";
            }
            listamenu[wybor][(ilosckolumn / 2) - (opcje[wybor].length() / 2)] = "\033[47m\033[30m" + opcje[wybor] + "\033[0m";
            if (wybor != iloscwierszy - 3) {
                listamenu[wybor + 1][(ilosckolumn / 2) - (opcje[wybor + 1].length() / 2)] = "\033[0m" + opcje[wybor + 1] + "\033[0m";
            }

            RysujMacierz(iloscwierszy, ilosckolumn, (std::string*)listamenu, "");
        }        
    }
}

int main()
{
    menu();
    zwolnijpamiecFunctionData();
    return 0;
}
//TO DO IN FUTURE: implementacja algorytmu Shunting yard algorithm(tokenizacja) i RPN do budowy funkcji 