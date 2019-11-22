#pragma once
#include "pch.h"

#include <cstdlib>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

bool lsb_intro(const string & txt_filename, const string & bmp_filename);
vector <char> read_to_vec(ifstream& istream);
void put_txt_to_bmp(vector<char>& txt_data, vector<char>& bmp_data);
void put_elem_cnt(vector<char>& bmp_data, size_t dataOffset, size_t size);
bool get_bit(unsigned char byte, int position);
bool get_bit_int(unsigned int num, int position);
void set_bit(char& byte, bool bit, int position);