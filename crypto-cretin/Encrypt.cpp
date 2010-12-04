//=======================================================\\

/*!

      @file        Encrypt.cpp

      @author      Chris Schell
 
      @brief
          Implementation of RSA encryption and decryption.

	  Copyright © 2009 DigiPen (USA) Corporation, All Rights Reserved
    Reproduction or disclosure of this file or its contents without 
    the prior written consent of DigiPen Institute of Technology is prohibited.

*/

//========================================================\\

#include "BrentJob.hpp"


unsigned GetFilesize(std::ifstream& in)
{
  in.seekg(0, std::ios::end);
  unsigned size = in.tellg();
  in.seekg(0);
  return size;
}

void Encrypt(std::ofstream& out, std::ifstream& in, const mpz_ptr N, const mpz_ptr e, unsigned ChunkSize)
{
  unsigned long Size = GetFilesize(in);

  mpz_t message;
  mpz_init(message);

  char* data = (char*)::operator new(ChunkSize);

  out << "N "         << N          << std::endl
      << "E "         << e          << std::endl
      << "ChunkSize " << ChunkSize  << std::endl
      << "Size "      << Size;

  //while not end of file
  while(!in.eof())
  {
    out << std::endl;

    //grab 40 bits from the input file
    memset(data, 0, ChunkSize);
    in.read(data, ChunkSize);

    //shove those 40 bits into mpz_t: message
    mpz_import(message, ChunkSize, 1, sizeof(char), 1, 0, data);

    //m = m^e(mod N)
    mpz_powm(message, message, e, N);

    //write out m
    out << message;
  }

  mpz_clear(message);
  delete data;
}

void Decrypt(std::ofstream& out, std::ifstream& in)
{
  std::string temp;
  mpz_class N;
  mpz_class e;
  unsigned ChunkSize;
  unsigned long Size;

  in >> temp >> N
     >> temp >> e
     >> temp >> ChunkSize
     >> temp >> Size;

  BrentManager.Init( mpz_class(N) );
  BrentManager.RunBrentFactorization( 10 );
  mpz_class p = BrentManager.GetResult();
  mpz_class q = N / p;


  //get phi(N) = (p-1)(q-1)
  mpz_class phi = (p-1)*(q-1);

  //get d = multinv(e, phi(N))
  mpz_class d(0);
  mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());

  Assert(d * mpz_class(e) % phi == mpz_class(1), "e has no inverse. e and phi(N) are most likely not relatively prime.");

  char* data = (char*)::operator new(ChunkSize);

   //while not end of file
  while(!in.eof())
  {
    //grab line
    std::string message;
    in >> message;

    if(message.empty() || message[0] == '/n')
      return;

    //shove that string into mpz_t: m
    mpz_class m(message);

    //m = m^d(mod N)
    mpz_powm(m.get_mpz_t(), m.get_mpz_t(), d.get_mpz_t(), N.get_mpz_t());

    //shove m into 40 bit buffer
    memset(data, 0, sizeof(char) * ChunkSize);
    mpz_export(data, 0, 1, ChunkSize, 1, 0, m.get_mpz_t());

    unsigned SizeToWrite = std::min(ChunkSize, unsigned(Size));

    //write out m binary
    out.write(data, SizeToWrite);

    Size -= ChunkSize;
  }

  delete data;
}
