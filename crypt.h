#ifndef CRYPT_H
#define CRYPT_H

#include <iostream>	
#include <cmath>
#include <vector>
#include "md5.h"

using namespace std;

// Стуктура для хранения результатов Алгоритма Евклида
struct ExtendedEvclidResult
{
    long gcd;
    long x;
    long y;
};

void MyExtendedEvclid (long int a, long int b);
void evklid(long int a, long int b);
int binfind(std::vector<long int> v, int x, int left, int right);
long int gcd (long int a, long int b);
long int MyPow(long int a, long int b, long int p);
bool Ferma(long long x);
long int gen();
long int get_number();
long int get_number_range(long int a, long int b);
long int Diff_hell();
void Babystep(int a, int y, long int p);
// шифры
void Shamir_Code ();
void Elgamal_Code();
void Vernam_Code();
void RSA();
//подписи
void sign_RSA();
void sign_Elgamal();
void sign_Gost();

#endif