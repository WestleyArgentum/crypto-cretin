
#include "encrypt.hpp"

gmp_randstate_t gRandomState;

mpz_class BrentFactorization(const mpz_class &N)
{
  if(N%2 == 0)
    return mpz_class(2);

  mpz_class y = 0,
            ys = 0,
            c = 0,
            m = 0;
  mpz_urandomm(y.get_mpz_t(), gRandomState, N.get_mpz_t());
  mpz_urandomm(c.get_mpz_t(), gRandomState, N.get_mpz_t());
  mpz_urandomm(m.get_mpz_t(), gRandomState, N.get_mpz_t());

  mpz_class g = 1,
            r = 1,
            q = 1,
            lMin = N + 1, //bad (impossible) minimum value to start off with
            lAbs, x, k;

  while(g == 1)
  {
    x = y;

    for(mpz_class i = 0; i < r; ++i)
    {
      y = ((y*y)%N + c)%N;
    }

    k = 0;

    while((k < r) && (g == 1))
    {
      ys = y;

      lMin = m < r-k ? m : r-k;
      for(mpz_class i = 0; i < lMin; ++i)
      {
        y = ((y*y)%N + c)%N;

        mpz_abs(lAbs.get_mpz_t(), mpz_class(x-y).get_mpz_t());
        q = (q*lAbs)%N;
      }

      mpz_gcd(g.get_mpz_t(), q.get_mpz_t(), N.get_mpz_t());
      k = k + m;
    }

    r = r*2;
  }

  if(g==N)
  {
    while(true)
    {
      ys = ((ys*ys)%N + c)%N;

      mpz_abs(lAbs.get_mpz_t(), mpz_class(x-ys).get_mpz_t());
      mpz_gcd(g.get_mpz_t(), lAbs.get_mpz_t(), N.get_mpz_t());

      if(g > 1)
        break;
    }
  }

  return g;
}


int CALLBACK WinMain(HINSTANCE iInstance, HINSTANCE, LPSTR iCommandLine, int iDisplayParameters)
{
  //Randomness initialization:
  gmp_randinit_default(gRandomState);
  CreateConsole();

  std::cout << "Brent: " << BrentFactorization(mpz_class("77")) << std::endl;
  std::cout << "Brent: " << BrentFactorization(mpz_class("77")) << std::endl;
  std::cout << "Brent: " << BrentFactorization(mpz_class("77")) << std::endl;

  std::cin.get();

  //std::string N = "15641574130333";
  //std::string p = "3954889",
  //            q = "3954997";
  //CreateConsole();
  //printf( "Hello, world!" );

  //std::ifstream in("test.txt", std::ios::binary | std::ios::in);
  //std::ofstream out("test_enc.txt");

  //Encrypt(out, in, mpz_class(N).get_mpz_t(), mpz_class("5").get_mpz_t());

  //std::ifstream in2("test_enc.txt", std::ios::in);
  //std::ofstream out2("test_dec.txt", std::ios::binary | std::ios::out);

  //Decrypt(out2, in2, mpz_class(N).get_mpz_t(), mpz_class("5").get_mpz_t());

  //Deinit random:
  gmp_randclear(gRandomState);

  return 0;
}