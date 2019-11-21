#include "pch.h"
#include "LSB_introduction.h"
using namespace std;

bool lsb_intro(const string & txt_filename, const string & bmp_filename)
{
	//use triple matrix: 24.5.5.
	//use 25.5 for bit operations
	//TODO: how refactor reading this two files?
	//read .txt
	ifstream in_file(txt_filename, ios::in | ios::binary);
	if (!in_file) {
		clog << "lsb_intro: cannot open input_file " + txt_filename << endl;
		return false;
	}
	vector<char> txt_data = read_to_vec(in_file);
	in_file.close();
	//read .bmp
	ifstream in_file_bmp(bmp_filename, ios::in | ios::binary);
	if (!in_file_bmp) {
		clog << "lsb_intro: cannot open input_file " + bmp_filename << endl;
		return false;
	}
	vector<char> bmp_data = read_to_vec(in_file_bmp);
	in_file_bmp.close();

	//char = 8 bit => for hide 1 char i need 4 times by 2 bits
	// => 1 char txt = 4 bytes bmp
	
	put_txt_to_bmp(txt_data, bmp_data);
	return true;
}

void put_txt_to_bmp(vector<char> txt_data, vector<char> bmp_data)
{

}

//template <typename T>
vector<char> read_to_vec(ifstream& ifstream)
{
	vector<char> data = {};
	if (!ifstream.eof() && !ifstream.fail())
	{
		ifstream.seekg(0, ios::end);
	    size_t file_size = ifstream.tellg();
		data.resize(file_size);

		ifstream.seekg(0, ios::beg);
		ifstream.read(&data[0], file_size);
	}
	return data;
}

