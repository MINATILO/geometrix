//
//! Copyright ? 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MOVING_SPHERE_POLYGON_INTERSECTION_HPP
#define GEOMETRIX_MOVING_SPHERE_POLYGON_INTERSECTION_HPP

#include <geometrix/primitive/sphere_traits.hpp>
#include <geometrix/primitive/polygon_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <cstdint>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

	struct moving_sphere_polygon_intersection_result
	{
		typedef void (moving_sphere_polygon_intersection_result::*bool_type)() const;

		moving_sphere_polygon_intersection_result()
		{}

		moving_sphere_polygon_intersection_result( bool isIntersecting, bool isPenetrating, bool isMovingAway )
			: result( (isIntersecting ? e_is_intersecting : 0) | (isPenetrating ? e_is_penetrating : 0) | (isMovingAway ? e_is_moving_away : 0) )
		{}

		bool is_intersecting() const { return (result & e_is_intersecting) != 0; }
		bool is_penetrating() const { return (result & e_is_penetrating) != 0; }
		bool is_touching() const { return is_intersecting() && !is_penetrating(); }
		bool is_moving_away() const { return (result & e_is_moving_away) != 0; }

		operator bool_type() const {
			return is_intersecting() ? &moving_sphere_polygon_intersection_result::bool_type_mfn : 0;
		}

	private:

		void bool_type_mfn() const{}

		enum Flags
		{
			  e_is_intersecting = 1
			, e_is_penetrating = 2
			, e_is_moving_away = 4
		};

		std::uint32_t result{0};

	};

	// Intersect sphere s with movement vector v with polygon p. If intersecting 
	// return time t of collision and point q at which sphere hits polygon 
	// If already intersecting, q is the closest point between the center of the sphere
	// and the polygon.
	template <typename Sphere, typename Vector, typename Plane, typename ArithmeticType, typename Point, typename NumberComparisonPolicy>
	inline moving_sphere_polygon_intersection_result moving_sphere_polygon_intersection( const Sphere& s, const Vector& velocity, const Plane& p, ArithmeticType &t, Point& q, const NumberComparisonPolicy& cmp )
	{
		//! NOT IMPLEMENTED.
		GEOMETRIX_ASSERT(false);
		return moving_sphere_polygon_intersection_result();
	}
}//namespace geometrix;

#endif //GEOMETRIX_MOVING_SPHERE_POLYGON_INTERSECTION_HPP
