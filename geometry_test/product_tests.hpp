//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_PRODUCT_TESTS_HPP
#define _BOOST_GEOMETRY_PRODUCT_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/products.hpp"

BOOST_AUTO_TEST_CASE( TestProducts )
{
	using namespace boost::numeric::geometry;

	typedef point_double_2d CPoint2D;
	typedef point_double_3d CPoint3D;
	
	CPoint2D a( 1., 0. );
    CPoint2D b( 0., 1. );

    double dot = dot_product( a, b );
    BOOST_CHECK_CLOSE( dot, 0., 1e-10 );

    dot = dot_product( a, a );
    BOOST_CHECK_CLOSE( dot, 1., 1e-10 );

    double cross = cross_product( a, b );
    BOOST_CHECK( cross > 0. );

    cross = cross_product( b, a );
    BOOST_CHECK( cross < 0. );

    CPoint2D zero( 0., 0. );
    cross = cross_product( zero, a, b );
    BOOST_CHECK( cross > 0. );

    cross = cross_product( zero, b, a );
    BOOST_CHECK( cross < 0. );

    CPoint3D a3( 1., 0., 0. );
    CPoint3D b3( 0., 1., 0. );
    cross = cross_product( a3, b3 );
    BOOST_CHECK( cross > 0. );

    cross = cross_product( b3, a3 );
    BOOST_CHECK( cross < 0. );

    CPoint3D zero3( 0., 0., 0. );
    cross = cross_product( zero3, a3, b3 );
    BOOST_CHECK( cross > 0. );

    cross = cross_product( zero3, b3, a3 );
    BOOST_CHECK( cross < 0. );
    
}

#endif //_BOOST_GEOMETRY_PRODUCT_TESTS_HPP



