my_math.h contains an implementation of the fast modularization method
Client_Diffie_Hellman.h(cpp), Server_Diffie_Hellman.h(cpp) containt an implementation of Diffie-Hellman protocol
ElGamal.h(cpp) containt an implementation of Diffie-Hellman cipher

in main:
1. comparing pow(a, b)mod p in a conventional and fast way
2. server and client genereta keys using Diffie_Hellman protocol
3. cipher message in in.txt using Elgamal algorythm and put to out_crypt.txt. 
   Then decrypt out_crypt.txt to out_decrypt.txt
