#include <iostream>
#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#include <bits/stdc++.h>
#endif
#include <vector>
#include <climits>

using namespace std;

// při zahájení nové hry vytiskne její název a pravidla
void pravidla() {
    int barva = rand() % (36 - 31) + 31; //nastaví náhodnou barvu pro název hry
    cout << "\033[" << barva << ";1m"
         << "  ____ ___ _\\_/  _  ____     _____  ____  _  ____   __                                         \n"
         << " |  _ \\_ _/ ___|| |/ /\\ \\   / / _ \\|  _ \\| |/ /\\ \\ / /           __  __  __   _____       ___  \n"
         << " | |_) | |\\___ \\| ' /  \\ \\ / / | | | |_) | ' /  \\ V /   _____    \\ \\/ /  \\ \\ / / __|     / _ \\ \n"
         << " |  __/| | ___) | . \\   \\ V /| |_| |  _ <| . \\   | |   |_____|    >  <    \\ V /\\__ \\_   | (_) |\n"
         << " |_|  |___|____/|_|\\_\\   \\_/  \\___/|_| \\_\\_|\\_\\  |_|             /_/\\_\\    \\_/ |___(_)   \\___/ \n"
         << "\033[0m\n"
         << " Pravidla:\n  Hráč na tahu zadá souřadnice požadovaného pole ve tvaru [a1].\n  Pokud je toto pole již obsazeno, hráč musí zvolit jiné pole.\n  Hráči se po každém tahu střídají.\n  Vyhrává ten, kdo má tři stejné symboly v řádku, sloupci či diagonále.\n  Pokud žádný z hráčů nevyhraje a všechna pole jsou již obsazena, nastává remíza.\n\n";
}

//vytvoří prázdné hrací pole a popíše osy (písmeny a čísly)
void plneniPole(vector<vector<char> > &hraciPole, int velikost) {
    char pismeno{'A'};   //první písmeno pro legendu
    char cislo{'1'};     //první číslo pro legendu
    for (int i = 0; i < velikost; i++) {
        vector<char> radek;
        for (int j = 0; j < velikost; j++) {
            radek.push_back(' ');
        }
        hraciPole.push_back(radek);
    }
    for (int i = 1; i < velikost; i++) {
        hraciPole[i][0] = pismeno++;
    }
    for (int i = 1; i < velikost; i++) {
        hraciPole[0][i] = cislo++;
    }
}

//vytiskne na výstup celé hrací pole v aktuálním stavu
void tisk(vector<vector<char> > hraciPole, int velikost, char symbol1, char symbol2) {
    cout << "\n   ";
    for (int i = 0; i < velikost; i++) {
        for (int j = 0; j < velikost; j++) {
            cout << "      ";
        }
        cout << "\n   ";
        for (int j = 0; j < velikost; j++) {
            cout << "     |";
        }
        cout << "\n   ";
        for (int j = 0; j < velikost; j++) {
            if (hraciPole[i][j] == symbol1) {
                cout << "  \033[36m" << symbol1 << "\033[0m  |";
            } else if (hraciPole[i][j] == symbol2) {
                cout << "  \033[33m" << symbol2 << "\033[0m  |";
            } else {
                cout << "  " << hraciPole[i][j] << "  |";
            }
        }
        cout << "\n   ";
        for (int j = 0; j < velikost; j++) {
            cout << "_____|";
        }
    }
    cout << "\n\n";
}

//nabídne hráči možnost hrát proti počítači a poté si zvolit obtížnost hry
void protiPc(bool &pc, int &obtiznost) {
    string pocitac;   //odpověď hráče, zda chce hrát proti počítači, či nikoliv
    cout << " Chcete hrát proti počítači? \033[32m[ano-1]\033[31m[ne-0]\033[0m \n ";
    cin >> pocitac;
    cin.ignore();
    transform(pocitac.begin(), pocitac.end(), pocitac.begin(), ::tolower);
    cout << "\n";
    if (pocitac == "1" || pocitac == "ano" || pocitac == "jo") {
        pc = true;
        do {
            cout << " Zvolte si obtížnost hry: lehká[1] střední[2] těžká[3] nemožná[4] hacker[5]\n ";
            cin >> obtiznost;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "\n";
        } while (obtiznost < 1 || obtiznost > 5);
    }
}

//hráči si mohou zvolit vlastní jména, jinak se doplní "hrac1"/"hrac2"
//v případě, že hraje počítač, doplní se místo jména druhého hráče text "POČÍTAČ"
void jmenaHracu(string &hrac1, char symbol1, string &hrac2, char symbol2, bool pc) {
    cout << " Jméno hráče 1 (hraje s " << symbol1 << "): ";
    getline(cin, hrac1);
    if (hrac1 == "") {
        hrac1 = "hrac1";
    }
    cout << " Jméno hráče 2 (hraje s " << symbol2 << "): ";
    if (pc) {
        hrac2 = "POČÍTAČ";
        cout << hrac2 << "\n";
    } else {
        getline(cin, hrac2);
        if (hrac2 == "") {
            hrac2 = "hrac2";
        }
    }
}

//otestuje, zda není celé hrací pole zaplněno, a nenastala tak remíza
//vrací hodnotu true (nastala-li remíza) nebo false (nenastala-li remíza)
bool testRemiza(vector<vector<char> > hraciPole, int velikost) {
    int pocetPrazdnychPoli = (velikost - 1) * (velikost - 1);
    for (int i = 1; i < velikost; i++) {
        for (int j = 1; j < velikost; j++) {
            if (hraciPole[i][j] != ' ') {
                pocetPrazdnychPoli--;
            }
        }
    }
    if (pocetPrazdnychPoli == 0) {
        return true;
    } else {
        return false;
    }
}

//otestuje, zda některý z hráčů nevyhrál
//vrací hodnotu true (vyhrál) nebo false (nevyhrál)
bool testVyhra(vector<vector<char> > hraciPole, int velikost, char hrac) {
    for (int i = 1; i < velikost; i++) {
        if (hraciPole[1][i] == hrac && hraciPole[2][i] == hrac && hraciPole[3][i] == hrac) {
            return true;
        }
    }
    for (int i = 1; i < velikost; i++) {
        if (hraciPole[i][1] == hrac && hraciPole[i][2] == hrac && hraciPole[i][3] == hrac) {
            return true;
        }
    }
    if (hraciPole[1][1] == hrac && hraciPole[2][2] == hrac && hraciPole[3][3] == hrac) {
        return true;
    }
    if (hraciPole[3][1] == hrac && hraciPole[2][2] == hrac && hraciPole[1][3] == hrac) {
        return true;
    }
    return false;
}

//vrací tři hodnoty - 1 (při výhře), 2 (při remíze) a 0 (pokud nedojde ani k výhře, ani k remíze)
int zkoumaniVyhry(vector<vector<char> > hraciPole, int velikost, char hrac) {
    if (testVyhra(hraciPole, velikost, hrac)) {
        return 1;
    } else if (testRemiza(hraciPole, velikost)) {
        return 2;
    } else {
        return 0;
    }
}

//vyhledává nejlepší tahy, implementace algoritmu Minimax, https://en.wikipedia.org/wiki/Minimax
int minimax(vector<vector<char> > hraciPole, int velikost, char hrac, char symbol1, char symbol2) {
    if (testVyhra(hraciPole, velikost, hrac)) {
        if (hrac == symbol2) {
            return 1;
        } else {
            return -1;
        }
    }
    if (testRemiza(hraciPole, velikost)) {
        return 0;
    }
    if (hrac == symbol1) {
        hrac = symbol2;
        int maxHodnotaPole = -1;    //nastavena na minimální možnou hodnotu pole
        for (int i = 1; i < velikost; i++) {
            for (int j = 1; j < velikost; j++) {
                if (hraciPole[i][j] == ' ') {
                    hraciPole[i][j] = hrac;
                    int hodnotaPole = minimax(hraciPole, velikost, hrac, symbol1, symbol2);
                    if (hodnotaPole > maxHodnotaPole) {
                        maxHodnotaPole = hodnotaPole;
                        if (maxHodnotaPole == 1) {
                            hraciPole[i][j] = ' ';
                            return 1;
                        }
                    }
                    hraciPole[i][j] = ' ';
                }
            }
        }
        return maxHodnotaPole;
    } else {
        hrac = symbol1;
        int minHodnotaPole = 1;     //nastavena na maximální možnou hodnotu pole
        for (int i = 1; i < velikost; i++) {
            for (int j = 1; j < velikost; j++) {
                if (hraciPole[i][j] == ' ') {
                    hraciPole[i][j] = hrac;
                    int hodnotaPole = minimax(hraciPole, velikost, hrac, symbol1, symbol2);
                    if (hodnotaPole < minHodnotaPole) {
                        minHodnotaPole = hodnotaPole;
                        if (minHodnotaPole == -1) {
                            hraciPole[i][j] = ' ';
                            return -1;
                        }
                    }
                    hraciPole[i][j] = ' ';
                }
            }
        }
        return minHodnotaPole;
    }
}

//volá fci minimax pro každé volné pole, dokud nenalezne vhodné pole pro tah
//vrátí souřadnice pole - řádek a sloupec
void volaMinimax(vector<vector<char> > hraciPole, int velikost, int &radek, int &sloupec, int obtiznost, char hrac,
                 char symbol1, char symbol2) {
    int maxHodnotaPole = -1;    //bodové hodnocení nejlepšího dosud nalezeného tahu
    for (int i = 1; i < velikost; i++) {
        for (int j = 1; j < velikost; j++) {
            if (hraciPole[i][j] == ' ') {
                hraciPole[i][j] = hrac;
                int hodnotaPole = minimax(hraciPole, velikost, hrac, symbol1, symbol2);
                if (maxHodnotaPole <= hodnotaPole) {
                    maxHodnotaPole = hodnotaPole;
                    radek = i;
                    sloupec = j;
                }
                hraciPole[i][j] = ' ';
                if (maxHodnotaPole == 1) {
                    return;
                }
            }
        }
    }
}

//podle zvolené obtížnosti vybere kód pro tah počítače
void
tahPc(vector<vector<char> > &hraciPole, int velikost, int &radek, int &sloupec, int obtiznost, char hrac, char symbol1,
      char symbol2) {
    switch (obtiznost) {
        case 1: {
            radek = rand() % 3 + 1;
            sloupec = rand() % 3 + 1;
        }
            break;
        case 2: {
            int pravdepodobnost = rand() % 100;
            if (pravdepodobnost > 80) {
                volaMinimax(hraciPole, velikost, radek, sloupec, obtiznost, hrac, symbol1, symbol2);
            } else {
                radek = rand() % 3 + 1;
                sloupec = rand() % 3 + 1;
            }
        }
            break;
        case 3: {
            int pravdepodobnost = rand() % 100;
            if (pravdepodobnost > 20) {
                volaMinimax(hraciPole, velikost, radek, sloupec, obtiznost, hrac, symbol1, symbol2);
            } else {
                radek = rand() % 3 + 1;
                sloupec = rand() % 3 + 1;
            }
        }
            break;
        case 4: {
            volaMinimax(hraciPole, velikost, radek, sloupec, obtiznost, hrac, symbol1, symbol2);
        }
            break;
        case 5: {
            for (int i = 1; i < velikost; i++) {
                for (int j = 1; j < velikost; j++) {
                    if (hraciPole[i][j] == ' ') {
                        hraciPole[i][j] = symbol2;
                    }
                }
            }
            tisk(hraciPole, velikost, symbol1, symbol2);
            return;
        }
            break;
        default: {  //není zde potřeba, nikdy se nevykoná
        }
    }
}

//požádá hráče o zadání souřadnic zvoleného pole, vyhodnotí zda hráč zadal platné souřadnice
void tahCloveka(int &radek, int &sloupec, int velikost) {
    char radek_char;    //řádek zadaný uživatelem
    char sloupec_char;  //sloupec zadaný uživatelem
    cout << "  Zadejte souřadnice požadovaného pole: \n  ";
    cin >> radek_char;
    cin >> sloupec_char;
    radek = tolower(radek_char) - 96;  //převede písmeno na číslo, které odpovídá jeho pořadí v abecedě
    sloupec = sloupec_char - 48;       //převede znak na číslo, které reprezentuje
    cin.ignore(INT_MAX, '\n');
    if (radek <= 0 || radek >= velikost || sloupec <= 0 || sloupec >= velikost) {
        cout << "\033[31m   Nesprávná hodnota pole!\033[0m \n";
        tahCloveka(radek, sloupec, velikost);
    }
}

//zavolá fci tahCloveka nebo tahPc a obdrží souřadnice pole; poté zjistí, zda pole není obsazené a do zvoleného pole uloží aktuální symbol
void
tahHrace(vector<vector<char> > &hraciPole, int velikost, char hrac, string hrac1, string hrac2, bool pc, char symbol1,
         char symbol2, int obtiznost) {
    int radek;    //vodorovná souřadnice zvoleného pole
    int sloupec;  //svislá souřadnice zvoleného pole
    if (hrac == symbol2 && pc) {
        tahPc(hraciPole, velikost, radek, sloupec, obtiznost, hrac, symbol1, symbol2);
        if (obtiznost == 5) {
            return;
        }
    } else {
        tahCloveka(radek, sloupec, velikost);
    }
    if (hraciPole[radek][sloupec] != ' ') {
        if (!(hrac == symbol2 && pc)) {
            cout << "\033[31m   Toto pole je již obsazené!\033[0m \n";
        }
        tahHrace(hraciPole, velikost, hrac, hrac1, hrac2, pc, symbol1, symbol2, obtiznost);
    } else {
        hraciPole[radek][sloupec] = hrac;
        tisk(hraciPole, velikost, symbol1, symbol2);
    }
}

int main() {
    //system("chcp 65001>nul");     //povolí vypsání českých znaků, Windows
    srand(time(NULL));
    string hratZnovu;   //uloží odpovědď hráče, zda chce hrát znovu, či nikoliv
    do {
        vector<vector<char> > hraciPole;    //skládá se z polí legendy a polí pro symboly
        int velikost{4};    //udává počet polí v jednom řádku, 3 hrací pole + 1 na legendu
        char symbol1 = 'O';  //symbol, se kterým hraje první hráč
        char symbol2 = 'X';  //symbol, se kterým hraje druhý hráč
        char hrac = symbol1; //nastaví se na první symbol (= symbol hráče, který začíná)
        bool pc = false;     //uchovává, zda hráč hraje proti počítači, či nikoliv
        int obtiznost;       //obtížnost při hře proti počítači
        string hrac1;        //jméno prvního hráče
        string hrac2;        //jméno druhého hráče
        bool vyhra = false;  //nastaví se na true, pokud jeden z hráčů vyhraje nebo nastane remíza

        plneniPole(hraciPole, velikost);
        pravidla();
        protiPc(pc, obtiznost);
        jmenaHracu(hrac1, symbol1, hrac2, symbol2, pc);
        tisk(hraciPole, velikost, symbol1, symbol2);

        //volá tahHrace a zkoumaniVyhry; pokud nastane výhra nebo remíza, vypíše info na obrazovku a aktuální hra se ukončí
        do {
            if (hrac == symbol1) {
                cout << " Tah hráče \033[36m" << hrac1 << " \033[0m(hraje s " << symbol1 << "):\n";
                tahHrace(hraciPole, velikost, hrac, hrac1, hrac2, pc, symbol1, symbol2, obtiznost);
                if (zkoumaniVyhry(hraciPole, velikost, symbol1) == 1) {
                    cout << "\033[32m" << " VYHRÁL HRÁČ\033[0m \033[36m" << hrac1 << "\033[0m (hrál s " << symbol1
                         << ")." << "\n\n";
                    vyhra = true;
                } else if (zkoumaniVyhry(hraciPole, velikost, symbol1) == 2) {
                    cout << "\033[31m" << " REMÍZA!" << "\033[0m" << "\n\n";
                    vyhra = true;
                } else {
                    hrac = symbol2;
                }
            }
            if (hrac == symbol2) {
                cout << " Tah hráče \033[33m" << hrac2 << " \033[0m(hraje s " << symbol2 << "):\n";
                tahHrace(hraciPole, velikost, hrac, hrac1, hrac2, pc, symbol1, symbol2, obtiznost);
                if (zkoumaniVyhry(hraciPole, velikost, symbol2) == 1) {
                    cout << "\033[32m" << " VYHRÁL HRÁČ\033[0m \033[33m" << hrac2 << "\033[0m (hrál s " << symbol2
                         << ")." << "\n\n";
                    vyhra = true;
                } else if (zkoumaniVyhry(hraciPole, velikost, symbol2) == 2) {
                    cout << "\033[31m" << " REMÍZA!" << "\033[0m" << "\n\n";
                    vyhra = true;
                } else {
                    hrac = symbol1;
                }
            }
        } while (!vyhra);
        cout << " Chcete hrát znovu? \033[32m[ano-1]\033[31m[ne-0]\033[0m \n ";
        cin >> hratZnovu;
        cin.ignore();
        transform(hratZnovu.begin(), hratZnovu.end(), hratZnovu.begin(), ::tolower);
        cout << "\n\n";
    } while (hratZnovu == "1" || hratZnovu == "ano" || hratZnovu == "jo");
    system("pause");
    return 0;
}




