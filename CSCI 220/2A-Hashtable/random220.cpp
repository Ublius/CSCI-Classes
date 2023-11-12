/*
 * random220.cpp
 * khellman@mines.edu, see random220.h for documentation.
 *
*/

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <chrono>

#include "random220.h"

#define DEFAULT    123456789L  /* initial seed, use 0 < DEFAULT < MODULUS   */
static long seed = DEFAULT;    /* seed is the state of the generator        */

static const long  MODULUS=    2147483647L;	/* DON'T CHANGE THIS VALUE                   */
static double prng()
{
	/* ---------------------------------------------------------------------
	 * Random is a Lehmer generator that returns a pseudo-random real number
	 * uniformly distributed between 0.0 and 1.0.  The period is (m - 1)
	 * where m = 2,147,483,647 amd the smallest and largest possible values
	 * are (1 / m) and 1 - (1 / m) respectively.                             
	 * ---------------------------------------------------------------------   
	 * From Discrete Event Simulation - A first course
	 * Leemis & Park
	 */
	const long  MULTIPLIER=48271L;     /* use 16807 for the "minimal standard"      */

  	const long Q = MODULUS / MULTIPLIER;
  	const long R = MODULUS % MULTIPLIER;
          long t;

	t = MULTIPLIER * (seed % Q) - R * (seed / Q);
	if (t > 0) 
		seed = t;
	else 
		seed = t + MODULUS;
	return ((double) seed / MODULUS);
}

long seed_now()
{
	using std::chrono::system_clock;
	seed = system_clock::to_time_t( system_clock::now() ) % MODULUS; 
	std::cerr << "seed_now " << seed << std::endl; 
	return seed;
}

long seed_prng( long s )
{
	seed = abs(s) % MODULUS; 
	std::cerr << "seed_prng " << seed << std::endl; 
	return seed;
}



bool bernoulli( const double p )
{
	// compilicated stuff, eh? yes, apparently so.
	assert( p>=0 );
	return prng() < p;
}

bool coinflip( ) { return bernoulli(0.5); }

double uniform( const double a, const double b )
{
	assert( a<=b );
	return a + prng()*(b-a);
}

long equalikely( const long a, const long b )
{
	assert( a<=b );
	return ::floor( uniform(a,b+1));
}

template<typename T>
void fisher_yates( std::vector<T>& v, size_t count )
{
	// fisher yates 
	for( size_t i=0; i<v.size()-1 && count!=0; ++i ) {
		T hold(v[i]);
		size_t to = equalikely(i,v.size()-1);
		v[i] = v[to];
		v[to] = hold;
		--count;
	}
}
