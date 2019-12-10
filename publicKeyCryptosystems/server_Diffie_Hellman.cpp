#include "server_Diffie_Hellman.h"



server_Diffie_Hellman::server_Diffie_Hellman(int p_pub, int g_pub)
	: p_pub_(p_pub), g_pub_(g_pub) {
	b_priv_ = gen_priv_key();
}


server_Diffie_Hellman::~server_Diffie_Hellman()
{
}

int server_Diffie_Hellman::get_pub_key()
{
	return comp_pub_key();
}

void server_Diffie_Hellman::set_comp_key(int other_key)
{
	comp_secret_key(other_key);
}

int server_Diffie_Hellman::show_secret()
{
	return key_secret;
}

int server_Diffie_Hellman::gen_priv_key()
{
	srand(time(0));
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(-100, 100);
	return distribution(generator);
}

int server_Diffie_Hellman::comp_pub_key()
{
	return my_math::modularization<int>(g_pub_, b_priv_, p_pub_);
}

int server_Diffie_Hellman::comp_secret_key(int other_key)
{
	key_secret = my_math::modularization(other_key, b_priv_, p_pub_);
	return my_math::modularization(other_key, b_priv_, p_pub_);
}
