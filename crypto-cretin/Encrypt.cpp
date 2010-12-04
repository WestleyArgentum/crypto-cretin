//=======================================================\\

/*!

      @file        Encrypt.cpp

      @author      Chris Schell
 
      @brief
          Quick implementation of RSA encryption and decryption.

	  Copyright © 2009 DigiPen (USA) Corporation, All Rights Reserved
    Reproduction or disclosure of this file or its contents without 
    the prior written consent of DigiPen Institute of Technology is prohibited.

*/

//========================================================\\

///@todo: Currently, if the last chuck of data is smaller that 40bits, when the file is decoded, 
///       0's are padded on the end, making the file slightly larger and different from the original
///       This needs to be fixed.

void Encrypt(std::ofstream& oStream, std::ifstream& iStream, const mpz_ptr N, const mpz_ptr e)
{
  //while not end of file
  while(!iStream.eof())
  {
    mpz_t message;
    mpz_init(message);

    //grab 40 bits from the input file
    char data[5] = {0,0,0,0,0};
    iStream.read(data, 5);

    //shove those 40 bits into mpz_t: message
    mpz_import(message, 5, 1, sizeof(char), 1, 0, data);

    //m = m^e(mod N)
    mpz_powm(message, message, e, N);

    //write out m
    oStream << message << std::endl;
  }
}

void Decrypt(std::ofstream& oStream, std::ifstream& iStream, const mpz_ptr N, const mpz_ptr e)
{
  ///@todo Actually factor N into p and q
  mpz_class p("3954889"),
            q("3954997");

  //get phi(N) = (p-1)(q-1)
  mpz_class phi = (p-1)*(q-1);

  //get d = multinv(e, phi(N))
  mpz_class d(0);
  mpz_invert(d.get_mpz_t(), e, phi.get_mpz_t());

  Assert(d * mpz_class(e) % phi == mpz_class(1), "e has no inverse. e and phi(N) are most likely not relatively prime.");

   //while not end of file
  while(!iStream.eof())
  {
    //grab line
    std::string message;
    iStream >> message;

    if(message.empty() || message[0] == '/n')
      return;

    //shove that string into mpz_t: m
    mpz_class m(message);

    //m = m^d(mod N)
    mpz_powm(m.get_mpz_t(), m.get_mpz_t(), d.get_mpz_t(), N);

    //shove m into 40 bit buffer
    char data[5] = {0,0,0,0,0};
    mpz_export(data, 0, 1, sizeof(data), 1, 0, m.get_mpz_t());

    //write out m binary
    oStream.write(data, 5);
  }

}
