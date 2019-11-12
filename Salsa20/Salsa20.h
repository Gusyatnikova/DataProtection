#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstddef>
#include <sstream>
using namespace std;
class Salsa20
{
private:
	//string key = "1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;16;";
	string key = "1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;18;";
	string t0 = "101;120;112;97;";
	string t1 = "110;100;32;49";
	string t2 = "54;45;98;121;";
	string t3 = "116;101;32;107;";
	string common;
	string plaintxt_str;
	char* plaintxt_char_arr;
	size_t plaintxt_size;

	string salsa20(std::string key_string, std::string plaintext);
	string salsa20(std::string, char*, size_t);//TODO: check is OK
	string salsa20_block(string, string);
	string salsa20_block(string, char*, size_t);
	vector<unsigned int> getVectorFrom2dArr(unsigned int **, int, int);
	vector <unsigned char> add_zeroes(vector <unsigned char>*, size_t);
	unsigned int** doubleround(vector<unsigned int> words);
	unsigned int** rowround(unsigned int**);
	unsigned int** columnround(vector <unsigned int>);
	unsigned int* quarterround(unsigned int, unsigned int, unsigned int, unsigned int);
	unsigned int littleendian(vector<unsigned char>*);
	unsigned char* littleendian_inverse(unsigned int);
	unsigned int modulo_addition(unsigned int, unsigned int);
	unsigned int bitwise_addition(unsigned int, unsigned int);
	unsigned int cycle_left_shift(unsigned int, unsigned int);
public:
	Salsa20();
	string encode(string);
	string encode(char*, size_t);
	string decode(string);
	~Salsa20();
};

