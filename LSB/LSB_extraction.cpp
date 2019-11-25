#include "pch.h"
#include "LSB_extraction.h"

using namespace std;
//TODO: What is pch.h ??? and why it need to be first in headers list
bool lsb_extra(const string & txt_filename, const string & bmp_filename){
	
	size_t char_cnt = extrct_cnt(bmp_filename);
	string text = get_text(bmp_filename, char_cnt);
	ofstream out_file("out.txt", ios::out);
	if (!out_file) {
		clog << "lsb_extra: cannot open input_file " + bmp_filename << endl;
		return false;
	}
	else {
		out_file << text;
		out_file.close();
		return true;
	}
}

size_t extrct_cnt(const string & bmp_filename)
{
	//work ok
	ifstream in_file_bmp(bmp_filename, ios::in | ios::binary);
	if (!in_file_bmp) {
		clog << "extrct_cnt: cannot open input_file " + bmp_filename << endl;
		return false;
	}
	vector<char> bmp_data = read_to_vec(in_file_bmp);
	in_file_bmp.close();
	const size_t header_size = 54;
	vector<char> header(&bmp_data[0], &bmp_data[header_size]);
	auto dataOffset = *reinterpret_cast<uint32_t *>(&header[10]);

	vector<bool> int_bits;
	for (int k = 0; k < 16; k++) {
		int_bits.push_back(get_bit(bmp_data[dataOffset + k], 6));
		int_bits.push_back(get_bit(bmp_data[dataOffset + k], 7));
	}
	return get_num<int>(int_bits);
}

string get_text(const string & bmp_filename, const int cnt_char)
{
	ifstream in_file_bmp(bmp_filename, ios::in | ios::binary);
	if (!in_file_bmp) {
		clog << "lsb_intro: cannot open input_file " + bmp_filename << endl;
		terminate();
	}
	const size_t header_size = 54;
	vector<char> bmp_data = read_to_vec(in_file_bmp);
	vector<char> header(&bmp_data[0], &bmp_data[header_size]);
	auto dataOffset = *reinterpret_cast<uint32_t *>(&header[10]);
	in_file_bmp.close();
	string text;
	vector<bool> bits;
	const int cnt_shift = 17;
	for (int i = 0, k = 0; i < cnt_char; i++) {
		for (int b = 0; b < 4; b++) {
			bits.push_back(get_bit(bmp_data[dataOffset + cnt_shift + k], 6));
			bits.push_back(get_bit(bmp_data[dataOffset + cnt_shift + k], 7));
			k++;
		}
		char cur_char = get_num<char>(bits);
 		bits.clear();
		text.append(1, cur_char);
	}
	return text;
}
template <typename T>
T get_num(vector<bool>& bits)
{
	//was tested - ok
	T result = 0;
	if (bits.size() == 8) {
		for (int i = 0; i < bits.size(); i++) {
			set_bit(result, bits[i], i);
		}
	}
	else {
		for (int i = 0; i < bits.size(); i++) {
			short pos = 31 -  i;
			if (bits[i] == 0) {
				result &= ~(0x01 << pos);
			}
			else {
				result |= (0x01 << pos);
			}
		}
	}
	int test = 4;
	return result;
}
