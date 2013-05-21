//
//! Copyright � 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_PRODUCT_TESTS_HPP
#define GEOMETRIX_PRODUCT_TESTS_HPP

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/arithmetic/vector_arithmetic.hpp>
#include <geometrix/algebra/algebra.hpp>

BOOST_AUTO_TEST_CASE( TestProducts )
{
    using namespace geometrix;
    using namespace geometrix::algebra;

    typedef point_double_2d point_2d;
    typedef point_double_3d point_3d;
    
    point_2d a( 1., 0. );
    point_2d b( 0., 1. );

    double dot = geometrix::dot_product( as_vector(a), as_vector(b) );
    BOOST_CHECK_CLOSE( dot, 0., 1e-10 );

    dot = geometrix::dot_product( as_vector(a), as_vector(a) );
    BOOST_CHECK_CLOSE( dot, 1., 1e-10 );

    double cross = exterior_product_area( as_vector(a), as_vector(b) );
    BOOST_CHECK( cross > 0. );

    cross = exterior_product_area( as_vector(b), as_vector(a) );
    BOOST_CHECK( cross < 0. );

    point_2d zero( 0., 0. );
    cross = exterior_product_area( a-zero, b-zero );
    BOOST_CHECK( cross > 0. );

    cross = exterior_product_area( b-zero, a-zero );
    BOOST_CHECK( cross < 0. );

    point_3d a3( 1., 0., 0. );
    point_3d b3( 0., 1., 0. );
    cross = exterior_product_area( as_vector(a3), as_vector(b3) );
    BOOST_CHECK( cross > 0. );

    cross = exterior_product_area( as_vector(b3), as_vector(a3) );
    BOOST_CHECK( cross < 0. );

    point_3d zero3( 0., 0., 0. );
    cross = exterior_product_area( a3-zero3, b3-zero3 );
    BOOST_CHECK( cross > 0. );

    cross = exterior_product_area( b3-zero3, a3-zero3 );
    BOOST_CHECK( cross < 0. );

    vector_double_3d vp1( 2., 0., 0. );
    vector_double_3d vp2( 0., 2., 0. );
    vector_double_3d vp3( 0., 0., 2. );
    cross = exterior_product_volume( vp1, vp2, vp3 );
    BOOST_CHECK_CLOSE( cross, 8.0, 1e-10 );

    typedef vector_double_2d vector_2;
    typedef vector_double_3d vector_3;
    fraction_tolerance_comparison_policy<double> compare(1e-10);

    vector_2 v1( 0.0, 1.0 );
    vector_2 v2( 1.0, 0.0 );
    dot = geometrix::dot_product( v1, v2 );
    BOOST_CHECK( compare.equals( dot, 0.0 ) );

    vector_2 v3( 2.0, 2.0 );    
    vector_2 v4( 1.0, 1.0 );
    dot <<= v3 * v4;
    BOOST_CHECK( compare.equals( dot, 4.0 ) );

    v3 = normalize( v3 );
    v4 = normalize( v4 );
}

#endif //GEOMETRIX_PRODUCT_TESTS_HPP



