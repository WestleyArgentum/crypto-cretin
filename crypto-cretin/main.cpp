
#include "encrypt.hpp"

int CALLBACK WinMain(HINSTANCE iInstance, HINSTANCE, LPSTR iCommandLine, int iDisplayParameters)
{
  std::string N = "15641574130333";
  std::string p = "3954889", 
              q = "3954997";
  CreateConsole();
  printf( "Hello, world!" );

  std::ifstream in("test.txt", std::ios::binary | std::ios::in);
  std::ofstream out("test_enc.txt");

  Encrypt(out, in, mpz_class(N).get_mpz_t(), mpz_class("5").get_mpz_t());

  std::ifstream in2("test_enc.txt", std::ios::in);
  std::ofstream out2("test_dec.txt", std::ios::binary | std::ios::out);

  Decrypt(out2, in2, mpz_class(N).get_mpz_t(), mpz_class("5").get_mpz_t());
  return 0;
}