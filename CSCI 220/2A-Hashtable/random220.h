/*
 * random220.h
 * CSCI220 random number untils, only slightly informed by the corpus of
 * computer simulation.
 * khellman@mines.edu
 *
 * The underlying routines uses a statistically sound, albeit small, psuedo
 * random number generator well accepted in the simulation community.  It is 
 * small, fast and sufficient for our needs.  The pRNG can be seeded with
 * seed_prng( seed ) or seed_now() both provided by this module.
 */

#ifndef RANDOM220_H
#define RANDOM220_H

#include <vector>

/***
 * a Bernoulli random variable.  returns true with probability 0<= p <=1
 */
bool bernoulli( const double p );

/***
 * a Uniform random variable, returns values within (a,b) with equal probability
 */
double uniform( const double a, const double b );

/***
 * an Equalikely random variable, returns integer within (a,b) with equal probability
 */
long equalikely( const long a, const long b );

/***
 * a fair coin flip
 */
bool coinflip( );

/***
 * basic Fisher Yates in-place shuffle of a vector, the optional count parameter
 * can specify a partial shuffling
 */
template<typename T>
void fisher_yates( std::vector<T>& v, size_t count = 0 );


/***
 * seeds the generator from the computer's wall clock time, prints the seed to
 * cerr, and returns the value used.
 */
long seed_now();

/***
 * seeds the generator from |s|, prints the seed to cerr, and returns the value
 * used.
 */
long seed_prng( long s );

#endif
