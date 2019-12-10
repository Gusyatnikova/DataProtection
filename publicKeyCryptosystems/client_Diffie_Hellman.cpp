#include "client_Diffie_Hellman.h"



client_Diffie_Hellman::client_Diffie_Hellman(int p_pub, int g_pub)
//Нормально ли, что я вызываю метод? у чего? объекта еще нет, он не статический....
	: p_pub_(p_pub), g_pub_(g_pub), a_priv_(gen_priv_key()) {}


client_Diffie_Hellman::~client_Diffie_Hellman()
{
}

int client_Diffie_Hellman::get_pub_key() const
{
	return comp_pub_key();
}

void client_Diffie_Hellman::set_comp_key(int other_key)
{
	comp_secret_key(other_key);
}

int client_Diffie_Hellman::show_secret()
{
	return key_secret;
}

int client_Diffie_Hellman::gen_priv_key()
{
	srand(time(0));
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(-100, 100);
	return distribution(generator);
}

int client_Diffie_Hellman::comp_pub_key() const
{
	return my_math::modularization<int>(g_pub_, a_priv_, p_pub_);
}

int client_Diffie_Hellman::comp_secret_key(int other_key)
{
	key_secret = my_math::modularization(other_key, a_priv_, p_pub_);
	return my_math::modularization(other_key, a_priv_, p_pub_);
}
