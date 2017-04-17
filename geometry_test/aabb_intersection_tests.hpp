//
//! Copyright � 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_AABB_INTERSECTION_TESTS_HPP
#define GEOMETRIX_AABB_INTERSECTION_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <geometrix/algorithm/intersection/segment_aabb_intersection.hpp>
#include <geometrix/algorithm/intersection/polyline_aabb_intersection.hpp>
#include <geometrix/algorithm/intersection/polygon_aabb_intersection.hpp>


#include "2d_kernel_units_fixture.hpp"

BOOST_FIXTURE_TEST_CASE(AABB_Segment_Intersection_Test_2D_Intersects, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	segment2 seg{ 0.5 * meters, 0.5 * meters, 1.5 * meters, 1.5 * meters };

	auto result = segment_aabb_intersection(seg.get_start(), seg.get_end(), box);

	BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Segment_Intersection_Test_2D_Does_NOT_Intersect, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	segment2 seg{ 0.5 * meters, 0.5 * meters, -1.5 * meters, -1.5 * meters };

	auto result = segment_aabb_intersection(seg.get_start(), seg.get_end(), box);

	BOOST_CHECK(result == false);
}

BOOST_FIXTURE_TEST_CASE(AABB_Segment_Intersection_Test_2D_WhollyInsideBox, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	segment2 seg{ 1.5 * meters, 1.5 * meters, 1.6 * meters, 1.6 * meters };

	auto result = segment_aabb_intersection(seg.get_start(), seg.get_end(), box);

	BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Segment_Intersection_Test_2D_IsAlongBorder, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	segment2 seg{ 1.0 * meters, 0.0 * meters, 1.0 * meters, 3.0 * meters };

	auto result = segment_aabb_intersection(seg.get_start(), seg.get_end(), box);

	BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polyline_Intersection_Test_2D_SyntaxCheck, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	polyline2 pline{ {1.0 * meters, 0.0 * meters}, {1.0 * meters, 3.0 * meters} };

	auto result = polyline_aabb_intersection(pline, box);

	BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polyline_Intersection_Test_2D_PolylineInsideBox, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	polyline2 pline{ { 1.1 * meters, 1.1 * meters }, { 1.9 * meters, 1.1 * meters }, { 1.9 * meters, 1.9 * meters },{ 1.1 * meters, 1.9 * meters } };

	auto result = polyline_aabb_intersection(pline, box);

	BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polyline_Intersection_Test_2D_PolylineOutsideBox, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	polyline2 pline{ { 0.9 * meters, 0.9 * meters },{ 2.1 * meters, 0.9 * meters },{ 2.1 * meters, 2.1 * meters },{ 0.9 * meters, 2.1 * meters } };

	auto result = polyline_aabb_intersection(pline, box);

	BOOST_CHECK(result == false);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polygon_Intersection_Test_2D_SyntaxCheck, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	polygon2 pgon{ { 1.0 * meters, 0.0 * meters },{ 1.0 * meters, 3.0 * meters }, {0.0 * meters, 0.0 * meters} };

	auto result = polygon_aabb_intersection(pgon, box);

	BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polygon_Intersection_Test_2D_PolygonInsideBox, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	polygon2 pgon{ { 1.1 * meters, 1.1 * meters },{ 1.9 * meters, 1.1 * meters },{ 1.9 * meters, 1.9 * meters },{ 1.1 * meters, 1.9 * meters } };

	auto result = polygon_aabb_intersection(pgon, box);

	BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polygon_Intersection_Test_2D_PolygonOutsideBoxOverlapping, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	polygon2 pgon{ { 0.9 * meters, 0.9 * meters },{ 2.1 * meters, 0.9 * meters },{ 2.1 * meters, 2.1 * meters },{ 0.9 * meters, 2.1 * meters } };

	auto result = polygon_aabb_intersection(pgon, box);

	BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polygon_BoundingBox_Intersection_Test_2D_PolygonOutsideBoxOverlapping, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	polygon2 pgon{ { 0.9 * meters, 0.9 * meters },{ 2.1 * meters, 0.9 * meters },{ 2.1 * meters, 2.1 * meters },{ 0.9 * meters, 2.1 * meters } };

	auto result = box.intersects(make_aabb<point2>(pgon, direct_comparison_policy()));

	BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polygon_BoundingBox_Intersection_Test_2D_PolygonInsideBoxOverlapping, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	polygon2 pgon{ { 1.1 * meters, 1.1 * meters },{ 1.9 * meters, 1.1 * meters },{ 1.9 * meters, 1.9 * meters },{ 1.1 * meters, 1.9 * meters } };

	auto result = box.intersects(make_aabb<point2>(pgon, direct_comparison_policy()));

	BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(AABB_Polygon_BoundingBox_Contains_Test_2D_PolygonInsideBoxOverlapping, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;
	aabb2 box({ 1.0 * meters, 1.0 * meters }, { 2.0 * meters, 2.0 * meters });
	polygon2 pgon{ { 1.1 * meters, 1.1 * meters },{ 1.9 * meters, 1.1 * meters },{ 1.9 * meters, 1.9 * meters },{ 1.1 * meters, 1.9 * meters } };

	auto result = box.contains(make_aabb<point2>(pgon, direct_comparison_policy()));

	BOOST_CHECK(result == true);
}

#endif//! GEOMETRIX_AABB_INTERSECTION_TESTS_HPP
