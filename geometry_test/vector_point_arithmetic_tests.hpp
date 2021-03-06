//
//! Copyright ? 2013
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
#include <geometrix/tensor/is_null.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/arithmetic/vector/bisect.hpp>
#include <geometrix/arithmetic/vector/lerp.hpp>
#include <geometrix/tensor/fusion_matrix.hpp>


// template <typename Vector1, typename Vector2, typename Vector3, typename NumberComparisonPolicy>
// inline bool is_vector_inside( const Vector1& A, const Vector2& B, const Vector3& C, const NumberComparisonPolicy& cmp )
#include <geometrix/tensor/fusion_matrix.hpp>


// template <typename Vector1, typename Vector2, typename Vector3, typename NumberComparisonPolicy>
// inline bool is_vector_inside( const Vector1& A, const Vector2& B, const Vector3& C, const NumberComparisonPolicy& cmp )
// {
// 	BOOST_CONCEPT_ASSERT( (geometrix::Vector2DConcept<Vector1>) );
// 	BOOST_CONCEPT_ASSERT( (geometrix::Vector2DConcept<Vector2>) );
// 	BOOST_CONCEPT_ASSERT( (geometrix::Vector2DConcept<Vector3>) );
// 	double a; a <<= (A^B) * (A^C);
// 	double b; b <<= (C^B) * (C^A);
// 	return cmp.greater_than_or_equal( a, 0 ) && cmp.greater_than_or_equal( b, 0 );
// }

BOOST_AUTO_TEST_CASE( TestPointVector3DArithmetic )
{
	using namespace geometrix;


	vector<double, 3> v1( 10., 20., 30. ), v2;
	point<double, 3> p1( 1., 2., 3. ), p2;

	typedef vector<double, 3> vec3;
	absolute_tolerance_comparison_policy<double> cmp( 1e-10 );

	//! Add vector to point.
	p2 = p1 + v1;
	BOOST_CHECK( numeric_sequence_equals( p2, vec3( 11., 22., 33. ), cmp ) );

	//! Subtract vector from point.
	p2 = p2 - v1;
	BOOST_CHECK( numeric_sequence_equals( p2, vec3( 1., 2., 3. ), cmp ) );

	//! Add point to point (fails to compile)
	//p2 <<= p1 + p1;

	//! Subtract a point from a point
	v2 = p2 - p1;
	BOOST_CHECK( numeric_sequence_equals( v2, vec3( 0., 0., 0. ), cmp ) );

	//! Check scalar multiplication
	v2 = 10.0 * as_vector( p1 );
	BOOST_CHECK( numeric_sequence_equals( v2, vec3( 10., 20., 30. ), cmp ) );

	//! Check scalar division
	v2 = v2 / 10.0;
	BOOST_CHECK( numeric_sequence_equals( v2, vec3( 1., 2., 3. ), cmp ) );

	//! Test intersection calculation
	double t = 0.5;
	point<double, 3> gp = p2 + t * (p1 - p2);
	ignore_unused_warning_of( gp );
}

BOOST_AUTO_TEST_CASE(TestPointVector2DArithmetic)
{
	using namespace geometrix;

	vector<double, 2> v1(10., 20.), v2;
	point<double, 2> p1(1., 2.), p2;

	typedef vector<double, 2> vec2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	//! Add vector to point.
	p2 = p1 + v1;
	BOOST_CHECK(numeric_sequence_equals(p2, vec2(11., 22.), cmp));

	//! Subtract vector from point.
	p2 = p2 - v1;
	BOOST_CHECK(numeric_sequence_equals(p2, vec2(1., 2.), cmp));

	//! Add point to point (fails to compile)
	//p2 <<= p1 + p1;

	//! Subtract a point from a point
	v2 = p2 - p1;
	BOOST_CHECK(numeric_sequence_equals(v2, vec2(0., 0.), cmp));

	//! Check scalar multiplication
	v2 = 10.0 * as_vector(p1);
	BOOST_CHECK(numeric_sequence_equals(v2, vec2(10., 20.), cmp));

	//! Check scalar division
	v2 = v2 / 10.0;
	BOOST_CHECK(numeric_sequence_equals(v2, vec2(1., 2.), cmp));

	//! Test intersection calculation
	double t = 0.5;
	point<double, 2> gp;
	gp <<= p2 + t * (p1 - p2);

	v2 = 10.0 * 2. * as_vector(p1);
	v2 = 2. * v1;
	BOOST_CHECK(numeric_sequence_equals(v2, vec2(20., 40.), cmp));
}

template <typename Vector>
void TestVectorGeneralRotation()
{
	using namespace geometrix;
	using namespace geometrix::result_of;

	typedef vector<double, 3> vec3;
	absolute_tolerance_comparison_policy<double> cmp( 1e-10 );

	BOOST_CONCEPT_ASSERT( (Vector3DConcept<Vector>) );
	Vector u( 0., 0., 1. );
	Vector v( 1., 0., 0. );
	Vector rv;

	//! Calculate v rotated around an axis represented by unit vector u by angle theta. 

	//! Rotate around pi/2
	double theta = constants::pi<double>() / 2.;
	rv = std::cos( theta )*(v - (u*v)*u) + std::sin( theta )*(u^v) + (u*v)*u;
	BOOST_CHECK( numeric_sequence_equals( rv, vec3( 0., 1., 0. ), cmp ) );

	//! Rotate around pi
	theta = constants::pi<double>();
	rv = std::cos( theta )*(v - (u*v)*u) + std::sin( theta )*(u^v) + (u*v)*u;
	BOOST_CHECK( numeric_sequence_equals( rv, vec3( -1., 0., 0. ), cmp ) );

	//! Rotate around 3*pi/2
	theta = 3.* constants::pi<double>() / 2.;
	rv = std::cos( theta )*(v - (u*v)*u) + std::sin( theta )*(u^v) + (u*v)*u;
	BOOST_CHECK( numeric_sequence_equals( rv, vec3( 0., -1., 0. ), cmp ) );

	//! Set u to a unit vector pointing towards (1.,1.,1.).
	u = norm( Vector( 1., 1., 1. ) );

	//! Rotate around pi/2
	theta = constants::pi<double>() / 2.0;
	rv = std::cos( theta )*(v - (u*v)*u) + std::sin( theta )*(u^v) + (u*v)*u;
	BOOST_CHECK( numeric_sequence_equals( rv, vec3( 0.3333333333, 0.9106836025, -0.2440169358 ), cmp ) );
}

BOOST_AUTO_TEST_CASE( TestPointVectorGeneralRotation )
{
	TestVectorGeneralRotation<vector_vector_3>();
}

BOOST_AUTO_TEST_CASE(TestVectorOrientation)
{
	using namespace geometrix;
	typedef vector_double_2d vector2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	{
		vector2 v1{ 1, 0 }, v2{ 0, 1 }, v3{ 1, 1 };
		bool b = is_vector_between(v1, v2, v3, true, cmp);
		BOOST_CHECK(b);
	}
}

BOOST_AUTO_TEST_CASE(TestIsNull)
{
	using namespace geometrix;
	typedef vector_double_2d vector2;
	vector2 v1{ 0, 0 }, v2{ 0, 1 }, v3{ 1, 1 };
	BOOST_CHECK(is_null(v1));
	BOOST_CHECK(!is_null(v2));
	BOOST_CHECK(!is_null(v3));
}

BOOST_AUTO_TEST_CASE(TestVectorBisectQuadrant0Vectors)
{
	using namespace geometrix;
	typedef vector_double_2d vector2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	vector2 v1{ 1, 0 }, v2{ 0, 1 }, v3 = normalize(vector2{ 1, 1 });
	auto b = bisect(v1, v2);
	BOOST_CHECK(numeric_sequence_equals(v3, b, cmp));
}

BOOST_AUTO_TEST_CASE(TestVectorBisect0toPiVectors)
{
	using namespace geometrix;
	typedef vector_double_2d vector2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	vector2 v1{ 1, 0 }, v2{ -1, 0 }, v3 = vector2{ 0, 1 };
	auto b = bisect(v1, v2);
	BOOST_CHECK(numeric_sequence_equals(v3, b, cmp));
}

BOOST_AUTO_TEST_CASE(TestVectorBisect5Pi_4vs7Pi_4)
{
	using namespace geometrix;
	typedef vector_double_2d vector2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	vector2 v1{ 1, -1 }, v2{ -1, -1 }, v3 = vector2{ 0, 1 };
	auto b = bisect(v1, v2);
	BOOST_CHECK(numeric_sequence_equals(v3, b, cmp));
}

BOOST_AUTO_TEST_CASE(TestVectorBisectPi_2vs3Pi_2)
{
	using namespace geometrix;
	typedef vector_double_2d vector2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	vector2 v1{ 0, 1 }, v2{ 0, -1 }, v3 = vector2{ -1, 0 };
	auto b = bisect(v1, v2);
	BOOST_CHECK(numeric_sequence_equals(v3, b, cmp));
}

BOOST_AUTO_TEST_CASE(TestVectorBisect0vs2Pi)
{
	using namespace geometrix;
	typedef vector_double_2d vector2;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

	vector2 v1{ 1, 0 }, v2{ 1, 0 }, v3 = vector2{ -1, 0 };
	auto b = bisect(v1, v2);
	BOOST_CHECK(numeric_sequence_equals(v3, b, cmp));
}

#endif //GEOMETRIX_VECTOR_POINT_ARITHMETIC_TESTS_HPP
