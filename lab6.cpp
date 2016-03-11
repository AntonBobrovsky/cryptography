#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <string.h>
#include <unistd.h>

using namespace std;

#define MIN 1000
#define MAX 10000

long int iso[MAX];
long int key[MAX][2];
long int g[MAX][2];
long int F[MAX];
long int N;
long int graph[MAX][2];
long int new_graph[MAX][2];
long int cycle[MAX];
long int new_cycle[MAX];
long int size_cycle = 0, num_gran = 0;



struct evklid_answer
{
	long int gcd;
	long int x;
	long int y;
};

struct evklid_answer result;
struct rib H;

void MyExtendedEvclid(long int a, long int b) {
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

	result.gcd = U[0];
	result.x = U[1];
	result.y = U[2];
}


long long MyPow (long long x, long long n, long int p) {
	long long res = 1;

	while (n) {
		if (n & 1) {
			res = (res * x) % p;
		}
		x = (x * x) % p;
		n >>= 1;
	}
	return res;
}

long long gcd(long long a, long long b) {
	if (b == 0)
		return a;
	return gcd(b, a % b);
}

bool ferma(long long x) {
	if (x == 2)
		return true;

	for (int i = 0; i < 100; i++) {
		long int a = (rand() % (x - 2)) + 2;
		if (gcd(a, x) != 1)
			return false;
		if ( MyPow(a, x - 1, x) != 1)
			return false;
	}
	return true;
}


long int my_rando(long int rangeMin, long int rangeMax) {
	return (long int)(rand()) % (rangeMax - rangeMin + 1) + rangeMin;
}

long int get_number(unsigned int min, unsigned int max) {
	long int P = 4;
	while ((ferma(P) != true)) {
		P = my_rando(min, max) | 1;
	};
	return P;
}


void get_parameters() {
	long int C, D;
	long int p = get_number(MIN, MAX);
	long int q = get_number(MIN, MAX);
	N = p * q;
	long int f = (p - 1) * (q - 1);
	for (int i = 0; i < num_gran; i++) {
		result.gcd = 0;
		while (result.gcd != 1) {
			C = rand() % (f);
			MyExtendedEvclid(C, f);
		}
		result.x > 0 ? D = result.x : D = result.x + f;
		key[i][0] = C;
		key[i][1] = D;
	}
}

void show_parameters() {
	for (int i = 0; i < num_gran; ++i) {
		cout << key[i][0] << " " << key[i][1] << endl;
	}

}

void get_isomorf_graph() {

	for (int i = 0; i < num_gran +1; ++i) {
		iso[i] = i;
	}

	for (int i = 1; i < num_gran; ++i) {
		swap(iso[i], iso[random() % (num_gran)]);
	}

	for (int i = 0; i < num_gran; ++i) {
		for (int j = 0; j < 2; ++j) {
			new_graph[i][j] = iso[graph[i][j]];
		}
	}
/////////////////////////////////////////////////////
	// for (int i = 0; i < num_gran; ++i) {
	// 	for (int j = 0; j < 2; ++j) {
	// 		H.a[i] = iso[new_graph[i][j]];
	// 		H.b[i] = iso[new_graph[i][j]];
	// 		cout << "H.a[i] = " << H.a[i]<< "  " << "H.b[i] = " << H.b[i] << endl;
		
	// 	}
	// }

	for (int i = 0; i < size_cycle; ++i) {
		new_cycle[i] = iso[cycle[i]];
	}
}


void shifr_graph() {
	for (int i = 0; i < num_gran; ++i) {
		for (int j = 0; j < 2; ++j) {
			new_graph[i][j] = MyPow(new_graph[i][j] + 100, key[i][1], N);
		}
	}
}

void decode_graph() {
	for (int i = 0; i < num_gran; ++i) {
		for (int j = 0; j < 2; ++j) {
			new_graph[i][j] = MyPow(new_graph[i][j], key[i][0], N) - 100;
		}
	}
}

void shifr_cycle() {
	for (int i = 0; i < size_cycle; ++i) {
		F[i] = MyPow(new_cycle[i] + 100, key[i][1], N);
	}
}

void decode_cycle() {
	for (int i = 0; i < size_cycle; ++i) {
		F[i] = MyPow(F[i], key[i][0], N) - 100;
	}
}


void print_graph() {
	cout << "Изоморфный граф " << endl;
	for (int i = 0; i < num_gran; ++i) {
		for (int j = 0; j < 2; ++j) {
			cout << new_graph[i][j] << " ";
		}
		cout << endl;
	}
}

void print_cycle() {
	for (int i = 0; i < size_cycle ; ++i) {
		cout << F[i] << " -> ";
	}
	cout << endl;
}

void check_graph() {
	cout << "Расшированный изоморфный граф: " << endl;

	decode_graph();
	print_graph();

	///боб делает перестановки


	cout << "Расшифрованный первоначальный граф" << endl;
	
	for (int i = 0; i < num_gran; ++i){
		graph[i][0] = iso[new_graph[i][0]];
		graph[i][1] = iso[new_graph[i][1]];
	}
	
	for (int i = 0; i < num_gran; ++i) {
		for (int j = 0; j < 2; ++j) {
			cout << graph[i][j] << " ";
		}
		cout << endl;
	}
}

bool check_bob(){
	int flg = 0;
	for (int i = 0; i < size_cycle; ++i) {
		new_cycle[i] = MyPow(new_cycle[i] + 100, key[i][1], N);
		if (new_cycle[i] == F[i])
			flg = 1;
		else
			flg = 0;
	}

	decode_cycle();

	if (flg == 1)
		return true;
	else
		false;
}	

void check_cycle() {
	cout << "Расшифровываем ребра в гамильтоновом цикле: " << endl;
	bool flg = false;
	for (int i = 0; i < num_gran; ++i) {
		new_graph[i][0] = MyPow(new_graph[i][0], key[i][0], N) - 100;

		for (int j = 0; j < size_cycle; ++j) {
			if (new_cycle[j] == new_graph[i][0]) {
				cout << "cycle = " << new_cycle[j] << endl;
				cout << new_graph[i][0] << " ребро <---- " << new_graph[i][1] << endl;
			}
		}
	}

	flg = check_bob();
	if (flg == true)
	{
		cout << "\nПроверка прошла!\n";
	}

	cout << "Расшифрованный цикл: " << endl;
	print_cycle();
}

void read_from_file() {

	ifstream in;
	in.open("data");
	in >> num_gran >> size_cycle;

	for (int i = 0; i < num_gran; ++i) {
		in >> graph[i][0];
		in >> graph[i][1];
	}

	for (int i = 0; i < size_cycle; ++i) {
		in >> cycle[i];
	}
}

int main() {

	srand(time(NULL));
	int check;

	read_from_file();
	get_parameters();

	get_isomorf_graph();
	cout << endl;
	print_graph();
	cout << endl;
	// print_cycle();
	cout << "====================" << endl;
	cout << "Зашифрованный граф: " << endl;
	shifr_graph();
	shifr_cycle();
	print_graph();
	cout << "====================" << endl;
	cout << endl;

	// cout << "Зашифрованный цикл: "<< endl;
	// print_cycle();
	// cout << endl;

	cout << "Введите:\n1 -> Question #1\n2 -> Question #2\n\ninput >>";
	cin >> check;
	cout << endl;
	switch (check) {
	case (1):
		check_graph();
		break;
	case (2):
		check_cycle();
		break;
	}
	return 0;
}




























// cout << "graph[" << i << "]" << "[" << j << "] = " << graph[i][j] << endl;
// 			cout << "Значение iso [" << graph[i][j] << "] = " << iso[graph[i][j]] <<  endl;