
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
    BrentJobManager ( mpz_class N_ ) : N(N_)
    {}

    ~BrentJobManager ();

    void RunBrentFactorization( unsigned num_iterations = 4 );

private:
    friend void ResultBrentFactorization ( unsigned id );

    std::vector<BrentJob*> jobs;
    Mutex mutex_jobs;
    mpz_class N;

    
};

extern BrentJobManager* BrentManager;


// Factorization --------------------------------------
extern gmp_randstate_t gRandomState;

mpz_class BrentFactorization(const mpz_class &N);


// Thread -----------------------------------------------
class BrentJob : public SD::ActiveObject
{
public:
    BrentJob ( mpz_class N_, unsigned id, SD::ActiveObject::StaticFunction callback_fn )
        : ActiveObject(id, callback_fn), N( N_ )
    {}

    virtual ~BrentJob ()
    {}

protected:
    virtual void InitThread  ();
    virtual void Run         ();
    virtual void FlushThread ();

private:
    mpz_class N;

public:
    mpz_class result;

};


// Callback for factorization ----------------------------------
void ResultBrentFactorization ( unsigned id );