// Westley Hennigh
// BrentJob.cpp : This is implementation for a thread that runs BrentFactorization

#include "BrentJob.hpp"


BrentJobManager BrentManager;

gmp_randstate_t gRandomState;
Mutex mutex_gRandom;


BrentJobManager::~BrentJobManager ()
{
  Lock lock( mutex_jobs );

  for ( unsigned i = 0; i < jobs.size(); ++i )
  {
    jobs[i]->Kill();
    delete jobs[i];
  }
}


void BrentJobManager::RunBrentFactorization( unsigned num_iterations /*= 4 */ )
{ 
  //Mutex mutex_mpz_random;

  Lock lock ( mutex_jobs );

  for ( unsigned i = 0; i < num_iterations; ++i )
  {
    jobs.push_back( new BrentJob( N, result, mutex_result, jobs.size(), &ResultBrentFactorization ) );
    jobs.back()->Wake();
    handles.push_back( jobs.back()->GetHandle() );
  }
    

}


mpz_class BrentJobManager::GetResult()
{
  WaitForMultipleObjects( handles.size(), &handles[0], true, INFINITE );

  //Lock lock (mutex_result);  // really this is just for kicks
  return result;
}


mpz_class BrentFactorization(const mpz_class &N)
{
  if(N%2 == 0)
    return mpz_class(2);

  mpz_class y = 0,
    ys = 0,
    c = 0,
    m = 0;

  {
    Lock lock( mutex_gRandom );
    mpz_urandomm(y.get_mpz_t(), gRandomState, N.get_mpz_t());
    mpz_urandomm(c.get_mpz_t(), gRandomState, N.get_mpz_t());
    mpz_urandomm(m.get_mpz_t(), gRandomState, N.get_mpz_t());
  }

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


void BrentJob::InitThread()
{}


void BrentJob::Run()
{

  while ( !isDying )
  {
    // if no one has found a result, run again
    {
      Lock lock ( mutex_result );
      if ( result != N )
        break;
    }

    mpz_class my_result = BrentFactorization( N );
    if ( my_result != N )
    {
      Lock lock ( mutex_result );
      result = my_result;
    }
  }

}


void BrentJob::FlushThread()
{}


void ResultBrentFactorization ( unsigned id )
{
  {
    Lock ( BrentManager.mutex_jobs );

    //mpz_class result = BrentManager.jobs[id]->result;

    delete BrentManager.jobs[id];
    BrentManager.jobs.erase( BrentManager.jobs.begin() + id );

    //if ( result != BrentManager.N )
    //{
    //  // call decryption function here
    //  std::cout << result << std::endl;
    //  std::cout.flush();
    //  return;
    //}
  }


  //BrentManager.RunBrentFactorization(1);  // add a new job
}
