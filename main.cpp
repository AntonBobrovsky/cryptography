#include "crypt.h"
#include "poker.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>

int main(int argc, char const *argv[])
{
    
    srand(time(0));
    
    sign_RSA();
    return 0;
}