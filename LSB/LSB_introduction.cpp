#include "pch.h"
#include "LSB_introduction.h"
using namespace std;
#pragma warning(disable : 4996)

bool lsb_intro(const string & txt_filename, const string & bmp_filename)
{
	ifstream in_file(txt_filename, ios::in | ios::binary);
	if (!in_file) {
		clog << "lsb_intro: cannot open input_file " + txt_filename << endl;
		return false;
	}
	vector<char> txt_data = read_to_vec(in_file);
	in_file.close();
	ifstream in_file_bmp(bmp_filename, ios::in | ios::binary);
	if (!in_file_bmp) {
		clog << "lsb_intro: cannot open input_file " + bmp_filename << endl;
		return false;
	}
	vector<char> bmp_data = read_to_vec(in_file_bmp);
	in_file_bmp.close();
	put_txt_to_bmp(txt_data, bmp_data);
	ofstream out_bmp(bmp_filename, ios::out | ios::binary);
	out_bmp.write(reinterpret_cast<const char*>(&bmp_data[0]), sizeof(char) * bmp_data.size());
	return true;
}

void put_txt_to_bmp(vector<char>& txt_data, vector<char>& bmp_data)
{
	const size_t header_size = 54;
	const unsigned char bit_in_byte = 8;
	const unsigned char replaced_bit_in_byte = 2;
	cout << "txt_size " << txt_data.size() << endl;
	cout << "bmp_size " << bmp_data.size() << endl;
	vector<char> header(&bmp_data[0], &bmp_data[header_size]);
	unsigned int fileSize = *reinterpret_cast<uint32_t *>(&header[2]);
	auto dataOffset = *reinterpret_cast<uint32_t *>(&header[10]);
	auto width = *reinterpret_cast<uint32_t *>(&header[18]);
	auto height = *reinterpret_cast<uint32_t *>(&header[22]);
	auto depth = *reinterpret_cast<uint16_t *>(&header[28]);
	std::cout << "fileSize: " << fileSize << std::endl;
	std::cout << "dataOffset: " << dataOffset << std::endl;
	std::cout << "width: " << width << std::endl;
	std::cout << "height: " << height << std::endl;
	std::cout << "depth: " << depth << "-bit" << std::endl;
	size_t channel_cnt = depth / bit_in_byte;
	size_t max_to_save = (width * height * channel_cnt * replaced_bit_in_byte) / bit_in_byte - 17;
	if (max_to_save >= txt_data.size()) {
		//k = 17 becouse i put in dataOffset + 4 (reserve for int) count of char to be write in .bmp
		put_elem_cnt(bmp_data, dataOffset, txt_data.size());
		for (int i = 0, k = 17; i < txt_data.size(); i++) {
			vector<bool> bits;
			for (int j = 0; j < 8; j++) {
				bits.push_back(get_bit(txt_data[i], j));
				if (j % 2 == 1) {
					set_bit(bmp_data[dataOffset + k], bits[0], 6);
					set_bit(bmp_data[dataOffset + k], bits[1], 7);
					k++;
					bits.clear();
				}
			}

		}
	}
	else {
		cout << ".txt file is too big! ";
		return;
	}

}

void put_elem_cnt(vector<char>& bmp_data, size_t dataOffset, size_t size)
{
	//need 32 bits => 16 bytes in .bmp for write size_t ;
	vector<bool> bits;
	for (int i = 0; i < 32; i++) {
		bits.push_back(get_bit_int(size, i));
	}
	for (int i = 0, k = 0; i < bits.size() - 1; i += 2, k++) {
		set_bit(bmp_data[dataOffset + k], bits[i], 6);
		set_bit(bmp_data[dataOffset + k], bits[i + 1], 7);
	}
}

bool get_bit(unsigned char byte, int position)
{
	if (position < 0 || position > 7) {
		cout << "error in get_bit ";
		terminate();
	}
	else {
		return(byte >> 7 - position) & 0x1;
	}
}

bool get_bit_int(unsigned int num, int position)
{
	if (position < 0 || position > 31) {
		cout << "error in get_bit_int ";
		terminate();
	}
	else {
		return(num >> 31 - position) & 0x1;
	}
}

void set_bit(char& byte, bool bit, int position)
{
	if (bit == 0) {
		byte &= ~(1 << position);
	}
	else {
		byte |= (1 << position);
	}
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

