//
//! Copyright ? 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POINT_OBB_DISTANCE_HPP
#define GEOMETRIX_ALGORITHM_POINT_OBB_DISTANCE_HPP
#pragma once

#include <geometrix/primitive/oriented_bounding_box.hpp>
#include <geometrix/primitive/point_traits.hpp>

namespace geometrix {

    namespace result_of
    {
        template <typename Point, typename PointOBB, typename VectorOBB>
        struct point_obb_distance_sqrd
        {
        private:
            using length_t = typename geometric_traits<Point>::arithmetic_type;
        public:
            using type = decltype(length_t() * length_t());
        };
    }

    template <typename Point, typename PointOBB, typename VectorOBB>
    inline typename result_of::point_obb_distance_sqrd<Point, PointOBB, VectorOBB>::type point_obb_distance_sqrd(const Point& p, const oriented_bounding_box<PointOBB, VectorOBB>& b)
    {
        using length_t = typename geometric_traits<Point>::arithmetic_type;
        using area_t = decltype(length_t() * length_t());
        using vector_t = vector<length_t, 2>;
        vector_t v = p - b.get_center();
        area_t sqDist = construct<area_t>(0);
        {
            // Project vector from box center to p on each axis, getting the distance
            // of p along that axis, and count any excess distance outside box extents
            length_t d = scalar_projection(v, b.get_axis(1));
            length_t excess = construct<length_t>(0);
            if (d < -b.get_halfwidth(1))
                excess=d+b.get_halfwidth(1);
            else if (d > b.get_halfwidth(1))
                excess=d-b.get_halfwidth(1);
            sqDist += excess * excess;
        }
        {
            // Project vector from box center to p on each axis, getting the distance
            // of p along that axis, and count any excess distance outside box extents
            length_t d = scalar_projection(v, b.get_axis(0));
            length_t excess = construct<length_t>(0);
            if (d < -b.get_halfwidth(0))
                excess = d + b.get_halfwidth(0);
            else if (d > b.get_halfwidth(0))
                excess = d - b.get_halfwidth(0);
            sqDist += excess * excess;
        }

        return sqDist;
    }

    namespace result_of
    {
        template <typename Point, typename PointOBB, typename VectorOBB>
        struct point_obb_distance
        {
            using type = typename geometric_traits<Point>::arithmetic_type;
        };
    }

    template <typename Point, typename PointOBB, typename VectorOBB>
    inline typename result_of::point_obb_distance<Point, PointOBB, VectorOBB>::type point_obb_distance(const Point& p, const oriented_bounding_box<PointOBB, VectorOBB>& b)
    {
        using std::sqrt;
        return sqrt(point_obb_distance_sqrd(p, b));
    }

    namespace result_of
    {
        template <typename Point>
        struct point_obb_closest_point
        {
            using length_t = typename arithmetic_type_of<Point>::type;
            using type = point<length_t, dimension_of<Point>::value>;
        };
    }
    
    // Given point p, return point q on (or in) OBB b, closest to p
    template <typename Point, typename PointOBB, typename VectorOBB>
    inline typename result_of::point_obb_closest_point<Point>::type point_obb_closest_point(const Point& p, const oriented_bounding_box<PointOBB, VectorOBB>& b)
    {
        using length_t = typename arithmetic_type_of<Point>::type;
        using vector_t = vector<length_t, dimension_of<Point>::value>;
        using point_t = point<length_t, dimension_of<Point>::value>;
        
        //! Start result at center of box; make steps from there
        auto q = construct<point_t>(b.get_center());
        vector_t d = p - b.get_center();

        //! For each OBB axis
        {
            //! project d onto that axis to get the distance
            //! along the axis of d from the box center
            length_t dist = scalar_projection(d, b.get_axis(0));
            //! If distance farther than the box extents, clamp to the box
            if (dist > b.get_halfwidth(0))
                dist = b.get_halfwidth(0);
            if (dist < -b.get_halfwidth(0))
                dist = -b.get_halfwidth(0);
            //! Step that distance along the axis to get world coordinate
            assign(q, q + dist * b.get_axis(0));
        }
        {
            //! ...project d onto that axis to get the distance
            //! along the axis of d from the box center
            length_t dist = scalar_projection(d, b.get_axis(1));
            //! If distance farther than the box extents, clamp to the box
            if (dist > b.get_halfwidth(1))
                dist = b.get_halfwidth(1);
            if (dist < -b.get_halfwidth(1))
                dist = -b.get_halfwidth(1);
            //! Step that distance along the axis to get world coordinate
            assign(q, q + dist * b.get_axis(1));
        }

        return q;
    }
}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_POINT_OBB_DISTANCE_HPP
