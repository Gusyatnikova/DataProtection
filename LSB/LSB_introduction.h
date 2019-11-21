#pragma once
#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

bool lsb_intro(const string & txt_filename, const string & bmp_filename);
vector <char> read_to_vec(ifstream& istream);
void put_txt_to_bmp(vector<char>& txt_data, vector<char>& bmp_data);