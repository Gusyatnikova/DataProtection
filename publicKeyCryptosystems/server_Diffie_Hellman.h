#pragma once
#include <time.h>
#include <random>

#include "my_math.h"
class server_Diffie_Hellman
{
public:
	server_Diffie_Hellman(int p, int g);
	~server_Diffie_Hellman();
	int get_pub_key();
	void set_comp_key(int other_key);
	int show_secret();
private:
	int p_pub_;
	int g_pub_;
	int b_priv_;
	int key_secret;
	int gen_priv_key();
	int comp_pub_key();
	int comp_secret_key(int other_key);
};

