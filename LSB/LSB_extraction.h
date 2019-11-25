#pragma once
#include "pch.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "LSB_introduction.h"
using namespace std;

bool lsb_extra(const string & txt_filename, const string & bmp_filename);
size_t extrct_cnt(const string & bmp_filename);
string get_text(const string & bmp_filename, int cnt);
template <typename T>
T get_num(vector<bool>& bits);
