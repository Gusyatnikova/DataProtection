#pragma once
#include <random>
#include <time.h>
#include "my_math.h"

class client_Diffie_Hellman
{
public:
	client_Diffie_Hellman(int p, int g);
	~client_Diffie_Hellman();
	int get_pub_key() const;
	void set_comp_key(int other_key);
	int show_secret();
private:
	int p_pub_;
	int g_pub_;
	int a_priv_;
	int key_secret;
	int gen_priv_key();
	int comp_pub_key() const; 
	int comp_secret_key(int other_key);

};

