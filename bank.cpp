#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <iterator> 
#include "md5.h"  

using namespace std;
// Стуктура для хранения результатов Алгоритма Евклида

struct ExtendedEvclidResult
{
    long gcd;
    long x;
    long y;
};
struct ExtendedEvclidResult result;

//Вычисление НОД
long int gcd (long int a, long int b){
   long int t = 0;
    while (b) {
        t = a % b;
        a = b;
        b = t;
    }
    return abs(a);
}

//Функция быстрого возведения числа (a) в степень (b) по модулю (p)
long int MyPow(long int a, long int b, long int p)
{
    long int x = 1;

    while (b) {
        if (b % 2 == 0) {
            b /= 2;
            a = (a * a) % p;
        }
        else {
            b--;
            x = (x * a) % p;
        }
    }
    return x;
}

// Проверка числа на простоту
bool Ferma(long int x) {
    int i;
    if (x == 2)
        return true;
    for (i = 0; i < 100; i++) {
        long int a = (rand() % (x - 2)) + 2;
        if (gcd(a, x) != 1)
            return false;
        if ( MyPow(a, x - 1, x) != 1)
            return false;
    }
    return true;
}

//получение простого числа
long int get_number() {
    long int P;
    do {
        P = rand(); 
    } while ((Ferma(P) != true));
    return P;
}

long int get_number_range(long int a, long int b) {
    long int P;
    
    do {
        P = a + rand() % b;
    } while ((Ferma(P) != true));
    return P;
}


void Evclid (long int a, long int b)
{
    int q = 0, r = 0, x1 = 0, x2 = 1, y1 = 1, y2 = 0;
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

unsigned long int MD5_Hash(long int p, long int num) {
    unsigned long int h = 0;
    MD5 md5 = MD5((char*)&num);
    for (int i = 0; i < 4; i++) {
        h = (h + md5.state[i]) % p;
    }

    return h;
}


class Bank {
private:
	long int p;
	long int q;
	vector<long int> c;
public:
	long int N;
	vector<long int> d;
	Bank() 
    {
        c = vector<long int> (9);
        d = vector<long int> (9);
	p = get_number_range(1, 10000);
    	q = get_number_range(1, 10000);
    	N = p * q;
    	long int fi = (p - 1) * (q - 1);

    	for (int i = 0; i < 9; ++i) {
    		while (result.gcd != 1) {
                d[i] = (get_number_range(1, fi - 1));
        		Evclid(d[i], fi);
   			}

            result.x > 0 ? c[i] = result.x : c[i] = result.x + fi;
   			result.gcd = 0;
    	}
        string mas[9] = {"1 рубль", "2 рубля", "5 рублей", "10 рублей", 
        "50 рублей", "100 рублей", "500 рублей", "1000 рублей", "5000 рублей"};
        cout << "\n\n==============Параметры банка=============="<< endl;
        cout << "p = " << p << endl;
        cout << "q = " << q << endl;
        cout << "N = " << N << endl;
        cout << "===========================================" << endl;
        for (int i = 0; i < 9; ++i) {
            cout << mas[i] << " ----->  c = " << c[i] << " d = " << d[i] << endl;
        }
        cout << "\n\n===========================================" << endl;
    }

    /*Выдача информации по запросу*/
    vector<long int> get_d(){
        return d;
    }

    long int get_N(){
        return N;
    }

    bool request_from_store(long int n, long int s)
    {   
        bool flag = false;
        long int hash = MD5_Hash(N, n);
        long int temp = 0;
        for (int i = 0; i < 9; ++i) {
            temp = MyPow(s, d[i], N);
            if (hash == temp) {
                flag = true;
                break;
            } else {
                flag = false;
            }
        }
        
        if (flag) {
            cout << "Подпись банкноты совпадает"; 
            return true;
        } else {
            cout << "Подпись банкноты НЕ сопадает"; 
            return false;
        }
    }

    long int request_from_client(long int n, long int num){
        return MyPow(n, c[num], N);
    }
};

class Store {
public:
    bool check_sign(long int n, long int s, Bank& bank)
    {
        bool flg = bank.request_from_store(n, s);
        if (flg)
        {
            return true;
        }
        else
            return false;
    }
};

class Client {
private:
    long int money_cl;
    long int N_cl;
    vector<long int> d_cl;
public:
    Client(Bank& bank, long int money)
    {
        d_cl = vector<long int> (9);
        money_cl = money;
        d_cl = bank.get_d();
        N_cl = bank.get_N();
        //cout << "N_cl = " << N_cl << endl;
        // for(int i = 0; i < d_cl.size(); i++)
        //     cout << d_cl[i] << ' ' << endl;   
    }

    void get_banknote(Bank& bank, int num, Store& store)
    {
        string mas[9] = {"1 рубль", "2 рубля", "5 рублей", "10 рублей", 
        "50 рублей", "100 рублей", "500 рублей", "1000 рублей", "5000 рублей"};
        static set<long int> used_n;
        //static long int used_nn;
        long int n = 0;

        while (1) {
            n = 2 + rand() % (N_cl - 1);
            // used_nn = n;
            // break;
            if (used_n.count(n) != 1) {
                used_n.insert(n);
                break;
            }
        }

        long int hash = MD5_Hash(N_cl, n);
        
        cout << "Для "<< mas[num] <<":\n";
        cout << "[*] n = " << n << endl;
        
        long int r;
        while (result.gcd != 1) {
            r = get_number_range(1, N_cl - 1);
            Evclid(r, N_cl);
        }

        cout << "[*] r = " << r << endl;
        long int tmp_n, tmp_s;
        //cout << d_cl[num] << endl;
        tmp_n = (hash * (MyPow(r, d_cl[num], N_cl))) % N_cl;
        tmp_s = bank.request_from_client(tmp_n, num);

        cout << "[*] tmp_n = " << tmp_n << endl;
        cout << "[*] tmp_s = " << tmp_s << endl;

        Evclid(r, N_cl);
        if (result.x < 0) {
            r = N_cl + result.x;
        } else {
            r = result.x;
        }

        long int s = (tmp_s * r) % N_cl;
        cout << "[*] s = " << s << '\n' << endl;

        if (store.check_sign(n, s, bank)) {
            cout << "\nПокупка совершенна" << endl;
        } else {
            cout << "\nОшибка" << endl;
        }
        cout << "===========================================" << endl;   
    }

    void buying(Bank& bank, Store& store)
    {
        long int nominals[9] = {1, 2, 5, 10, 50, 100, 500, 1000, 5000};  
        string mas[9] = {"1 рубль", "2 рубля", "5 рублей", "10 рублей", 
        "50 рублей", "100 рублей", "500 рублей", "1000 рублей", "5000 рублей"};
        
        long int m = money_cl;
        long int tmp_m = 0;

        for (int i = 0; i < 9; ++i) {
            if (m < nominals[i] || i == 8) {
                // --i;
                while (tmp_m != m) {
                    if (tmp_m + nominals[i] <= m) {
                        cout << "**Подготовка к платежу купюры номиналом:" << mas[i] << endl;
                        get_banknote(bank, i, store);
                        tmp_m += nominals[i];
                    } else {
                        --i;
                    }
                    cout << endl;
                }
                break;
            }
        }
    }  
};

int main(int argc, char const *argv[])
{
    srand(time(0));
    
    Bank Sberbank;
    Store store;
    Client Anton(Sberbank, 2000);
    Anton.buying(Sberbank, store);

    return 0;
}