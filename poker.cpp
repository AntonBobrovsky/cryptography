#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <map>  
#include "crypt.h"
#include "poker.h"
#define N_CARDS 52
#define N_PLAYERS 5

using namespace std;

struct ExtendedEvclidResult result;
int p;
long int keys[N_PLAYERS][2]; // массив для хранения ключей
string suit[] = {"♥", "♦", "♣", "♠"};
string facevalue[] = {"Двойка", "Тройка", "Четверка", "Пятерка", "Шестерка", "Семерка", "Восьмерка", "Девятка", "Десятка", "Валет", "Дама", "Король", "Туз"};
long int numbers[52]; //номера карт
vector<string> cards; //названия карт
map <long int,string> allCards; //соответствие номеров к картам
vector<string> table;

void printMap(map <long int,string> allCards){
  for (auto it = allCards.begin(); it != allCards.end(); ++it) {
      cout << (*it).first << " -> " << (*it).second << endl;
    }
}

void Evclid (long int a, long int b)
{
    int q = 0, r = 0, x1 = 0, x2 = 1, y1 = 1, y2 = 0, d = 0;
    long x, y;

    if (b == 0) {
    
        result.gcd = a;
        result.x = 1;
        result.y = 0;
    }

    while (b > 0) {
        q = a / b, r = a - q *b;
        x = x2 - q * x1, y = y2 - q * y1;
        a = b, b = r;
        x2 = x1, x1 = x, y2 = y1, y1 = y;
    }

    result.gcd = a;
    result.x = x2;
    result.y = y2;
}

void generation_numbers() {
 
 for (int i = 0; i < N_CARDS; ++i)
    {
        numbers[i] = i + 2;
    }
}

void generation_parameters() {
  long int C, D;
  
  p = get_number_range(1000, 10000);
  for (int i = 0; i < N_PLAYERS; i++) {
    result.gcd = 0;
    while (result.gcd != 1) {
        C = rand() % (p - 1);
        Evclid(C, p - 1);
    }
    result.x > 0 ? D = result.x : D = result.x + (p - 1);
    keys[i][0] = C;
    keys[i][1] = D;
  }  
}

void initialization_cards() {
 
cout << "===========================================" << endl;

    /*Инициализация карт*/
    for(int i = 0; i < 4; i++) {
       for(int j = 0; j < 13; j++) {
           string s;
           s.append(facevalue[j]);
           s.append(" ");
           s.append(suit[i]);
           cards.push_back(s);
       }
    }
    /* Присваеваем каждой карте свой номер */ 
    for (int i = 0; i < cards.size(); ++i) {
       allCards.insert ( pair<long int,string>(numbers[i],cards[i]) );
    }
    printMap(allCards);
    cout << "===========================================" << endl;  

}


int decoding(int begin, int k, int karta) { 

    for (int i = begin + 1; i < N_PLAYERS; ++i)
    {
        karta = MyPow(karta, keys[i][k], p);
    }
    
    for (int i = 0; i < begin + 1; ++i)
    {
        karta = MyPow(karta, keys[i][k], p);
    }
    
    return karta;
}


void mentalPoker() {

    generation_parameters();
    generation_numbers(); 
    initialization_cards();
    
    for (int j = 0; j < N_PLAYERS; ++j)
    {
        for (int i = 0; i < N_CARDS; ++i)
        {
            numbers[i] = MyPow(numbers[i] , keys[j][0], p);
        }
        for (int x = 0; x < N_CARDS; ++x)
        {
            swap(numbers[x], numbers[rand() % N_CARDS]);
        }
    }

    for (int i = 0; i < N_CARDS; ++i)
    {
        cout << numbers[i] << " ";
    }
    cout << std::endl;

    cout << "===========================================" << endl;  

    long int karta1, karta2;
    for (int i = 0; i < N_PLAYERS; ++i)
    {
        do {
            karta1 = rand() % N_CARDS;
        } while (numbers[karta1] == 0 );
        cout << "Игрок " << i+1 << " получает карту -> " << decoding(i, 1, numbers[karta1]) << endl;
        numbers[karta1] = 0;
        
        do {
            karta2 = rand() % N_CARDS;
        } while (numbers[karta2] == 0 );
        cout << "Игрок " << i+1 << " получает карту -> " << decoding(i, 1, numbers[karta2]) << endl;
        numbers[karta2] = 0;

        cout << "===========================================" << endl;  
    }

    for (int j = 0; j < N_PLAYERS; ++j)
    {
        for (int i = 0; i < N_CARDS; ++i)
        {
            numbers[i] = MyPow(numbers[i] , keys[j][1], p);
        }
    }

    for (int j = 0; j < N_CARDS; ++j)
    {
        if (numbers[j] != 0)
        {
            auto itMap = allCards.begin();
            itMap = allCards.find(numbers[j]);
            table.push_back( itMap->second );
        }
    }
    cout << "Оставшиеся карты: " << table.size() << endl;
    for(int i = 0; i < table.size(); i++)
        cout << table[i] << endl;
}