#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include "crypt.h"
#include "md5.h"


using namespace std;
struct ExtendedEvclidResult result_Evclid;


int binfind(std::vector<long int> v, int x, int left, int right)
{
    if (left > right)
    {
        return -1;
    }

    int mid = (left + right) / 2;
        
    if (v[mid] == x)
        return mid;
    if (v[mid] < x)
        return binfind(v, x, mid + 1, right);
    if (v[mid] > x)
        return binfind(v, x, left, mid - 1);
}

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
        P = rand(); //* rand() + rand();
        // //cout << P << endl;
    } while ((Ferma(P) != true));
    return P;
}

long int get_number_range(long int a, long int b) {
    long int P;
    
    do {
        P = a + rand() % b;
        //cout << P << endl;
    } while ((Ferma(P) != true));
    return P;
}

//Расширенный Алгоритм Евклида
void MyExtendedEvclid (long int a, long int b)
{
    int q = 0, r = 0, x1 = 0, x2 = 1, y1 = 1, y2 = 0, d = 0;
    long x, y;

    if (b == 0) {
    
        result_Evclid.gcd = a;
        result_Evclid.x = 1;
        result_Evclid.y = 0;
    }

    while (b > 0) {
        q = a / b, r = a - q *b;
        x = x2 - q * x1, y = y2 - q * y1;
        a = b, b = r;
        x2 = x1, x1 = x, y2 = y1, y1 = y;
    }

    result_Evclid.gcd = a;
    result_Evclid.x = x2;
    result_Evclid.y = y2;
}

void evklid(long int a, long int b) {
    long long U[3] = {a, 1, 0};
    long long V[3] = {b, 0, 1};
    long long T[3] = {1};
    long long q;
    while (T[0] != 0) {
        q = (U[0] / V[0]);
        for (int i = 0; i < 3; ++i) {
            T[i] = U[i] - q * V[i];
        }
        for (int j = 0; j < 3; ++j) {
            U[j] = V[j];
            V[j] = T[j];
        }
    };

    result_Evclid.gcd = U[0];
    result_Evclid.x = U[1];
    result_Evclid.y = U[2];
}

//Функция построения общего ключа для двух абонентов по схеме Диффи-Хеллмана
long int Diff_hell() {
    
    int g = 2, f;
    long int p, q;
    
    do {
        p = get_number();
        q = (p - 1) / 2;
    } while (!Ferma(q));
   
    do {
        f = MyPow(g, q, p);
        g++;
    } while (f == 1) ;

    int Xa, Xb, Ya, Yb;
    
    Xa = rand() % (p - 1);
    Xb = rand() % (p - 1);
    cout << "Xa: " << Xa << endl;
    cout << "Xb: " << Xb << endl;

    Ya = MyPow(g, Xa, p);
    Yb = MyPow(g, Xb, p);
    cout << "Ya: " << Ya << endl;
    cout << "Yb: " << Yb << endl;

    int Zab = MyPow(Yb, Xa, p);
    int Zba = MyPow(Ya, Xb, p);
    cout << "Zab: " << Zab << endl;
    cout << "Zba: " << Zba << endl;
    if (Zab == Zba)
    {
        return Zba;
    }
    else
        return -1;
}


void Babystep(int a, int y, long int p) {
    
    bool flag;
    int m, k, I = 0, J = 0, result;
    long int bin;
    
    //ШАГ 1
    m = k = sqrt(p) + 1;
    
    std::vector<long int> mas_k(k), mas_m(m), temp(m);

    //ШАГ 2
    for (int i = 1; i < m + 1; i++){
        mas_m[i - 1] = MyPow(a, i * m, p);
    }
    
    for (int i = 0; i < k; i++){
        mas_k[i] = ((y % p) * MyPow(a, i, p)) % p;
    }
    
    //ШАГ 3
    temp = mas_m;
    sort(temp.begin(), temp.end());
    for (int i = 0; i < mas_k.size(); i++){
        bin = binfind(temp, mas_k[i], 0, temp.size());
        if (bin != -1) {
            for (int j = 0; j < mas_m.size(); j++) {
                if (mas_m[j] == mas_k[i]) {
                    I = j + 1;
                    J = i ;
                    flag = true;
                    break;
                }
            }
        }
    }
    result = I * m - J;
    cout << "Результат работы алгоритма Babystep: " << result << endl;
}



////////////////АЛГОРИТМЫ ШИФРОВАНИЯ////////////////

void Shamir_Code (){
    
    long int m = 4567864; 
    long int Ca, Cb, Da, Db, x1, x2, x3, x4;
    long int p = get_number();

    while (1) {
        Ca = rand();
        cout << Ca << endl;
        MyExtendedEvclid(Ca, p - 1);
        if (result_Evclid.gcd == 1) {
            break;
            }    
    }

    if (result_Evclid.x < 0) {
        Da = p - 1 + result_Evclid.x;
    } else {
        Da = result_Evclid.x;
    }

    while (1) {
        Cb = rand();
        MyExtendedEvclid(Cb, p - 1);
        if (result_Evclid.gcd == 1) {
            break;
        }    
    }

    if (result_Evclid.x < 0) {
        Db = p - 1 + result_Evclid.x;
    } else {
        Db = result_Evclid.x;
    }

    
    x1 = MyPow(m, Ca, p);
    x2 = MyPow(x1, Cb, p);
    x3 = MyPow(x2, Da, p);
    x4 = MyPow(x3, Db, p);
   

    cout << "x1 = " << x1 << endl;
    cout << "x2 = " << x2 << endl;
    cout << "x3 = " << x3 << endl;
    cout << "x4 = " << x4 << endl;
    
    
}

void Elgamal_Code(){

    long int p, d;

    while (1) {
        p = rand();
        if (Ferma(p) == true){
            d = (p - 1) / 2;
            if (Ferma(d) == true){
                break;
            }
        }
    }
    
    long int g = 1;
    while (MyPow(g, d, p) == 1) {
        g++;
    }


    ifstream message("message", ios::binary);

    if (!message) {
        cerr << "File not found" << std::endl;
        exit(1);
    }

    // Закрытые ключи
    long int Cb = get_number_range(2, p - 2);
    
    // Открытые ключи
    long int Db = MyPow(g, Cb, p);

    //cout << Cb << endl;
    //d = MyPow(g, Cb, p);
    ofstream out("Elgamal_code", ios::binary | ios::out);

    char buf;
    
    // Шифровка
    while (message.read(&buf, sizeof(char))) {
        long int k = get_number_range(1, p - 2);
        
        // Отправка сообщения от А к Б
        long int r = MyPow(g, k, p);
        long int e = (buf * (MyPow(Db, k, p))) % p;

        out.write((char*)&e, sizeof(long int));
        out.write((char*)&r, sizeof(long int));
    }
    message.close();
    out.close();

    // Расшифровка
    long int e, r;

    ifstream temp("Elgamal_code", ios::binary | ios::in);
    ofstream decode_message("Elgamal_decode", ios::binary | ios::out);
    
    while (temp.read((char*)&e, sizeof(long int))) {
        temp.read((char*)&r, sizeof(long int));
        unsigned short mrecd = (e * (MyPow(r, p - 1 - Cb, p))) % p;
        decode_message.write((char*)&mrecd, sizeof(char));
    }
    decode_message.close();
    temp.close();
}

void Vernam_Code()
{

    ifstream message("message", ios::binary | ios::in);
    if (!message.is_open()) {
        cerr << "File not found" << endl;
    }
 
    // Устанавливаем указатель на 0 байт от конца файла
    message.seekg (0, ios_base::end);
    int size = message.tellg();
    cout << size << endl;
    message.seekg (0, ios_base::beg);

    // Создаем ключи
    ofstream keys_out("Vernam_keys", ios::binary | ios::out);
    for (int i = 0; i < size; ++i) {
        long int temp = rand();
        keys_out.write((char*)&temp, sizeof(long int));
    }
    keys_out.close();

    //Шифровка
    long int temp;
    long int buf;
    ofstream out("Vernama_code", ios::binary | ios::out);
    ifstream keys_in("Vernama_keys", ios::binary | ios::in);

    while (message.read((char*)&buf, sizeof(char))) {
        long int key;
        keys_in.read((char*)&key, sizeof(long int));
        temp = buf ^ key;
        out.write((char*)&temp, sizeof(long int));
    }

    message.close();
    out.close();
    //перемещаем указатель в начало файла 
    keys_in.seekg(0, std::ios::beg);


    //расшифровка
    ofstream decode_message("Vernama_decode", ios::binary | ios::out);
    ifstream temp2("Vernama_code", ios::binary | ios::in);

    for (int i = 0; temp2.read((char*)&buf, sizeof(long int)); ++i) {
        long int key;
        keys_in.read((char*)&key, sizeof(long int));
        temp = buf ^ key;
        decode_message.write((char*)&temp, sizeof(char));
    }
    keys_in.close();
    decode_message.close();
    temp2.close();
}

void RSA(){
    
    long int p = get_number_range(1, 100000);
    long int q = get_number_range(1, 100000);

    long int n = p * q;
    long int fi = (p - 1) * (q - 1);

    long int d, c;

    while (1) {
        d = get_number_range(1, fi - 1);
        if (gcd(d, fi) == 1) {
            break;
        }
    }
    
    MyExtendedEvclid(d, fi);
    if (result_Evclid.x < 0) {
        c = fi + result_Evclid.x;
    } else {
        c = result_Evclid.x;
    }

    ifstream message("message", ios::binary | ios::in);
    if (!message.is_open()) {
        cerr << "File not found " << endl;
    }

    long int m, e;

    // шифровка
    ofstream out("RSA_code", ios::binary | ios::out);
    
    while (message.read((char*)&m, sizeof(char))) {
        e = MyPow(m, d, n);
        out.write((char*)&e, sizeof(long int));
    }
    
    message.close();
    out.close();

    // расшифровка
    ifstream temp("RSA_code", ios::binary | ios::in);
    ofstream decode_message("RSA_decode",ios::binary | ios::out);
    
    while (temp.read((char*)&e, sizeof(long int))) {
        m = MyPow(e, c, n);
        decode_message.write((char*)&m, sizeof(char));
    }
    
    temp.close();
    decode_message.close();
}


///////цифровая подпись//////////


unsigned long int MD5_Hash(long int p, string buf) {
    unsigned long int h = 0;
    MD5 md5 = MD5(buf);
    for (int i = 0; i < 4; i++) {
        h = (h + md5.state[i]) % p;
    }

    return h;
}


void sign_RSA() {
    

    long int p = get_number_range(1, pow(2, 15));
    long int q = get_number_range(1, pow(2, 15));
    long int n = p * q;
    long int fi = (p - 1) * (q - 1);


    long int d, c;

    while (1) {
        d = get_number_range(1, fi - 1);
        if (gcd(d, fi) == 1) {
            break;
        }
    }
    
    MyExtendedEvclid(d, fi);
    if (result_Evclid.x < 0) {
        c = fi + result_Evclid.x;
    } else {
        c = result_Evclid.x;
    }

    if ((c * d) % fi != 1)
    {
        cout << "ERROR" << endl;
    }

    long int m, s, w;
    long int y;

///////////////////
    ifstream message("message", ios::binary | ios::in);
    if (!message.is_open()) {
        cerr << "File not found " << endl;
    }
    ofstream out("PODPIS_RSA", ios::binary | ios::out);

    string data;
    string tmp;

    while (!message.eof())                         
    {
        if (message.eof()) break;              
        message >> tmp;
        data += tmp += " ";                         
    }                                          
    cout << "message: " << data << endl;
    message.close(); 
///////////////////

    y = MD5_Hash(n, data);  
    s = MyPow(y, c, n);
    cout << "s = " << s << endl;
    out.write((char*)&s, sizeof(long int));
    out.close(); 
    
    long int w1;
    long int w2;
    ifstream temp("PODPIS_RSA", ios::binary | ios::in);

    temp.read((char*)&w1, sizeof(long int)); 
    cout << "w1 = " << w1 <<endl;
    
    w2 = MyPow(w1, d, n);
    if (w2 == y)
    {
        cout << "Подпись совпадает \n";
    }
    else
        cout << "НЕ СОВПАДАЕТ \n";
    
    temp.close();
}


void sign_Elgamal() {

    long int p, d;

    while (1) {
        p = rand();
        if (Ferma(p) == true){
            d = (p - 1) / 2;
            if (Ferma(d) == true){
                break;
            }
        }
    }
    
    long int g = 1;
    while (MyPow(g, d, p) == 1) {
        g++;
    }

    long int x = get_number_range(1, p - 1);
    long int y = MyPow(g, x, p);

    ifstream message("message", ios::binary);
    if (!message) {
        cerr << "File not found" << std::endl;
        exit(1);
    }
    ofstream out("PODPIS_GAMAL", ios::binary | ios::out);
    
    string data;

    while (!message.eof())                         
    {
        if (message.eof()) break;              
        message >> data;                         
    }                                          

    cout << "message: " << data << endl;
    message.close(); 
    
    long int m, h, k, k1, s, u, r;
    h = MD5_Hash(p, data);
    while (1) {
        k = get_number_range(1, p - 2);
        MyExtendedEvclid(k, p - 1);
        if (result_Evclid.gcd == 1) {
           break;
        }    
    }

    if (result_Evclid.x < 0) {
        k1 = p - 1 + result_Evclid.x;
    } else {
        k1 = result_Evclid.x;
    }
    //cout << "k = " << k << "k1 = " << k1 << endl;

    r = MyPow(g, k, p);
    u = (h - (x * r)) % (p - 1);
    s = (k1 * u) % (p - 1);

    out << r << " " << s;    
    message.close();
    out.close();

    //проверяем подпись
    ifstream temp("PODPIS_GAMAL", ios::binary | ios::in);
    long int tmp1, tmp2, tmp, my, w, w2;
    temp >> w;
    temp >> w2;
    temp.close();
        
    tmp1 = pow(y, w);
    tmp2 = pow(r, w2);
    tmp = tmp1 * tmp2;
        
    my = MyPow(g, h, p);

    if (tmp = my)
        cout << "Подпись совпадает \n";
    else
        cout << "НЕ СОВПАДАЕТ \n";
    
}


void sign_Gost() {

    long int q, b, y;
    long int p;
    long int a = 1, s, r, k, h , x;
    
    q = get_number_range(100, 5000);
    b = rand();
    p = b * q + 1;

    while (!Ferma(p)) {
        q = get_number_range(100, 5000);
        b = rand();
        p = b * q + 1;
    }   

    long int g;
    while ( a <= 1) {
    g = rand();
    if (g < 0)
    {
        continue;
    }
    a = MyPow(g, (p-1)/q, p);
    }

//////////////////////////
     ifstream message("message", ios::binary | ios::in);
    if (!message.is_open()) {
        cerr << "File not found " << endl;
    }
    ofstream out("PODPIS_RSA", ios::binary | ios::out);

    string data;

    while (!message.eof())                         
    {
        if (message.eof()) break;              
        message >> data;                         
    }                                          
    cout << "message: " << data << endl;
    message.close(); 
//////////////////////////
    x = rand() % q;
    y = MyPow(a, x, p);
    h = MD5_Hash(q, data);
    
    //cout << p << " " << q << " " << a << endl;
    while (1) {
        k = rand() % q;
        r = MyPow(a, k, p) % q;
        if (r == 0) {
            continue;
        }
        s = (k * h + x * r) % q;
        if (s > 0) {
            break;
        }
    }
    
    MyExtendedEvclid(q, h);
    long long int h1, u1, u2, v;
    result_Evclid.y > 0 ? h1 = result_Evclid.y : h1 = result_Evclid.y + q;
    
    // cout << "R = "<< r  << " S = " << s << " h1 = " << h1 << endl;
    
    u1 = (s * h1) % q;
    u2 = (-r * h1) % q;

    // cout << "q = " << q << endl;
    
    // cout << u1 << endl;
    if (u2 < 0) u2 += q;
    // cout << "u2 = " << u2 << endl;
    
    v = ((MyPow(a, u1, p) * MyPow(y, u2, p)) % p) % q;
    
    // cout << v << endl;
    
    if(v == r ) {
        cout << "Подпись совпадает" << endl;
    } else {
        cout << "НЕ СОВПАДАЕТ" << endl;
    }

}