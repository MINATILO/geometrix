//
//! Copyright ? 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_INTERSECTION_TESTS_HPP
#define GEOMETRIX_INTERSECTION_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algorithm/segment_intersection.hpp>
#include <geometrix/algorithm/line_intersection.hpp>
#include <geometrix/algorithm/intersection/moving_sphere_plane_intersection.hpp>
#include <geometrix/algorithm/intersection/moving_sphere_segment_intersection.hpp>
#include <geometrix/algorithm/intersection/segment_capsule_intersection.hpp>
#include <geometrix/algorithm/intersection/circle_circle_intersection.hpp>
#include <geometrix/algorithm/intersection/ray_line_intersection.hpp>
#include <geometrix/algorithm/orientation.hpp>
#include <geometrix/primitive/sphere.hpp>
#include <geometrix/primitive/plane.hpp>
#include <geometrix/primitive/line.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/utility/ignore_unused_warnings.hpp>
#include "2d_kernel_fixture.hpp"
#include "2d_kernel_units_fixture.hpp"

#include <iostream>

BOOST_AUTO_TEST_CASE( TestIntersections )
{
    using namespace geometrix;

    typedef point_double_2d point_2d;

    point_2d p1( 0., 0. );
    point_2d p2( 1., 1. );
    point_2d p3( 1., 0. );
    point_2d p4( 0., 1. );

    segment<point_2d> seg1( p1, p2 );
    segment<point_2d> seg2( p3, p4 );

    {
        point_2d xPoints[2];
        intersection_type iType = segment_segment_intersection( seg1, seg2, xPoints, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << xPoints[0].get<0>() << ", " << xPoints[0].get<1>() << std::endl;
    }

    {
        point_2d xPoints[2];
        intersection_type iType = segment_segment_intersection( seg2, seg1, xPoints, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << xPoints[0].get<0>() << ", " << xPoints[0].get<1>() << std::endl;
    }

    {
        point_2d p;
        segment<point_2d> seg( p1, p3 );

        intersection_type iType = line_segment_intersect( point_2d( 0.5, -50. ), point_2d( 0.5, 50. ), seg, p, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << p.get<0>() << ", " << p.get<1>() << std::endl;

        iType = line_segment_intersect( point_2d( 0.5, -50. ), point_2d( 0.5, -40. ), seg, p, fraction_tolerance_comparison_policy<double>(1e-10) );
        BOOST_CHECK( iType == e_crossing );
        std::cout << iType << " at point: " << p.get<0>() << ", " << p.get<1>() << std::endl;
    }
}

BOOST_AUTO_TEST_CASE( TestMovingCircleLineIntersection )
{
    using namespace geometrix;

    typedef point_double_2d point2;
    typedef vector_double_2d vector2;
    typedef circle_double_2d circle2;
    typedef line<point2> line2;
    typedef segment_double_2d segment2;
    double t;
    point2 q;
    absolute_tolerance_comparison_policy<double> cmp( 1e-10 );

    //! General case intersecting.
    {
        circle2 circle{point2{1.0, 1.0}, 1.0};
        line2 line{point2{-1, -1}, vector2{0, 1}};
        vector2 velocity{-3, 0};

        BOOST_CHECK( moving_sphere_plane_intersection( circle, velocity, line, t, q, cmp ) );
        BOOST_CHECK( numeric_sequence_equals( q, point2{-1, 1}, cmp ) );
    }

    //! Intersect from starting position inside segment.
    {
        circle2 circle{point2{1.0, 1.0}, 0.5};
        segment2 seg{point2{1, -2}, vector2{1, 2}};
        vector2 velocity{-3, 0};
        BOOST_CHECK( moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp ) );
        BOOST_CHECK( numeric_sequence_equals( q, point2{1, 1}, cmp ) );
    }

    //! initial: intersecting line, final not intersecting segment and moving away.
    {
        circle2 circle{point2{1.0, -3.0}, 0.5};
        segment2 seg{point2{1, -2}, vector2{1, 2}};
        vector2 velocity{-3, 0};
        BOOST_CHECK( !moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp ) );
    }

    //! initial: intersecting line not segment, moving toward lower endpoint
    {
        circle2 circle{point2{1.0, -3.0}, 0.5};
        segment2 seg{point2{1, -2}, vector2{1, 2}};
        vector2 velocity{0, 2};
        bool result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
        BOOST_CHECK( result );
        BOOST_CHECK( numeric_sequence_equals( q, point2{1, -2}, cmp ) );
        BOOST_CHECK( cmp.equals( t, 0.25 ) );
    }

    //! initial: intersecting line not segment, moving toward upper endpoint
    {
        circle2 circle{point2{1.0, 3.0}, 0.5};
        segment2 seg{point2{1, -2}, vector2{1, 2}};
        vector2 velocity{0, -2};
        bool result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
        BOOST_CHECK( result );
        BOOST_CHECK( numeric_sequence_equals( q, point2{1, 2}, cmp ) );
        BOOST_CHECK( cmp.equals( t, 0.25 ) );
    }

    //! initial: not intersecting line nor segment, moving toward upper endpoint
    {
        circle2 circle{point2{2.0, 3.0}, 0.5};
        segment2 seg{point2{1, -2}, vector2{1, 2}};
        vector2 velocity{-2, -2};
        bool result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
        BOOST_CHECK( result );
        BOOST_CHECK( numeric_sequence_equals( q, point2{1, 2}, cmp ) );
        BOOST_CHECK( cmp.equals( t, 0.32322330470336319 ) );
    }

    //! initial: not intersecting line nor segment, moving toward lower endpoint
    {
        circle2 circle{point2{2.0, -3.0}, 0.5};
        segment2 seg{point2{1, -2}, vector2{1, 2}};
        vector2 velocity{-2, 2};
        bool result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
        BOOST_CHECK( result );
        BOOST_CHECK( numeric_sequence_equals( q, point2{1, -2}, cmp ) );
        BOOST_CHECK( cmp.equals( t, 0.32322330470336319 ) );
    }

    //! Sample bug 1
    {
        segment2 seg{55.84506916673854, 23.547610300593078, 56.529054251296813, -23.077372963791056};

        double radius = 0.31111750477426175;
        point2 position = point2{56.752395087297181, -23.585190612055708};
        vector2 velocity = vector2{-0.03648659998106174, 2.7389785331583272};
        circle2 circle{position, radius};

        bool result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
        BOOST_CHECK( result );
        BOOST_CHECK( numeric_sequence_equals( q, point2{56.529054251296813, -23.077372963791053}, cmp ) );
        BOOST_CHECK( cmp.equals( t, 0.10490959954263361 ) );
    }

    //! Sample bug 2 - stationary velocity.
    {
        segment2 seg{55.84506916673854, 23.547610300593078, 56.529054251296813, -23.077372963791056};

        double radius = 0.31111750477426175;
        point2 position = point2{56.752395087297181, -23.585190612055708};
        vector2 velocity = vector2{0,0};
        circle2 circle{position, radius};

        auto result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
        BOOST_CHECK( !result );
        BOOST_CHECK( cmp.equals( t, 0 ) );
    }

    //! Sample bug 3 - hits middle initially but reports incorrectly.
    {
        segment2 seg{-5.1316132118228097, 2.5356948795595144, -5.2326372913244086, -1.6241013071471571};
        point2 position{-4.8906096250382545, -0.24123259784358375};
        vector2 velocity{-2.5483733948031464, -3.6937969925168712};
        double radius = 0.30835263973557986;
        circle2 circle{position, radius};
        auto result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
        BOOST_CHECK( result.is_intersecting() && result.is_on_line_at_start() );
        BOOST_CHECK( cmp.equals( t, 0 ) );
    }

    //! Sample bug 4
    {
        segment2 seg{2.6941811136190381, -2.9910271024389203, -1.0194636759712239, 2.654467593997424};
        point2 position{3.0585517353902505, -2.9145958000370644};
        vector2 velocity{-0.72444262735718412, -2.0398104426204622};
        double radius = 0.31777965955349585;
        circle2 circle{position, radius};
        auto result = moving_sphere_segment_intersection( circle, velocity, seg, t, q, cmp );
        BOOST_CHECK( !result );
    }

    //! result type test
    {
        moving_sphere_segment_intersection_result r( true, true, true, false, false );
        BOOST_CHECK( r.is_intersecting() );
        BOOST_CHECK( r.is_on_line_at_start() );
        BOOST_CHECK( r.is_endpoint() );
    }

    {
        moving_sphere_segment_intersection_result r( true, true, false, false, false );
        BOOST_CHECK( r.is_intersecting() );
        BOOST_CHECK( r.is_on_line_at_start() );
        BOOST_CHECK( !r.is_endpoint() );
    }

    {
        moving_sphere_segment_intersection_result r( true, false, true, false, false );
        BOOST_CHECK( r.is_intersecting() );
        BOOST_CHECK( !r.is_on_line_at_start() );
        BOOST_CHECK( r.is_endpoint() );
    }

    {
        moving_sphere_segment_intersection_result r( true, false, false, false, false );
        BOOST_CHECK( r.is_intersecting() );
        BOOST_CHECK( !r.is_on_line_at_start() );
        BOOST_CHECK( !r.is_endpoint() );
    }
}

#include <geometrix/algorithm/intersection/segment_polyline_intersection.hpp>
#include <geometrix/primitive/vector_point_sequence.hpp>
BOOST_AUTO_TEST_CASE(TestSegmentPolylineIntersections)
{
    using namespace geometrix;

    typedef point_double_2d point2;
    typedef segment_double_2d segment2;
    typedef polyline<point2> polyline2;
    absolute_tolerance_comparison_policy<double> cmp(1e-10);
    std::vector<std::tuple<intersection_type, std::size_t, std::size_t, point2, point2>> intersections;
    auto visitor = [&intersections](intersection_type iType, std::size_t i, std::size_t j, point2 xPoint0, point2 xPoint1) -> bool
    {
        intersections.emplace_back(iType, i, j, xPoint0, xPoint1);
        return false;//! keep going.
    };

    {
        polyline2 geometry;
        geometry.push_back(point2(0., 0.));
        geometry.push_back(point2(10., 0.));
        geometry.push_back(point2(15., 5.));
        geometry.push_back(point2(10., 10.));
        geometry.push_back(point2(0., 10.));
        geometry.push_back(point2(5., 5.));

        segment2 segment{-1.0, -1.0, -2.0, -2.0};

        BOOST_CHECK(segment_polyline_intersect(segment, geometry, visitor, cmp) == false);
        BOOST_CHECK(intersections.empty());

        segment2 segment2{ 10.0, 0.0, 15.0, 5.0 };

        BOOST_CHECK(segment_polyline_intersect(segment2, geometry, visitor, cmp) == true);
        BOOST_CHECK(intersections.size() == 3);
    }
}

BOOST_AUTO_TEST_CASE(TestSegmentCapsuleIntersection)
{
    using namespace geometrix;

    typedef point_double_2d point2;
    typedef vector_double_2d vector2;
    typedef segment_double_2d segment2;

    ignore_unused_warning_of<vector2>();

    point2 a, b, c, d, xPoint;
    double r, t;
    absolute_tolerance_comparison_policy<double> cmp(1e-10);

    //! Endpoint a intersection from outside segment.
    {
        c = point2{1.0, 1.0};
        d = point2{ 4.0, 1.0 };
        r = 0.5;
        a = point2{ 0.0, 0.0 };
        b = point2{ 2.0, 3.0 };

        bool result = segment_capsule_intersection(a, b, c, d, r, t, xPoint, cmp);
        segment2 ab = { a, b };
        segment2 cd = { c, d };
        point2 q = a + t * (b - a);
        BOOST_CHECK(numeric_sequence_equals(q, xPoint, cmp));
        BOOST_CHECK(numeric_sequence_equals(q, point2{ 0.53846153846153821, 0.80769230769230727 }, cmp));
        auto qr = make_sphere<2>(q, r);
        auto ar = make_sphere<2>(a, r);
        auto br = make_sphere<2>(b, r);
        auto cr = make_sphere<2>(c, r);
        auto dr = make_sphere<2>(d, r);
        BOOST_CHECK(result);
    }

    //! Intersection inside.
    {
        c = point2{ 1.0, 1.0 };
        d = point2{ 4.0, 1.0 };
        r = 0.5;
        a = point2{ 1.25, 0.0 };
        b = point2{ 1.25, 3.0 };

        bool result = segment_capsule_intersection(a, b, c, d, r, t, xPoint, cmp);
        segment2 ab = { a, b };
        segment2 cd = { c, d };
        point2 q = a + t * (b - a);
        BOOST_CHECK(numeric_sequence_equals(q, xPoint, cmp));
        BOOST_CHECK(numeric_sequence_equals(q, point2{ 1.2500000000000000, 0.50000000000000000 }, cmp));
        auto qr = make_sphere<2>(q, r);
        auto ar = make_sphere<2>(a, r);
        auto br = make_sphere<2>(b, r);
        auto cr = make_sphere<2>(c, r);
        auto dr = make_sphere<2>(d, r);
        BOOST_CHECK(result);
    }

    //! Intersect b endpoint.
    {
        c = point2{ 1.0, 1.0 };
        d = point2{ 4.0, 1.0 };
        r = 0.5;
        a = point2{ 4.25, 0.0 };
        b = point2{ 4.25, 3.0 };

        bool result = segment_capsule_intersection(a, b, c, d, r, t, xPoint, cmp);
        segment2 ab = { a, b };
        segment2 cd = { c, d };
        point2 q = a + t * (b - a);
        BOOST_CHECK(numeric_sequence_equals(q, xPoint, cmp));
        BOOST_CHECK(numeric_sequence_equals(q, point2{ 4.2500000000000000, 0.56698729810778070 }, cmp));
        auto qr = make_sphere<2>(q, r);
        auto ar = make_sphere<2>(a, r);
        auto br = make_sphere<2>(b, r);
        auto cr = make_sphere<2>(c, r);
        auto dr = make_sphere<2>(d, r);
        BOOST_CHECK(result);
    }
}

#include <geometrix/algorithm/intersection/moving_sphere_aabb_intersection.hpp>
BOOST_AUTO_TEST_CASE(TestMovingCircleAABBIntersection)
{
    using namespace geometrix;

    typedef point_double_2d point2;
    typedef vector_double_2d vector2;
    typedef circle_double_2d circle2;
    typedef segment_double_2d segment2;
    typedef axis_aligned_bounding_box<point2> aabb2;
    typedef polygon<point2> polygon2;

    ignore_unused_warning_of<segment2>();

    double t;
    point2 q;
    absolute_tolerance_comparison_policy<double> cmp(1e-10);

    //! General case intersecting.
    {
        circle2 circle{ point2{ 1.0, 1.0 }, 0.25 };
        aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
        polygon2 box{ point2{-1,-1}, point2{0, -1}, point2{0, 0}, point2{-1, 0} };
        vector2 velocity{ -1, -1 };

        BOOST_CHECK(moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
        circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
        BOOST_CHECK(numeric_sequence_equals(q, point2{ 0, 0 }, cmp));
    }

    //! Bottom case middle intersecting.
    {
        circle2 circle{ point2{ -0.5, -2.0 }, 0.25 };
        aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
        polygon2 box{ point2{ -1,-1 }, point2{ 0, -1 }, point2{ 0, 0 }, point2{ -1, 0 } };
        vector2 velocity{ 0, 1 };

        BOOST_CHECK(moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
        circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
        BOOST_CHECK(numeric_sequence_equals(q, point2{ -0.50000000000000000, -1.0000000000000000 }, cmp));
    }

    //! Side case middle intersecting.
    {
        circle2 circle{ point2{ -2.0, -0.5 }, 0.25 };
        aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
        polygon2 box{ point2{ -1,-1 }, point2{ 0, -1 }, point2{ 0, 0 }, point2{ -1, 0 } };
        vector2 velocity{ 1, 0 };

        BOOST_CHECK(moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
        circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
        BOOST_CHECK(numeric_sequence_equals(q, point2{ -1.00000000000000000, -0.5000000000000000 }, cmp));
    }

    //! Side case angled intersecting.
    {
        circle2 circle{ point2{ -2.0, -1.0 }, 0.25 };
        aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
        polygon2 box{ point2{ -1,-1 }, point2{ 0, -1 }, point2{ 0, 0 }, point2{ -1, 0 } };
        vector2 velocity{ 1, 1 };

        BOOST_CHECK(moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
        circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
        BOOST_CHECK(numeric_sequence_equals(q, point2{ -1.0000000000000000, -0.25000000000000000 }, cmp));
    }

    //! Side case overlapping intersecting.
    {
        circle2 circle{ point2{ -1.25, -1.75 }, 0.25 };
        aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
        polygon2 box{ point2{ -1,-1 }, point2{ 0, -1 }, point2{ 0, 0 }, point2{ -1, 0 } };
        vector2 velocity{ 0, 1 };

        BOOST_CHECK(moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
        circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
        BOOST_CHECK(numeric_sequence_equals(q, point2{ -1, -1 }, cmp));
    }

    //! Passing corner at 45.
    {
        double radius = 0.25;
        double sqrt2 = sqrt(2.0);
        vector2 velocity{ 1, -1 };
        circle2 circle{ point2{ radius/sqrt2, radius / sqrt2 } - velocity, radius };
        aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
        polygon2 box{ point2{ -1,-1 }, point2{ 0, -1 }, point2{ 0, 0 }, point2{ -1, 0 } };

        BOOST_CHECK(!moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
        circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
        //BOOST_CHECK(numeric_sequence_equals(q, point2{ 0, 0 }, cmp));
    }

    //! Passing corner at 45 shave.
    {
        circle2 circle{ point2{ -1., 1.25 }, 0.25 };
        aabb2 bb{ point2{ -1, -1 }, point2{ 0, 0 } };
        polygon2 box{ point2{ -1,-1 }, point2{ 0, -1 }, point2{ 0, 0 }, point2{ -1, 0 } };
        vector2 velocity{ 1, -1 };

        BOOST_CHECK(moving_sphere_aabb_intersection(circle, velocity, bb, t, q, cmp));
        circle2 qr(circle.get_center() + t * velocity, circle.get_radius());
        BOOST_CHECK(numeric_sequence_equals(q, point2{ 0, 0 }, cmp));
    }
}

#include <geometrix/algorithm/intersection/moving_sphere_obb_intersection.hpp>

template <typename Point, typename Vector, typename NumberComparisonPolicy>
bool test_obb_collision(const Point& p, double radius, const Vector& velocity, const Point& ocenter, const Vector& odirection, double& t, Point& q, const NumberComparisonPolicy& cmp)
{
    using namespace geometrix;
    typedef obb_double_2d obb2;
    typedef aabb_double_2d aabb2;
    typedef point_double_2d point2;
    typedef vector_double_2d vector2;
    typedef polygon<point2> polygon2;
    typedef segment<point2> segment2;
    typedef circle_double_2d circle2;

    obb2 obb(ocenter, odirection, left_normal(odirection), 0.5, 0.5);
    point2 rb = obb[0];// .get_right_backward_point();
    point2 rf = obb[1];// .get_right_forward_point();
    point2 lf = obb[2];// .get_left_forward_point();
    point2 lb = obb[3];// .get_left_backward_point();
    circle2 circle{ p, radius };

    vector2 xAxis{ 1,0 };
    point2 rb2 = rotate_point(rb, obb.get_axis(0), xAxis, obb.get_center());
    point2 rf2 = rotate_point(rf, obb.get_axis(0), xAxis, obb.get_center());
    point2 lf2 = rotate_point(lf, obb.get_axis(0), xAxis, obb.get_center());
    point2 lb2 = rotate_point(lb, obb.get_axis(0), xAxis, obb.get_center());
    polygon2 s2{ rb2, rf2, lf2, lb2 };
    aabb2 aabb = make_aabb<point2>(s2);
    point2 rp = rotate_point(p, obb.get_axis(0), xAxis, obb.get_center());
    circle2 rcircle{ rp, radius };
    vector2 rvelocity = rotate_vector(velocity, obb.get_axis(0), xAxis);
    segment2 step{ p, p + velocity };
    segment2 rstep{ rp, rp + rvelocity };
    auto r0 = moving_sphere_aabb_intersection(rcircle, rvelocity, aabb, t, q, cmp);
    circle2 cqr(rp + t * rvelocity, radius);

    bool result = moving_sphere_obb_intersection(circle, velocity, obb, t, q, cmp);
    circle2 qr(p + t * velocity, radius);
    return result;
}

BOOST_AUTO_TEST_CASE(TestRotateOBB)
{
    using namespace geometrix;
    typedef point_double_2d point2;
    typedef vector_double_2d vector2;
    absolute_tolerance_comparison_policy<double> cmp(1e-10);
    double t = 0;
    point2 q;

    //! Approaching face 3-0 from 45.
    {
        point2 p{ 0,0 };
        double radius = 0.25;
        point2 ocenter{ 1,1 };
        vector2 odirection = normalize(vector2{ 1,1 });
        vector2 velocity{ 1,1 };
        bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
        BOOST_CHECK(result);
        BOOST_CHECK(numeric_sequence_equals(q, point2{ 0.64644660940672627, 0.64644660940672627 }, cmp));
    }

    //! Approaching face 0-1 from Below at 135.
    {
        point2 p{ 1, 0 };
        double radius = 0.25;
        point2 ocenter{ 1,1 };
        vector2 odirection = normalize(vector2{ 1,1 });
        vector2 velocity{ -1, 1 };
        bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
        BOOST_CHECK(result);
        BOOST_CHECK(numeric_sequence_equals(q, point2{ 1.0000000000000000, 0.29289321881345259 }, cmp));
    }

    //! Approaching face 0-1 from Below at 90.
    {
        point2 p{ 1.8, -1 };
        double radius = 0.25;
        point2 ocenter{ 1,1 };
        vector2 odirection = normalize(vector2{ 1,1 });
        vector2 velocity{ 0, 1 };
        bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
        BOOST_CHECK(result);
        BOOST_CHECK(numeric_sequence_equals(q, point2{ 1.6232233047033631, 0.91611652351681572 }, cmp));
    }

    //! Approaching face 1-2 from Side at pi.
    {
        point2 p{ 3, 1 };
        double radius = 0.25;
        point2 ocenter{ 1,1 };
        vector2 odirection = normalize(vector2{ 1,1 });
        vector2 velocity{ -1, 0 };
        bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
        BOOST_CHECK(result);
        BOOST_CHECK(numeric_sequence_equals(q, point2{ 1.7071067811865475, 1.0000000000000000 }, cmp));
    }

    //! Approaching face 1-2 from Side at 225.
    {
        point2 p{ 2.0, 2 };
        double radius = 0.25;
        point2 ocenter{ 1,1 };
        vector2 odirection = normalize(vector2{ 1,1 });
        vector2 velocity{ -1, -1 };
        bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
        BOOST_CHECK(result);
        BOOST_CHECK(numeric_sequence_equals(q, point2{ 1.3535533905932737, 1.3535533905932737 }, cmp));
    }

    //! Approaching face 1-2 from Top at 270.
    {
        point2 p{ 1.0, 3 };
        double radius = 0.25;
        point2 ocenter{ 1,1 };
        vector2 odirection = normalize(vector2{ 1,1 });
        vector2 velocity{ 0, -1 };
        bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
        BOOST_CHECK(result);
        BOOST_CHECK(numeric_sequence_equals(q, point2{ 1.0000000000000000, 1.7071067811865475 }, cmp));
    }

    //! Approaching face 2-3 from Side at 315.
    {
        point2 p{ 0.0, 3 };
        double radius = 0.25;
        point2 ocenter{ 1,1 };
        vector2 odirection = normalize(vector2{ 1,1 });
        vector2 velocity{ 1, -1 };
        bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
        BOOST_CHECK(result);
        BOOST_CHECK(numeric_sequence_equals(q, point2{ 0.99999999999999967, 1.7071067811865459 }, cmp));
    }

    //! Approaching face 2-3 from Side at 315.
    {
        point2 p{ -1, 3 };
        double radius = 0.25;
        point2 ocenter{ 1,1 };
        vector2 odirection = normalize(vector2{ 1,1 });
        vector2 velocity{ 1, -1 };
        bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
        BOOST_CHECK(result);
        BOOST_CHECK(numeric_sequence_equals(q, point2{ 0.64644660940672616, 1.3535533905932737 }, cmp));
    }
    
    //! Moving toward side 0-1
    {
        double radius = 0.25;
        point2 ocenter{ 1,1 };
        vector2 odirection = normalize(vector2{ 1,1 });
        auto p = point2{ ocenter + 4.0 * radius * right_normal(odirection) };
        vector2 velocity{ -1, 1 };
        bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
        BOOST_CHECK(result);
    }

    //! Moving away from side 0-1
    {
        double radius = 0.25;
        point2 ocenter{ 1,1 };
        vector2 odirection = normalize(vector2{ 1,1 });
        auto p = point2{ ocenter + 4.0 * radius * right_normal(odirection) };
        vector2 velocity{ 1, -1 };
        bool result = test_obb_collision(p, radius, velocity, ocenter, odirection, t, q, cmp);
        BOOST_CHECK(!result);
    }
}

#include <geometrix/algorithm/intersection/sphere_obb_intersection.hpp>
BOOST_FIXTURE_TEST_CASE(sphere_obb_intersection_NotIntersecting, geometry_kernel_2d_fixture)
{
    using namespace geometrix;

    auto p = point2{ 1.3, 0.0 };
    auto r = double{ .25 };
    auto s = make_sphere<2>(p, r);
    auto obb = obb2
    {
        { 0.0, 0.0 }
      , { 1.0, 0.0 }
      , { 0.0, 1.0 }
      , 1.0
      , 1.0
    };

    auto result = sphere_obb_intersection(s, obb);

    BOOST_CHECK(!result);
}

BOOST_FIXTURE_TEST_CASE(sphere_obb_intersection_TouchIntersecting, geometry_kernel_2d_fixture)
{
    using namespace geometrix;

    auto p = point2{ 1.25, 0.0 };
    auto r = double{ .25 };
    auto s = make_sphere<2>(p, r);
    auto obb = obb2
    {
        { 0.0, 0.0 }
      , { 1.0, 0.0 }
      , { 0.0, 1.0 }
      , 1.0
      , 1.0
    };

    auto result = sphere_obb_intersection(s, obb);

    BOOST_CHECK(result);
}

BOOST_FIXTURE_TEST_CASE(sphere_obb_intersection_WhollyIntersecting, geometry_kernel_2d_fixture)
{
    using namespace geometrix;

    auto p = point2{ 0.0, 0.0 };
    auto r = double{ .25 };
    auto s = make_sphere<2>(p, r);
    auto obb = obb2
    {
        { 0.0, 0.0 }
      , { 1.0, 0.0 }
      , { 0.0, 1.0 }
      , 1.0
      , 1.0
    };

    auto result = sphere_obb_intersection(s, obb);

    BOOST_CHECK(result);
}

#include <geometrix/algorithm/intersection/ray_segment_intersection.hpp>
BOOST_AUTO_TEST_CASE(TestRaySegmentIntersection)
{
    using namespace geometrix;
    typedef point_double_2d point2;
    typedef vector_double_2d vector2;
    typedef segment<point2> segment2;
    absolute_tolerance_comparison_policy<double> cmp(1e-10);
    double t = 0;
    point2 q[2];

    {
        point2 center{ 1, 1 };
        vector2 dir = normalize(vector2{ 1, 1 });
        segment2 segment{ 2, 0, 0, 2 };
        intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
        BOOST_CHECK(result == e_crossing);
        BOOST_CHECK(cmp.equals(t, 0));
        BOOST_CHECK(numeric_sequence_equals(q[0], point2{ 1,1 }, cmp));
    }

    {
        point2 center{ 1, 1 };
        vector2 dir = normalize(vector2{ 1, 1 });
        segment2 segment{ 3, 0, 0, 3 };
        intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
        BOOST_CHECK(result == e_crossing);
        BOOST_CHECK(cmp.equals(t, 0.70710678118654757));
        BOOST_CHECK(numeric_sequence_equals(q[0], point2{ 1.5,1.5 }, cmp));
    }

    {
        point2 center{ 1, 1 };
        vector2 dir = normalize(vector2{ 1, 1 });
        segment2 segment{ 3, 3, 5, 5 };
        intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
        BOOST_CHECK(result == e_overlapping);
        BOOST_CHECK(cmp.equals(t, 2.8284271247461898));
        point2 np = center + t * dir;
        ignore_unused_warning_of(np);
        BOOST_CHECK(numeric_sequence_equals(q[0], point2{ 3, 3 }, cmp));
        BOOST_CHECK(numeric_sequence_equals(q[1], point2{ 5, 5 }, cmp));
    }

    {
        point2 center{ 1, 1 };
        vector2 dir = normalize(vector2{ 1, 1 });
        segment2 segment{ 0, 0, 4, 4 };
        intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
        BOOST_CHECK(result == e_overlapping);
        BOOST_CHECK(cmp.equals(t, 0));
        BOOST_CHECK(numeric_sequence_equals(q[0], point2{ 1, 1 }, cmp));
        BOOST_CHECK(numeric_sequence_equals(q[1], point2{ 4, 4 }, cmp));
    }

    {
        point2 center{ 1, 1 };
        vector2 dir = normalize(vector2{ 1, 1 });
        segment2 segment{ 4, 4, 0, 0 };
        intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
        BOOST_CHECK(result == e_overlapping);
        BOOST_CHECK(cmp.equals(t, 0));
        BOOST_CHECK(numeric_sequence_equals(q[0], point2{ 1, 1 }, cmp));
        BOOST_CHECK(numeric_sequence_equals(q[1], point2{ 4, 4 }, cmp));
    }

    {
        point2 center{ 1, 1 };
        vector2 dir = normalize(vector2{ 1, 1 });
        segment2 segment{ 4, 4, 4, 10 };
        intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
        BOOST_CHECK(result == e_endpoint);
        BOOST_CHECK(cmp.equals(t, 4.2426406871192857));
        BOOST_CHECK(numeric_sequence_equals(q[0], point2{ 4, 4 }, cmp));
    }

    {
        point2 center{ 1, 1 };
        vector2 dir = normalize(vector2{ -1, -1 });
        segment2 segment{ 4, 4, 4, 10 };
        intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
        BOOST_CHECK(result == e_non_crossing);
    }

    {
        vector2 dir = left_normal(normalize(vector2{ 1, 1 }));
        point2 center{ point2{4, 4} + dir };
        segment2 segment{ 4, 4, 4, 10 };
        intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
        BOOST_CHECK(result == e_non_crossing);
    }
}

BOOST_FIXTURE_TEST_CASE(circle_circle_intersection_test, geometry_kernel_2d_fixture)
{
    using namespace geometrix;

    {
        double radius = 0.25;
        circle2 A{ point2{ 0, 1 }, radius };
        circle2 B{ point2{ 0, 0.5 }, radius };
        auto result = circle_circle_intersection(A, B, cmp);

        BOOST_CHECK(result.State == circle_intersection_state::one_intersection);
    }

    {
        double radius = 0.35;
        circle2 A{ point2{ 0, 1 }, radius };
        circle2 B{ point2{ 0, 0.5 }, radius };
        auto result = circle_circle_intersection(A, B, cmp);

        //auto p0 = *result.IntersectionPoint0;
        //auto p1 = *result.IntersectionPoint1;
        BOOST_CHECK(result.State == circle_intersection_state::two_intersections);
    }
}

#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <geometrix/algorithm/intersection/polyline_polyline_intersection.hpp>
BOOST_FIXTURE_TEST_CASE(polyline_polyline_intersect_test, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto flatten = [](const polyline<point<double, 3>>& pline) -> polyline2
    {
        using boost::adaptors::transformed;
        using point3 = point<double,3>;
        polyline2 result;
        boost::copy(pline | transformed([](const point3& p) { return construct<point2>(p); }), std::back_inserter(result));
        return result;
    };

    auto leftPoints = polyline<point<double,3>>
    {
        {589246.25283200003, 4473667.6991360001, 196.30770899999999}
        ,{589246.73788499995, 4473668.2119509997, 196.29904199999999}
        ,{589247.458032, 4473669.0006839996, 196.29969800000001}
        ,{589248.18961, 4473669.6636760002, 196.29995700000001}
        ,{589248.88689600001, 4473670.2580829998, 196.300049}
        ,{589249.60704300005, 4473670.8296290003, 196.29402200000001}
    };

    auto flatLeft = flatten(leftPoints);

    auto rightPoints = polyline<point<double, 3>>
    {
         {589247.60650500003, 4473662.1570589999, 196.267899}
        ,{589248.601287, 4473662.478255, 196.26705899999999}
        ,{589250.09216300002, 4473662.9604110001, 196.24153100000001}
        ,{589251.58303900005, 4473663.4425659999, 196.233475}
    };

    auto flatRight = flatten(rightPoints);

    std::vector<point2> ipoints;
    auto visitor = [&ipoints](intersection_type iType, std::size_t /*i1*/, std::size_t /*j1*/, std::size_t /*i2*/, std::size_t /*j2*/, const point2& x1, const point2& x2)
    {
        ipoints.push_back(x1);
        if (iType == e_overlapping)
            ipoints.push_back(x2);
        return false;
    };

    auto r = polyline_polyline_intersect(flatLeft, flatRight, visitor, cmp);

    BOOST_CHECK(r==false);
}

#include <geometrix/algorithm/intersection/sphere_aabb_intersection.hpp>
BOOST_FIXTURE_TEST_CASE(sphere_aabb_intersection_NotIntersecting, geometry_kernel_2d_fixture)
{
    using namespace geometrix;

    auto p = point2{ 1.3, 0.0 };
    auto r = double{ .25 };
    auto s = make_sphere<2>(p, r);
    auto aabb = aabb2
    {
        { 0.0, 0.0 }
      , { 1.0, 1.0 }
    };

    auto result = sphere_aabb_intersection(s, aabb);

    BOOST_CHECK(!result);
}

BOOST_FIXTURE_TEST_CASE(sphere_aabb_intersection_SideIntersecting, geometry_kernel_2d_fixture)
{
    using namespace geometrix;

    auto p = point2{ 1.2, 0.5 };
    auto r = double{ .25 };
    auto s = make_sphere<2>(p, r);
    auto aabb = aabb2
    {
        { 0.0, 0.0 }
      , { 1.0, 1.0 }
    };

    auto result = sphere_aabb_intersection(s, aabb);

    BOOST_CHECK(result);
}

BOOST_FIXTURE_TEST_CASE(sphere_aabb_intersection_WhollyIntersecting, geometry_kernel_2d_fixture)
{
    using namespace geometrix;

    auto p = point2{ 0.5, 0.5 };
    auto r = double{ .25 };
    auto s = make_sphere<2>(p, r);
    auto aabb = aabb2
    {
        { 0.0, 0.0 }
      , { 1.0, 1.0 }
    };

    auto result = sphere_aabb_intersection(s, aabb);

    BOOST_CHECK(result);
}

#include <boost/optional.hpp>
BOOST_AUTO_TEST_CASE(TestConvertToBoostOptional)
{
    using namespace geometrix;
    typedef vector_double_2d vector2;
    typedef point_double_2d point2;

    typedef boost::optional<vector2> ovector2;

    point2 p{ 0, 0 };
    vector2 v{ 10, 10 };
    ovector2 ovp = vector2(p + 10 * v);
}

BOOST_FIXTURE_TEST_CASE(ray_line_intersection_SimpleCrossing, geometry_kernel_2d_fixture)
{
    using namespace geometrix;

    auto s1 = segment2{ { 1.0, 1.0 },{ 5.0, 5.0 } };
    auto s2 = segment2{ { 3.0, 0.0 },{ 2.0, 1.0 } };

    auto o = s2.get_start();
    auto v = normalize(s2.get_end()- s2.get_start());

    auto l = make_line<point2, vector2>(s1.get_start(), s1.get_end() - s1.get_start());

    double t = 0.0;
    point2 q;
    auto result = ray_line_intersection(o, v, l, t, q, cmp);

    BOOST_CHECK(result == true);
    BOOST_CHECK(!result.is_overlapping());
    BOOST_CHECK(numeric_sequence_equals(q, point2{ 1.5, 1.5 }, cmp));
}

BOOST_FIXTURE_TEST_CASE(ray_line_intersection_NotCrossing, geometry_kernel_2d_fixture)
{
    using namespace geometrix;

    auto s1 = segment2{ { 1.0, 1.0 },{ 5.0, 5.0 } };
    auto s2 = segment2{ { 3.0, 0.0 },{ 2.0, 1.0 } };

    auto o = s2.get_start();
    auto v = normalize(s2.get_start() - s2.get_end());

    auto l = make_line<point2, vector2>(s1.get_start(), s1.get_end() - s1.get_start());

    double t = 0.0;
    point2 q;
    auto result = ray_line_intersection(o, v, l, t, q, cmp);

    BOOST_CHECK(result == false);
}

template <typename PointSequence, typename Point, int Divisions = 100, typename std::enable_if<geometrix::is_polyline<PointSequence>::value, int>::type = 0>
inline PointSequence make_circle_as_sequence(const Point& center, double r)
{
    using namespace geometrix;
    auto v = vector_double_2d{ r, 0.0 };
    ignore_unused_warning_of(v);
    auto s = constants::two_pi<double>() / Divisions, t = 0.;
    auto poly = PointSequence{};
    for (auto i = 0UL; i <= Divisions; ++i, t += s)
        poly.emplace_back(center + vector_double_2d{ r * cos(t), r * sin(t) });
    return std::move(poly);
}

template <typename PointSequence, typename Point, int Divisions = 100, typename std::enable_if<geometrix::is_polygon<PointSequence>::value, int>::type = 0>
inline PointSequence make_circle_as_sequence(const Point& center, double r)
{
    using namespace geometrix;
    auto v = vector_double_2d{ r, 0.0 };
    ignore_unused_warning_of(v);
    auto s = constants::two_pi<double>() / Divisions, t = 0.;
    auto poly = PointSequence{};
    for (auto i = 0UL; i < Divisions; ++i, t += s)
        poly.emplace_back(center + vector_double_2d{ r * cos(t), r * sin(t) });
    return std::move(poly);
}

#include <geometrix/algorithm/intersection/separating_axis_convex_polygons.hpp>
BOOST_FIXTURE_TEST_CASE(polygon_polygon_sat_test, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto pgon1 = make_circle_as_sequence<polygon2, point2, 4>(point2{ 0,0 }, 3.0);
    auto pgon2 = make_circle_as_sequence<polygon2, point2, 4>(point2{ 0,6 }, 3.0);
    auto result = convex_polygons_intersection(pgon1, pgon2, cmp);
    BOOST_CHECK(!result);
}

#include <geometrix/algorithm/intersection/moving_separating_axis_convex_polygons.hpp>
BOOST_FIXTURE_TEST_CASE(moving_polygon_polygon_sat_test_2above1_will_collide, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto pgon1 = make_circle_as_sequence<polygon2, point2, 4>(point2{ 0,0 }, 3.0);
    auto pgon2 = make_circle_as_sequence<polygon2, point2, 4>(point2{ 0,7 }, 3.0);
    auto v1 = vector2{0.0, 1.0};
    auto v2 = vector2{0.0, 0.5};
    auto tmax = std::numeric_limits<double>::infinity();
    auto tfirst = 0.;
    auto tlast = 0.;
    auto result = moving_convex_polygons_intersection(pgon1, v1, pgon2, v2, tmax, tfirst, tlast, cmp);
    BOOST_CHECK(result);
}

BOOST_FIXTURE_TEST_CASE(moving_polygon_polygon_sat_test_1above2_wont_collide, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto pgon1 = make_circle_as_sequence<polygon2, point2, 4>(point2{ 0,7 }, 3.0);
    auto pgon2 = make_circle_as_sequence<polygon2, point2, 4>(point2{ 0,0 }, 3.0);
    auto v1 = vector2{0.0, 1.0};
    auto v2 = vector2{0.0, 0.5};
    auto tmax = std::numeric_limits<double>::infinity();
    auto tfirst = 0.;
    auto tlast = 0.;
    auto result = moving_convex_polygons_intersection(pgon1, v1, pgon2, v2, tmax, tfirst, tlast, cmp);
    BOOST_CHECK(!result);
}

BOOST_FIXTURE_TEST_CASE(moving_polygon_polygon_sat_test_2bside1_wont_collide, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto pgon1 = make_circle_as_sequence<polygon2, point2, 4>(point2{ 7,0 }, 3.0);
    auto pgon2 = make_circle_as_sequence<polygon2, point2, 4>(point2{ 0,0 }, 3.0);
    auto v1 = vector2{0.0, 1.0};
    auto v2 = vector2{0.0, 1.0};
    auto tmax = std::numeric_limits<double>::infinity();
    auto tfirst = 0.;
    auto tlast = 0.;
    auto result = moving_convex_polygons_intersection(pgon1, v1, pgon2, v2, tmax, tfirst, tlast, cmp);
    BOOST_CHECK(!result);
}

#include <geometrix/algorithm/intersection/moving_obb_obb_intersection.hpp>

BOOST_FIXTURE_TEST_CASE(moving_obb_obb_2above1_will_collide, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto ob1 = obb2{point2{ 0,0 }, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 3.0, 3.0};
    auto ob2 = obb2{point2{ 0,7 }, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 3.0, 3.0};
    auto v1 = vector2{0.0, 1.0};
    auto v2 = vector2{0.0, 0.5};
    auto tmax = std::numeric_limits<double>::infinity();
    auto tfirst = 0.;
    auto tlast = 0.;
    auto result = moving_obb_obb_intersection(ob1, v1, ob2, v2, tmax, tfirst, tlast);
    BOOST_CHECK(result);
}

BOOST_FIXTURE_TEST_CASE(moving_obb_obb_2above1_wont_collide, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto ob1 = obb2{point2{ 0,0 }, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 3.0, 3.0};
    auto ob2 = obb2{point2{ 0,7 }, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 3.0, 3.0};
    auto v1 = vector2{0.0, 0.4};
    auto v2 = vector2{0.0, 0.5};
    auto tmax = std::numeric_limits<double>::infinity();
    auto tfirst = 0.;
    auto tlast = 0.;
    auto result = moving_obb_obb_intersection(ob1, v1, ob2, v2, tmax, tfirst, tlast);
    BOOST_CHECK(!result);
}

BOOST_FIXTURE_TEST_CASE(moving_obb_obb_2rightof1_wont_collide, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto ob1 = obb2{point2{ 0,0 }, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 3.0, 3.0};
    auto ob2 = obb2{point2{ 7,0 }, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 3.0, 3.0};
    auto v1 = vector2{0.0, 0.4};
    auto v2 = vector2{0.0, 0.5};
    auto tmax = std::numeric_limits<double>::infinity();
    auto tfirst = 0.;
    auto tlast = 0.;
    auto result = moving_obb_obb_intersection(ob1, v1, ob2, v2, tmax, tfirst, tlast);
    BOOST_CHECK(!result);
}

BOOST_FIXTURE_TEST_CASE(moving_obb_obb_1rightof2_wont_collide, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto ob1 = obb2{point2{ 7,0 }, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 3.0, 3.0};
    auto ob2 = obb2{point2{ 0,0 }, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 3.0, 3.0};
    auto v1 = vector2{0.0, 0.4};
    auto v2 = vector2{0.0, 0.5};
    auto tmax = std::numeric_limits<double>::infinity();
    auto tfirst = 0.;
    auto tlast = 0.;
    auto result = moving_obb_obb_intersection(ob1, v1, ob2, v2, tmax, tfirst, tlast);
    BOOST_CHECK(!result);
}

BOOST_FIXTURE_TEST_CASE(moving_obb_obb_crossing_paths, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto ob1 = obb2{point2{ 3,0 }, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 1.0, 1.0};
    auto ob2 = obb2{point2{ 0,3 }, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 1.0, 1.0};
    auto v1 = vector2{0.0, 1.0};
    auto v2 = vector2{1.0, 0.0};
    auto tmax = std::numeric_limits<double>::infinity();
    auto tfirst = 0.;
    auto tlast = 0.;
    auto result = moving_obb_obb_intersection(ob1, v1, ob2, v2, tmax, tfirst, tlast);
    BOOST_CHECK(result);
}

BOOST_FIXTURE_TEST_CASE(units_moving_obb_obb_crossing_paths, geometry_kernel_2d_units_fixture)
{
    using namespace geometrix;
    auto ob1 = obb2{point2{ 3 * boost::units::si::meters,0 * boost::units::si::meters }, dimensionless2{0.0, 1.0}, dimensionless2{1.0, 0.0}, 1.0 * boost::units::si::meters, 1.0 * boost::units::si::meters};
    auto ob2 = obb2{point2{ 0 * boost::units::si::meters,3 * boost::units::si::meters }, dimensionless2{0.0, 1.0}, dimensionless2{1.0, 0.0}, 1.0 * boost::units::si::meters, 1.0 * boost::units::si::meters};
    auto v1 = velocity2{0.0 * boost::units::si::meters_per_second, 1.0 * boost::units::si::meters_per_second};
    auto v2 = velocity2{1.0 * boost::units::si::meters_per_second, 0.0 * boost::units::si::meters_per_second};
    auto tmax = std::numeric_limits<double>::infinity() * boost::units::si::seconds;
    auto tfirst = 0. * boost::units::si::seconds;
    auto tlast = 0. * boost::units::si::seconds;
    auto result = moving_obb_obb_intersection(ob1, v1, ob2, v2, tmax, tfirst, tlast);
    BOOST_CHECK(result);
}
#include <geometrix/algorithm/intersection/obb_obb_intersection.hpp>
BOOST_FIXTURE_TEST_CASE(obb_obb_overlapping_lower_right_to_upper_left, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto ob1 = obb2{point2{1, 0}, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 1.0, 1.0};
    auto ob2 = obb2{point2{0, 1}, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 1.0, 1.0};
    auto result = obb_obb_intersection(ob1, ob2, cmp);
    BOOST_CHECK(result);
}

BOOST_FIXTURE_TEST_CASE(obb_obb_overlapping_upper_left_to_lower_right, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto ob1 = obb2{point2{0, 1}, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 1.0, 1.0};
    auto ob2 = obb2{point2{1, 0}, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 1.0, 1.0};
    auto result = obb_obb_intersection(ob1, ob2, cmp);
    BOOST_CHECK(result);
}

BOOST_FIXTURE_TEST_CASE(obb_obb_corners_touching, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto ob1 = obb2{point2{0, 2}, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 1.0, 1.0};
    auto ob2 = obb2{point2{2, 0}, vector2{0.0, 1.0}, vector2{1.0, 0.0}, 1.0, 1.0};
    auto result = obb_obb_intersection(ob1, ob2, cmp);
    BOOST_CHECK(result);
}

BOOST_FIXTURE_TEST_CASE(obb_obb_along_forty_fives_corners_not_touching, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto u1 = normalize(vector2{ 1,1 });
    auto v1 = left_normal(u1);
    auto ob1 = obb2{point2{0, 2}, u1, v1, 1.0, 1.0};

    auto u2 = normalize(vector2{ -1,1 });
    auto v2 = left_normal(u2);
    auto ob2 = obb2{point2{2, 0}, u2, v2, 1.0, 1.0};
    auto result = obb_obb_intersection(ob1, ob2, cmp);
    BOOST_CHECK(!result);
}

BOOST_FIXTURE_TEST_CASE(obb_obb_along_forty_fives_corners_touching, geometry_kernel_2d_fixture)
{
    using namespace geometrix;
    auto u1 = normalize(vector2{ 1,1 });
    auto v1 = left_normal(u1);
    auto ob1 = obb2{point2{0, 2}, u1, v1, 1.0, 1.0};

    auto u2 = normalize(vector2{ -1,1 });
    auto v2 = left_normal(u2);
    auto r = ob1.get_rectangle();
    ignore_unused_warning_of(r);
    auto ob2 = obb2{point2{2.0 * sqrt(2.0), 2.0}, u2, v2, 1.0, 1.0};
    auto result = obb_obb_intersection(ob1, ob2, cmp);
    BOOST_CHECK(result);
}

BOOST_FIXTURE_TEST_CASE(united_obb_obb_along_forty_fives_corners_touching, geometry_kernel_2d_units_fixture)
{
    using namespace geometrix;
    auto u1 = normalize(vector2{ 1 * boost::units::si::meters, 1 * boost::units::si::meters });
    auto v1 = left_normal(u1);
    auto ob1 = obb2{point2{0* boost::units::si::meters, 2* boost::units::si::meters}, u1, v1, 1.0* boost::units::si::meters, 1.0* boost::units::si::meters};

    auto u2 = normalize(vector2{ -1* boost::units::si::meters,1* boost::units::si::meters });
    auto v2 = left_normal(u2);
    auto ob2 = obb2{point2{2.0 * sqrt(2.0) * boost::units::si::meters, 2.0* boost::units::si::meters}, u2, v2, 1.0* boost::units::si::meters, 1.0* boost::units::si::meters};
    auto result = obb_obb_intersection(ob1, ob2, cmp);
    BOOST_CHECK(result);
}

#include <geometrix/utility/scope_timer.ipp>

#ifdef NDEBUG
#define GEOMETRIX_NUMBER_RUNS 1000000
#else
#define GEOMETRIX_NUMBER_RUNS 10
#endif

#include "oriented_bounding_box_2d.hpp"
BOOST_FIXTURE_TEST_CASE(OBB_custom_timing, geometry_kernel_2d_units_fixture)
{
    using namespace geometrix;
    auto u1 = normalize(vector2{ 1 * boost::units::si::meters, 1 * boost::units::si::meters });
    auto v1 = left_normal(u1);
    auto ob1 = obb2{point2{0* boost::units::si::meters, 2* boost::units::si::meters}, u1, v1, 1.0* boost::units::si::meters, 1.0* boost::units::si::meters};

	polygon2 pob1 = { ob1[0], ob1[1], ob1[2], ob1[3] };
	auto obb_1 = oriented_bounding_box_2d<point2>(pob1);

    auto u2 = normalize(vector2{ -1* boost::units::si::meters,1* boost::units::si::meters });
    auto v2 = left_normal(u2);
    auto ob2 = obb2{point2{sqrt(2.0) * boost::units::si::meters, 2.0* boost::units::si::meters}, u2, v2, 1.0* boost::units::si::meters, 1.0* boost::units::si::meters};
	polygon2 pob2 = { ob2[0], ob2[1], ob2[2], ob2[3] };
	auto obb_2 = oriented_bounding_box_2d<point2>(pob2);
	std::array<bool, GEOMETRIX_NUMBER_RUNS> results;
	{
		GEOMETRIX_MEASURE_SCOPE_TIME("custom_obb_hit");
		for (auto i = std::size_t{}; i < results.size(); ++i) 
			results[i] = obb_1.intersects(obb_2, cmp);
	}
	
	for (auto i = std::size_t{}; i < results.size(); ++i)
		BOOST_CHECK(results[i]);
}

BOOST_FIXTURE_TEST_CASE(OBB_custom_timing_miss, geometry_kernel_2d_units_fixture)
{
    using namespace geometrix;
    auto u1 = normalize(vector2{ 1 * boost::units::si::meters, 1 * boost::units::si::meters });
    auto v1 = left_normal(u1);
    auto ob1 = obb2{point2{0* boost::units::si::meters, 2* boost::units::si::meters}, u1, v1, 1.0* boost::units::si::meters, 1.0* boost::units::si::meters};

	polygon2 pob1 = { ob1[0], ob1[1], ob1[2], ob1[3] };
	auto obb_1 = oriented_bounding_box_2d<point2>(pob1);

    auto u2 = normalize(vector2{ -1* boost::units::si::meters,1* boost::units::si::meters });
    auto v2 = left_normal(u2);
    auto ob2 = obb2{point2{3.0 * sqrt(2.0) * boost::units::si::meters, 2.0* boost::units::si::meters}, u2, v2, 1.0* boost::units::si::meters, 1.0* boost::units::si::meters};
	polygon2 pob2 = { ob2[0], ob2[1], ob2[2], ob2[3] };
	auto obb_2 = oriented_bounding_box_2d<point2>(pob2);
	std::array<bool, GEOMETRIX_NUMBER_RUNS> results;
	{
		GEOMETRIX_MEASURE_SCOPE_TIME("custom_obb_miss");
		for (auto i = std::size_t{}; i < results.size(); ++i) 
			results[i] = obb_1.intersects(obb_2, cmp);
	}
	
	for (auto i = std::size_t{}; i < results.size(); ++i)
		BOOST_CHECK(!results[i]);
}

BOOST_FIXTURE_TEST_CASE(obb_timing, geometry_kernel_2d_units_fixture)
{
    using namespace geometrix;
    auto u1 = normalize(vector2{ 1 * boost::units::si::meters, 1 * boost::units::si::meters });
    auto v1 = left_normal(u1);
    auto ob1 = obb2{point2{0* boost::units::si::meters, 2* boost::units::si::meters}, u1, v1, 1.0* boost::units::si::meters, 1.0* boost::units::si::meters};

    auto u2 = normalize(vector2{ -1* boost::units::si::meters,1* boost::units::si::meters });
    auto v2 = left_normal(u2);
    auto ob2 = obb2{point2{sqrt(2.0) * boost::units::si::meters, 2.0* boost::units::si::meters}, u2, v2, 1.0* boost::units::si::meters, 1.0* boost::units::si::meters};
	std::array<bool, GEOMETRIX_NUMBER_RUNS> results;
	{
		GEOMETRIX_MEASURE_SCOPE_TIME("obb_hit");
		for (auto i = std::size_t{}; i < results.size(); ++i)
			results[i] = obb_obb_intersection(ob1, ob2, cmp);
	}
	
	for (auto i = std::size_t{}; i < results.size(); ++ i)
		BOOST_CHECK(results[i]);
}

BOOST_FIXTURE_TEST_CASE(obb_timing_miss, geometry_kernel_2d_units_fixture)
{
    using namespace geometrix;
    auto u1 = normalize(vector2{ 1 * boost::units::si::meters, 1 * boost::units::si::meters });
    auto v1 = left_normal(u1);
    auto ob1 = obb2{point2{0* boost::units::si::meters, 2* boost::units::si::meters}, u1, v1, 1.0* boost::units::si::meters, 1.0* boost::units::si::meters};

    auto u2 = normalize(vector2{ -1* boost::units::si::meters,1* boost::units::si::meters });
    auto v2 = left_normal(u2);
    auto ob2 = obb2{point2{3.0 * sqrt(2.0) * boost::units::si::meters, 2.0* boost::units::si::meters}, u2, v2, 1.0* boost::units::si::meters, 1.0* boost::units::si::meters};
	std::array<bool, GEOMETRIX_NUMBER_RUNS> results;
	{
		GEOMETRIX_MEASURE_SCOPE_TIME("obb_miss");
		for (auto i = std::size_t{}; i < results.size(); ++i)
			results[i] = obb_obb_intersection(ob1, ob2, cmp);
	}
	
	for (auto i = std::size_t{}; i < results.size(); ++ i)
		BOOST_CHECK(!results[i]);
}


BOOST_FIXTURE_TEST_CASE(test_oriented_intersection, geometry_kernel_2d_units_fixture)
{
    using namespace geometrix;
    auto A = point2{ -1.0 * boost::units::si::meters, 0.0 * boost::units::si::meters };
    auto B = point2{ 1.0 * boost::units::si::meters, 0.0 * boost::units::si::meters };
    auto C = point2{ 0.0 * boost::units::si::meters, -1.0 * boost::units::si::meters };
    auto D = point2{ 0.0 * boost::units::si::meters, 1.0 * boost::units::si::meters };

	BOOST_CHECK(oriented_intersection(A, B, C, D, cmp) == e_intersect_left_right);
	BOOST_CHECK(oriented_intersection(B, A, C, D, cmp) == e_intersect_right_left);
	BOOST_CHECK(oriented_intersection(A, B, D, C, cmp) == e_intersect_right_left);
	BOOST_CHECK(oriented_intersection(B, A, D, C, cmp) == e_intersect_left_right);
	BOOST_CHECK(oriented_intersection(A, B, A, B, cmp) == e_overlapping);
	BOOST_CHECK(oriented_intersection(A, D, C, B, cmp) == e_non_crossing);

    auto E = point2{ -0.5 * boost::units::si::meters, 0.0 * boost::units::si::meters };
    auto F = point2{ -0.8 * boost::units::si::meters, 0.0 * boost::units::si::meters };
	BOOST_CHECK(oriented_intersection(A, F, C, D, cmp) == e_non_crossing);
	BOOST_CHECK(oriented_intersection(A, F, E, B, cmp) == e_non_crossing);

	BOOST_CHECK(oriented_intersection(A, F, E, F, cmp) == e_endpoint);
	BOOST_CHECK(oriented_intersection(A, F, F, B, cmp) == e_endpoint);

}

#endif //GEOMETRIX_INTERSECTION_TESTS_HPP
