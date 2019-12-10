#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "my_math.h"
using namespace std;
class ElGamal
{
public:
	ElGamal();
	~ElGamal();
	//	p - простое число
	//	g - первообразный корень p
	//	0 < x < p - 1
	//	m - шифруемое сообщение m < p
	void crypt(int p, int g, int x, string inFileName, string outFileName);
	void decrypt(int p, int x, string inFileName, string outFileName);
};

