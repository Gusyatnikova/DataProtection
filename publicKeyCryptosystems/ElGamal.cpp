#include "ElGamal.h"



ElGamal::ElGamal()
{
}


ElGamal::~ElGamal()
{
}

void ElGamal::crypt(int p, int g, int x, string inFileName, string outFileName)
{
	ifstream inf(inFileName.c_str());
	ofstream outf(outFileName.c_str());

	int y = my_math::modularization(g, x, p);
	//cout << "Открытый ключ (p,g,y)=" << "(" << p << "," << g << "," << y << ")" << endl;
	//cout << "Закрытый ключ x=" << x << endl;

	//cout << "\nШифруемый текст:" << endl;

	while (inf.good()) {
		int m = inf.get();
		if (m > 0) {
			//cout << (char)m;
			int k = rand() % (p - 2) + 1; // 1 < k < (p-1)
			int a = my_math::modularization(g, k, p);
			int b = my_math::mul_modul(my_math::modularization(y, k, p), m, p);
			outf << a << " " << b << " ";
		}
	}
	//cout << endl;
	inf.close();
	outf.close();
}

void ElGamal::decrypt(int p, int x, string inFileName, string outFileName)
{
	ifstream inf(inFileName.c_str());
	ofstream outf(outFileName.c_str());

	while (inf.good()) {
		int a = 0;
		int b = 0;
		inf >> a;
		inf >> b;

		if (a != 0 && b != 0) {
			//wcout<<a<<" "<<b<<endl;

			int deM = my_math::mul_modul(b, my_math::modularization(a, p - 1 - x, p), p);
			// m=b*(a^x)^(-1)mod p =b*a^(p-1-x)mod p
			char m = static_cast<char>(deM);
			outf << m;
			wcout << m;
		}
	}

	wcout << endl;

	inf.close();
	outf.close();
}
