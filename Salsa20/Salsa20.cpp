 #include "pch.h"
#include <iostream>
#include "Salsa20.h"
#pragma warning(disable : 4996)
int main()
{
	//dont decode spaces
	//read input file to input string  
	string input_str;
	string tmp;
	fstream in_file("D:\\education\\НГУ 7\\Защита информации\\plaintext.txt");
	while (in_file >> tmp) {
		input_str.append(tmp).append(" ");
	}
	int len0 = input_str.size();
	in_file.close();
	Salsa20 salsa20;
	string salsa20_chipertext = salsa20.encode(input_str);
	ofstream out("D:\\education\\НГУ 7\\Защита информации\\chipertext.txt");
	out << salsa20_chipertext;
	out.close();
	int len1 = salsa20_chipertext.size();
	string decoded_str = salsa20.decode(salsa20_chipertext);
	int len2 = decoded_str.size();
	ofstream decoded_file("D:\\education\\НГУ 7\\Защита информации\\decoded.txt");
	decoded_file << decoded_str;
	decoded_file.close();
}

vector<unsigned int> Salsa20::getVectorFrom2dArr(unsigned int ** arr, int row_cnt, int clmn_cnt)
{
	std::vector<unsigned int> result;
	for (int i = 0; i < row_cnt; i++) {
		for (int j = 0; j < clmn_cnt; j++) {
			result.push_back(arr[i][j]);
		}
	}
	return result;
}

string Salsa20::salsa20(std::string key_string, std::string plaintext)
{
	size_t src_lenght = key_string.length();
	size_t block_cnt = plaintext.length() / 64;
	std::string result;
	size_t result_size;
	if (block_cnt == 0) {
		result.append(salsa20_block(key_string, plaintext));
	}
	for (int i = 0; i < block_cnt; i++) {
		result.append(salsa20_block(key_string, plaintext.substr(i * 64, 64)));
		result_size = result.length();
	}


	return result;
}

std::string Salsa20::salsa20(std::string key_string, char * plaintext, size_t plaintext_len)
{
	size_t src_lenght = key_string.length();
	size_t block_cnt = plaintext_len / 64;
	std::string result;
	size_t result_size;
	if (block_cnt == 0) {
		result.append(salsa20_block(key_string, plaintext));
	}
	for (int i = 0; i < block_cnt; i++) {
		char cur_block[64];
		for (int j = 0; j < 64; j++) {
			cur_block[j] = plaintext[i * 64 + j];
		}
		result.append(salsa20_block(key_string, cur_block, 64));
		result_size = result.length();
	}

	return result;
}

std::string Salsa20::salsa20_block(std::string key_string, std::string block_plaintext)
{
	std::vector <unsigned char> filled_plain_arr;
	std::vector <unsigned char> row_input;
	for (auto it = key_string.begin(); it != key_string.end(); it++) {
		char cur[3];
		size_t i = 0;
		while (*it != 59 && it != key_string.end()) {
			cur[i] = *it;
			i++;
			it++;
		}
		row_input.push_back(std::stoul(std::string(cur), nullptr, 0));
	}
	size_t key_lenght = row_input.size();
	filled_plain_arr = add_zeroes(&row_input, key_lenght);

	std::vector<unsigned int> words;
	//fill words
	for (int i = 0; i <= 15; i++) {
		std::vector<unsigned char> cur_char_arr;
		for (int j = 0; j < 4; j++) {
			cur_char_arr.push_back(filled_plain_arr[4 * i + j]);
		}
		words.push_back(littleendian(&cur_char_arr));
	}

	//ciphered text:
	unsigned int** dblrnd_res = new unsigned int *[4];
	for (int i = 0; i < 4; i++) {
		dblrnd_res[i] = new unsigned int[4];
	}
	//words[0] = 0xde501066;
	//words[1] = 0x6f9eb8f7;
	//words[2] = 0xe4fbbd9b;
	//words[3] = 0x454e3f57;
	//words[4] = 0xb75540d3;
	//words[5] = 0x43e93a4c;
	//words[6] = 0x3a6f2aa0;
	//words[7] = 0x726d6b36;
	//words[8] = 0x9243f484;
	//words[9] = 0x9145d1e8;
	//words[10] = 0x4fa9d247;
	//words[11] = 0xdc8dee11;
	//words[12] = 0x054bf545;
	//words[13] = 0x254dd653; 
	//words[14] = 0xd9421b6d; 
	//words[15] = 0x67b276c1;
	//start hash!!!!
	dblrnd_res = doubleround(words);
	char buf[13];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			unsigned int a = dblrnd_res[i][j];
			itoa(a, buf, 16);
			std::string str = std::string(buf);
		}
	}
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			itoa(dblrnd_res[i][j], buf, 16);
		}
	}

	std::vector <unsigned char> salsa20res;
	std::string str_salsa20res;
	std::vector <unsigned char> plaintext_arr;
	for (auto it = block_plaintext.begin(); it != block_plaintext.end(); it++) {
		plaintext_arr.push_back(*it);
	}
	plaintext_arr = add_zeroes(&plaintext_arr, block_plaintext.size());
	//try reverse words
	//std::reverse(words.begin(), words.end());
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			unsigned int cur_word = words.back();
			salsa20res.push_back(littleendian_inverse(modulo_addition(dblrnd_res[i][j], words.back()))[0]);
			salsa20res.push_back(littleendian_inverse(modulo_addition(dblrnd_res[i][j], words.back()))[1]);
			salsa20res.push_back(littleendian_inverse(modulo_addition(dblrnd_res[i][j], words.back()))[2]);
			salsa20res.push_back(littleendian_inverse(modulo_addition(dblrnd_res[i][j], words.back()))[3]);
			words.pop_back();
		}
	}
	string hashRes;
	for (auto it = salsa20res.begin(); it != salsa20res.end(); it++) {
		std::stringstream ss;
		ss << *it;
		hashRes.append(ss.str()).append(" ");
	}
	ofstream out("D:\\education\\НГУ 7\\Защита информации\\hashOutput2.txt");
	out << hashRes;
	out.close();
	int l = block_plaintext.size();
	for (int i = 0; i < block_plaintext.size(); i++) {
		str_salsa20res.insert(str_salsa20res.length(), 1, char(bitwise_addition(salsa20res[i], plaintext_arr[i])));
	}
	_CrtSetDebugFillThreshold(0);
	return str_salsa20res;
}

string Salsa20::salsa20_block(std::string key_string, char * block_plaintext, size_t block_len)
{
	std::vector <unsigned char> filled_plain_arr;
	std::vector <unsigned char> row_input;
	for (auto it = key_string.begin(); it != key_string.end(); it++) {
		char cur[3];
		size_t i = 0;
		while (*it != 59 && it != key_string.end()) {
			cur[i] = *it;
			i++;
			it++;
		}
		row_input.push_back(std::stoul(std::string(cur), nullptr, 0));
	}
	size_t key_lenght = row_input.size();
	filled_plain_arr = add_zeroes(&row_input, key_lenght);

	std::vector<unsigned int> words;
	//fill words
	for (int i = 0; i <= 15; i++) {
		std::vector<unsigned char> cur_char_arr;
		for (int j = 0; j < 4; j++) {
			cur_char_arr.push_back(filled_plain_arr[4 * i + j]);
		}
		words.push_back(littleendian(&cur_char_arr));
	}

	//ciphered text:
	unsigned int** dblrnd_res = new unsigned int *[4];
	for (int i = 0; i < 4; i++) {
		dblrnd_res[i] = new unsigned int[4];
	}
	//words[0] = 0xde501066;
	//words[1] = 0x6f9eb8f7;
	//words[2] = 0xe4fbbd9b;
	//words[3] = 0x454e3f57;
	//words[4] = 0xb75540d3;
	//words[5] = 0x43e93a4c;
	//words[6] = 0x3a6f2aa0;
	//words[7] = 0x726d6b36;
	//words[8] = 0x9243f484;
	//words[9] = 0x9145d1e8;
	//words[10] = 0x4fa9d247;
	//words[11] = 0xdc8dee11;
	//words[12] = 0x054bf545;
	//words[13] = 0x254dd653; 
	//words[14] = 0xd9421b6d; 
	//words[15] = 0x67b276c1;
	//start hash!!!!
	dblrnd_res = doubleround(words);
	char buf[13];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			unsigned int a = dblrnd_res[i][j];
			itoa(a, buf, 16);
			std::string str = std::string(buf);
		}
	}
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));
	dblrnd_res = doubleround(getVectorFrom2dArr(dblrnd_res, 4, 4));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			itoa(dblrnd_res[i][j], buf, 16);
		}
	}

	std::vector <unsigned char> salsa20res;
	std::string str_salsa20res;
	std::vector <unsigned char> plaintext_arr;
	for (size_t i = 0; i < block_len; i++) {
		plaintext_arr.push_back(block_plaintext[i]);
	}
	plaintext_arr = add_zeroes(&plaintext_arr, block_len);
	//try reverse words
	//std::reverse(words.begin(), words.end());
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			unsigned int cur_word = words.back();
			salsa20res.push_back(littleendian_inverse(modulo_addition(dblrnd_res[i][j], words.back()))[0]);
			salsa20res.push_back(littleendian_inverse(modulo_addition(dblrnd_res[i][j], words.back()))[1]);
			salsa20res.push_back(littleendian_inverse(modulo_addition(dblrnd_res[i][j], words.back()))[2]);
			salsa20res.push_back(littleendian_inverse(modulo_addition(dblrnd_res[i][j], words.back()))[3]);
			words.pop_back();
		}
	}
	for (int i = 0; i < block_len; i++) {
		str_salsa20res.insert(str_salsa20res.length(), 1, char(bitwise_addition(salsa20res[i], plaintext_arr[i])));
	}
	return str_salsa20res;
}

std::vector<unsigned char> Salsa20::add_zeroes(std::vector<unsigned char>* arr, size_t src_lenght)
{
	std::vector <unsigned char> res_arr;
	for (int i = 0; i < 64; i++) {
		if (i < src_lenght) {
			res_arr.push_back((*arr)[i]);
		}
		else {
			res_arr.push_back(0);
		}
	}
	return res_arr;
}

unsigned int ** Salsa20::doubleround(std::vector<unsigned int> words)
{
	unsigned int ** result;
	result = new unsigned int*[4];
	for (int i = 0; i < 4; i++) {
		result[i] = new unsigned int[4];
	}
	result = columnround(words);
	result = rowround(result);
	return result;
}

unsigned int ** Salsa20::rowround(unsigned int ** words)
{
	//was tested - ok
	unsigned int ** result;
	result = new unsigned int*[4];
	for (int i = 0; i < 4; i++) {
		result[i] = new unsigned int[4];
	}
	//test
	char buf[13];
	// 0 1 2 3
	// 4 5 6 7
	// 8 9 10 11
	// 12 13 14 15
	result[0][0] = quarterround(words[0][0], words[0][1], words[0][2], words[0][3])[0];
	result[0][1] = quarterround(words[0][0], words[0][1], words[0][2], words[0][3])[1];
	result[0][2] = quarterround(words[0][0], words[0][1], words[0][2], words[0][3])[2];
	result[0][3] = quarterround(words[0][0], words[0][1], words[0][2], words[0][3])[3];

	result[1][1] = quarterround(words[1][1], words[1][2], words[1][3], words[1][0])[0];
	result[1][2] = quarterround(words[1][1], words[1][2], words[1][3], words[1][0])[1];
	result[1][3] = quarterround(words[1][1], words[1][2], words[1][3], words[1][0])[2];
	result[1][0] = quarterround(words[1][1], words[1][2], words[1][3], words[1][0])[3];

	result[2][2] = quarterround(words[2][2], words[2][3], words[2][0], words[2][1])[0];
	result[2][3] = quarterround(words[2][2], words[2][3], words[2][0], words[2][1])[1];
	result[2][0] = quarterround(words[2][2], words[2][3], words[2][0], words[2][1])[2];
	result[2][1] = quarterround(words[2][2], words[2][3], words[2][0], words[2][1])[3];

	result[3][3] = quarterround(words[3][3], words[3][0], words[3][1], words[3][2])[0];
	result[3][0] = quarterround(words[3][3], words[3][0], words[3][1], words[3][2])[1];
	result[3][1] = quarterround(words[3][3], words[3][0], words[3][1], words[3][2])[2];
	result[3][2] = quarterround(words[3][3], words[3][0], words[3][1], words[3][2])[3];
	return result;
}

unsigned int ** Salsa20::columnround(std::vector<unsigned int> words)
{
	//was tested - OK
	unsigned int ** result;
	result = new unsigned int*[4];
	for (int i = 0; i < 4; i++) {
		result[i] = new unsigned int[4];
	}
	//test
	/*for (auto it = words.begin(); it != words.end(); ++it) {
		*it = 0;
	}
	words[0] = 1;
	words[4] = 1;
	words[8] = 1;
	words[12] = 1;*/
	//end test
//words[0] = 0x08521bd6;
//words[1] = 0x1fe88837;
//words[2] = 0xbb2aa576;
//words[3] = 0x3aa26365;
//words[4] = 0xc54c6a5b;
//words[5] = 0x2fc74c2f;
//words[6] = 0x6dd39cc3;
//words[7] = 0xda0a64f6;
//words[8] = 0x90a2f23d;
//words[9] = 0x067f95a6;
//words[10] = 0x06b35f61;
//words[11] = 0x41e4732e;
//words[12] = 0xe859c100;
//words[13] = 0xea4d84b7;
//words[14] = 0x0f619bff;
//words[15] = 0xbc6e965a;
	char buf[13];
	result[0][0] = quarterround(words[0], words[4], words[8], words[12])[0];

	result[1][0] = quarterround(words[0], words[4], words[8], words[12])[1];
	result[2][0] = quarterround(words[0], words[4], words[8], words[12])[2];
	result[3][0] = quarterround(words[0], words[4], words[8], words[12])[3];

	result[1][1] = quarterround(words[5], words[9], words[13], words[1])[0];
	result[2][1] = quarterround(words[5], words[9], words[13], words[1])[1];
	result[3][1] = quarterround(words[5], words[9], words[13], words[1])[2];
	result[0][1] = quarterround(words[5], words[9], words[13], words[1])[3];

	result[2][2] = quarterround(words[10], words[14], words[2], words[6])[0];
	result[3][2] = quarterround(words[10], words[14], words[2], words[6])[1];
	result[0][2] = quarterround(words[10], words[14], words[2], words[6])[2];
	result[1][2] = quarterround(words[10], words[14], words[2], words[6])[3];

	result[3][3] = quarterround(words[15], words[3], words[7], words[11])[0];
	result[0][3] = quarterround(words[15], words[3], words[7], words[11])[1];
	result[1][3] = quarterround(words[15], words[3], words[7], words[11])[2];
	result[2][3] = quarterround(words[15], words[3], words[7], words[11])[3];
	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 4; j++) {
	//		unsigned int a = result[i][j];
	//		itoa(a, buf, 16);
	//	}
	//}
	return result;
}

unsigned int * Salsa20::quarterround(unsigned int y0, unsigned int y1, unsigned int y2, unsigned int y3)
{
	//OK
//y0 = 0xe7e8c006;
//y1 = 0xc4f9417d;
//y2 = 0x6479b4b2;
//y3 = 0x68c67137;
//y0 = 0xd3917c5b; 
//y1 = 0x55f1c407; 
//y2 = 0x52a58a7a; 
//y3 = 0x8f887a3b;
	unsigned int* result = new unsigned int[4];
	//char buf0[10];
	//char buf1[10];
	//char buf2[10];
	//char buf3[10];
	result[1] = bitwise_addition(y1, cycle_left_shift(modulo_addition(y0, y3), 7));
	//itoa(result[1], buf1, 16);
	result[2] = bitwise_addition(y2, cycle_left_shift(modulo_addition(result[1], y0), 9));
	//itoa(result[2], buf2, 16);
	result[3] = bitwise_addition(y3, cycle_left_shift(modulo_addition(result[2], result[1]), 13));
	//itoa(result[3], buf3, 16);
	result[0] = bitwise_addition(y0, cycle_left_shift(modulo_addition(result[3], result[2]), 18));
	//itoa(result[0], buf0, 16);

	return result;
}

unsigned int Salsa20::littleendian(vector<unsigned char>* bytes)
{
	unsigned int res = (unsigned int)(*bytes)[0] + pow(2, 8) * (*bytes)[1] + pow(2, 16) * (*bytes)[2] + pow(2, 24) * (*bytes)[3];
	if (res < 0) {
		bool error = true;
	}
	return (unsigned int)(*bytes)[0] + pow(2, 8) * (*bytes)[1] + pow(2, 16) * (*bytes)[2] + pow(2, 24) * (*bytes)[3];
}

unsigned char * Salsa20::littleendian_inverse(unsigned int paramInt)
{
	unsigned char* result = new unsigned char[4];
	result[0] = paramInt & 0x000000ff;
	result[1] = (paramInt & 0x0000ff00) >> 8;
	result[2] = (paramInt & 0x00ff0000) >> 16;
	result[3] = (paramInt & 0xff000000) >> 24;
	return result;
}

unsigned int Salsa20::modulo_addition(unsigned int first, unsigned int second)
{
	return first + second;
}

unsigned int Salsa20::bitwise_addition(unsigned int first, unsigned int second)
{
	return first ^ second;
}

unsigned int Salsa20::cycle_left_shift(unsigned int number, unsigned int cnt)
{
	return (((number) << (cnt)) | ((number) >> (32 - (cnt))));
}

Salsa20::Salsa20()
{
	common.append(t0).append(key).append(t1).append(t2).append(key).append(t3);
}

string Salsa20::encode(string plaintext)
{
	return salsa20(common, plaintext);
}

string Salsa20::encode(char * plaintext, size_t plaintext_size)
{
	return salsa20(common, plaintext, plaintext_size);
}

string Salsa20::decode(string chippertext)
{
	return salsa20(common, chippertext);
}


Salsa20::~Salsa20()
{
}
