//
//! Copyright � 2013
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_POINT_ARITHMETIC_TESTS_HPP
#define GEOMETRIX_VECTOR_POINT_ARITHMETIC_TESTS_HPP

#include "geometrix_test.hpp"
#include "vector_kernal.hpp"

#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/tensor/fusion_matrix.hpp>

BOOST_AUTO_TEST_CASE( TestPointVector3DArithmetic )
{
    using namespace geometrix;
    using namespace geometrix::algebra;

    vector<double, 3> v1(10., 20., 30.), v2;
    point<double,3> p1(1., 2., 3.), p2;
    
    //! Add vector to point.
    p2 = p1 + v1;
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_3D(p2, (11., 22., 33.), 1e-10);

    //! Subtract vector from point.
    p2 = p2 - v1;
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_3D(p2, (1., 2., 3.), 1e-10);

    //! Add point to point (fails to compile)
    //p2 <<= p1 + p1;

    //! Subtract a point from a point
    v2 = p2 - p1;
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_3D(v2, (0., 0., 0.), 1e-10);

    //! Check scalar multiplication
    v2 = 10.0 * as_vector(p1);
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_3D(v2, (10., 20., 30.), 1e-10);
    
    //! Check scalar division
    v2 = v2 / 10.0;
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_3D(v2, (1., 2., 3.), 1e-10);

    //! Test intersection calculation
    double t = 0.5;
    point<double,3> gp = p2 + t * (p1-p2);
}

BOOST_AUTO_TEST_CASE( TestPointVector2DArithmetic )
{
    using namespace geometrix;
    using namespace geometrix::algebra;

    vector<double,2> v1(10., 20.), v2;
    point<double,2> p1(1., 2.), p2;

    //! Add vector to point.
    p2 = p1 + v1;
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_2D(p2, (11., 22.), 1e-10);

    //! Subtract vector from point.
    p2 = p2 - v1;
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_2D(p2, (1., 2.), 1e-10);

    //! Add point to point (fails to compile)
    //p2 <<= p1 + p1;

    //! Subtract a point from a point
    v2 = p2 - p1;
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_2D(v2, (0., 0.), 1e-10);

    //! Check scalar multiplication
    v2 = 10.0 * as_vector(p1);
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_2D(v2, (10., 20.), 1e-10);

    //! Check scalar division
    v2 = v2 / 10.0;
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_2D(v2, (1., 2.), 1e-10);

    //! Test intersection calculation
    double t = 0.5;
    point<double,2> gp;
    gp <<= p2 + t * (p1-p2);
}

template <typename Vector>
void TestVectorGeneralRotation()
{
    using namespace geometrix;
    using namespace geometrix::result_of;
    using namespace geometrix::algebra;

    BOOST_CONCEPT_ASSERT((Vector3DConcept<Vector>));
    Vector u(0., 0., 1.);
    Vector v(1., 0., 0.);
    Vector rv;
       
    //! Calculate v rotated around an axis represented by unit vector u by angle theta. 

    //! Rotate around pi/2
    double theta = constants<double>::pi()/2.;
    rv = std::cos(theta)*(v-(u*v)*u) + std::sin(theta)*(u^v) + (u*v)*u;
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_3D(rv, (0., 1., 0.), 1e-10);
    
    //! Rotate around pi
    theta = constants<double>::pi();
    rv = std::cos(theta)*(v-(u*v)*u) + std::sin(theta)*(u^v) + (u*v)*u;
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_3D(rv, (-1., 0., 0.), 1e-10);
    
    //! Rotate around 3*pi/2
    theta = 3.* constants<double>::pi()/2.;
    rv = std::cos(theta)*(v-(u*v)*u) + std::sin(theta)*(u^v) + (u*v)*u;    
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_3D(rv, (0., -1., 0.), 1e-10);

    //! Set u to a unit vector pointing towards (1.,1.,1.).
    u = norm(Vector(1., 1., 1.));

    //! Rotate around pi/2
    theta = constants<double>::pi()/2.0;
    rv = std::cos(theta)*(v-(u*v)*u) + std::sin(theta)*(u^v) + (u*v)*u;
    GEOMETRIX_CHECK_SEQUENCE_CLOSE_3D(rv, (0.3333333333, 0.9106836025, -0.2440169358), 1e-10);
}

BOOST_AUTO_TEST_CASE( TestPointVectorGeneralRotation )
{
    TestVectorGeneralRotation<vector_vector_3>();
}

#endif //GEOMETRIX_VECTOR_POINT_ARITHMETIC_TESTS_HPP
