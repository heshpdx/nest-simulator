/*
 *  numerics.h
 *
 *  This file is part of NEST.
 *
 *  Copyright (C) 2004 The NEST Initiative
 *
 *  NEST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  NEST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NEST.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef NUMERICS_H
#define NUMERICS_H

// Generated includes:
#include "config.h"

// C++ includes:
#include <cassert>
#include <cmath>
#include <limits>

#if HAVE_EXPM1
#include <math.h>
#endif

#if defined( HAVE_STD_ISNAN )
#include <cmath>
#elif defined( HAVE_ISNAN )
#include <math.h>
#endif

namespace numerics
{

extern const double e;
extern const double pi;
extern const double nan;

/** Supply expm1() function independent of system.
 *  @note Implemented inline for efficiency.
 */
inline double
expm1( double x )
{
#if HAVE_EXPM1
  return ::expm1( x ); // use library implementation if available
#else
  // compute using Taylor series, see GSL
  // e^x-1 = x + x^2/2! + x^3/3! + ...
  if ( x == 0 )
  {
    return 0;
  }
  if ( std::abs( x ) > std::log( 2.0 ) )
  {
    return std::exp( x ) - 1;
  }
  else
  {
    double sum = x;
    double term = x * x / 2;
    long n = 2;

    while ( std::abs( term ) > std::abs( sum ) * std::numeric_limits< double >::epsilon() )
    {
      sum += term;
      ++n;
      term *= x / n;
    }

    return sum;
  }
#endif
}

template < typename T >
bool
is_nan( T f )
{
#if defined( HAVE_STD_ISNAN )
  return std::isnan( f );
#elif defined( HAVE_ISNAN )
  return isnan( f );
#else
  assert( false ); // HAVE_STD_ISNAN or HAVE_ISNAN is required
  return false;
#endif
}
}


// later also in namespace
/**
 * Round to nearest int, rounding midpoints upwards.
 *
 * @return Result as long
 * @note [-1/2, 1/2) -> 0 and in general [ (2n-1)/2, (2n+1)/2 ) -> n
 * @see dround
 */
long ld_round( double );

/**
 * Round to nearest int, rounding midpoints upwards.
 *
 * @return Result as double
 * @note [-1/2, 1/2) -> 0 and in general [ (2n-1)/2, (2n+1)/2 ) -> n
 * @see ld_round
 */
double dround( double );

/**
 * Return integer part of argument.
 */
double dtruncate( double );

/**
 * Returns true if n is integer up to rounding error.
 * @return bool
 */
bool is_integer( double );

/**
 * Returns inverse of integer a modulo m
 */
long mod_inverse( long a, long m );

/**
 * Returns first matching index.
 *
 * Consider
 * - a container of arbitrary length containing elements (e.g. GIDs) which map to certain values,
 *     e.g., VP(GID), with a given *period*, e.g., the number of virtual processes
 * - that the phase (e.g., the VP number) of the first entry in the container is *phase0*
 * - that we slice the container with a given *step*
 * - that we want to know the index of the first element in the container with a given *phase*
 *
 * This function returns that index. It is the responsibility of the caller to check if the returned index
 * is within the bounds of the actual container—the algorithm assumes an infinite container.
 *
 * @note The function returns *invalid_index* if no solution exists.
 *
 * idx 0 1 2 3 4 5 6 7 8 9  10 11 | 12 13 14 15 16 17 18
 * gid 1 2 3 4 5 6 7 8 9 10 11 12 | 13 14 15 16 17 18 19
 * vp  1 2 3 0 1 2 3 0 1 2  3  0  | 1  2  3  0  1  2  3
 *    *     *     *     *        | *        *        *
 *    1     0     3     2        |
 *
 *  Asterisks mark first indices for the phases given below them, | indicates repetition starts
 *
 */
long first_index( long period, long phase0, long step, long phase );

#endif
