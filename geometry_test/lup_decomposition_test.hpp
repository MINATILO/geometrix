//
//! Copyright ? 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_LUP_DECOMPOSITION_TEST_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_LUP_DECOMPOSITION_TEST_HPP

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/cstdint.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>

#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/algebra/lup_decomposition.hpp>

#include "tuple_kernal.hpp"
#include "vector_kernal.hpp"

#if defined(GEOMETRIX_RUN_TIMINGS)
    #include "scope_timer.hpp"
#endif

template <typename T, typename T2, std::size_t N>
inline void lup_decomposition_rt( geometrix::matrix<T, N, N>& m, boost::array<T2, N>& pi )
{
    //! Initialize PI as zeros.
    for( std::size_t i = 0; i < N; ++i )
        pi[i] = i;
    for( std::size_t k = 0; k < N - 1; ++k )
    {
        T p = 0;
        std::size_t k_ = 0;
        for( std::size_t i = k; i < N; ++i )
        {
            if( std::abs(m[i][k]) > p )
            {
                p = std::abs(m[i][k]);
                k_ = i;
            }
        }

        if( p == 0 )
            throw std::logic_error( "cannot lup decompose a singular matrix." );
        std::swap( pi[k], pi[k_] );
        for( std::size_t i = 0; i < N; ++i )
            std::swap( m[k][i], m[k_][i] );
        for( std::size_t i = k + 1; i < N; ++i )
        {
            m[i][k] /= m[k][k];
            for( std::size_t j = k + 1; j < N; ++j )
                m[i][j] -= m[i][k]*m[k][j];
        }
    }
}

template <typename T, typename T2, std::size_t N>
inline boost::array<T, N> lup_solve_rt( geometrix::matrix<T, N, N>& lu, const boost::array<T2,N>& pi, const boost::array<T,N>& b )
{
    boost::array<T, N> x, y;
    for( std::size_t i = 0; i < N; ++i )
    {
        T sum = 0;
        for( std::size_t j = 0; j < i; ++j )
            sum += lu[i][j] * y[j];
        y[i] = b[pi[i]] - sum;
    }
    for( std::size_t i = N-1; true; --i )
    {
        T sum = 0;
        for( std::size_t j = i+1; j < N; ++j )
            sum += lu[i][j] * x[j];
        x[i] = ( y[i] - sum ) / lu[i][i];

        if( i == 0 )
            break;
    }

    return x;
}

BOOST_AUTO_TEST_CASE( TestLUPDecomposition )
{
    using namespace geometrix;
    
        
    matrix<double,4,4> m = {{ {2, 0, 2, .6},
                              {3, 3, 4, -2},
                              {5, 5, 4, 2},
                              {-1,-2,3.4,-1} }};
    boost::array<std::size_t, 4> p;

    matrix<double,4,4> m2 = {{ {2, 0, 2, .6}, {3, 3, 4, -2}, {5, 5, 4, 2}, {-1,-2,3.4,-1} }};
    boost::array<std::size_t, 4> p2;
    lup_decomposition( m, p );
    lup_decomposition_rt( m2, p2 );

    #if defined(GEOMETRIX_RUN_TIMINGS)
    boost::uint64_t runs = 1000000;
    {        
        MEASURE_SCOPE_TIME( "CompileTimeLUP" );        
        for( boost::uint64_t i = 0; i < runs; ++i )
        {
            lup_decomposition( m, p );
        }
    }
    {
        MEASURE_SCOPE_TIME( "RunTimeLUP" );
        for( boost::uint64_t i = 0; i < runs; ++i )
        {
            lup_decomposition_rt( m2, p2 );
        }
    }
    #endif

    BOOST_CHECK(p[0] == p2[0]);
    BOOST_CHECK(p[1] == p2[1]);
    BOOST_CHECK(p[2] == p2[2]);
    BOOST_CHECK(p[3] == p2[3]);
}

BOOST_AUTO_TEST_CASE( TestLUPSolver )
{
    using namespace geometrix;
    
        
    matrix<double,4,4> m = {{ {1, 2, 0, 5}, {3, 5, 4, 6}, {5, 6, 3, 7}, {8, 10, 9, 9} }};
    boost::array<double, 4> b = { { 0.1, 12.5, 10.3, 8. } };

    boost::array<std::size_t, 4> p;
    matrix<double,4,4> lu = m;
    lup_decomposition( lu, p );

    boost::array<double,4> x1 = lup_solve( lu, p, b );
    boost::array<double,4> x2 = lup_solve_rt( lu, p, b );
#if defined(GEOMETRIX_RUN_TIMINGS)
    boost::uint64_t runs = 100000;    
    {
        MEASURE_SCOPE_TIME( "CompileTimeLUPSolve" );
        for( boost::uint64_t i = 0; i < runs; ++i )
        {
            boost::array<double,4> x = lup_solve( lu, p, b );
        }
    }
    {
        MEASURE_SCOPE_TIME( "RunTimeLUPSolve" );
        for( boost::uint64_t i = 0; i < runs; ++i )
        {
            boost::array<double,4> x = lup_solve_rt( lu, p, b );
        }
    }
#endif
    
    BOOST_CHECK(x1[0] == x2[0]);
    BOOST_CHECK(x1[1] == x2[1]);
    BOOST_CHECK(x1[2] == x2[2]);
    BOOST_CHECK(x1[3] == x2[3]);
}

#endif //GEOMETRIX_LINEAR_ALGEBRA_LUP_DECOMPOSITION_TEST_HPP
