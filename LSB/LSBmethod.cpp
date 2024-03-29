﻿#include "pch.h"

#include <iostream>
#include <string>

#include "LSB_introduction.h"
#include "Verification.h"
#include "LSB_extraction.h"


int main()
{
	cout << "Enter command: Introduction|Extraction (in|ex) ";
	string command;
	//cin >> command;
	command = "in";
	if (!check_command(command)) {
		cout << "incorrect command!" << endl;;
		return 0;
	}
	cout << "Enter name of .txt file: ";
	string txt_filename;
	//cin >> txt_filename;
	cout << "Enter name of .bmp file: ";
	string bmp_filename;
	//cin >> bmp_filename;
	
	txt_filename = "ex.txt";
	bmp_filename = "a.bmp";
	if (command == "Introduction" || command == "in") {
		if (lsb_intro(txt_filename, bmp_filename) != true) {
			cout << "incorrect filename(s)";
			return 0;
		}
		lsb_extra("out.txt", bmp_filename);
	}
	else {
		if (lsb_extra(txt_filename, bmp_filename) != 0) {
			cout << "incorrect input filename(s)!" << endl;
			return 0;
		}
	}
	return 0;
}

