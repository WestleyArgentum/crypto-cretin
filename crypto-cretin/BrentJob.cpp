// Westley Hennigh
// BrentJob.cpp : This is implementation for a thread that runs BrentFactorization

#include "BrentJob.hpp"


//static unsigned BRENT_COUNT = 0;
//Mutex Mutex_Brent_Count;

BrentJobManager* BrentManager = NULL;
gmp_randstate_t gRandomState;


BrentJobManager::~BrentJobManager ()
{
    Lock lock( mutex_jobs );

    for ( unsigned i = 0; i < jobs.size(); ++i )
        jobs[i]->Kill();

    //^! leaking memory caused by dynamic allocation of BrentJobs!
    //   I need to wait for the jobs to all finish before returning.
}


void BrentJobManager::RunBrentFactorization( unsigned num_iterations /*= 4 */ )
{ 
    //Mutex mutex_mpz_random;

    Lock lock ( mutex_jobs );

    for ( unsigned i = 0; i < num_iterations; ++i )
        jobs.push_back( new BrentJob( N, jobs.size(), &ResultBrentFactorization ) );

    for ( unsigned i = 0; i < num_iterations; ++i )
        jobs[i]->Wake();

}


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


void BrentJob::InitThread()
{}


void BrentJob::Run()
{
    result = BrentFactorization( N );

    // report that we are finished
    if (callback_fn)
        callback_fn( id );
}


void BrentJob::FlushThread()
{}


void ResultBrentFactorization ( unsigned id )
{
    {
        Lock ( BrentManager->mutex_jobs );

        if ( BrentManager->jobs[id]->result != BrentManager->N )
        {
            // call decryption function here
            std::cout << BrentManager->jobs[id]->result << std::endl;
            std::cout.flush();
            return;
        }
    }

    BrentManager->RunBrentFactorization(1);  // add a new job
}
