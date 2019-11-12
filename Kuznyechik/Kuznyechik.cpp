#include "pch.h"
#include <iostream>
#include "Kuznyechik.h"

/* 
	Kusnyecheck is a symmetric block encryption algorithm 
	with a block size of 128 bits (16 bytes) 
	and a key length of 256 bits (32 bytes)
	and have 10 rounds.
*/

int main()
{
	Kuznyechik kuznyechik;
    string plaintxt = "";
	string tmp;
	fstream in_file("D:\\education\\НГУ 7\\Защита информации\\kuznyechik\\plaintext.txt");
	while (in_file >> tmp) {
		plaintxt.append(tmp).append(" ");
	}
	in_file.close();
	//string ciphertxt = kuznyechik.encode(plaintxt).substr(0, 16);

	string ciphertxt = kuznyechik.encode(plaintxt);
	size_t l1 = ciphertxt.size();
	ofstream cipher_file("D:\\education\\НГУ 7\\Защита информации\\kuznyechik\\ciphertext.txt");
	cipher_file << ciphertxt;
	cipher_file.close();
	string decoded_str = kuznyechik.decode(ciphertxt);
	ofstream decoded_file("D:\\education\\НГУ 7\\Защита информации\\kuznyechik\\decoded.txt");
	decoded_file << decoded_str;
	decoded_file.close();
	int test = -1;
}


string Kuznyechik::encode(string plaintxt_str)
{
	//TODO: encode blocks
	//get all and give 16 byte for blocks ()
	//stretch_key(key);
	vector<uint8_t> cur(plaintxt_str.begin(), plaintxt_str.end());
	size_t block_cnt = plaintxt_str.length() / 16;
	string result;
	if (0 == block_cnt) {
		uint8_t ciphertxt_block[16];
		uint8_t block_filled[16];
		uint8_t *p = &cur[0];
		fill_zeroes(p, cur.size(), block_filled);
		//key?
		encode_block(key, block_filled, ciphertxt_block);
		char* res_arr = reinterpret_cast<char *>(ciphertxt_block);
		for (int i = 0; i < 16; i++) {
			uint8_t t1 = block_filled[i];
			unsigned char test = res_arr[i];
			int a = 4;
		}
		return string(res_arr);
	}
	else {
		size_t len_last_block = plaintxt_str.length() - block_cnt * 16;
		for (int i = 0; i <= block_cnt; i++) {
			uint8_t cur_block[16];
			uint8_t cur_ciphertxt[16];
			fill_zeroes(cur_block, 0, cur_block);
			for (size_t k = 0; k < 16 && (i*16 + k) < cur.size(); k++) {
				cur_block[k] = cur[i * 16 + k];
			}
			encode_block(key, cur_block, cur_ciphertxt);
			//unsigned char* res_arr = reinterpret_cast<unsigned char *>(cur_ciphertxt);
			//string cur_str = string(res_arr, res_arr + sizeof res_arr / sizeof res_arr[0]);
			//result.append(cur_str);
			for (int i = 0; i < 16; i++) {
				result.append(1, static_cast<char>(cur_ciphertxt[i]));
			}
			int test = -1;
		}
		return result;
	}
	return string();
}

string Kuznyechik::decode(string ciphertxt)
{
	size_t block_cnt = ciphertxt.length() / 16;
	string result;
		uint8_t p[16];
		uint8_t plaintxt_block[16];
		for (size_t i = 0; i < block_cnt; i++) {
			for (int k = 0; k < 16; k++) {
				p[k] = ciphertxt[i*16 + k];
			}
			decode_block(key, p, plaintxt_block);
			for (int i = 0; i < 16; i++) {
				result.append(1, static_cast<unsigned char>(plaintxt_block[i]));
			}
			//for (int i = 0; i < 16; i++) {
			//	uint8_t t1 = p[i];
			//	unsigned char t2 = plaintxt_block[i];
			//	int t3 = 4;
			//}
			//unsigned char* res_arr = reinterpret_cast<unsigned char *>(plaintxt_block);
			//for (int i = 0; i < 16; i++) {
			//	char test = res_arr[i];
			//	int a = 4;
			//}
			//string cur_str = string(res_arr, res_arr + sizeof res_arr / sizeof res_arr[0]);
		}
		//return string((char *)plaintxt_block);
		
		return result;
}

Kuznyechik::Kuznyechik()
{
	init_cipher();
	stretch_key(key);
}


void Kuznyechik::init_cipher()
{
	uint8_t x[16];
	for (size_t i = 0; i < 16; i++) {
		for (size_t j = 0; j < 256; j++) {
			for (size_t k = 0; k < 16; k++) {
				x[k] = 0;
			}
			x[i] = p_table[j];
			l(x, x);
			for (size_t m = 0; m < 16; m++) {
				ls_enc_lookup[i][j][m] = x[m];
			}
			for (size_t m = 0; m < 16; m++) {
				x[m] = 0;
			}
			x[i] = uint8_t(j);
			l_inv(x, x);
			for (size_t m = 0; m < 16; m++) {
				l_inv_lookup[i][j][m] = x[m];
			}
			for (size_t m = 0; m < 16; m++) {
				x[m] = 0;
			}
			x[i] = inverse_p_table[j];
			l_inv(x, x);
			for (size_t m = 0; m < 16; m++) {
				sl_dec_lookup[i][j][m] = x[m];
			}
		}
	}
}

uint8_t Kuznyechik::gf_multiplication(uint8_t x, uint8_t y)
{
	uint8_t res = 0;
	while (y != 0) {
		if (y & 1 == 1) {
			res = res ^ x;
		}
		if (x & 0x80 != 0) {
			x = (x << 1) ^ 0xC3;
		}
		else {
			x = x << 1;
		}
		y = y >> 1;
	}
	return res;
}

void Kuznyechik::l(uint8_t block[16], uint8_t result[16])
{
	// Takes 128-bit block and returns result of L-function.
	uint8_t x;
	//16 rounds of transformation R 
	for (size_t j = 0; j < 16; j++) {
		x = block[15];
		for (int i = 14; i >= 0; i--) {
			block[i + 1] = block[i];
			//multiplication and addition in GF
			x = x ^ gf_multiplication(block[i], l_vector[i]);
		}
		block[0] = x;
	}
	for (int k = 0; k < 16; k++) {
		result[k] = block[k];
	}
}

void Kuznyechik::l_inv(uint8_t block[16], uint8_t result[16])
{
	uint8_t x = 0;
	for (size_t j = 0; j < 16; j++) {
		x = block[0];
		for (size_t i = 0; i < 15; i++) {
			block[i] = block[i + 1];
			x = x ^ gf_multiplication(block[i], l_vector[i]);
		}
		block[15] = x;
	}
	for (size_t k = 0; k < 16; k++) {
		result[k] = block[k];
	}
}

void Kuznyechik::stretch_key(uint8_t key[32])
{
	uint8_t x[16], y[16], C[16], z[16];
	//first step - split input key to pair: x, y
	for (size_t i = 0; i < 16; i++) {
		x[i] = key[i];
		y[i] = key[i + 16];
	}
	// one pair = 8 iteration Feystels's
	for (size_t i = 0; i < 16; i++) {
		round_keys[0][i] = x[i];
		round_keys[1][i] = y[i];
	}
	for (size_t i = 1; i <= 32; i++) {
		for (size_t k = 0; k < 16; k++) {
			C[k] = 0;
		}
		C[15] = uint8_t(i);
		l(C, C);
		//compute sequence of round keys
		for (size_t k = 0; k < 16; k++) {
			z[k] = p_table[x[k] ^ C[k]];
		}
		l(z, z);
		for (size_t k = 0; k < 16; k++) {
			z[k] = z[k] ^ y[k];
		}
		for (size_t i = 0; i < 16; i++) {
			y[i] = x[i];
			x[i] = z[i];
		}
		if (i % 8 == 0) {
			//store each pair of round keys
			size_t r = i >> 2;
			for (size_t i = 0; i < 16; i++) {
				round_keys[r][i] = x[i];
				round_keys[r + 1][i] = y[i];
			}
		}
	}
}

void Kuznyechik::fill_zeroes(uint8_t* block, size_t symb_cnt, uint8_t result[16])
{
	for (size_t i = 0; i < symb_cnt; i++) {
		result[i] = block[i];
	}
	for (size_t k = symb_cnt; k < 16; k++) {
		result[k] = 0;
	}
}

Kuznyechik::~Kuznyechik()
{
}

void Kuznyechik::encode_block(uint8_t key[32], uint8_t block[16], uint8_t ciphertext[16])
{
	uint8_t ct[16], r[16];
	//get keys
	//stretch_key(key);
	for (size_t i = 0; i < 16; i++) {
		ct[i] = block[i];
	}
	//ecoding process:
	for (size_t i = 0; i < 9; i++) {//9 rounds
		for (size_t k = 0; k < 16; k++) {
			//xor with current round key
			ct[k] = ct[k] ^ round_keys[i][k];
		}
		for (size_t k = 0; k < 16; k++) {
			//prepare for lookup
			r[k] = ls_enc_lookup[0][ct[0]][k];
		}
		for (size_t j = 1; j <= 15; j++) {
			//15 values from lookup table to XOR
			//calc XOR with lookup table elements
			//Each element corresponds to particular
			//value of byte at current block position ct[j]
			for (size_t k = 0; k < 16; k++) {
				r[k] = r[k] ^ ls_enc_lookup[j][ct[j]][k];
			}
		}
		for (size_t m = 0; m < 16; m++) {
			ct[m] = r[m];
		}
	}
	for (size_t m = 0; m < 16; m++) {
		//XOR with last round key
		ct[m] = ct[m] ^ round_keys[9][m];
		ciphertext[m] = ct[m];
	}
}

void Kuznyechik::decode_block(uint8_t key[32], uint8_t block[16], uint8_t result[16])
{
	uint8_t pt[16], r[16];
	//stretch_key(key);
	for (size_t i = 0; i < 16; i++) {
		pt[i] = block[i];
	}
	for (size_t i = 9; i > 0; i--) {
		for (int k = 0; k < 16; k++) {
			pt[k] = pt[k] ^ round_keys[i][k];
		}
		for (size_t k = 0; k < 16; k++) {
			r[k] = l_inv_lookup[0][pt[0]][k];
		}
		for (size_t j = 1; j <= 15; j++) {
			for (size_t k = 0; k < 16; k++) {
				r[k] = r[k] ^ l_inv_lookup[j][pt[j]][k];
			}
		}
		for (size_t k = 0; k < 16; k++) {
			pt[k] = r[k];
		}
		for (size_t k = 0; k < 16; k++) {
			pt[k] = inverse_p_table[pt[k]];
		}
	}
	for (size_t k = 0; k < 16; k++) {
		pt[k] = pt[k] ^ round_keys[0][k];
	}
	for (size_t k = 0; k < 16; k++) {
		result[k] = pt[k];
	}
}
