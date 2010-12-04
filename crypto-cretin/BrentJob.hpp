
// BrentJob.hpp : This will run BrentFactorization on multiple threads.

#pragma once

#include "ActiveObject.hpp"
#include "Mutex.hpp"

#include <vector>


// "Manager" -----------------------------------
class BrentJob;

class BrentJobManager
{
public:
    BrentJobManager ()  {;}
    ~BrentJobManager ();

    void Init ( mpz_class N_ )
    {
      N = N_;

      Lock lock ( mutex_result );
      result = N;
    }

    void RunBrentFactorization( unsigned num_iterations = 4 );

    mpz_class GetResult ();

private:
    friend void ResultBrentFactorization ( unsigned id );

    std::vector<BrentJob*> jobs;
    std::vector<HANDLE> handles;
    Mutex mutex_jobs;
    mpz_class N;

    mpz_class result;
    Mutex mutex_result;

    
};

extern BrentJobManager BrentManager;


// Factorization --------------------------------------
extern gmp_randstate_t gRandomState;
extern Mutex mutex_gRandom;

mpz_class BrentFactorization(const mpz_class &N);


// Thread -----------------------------------------------
class BrentJob : public SD::ActiveObject
{
public:
    BrentJob ( mpz_class N_, mpz_class& result_, Mutex& mutex_result_, unsigned id, SD::ActiveObject::StaticFunction callback_fn )
        : ActiveObject(id, callback_fn), N( N_ ), result( result_ ), mutex_result( mutex_result_ )
    {}

    virtual ~BrentJob ()
    {}

protected:
    virtual void InitThread  ();
    virtual void Run         ();
    virtual void FlushThread ();

private:
    mpz_class N;
    mpz_class& result;
    Mutex& mutex_result;

};


// Callback for factorization ----------------------------------
void ResultBrentFactorization ( unsigned id );