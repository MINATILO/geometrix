//
//! Copyright � 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_IS_SEGMENT_IN_RANGE_HPP
#define GEOMETRIX_IS_SEGMENT_IN_RANGE_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/exterior_product.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algorithm/line_intersection.hpp>
#include <geometrix/algorithm/intersection/ray_segment_intersection.hpp>

namespace geometrix {

	//! Test if a segment intersects the cone defined by two rays from a common origin.
	template <typename Vector1, typename Vector2, typename Segment, typename Point>
	inline bool is_segment_in_range_2d( const Segment& segment, const Vector1& lo, const Vector2& hi, const Point& origin )
	{
		using namespace geometrix;

		BOOST_CONCEPT_ASSERT( (Vector2DConcept<Vector1>) );
		BOOST_CONCEPT_ASSERT( (Vector2DConcept<Vector2>) );
		BOOST_CONCEPT_ASSERT( (Vector2DConcept<Point>) );
		using segment_point_type = typename geometric_traits<Segment>::point_type;
		using length_t = typename select_arithmetic_type_from_sequences<segment_point_type, Point>::type;
		using area_t = decltype(length_t() * length_t());
		typedef vector<length_t, 2> vector_type;

		vector_type vSegStart = get_start(segment) - origin;
		vector_type vSegEnd = get_end(segment) - origin;

		const auto detLoSegStart = exterior_product_area( lo, vSegStart );
		const auto detHiSegStart = exterior_product_area( hi, vSegStart );

		//! Are either points inside the range
		if( detLoSegStart >= constants::zero<area_t>() && detHiSegStart <= constants::zero<area_t>() )
			return true;

		const auto detLoSegEnd = exterior_product_area( lo, vSegEnd );
		const auto detHiSegEnd = exterior_product_area( hi, vSegEnd );

		if( detLoSegEnd >= constants::zero<area_t>() && detHiSegEnd <= constants::zero<area_t>())
			return true;

		//! Are both points outside the range?
		const auto dotHiSegStart = dot_product( hi, vSegStart );
		const auto dotLoSegEnd = dot_product( lo, vSegEnd );

		//! Check if start is outside of the hi end and end is outside the lo end.
		if( detHiSegStart >= constants::zero<area_t>() && detLoSegEnd < constants::zero<area_t>() && dotHiSegStart > constants::zero<area_t>() && dotLoSegEnd > constants::zero<area_t>())
			return true;

		const auto dotLoSegStart = dot_product( lo, vSegStart );
		const auto dotHiSegEnd = dot_product( hi, vSegEnd );

		//! Check if end is outside the hi end and start is outside the lo end.
		if( detHiSegEnd >= constants::zero<area_t>() && detLoSegStart < constants::zero<area_t>() && dotHiSegEnd > constants::zero<area_t>() && dotLoSegStart > constants::zero<area_t>())
			return true;

		//! Special case where both segment endpoints lay on a range vector.
		//! In that case the segment either is inside the range or outside.
		if( detHiSegStart == constants::zero<area_t>() && detLoSegEnd == constants::zero<area_t>())
			return get_orientation( get_start(segment), get_end(segment), origin, absolute_tolerance_comparison_policy<length_t>( constants::zero<length_t>() ) ) != oriented_left;
		
		if( detHiSegEnd == constants::zero<area_t>() && detLoSegStart == constants::zero<area_t>())
			return get_orientation( get_start(segment), get_end(segment), origin, absolute_tolerance_comparison_policy<length_t>(constants::zero<length_t>()) ) != oriented_right;
		
		//! Test the intersections
		absolute_tolerance_comparison_policy<length_t> cmp(constants::zero<length_t>());
		return ray_segment_intersection(origin, lo, segment, cmp) != e_non_crossing || ray_segment_intersection(origin, hi, segment, cmp) != e_non_crossing;
	}

	//! Test if a segment intersects the cone defined by two rays from a common origin.
	template <typename Vector1, typename Vector2, typename Segment, typename Point, typename NumberComparisonPolicy>
	inline bool is_segment_in_range_2d( const Segment& segment, const Vector1& lo, const Vector2& hi, const Point& origin, Point* xPoints, const NumberComparisonPolicy& cmp )
	{
		using namespace geometrix;

		BOOST_CONCEPT_ASSERT( (Vector2DConcept<Vector1>) );
		BOOST_CONCEPT_ASSERT( (Vector2DConcept<Vector2>) );
		BOOST_CONCEPT_ASSERT( (Vector2DConcept<Point>) );
		typedef typename geometric_traits<Segment>::point_type segment_point_type;
		typedef typename select_arithmetic_type_from_sequences<segment_point_type, Point>::type length_t;
		using area_t = decltype(length_t() * length_t());
		typedef vector<length_t, 2> vector_type;

		vector_type vSegStart = get_start( segment ) - origin;
		vector_type vSegEnd = get_end( segment ) - origin;

		const auto detLoSegStart = exterior_product_area( lo, vSegStart );
		const auto detHiSegStart = exterior_product_area( hi, vSegStart );
		const auto detLoSegEnd = exterior_product_area( lo, vSegEnd );
		const auto detHiSegEnd = exterior_product_area( hi, vSegEnd );

		//! Are either points inside the range
		bool startIn = detLoSegStart >= constants::zero<area_t>() && detHiSegStart <= constants::zero<area_t>();
		bool endIn = detLoSegEnd >= constants::zero<area_t>() && detHiSegEnd <= constants::zero<area_t>();

		//! If both are inside... done.
		if( startIn && endIn )
		{
			xPoints[0] = get_start( segment );
			xPoints[1] = get_end( segment );
			return true;
		}

		if( startIn != endIn )
		{
			Point xPointLo, xPointHi;
			intersection_type loIType = line_segment_intersect( origin, origin + lo, segment, xPointLo, cmp );			
			if( loIType == e_crossing )
			{
				xPoints[0] = xPointLo;
				xPoints[1] = startIn ? get_start( segment ) : get_end( segment );
				return true;
			}
			else if( loIType == e_overlapping )
			{
				xPoints[0] = get_start( segment );
				xPoints[1] = get_start( segment );
				return true;
			}

			intersection_type hiIType = line_segment_intersect( origin, origin + hi, segment, xPointHi, cmp );
			if( (hiIType == e_crossing || hiIType == e_endpoint) )
			{
				if( loIType == e_endpoint )
					xPoints[0] = xPointLo;
				else
					xPoints[0] = startIn ? get_start( segment ) : get_end( segment );
				xPoints[1] = xPointHi;				
				return true;
			}
			else if( hiIType == e_overlapping )
			{
				xPoints[0] = get_start( segment );
				xPoints[1] = get_start( segment );
				return true;
			}

			return false;
		}

		//! Are both points outside the range?
		const auto dotHiSegStart = dot_product( hi, vSegStart );
		const auto dotLoSegEnd = dot_product( lo, vSegEnd );
		const auto dotLoSegStart = dot_product( lo, vSegStart );
		const auto dotHiSegEnd = dot_product( hi, vSegEnd );

		//! Check if start is outside of the hi end and end is outside the lo end.
		//! or check if end is outside the hi end and start is outside the lo end.
		if( ( detHiSegStart >= constants::zero<area_t>() && detLoSegEnd < constants::zero<area_t>() && dotHiSegStart > constants::zero<area_t>() && dotLoSegEnd > constants::zero<area_t>() ) ||
			( detHiSegEnd >= constants::zero<area_t>() && detLoSegStart < constants::zero<area_t>() && dotHiSegEnd > constants::zero<area_t>() && dotLoSegStart > constants::zero<area_t>() ) )
		{
			//! Crosses both in this case.
			Point xPointLo, xPointHi;
			intersection_type loIType = line_segment_intersect( origin, origin + lo, segment, xPointLo, cmp );
			intersection_type hiIType = line_segment_intersect( origin, origin + hi, segment, xPointHi, cmp );
			if( (loIType == e_crossing || loIType == e_endpoint) && (hiIType == e_crossing || hiIType == e_endpoint) )
			{
				xPoints[0] = xPointLo;
				xPoints[1] = xPointHi;
				return true;
			}
			else if( loIType == e_overlapping || hiIType == e_overlapping )
			{
				xPoints[0] = get_start( segment );
				xPoints[1] = get_start( segment );
				return true;
			}
		}
		
		//! Special case where both segment endpoints lay on a range vector.
		//! In that case the segment either is inside the range or outside.
		if( ( detHiSegStart == constants::zero<area_t>() && detLoSegEnd == constants::zero<area_t>() && get_orientation( get_start( segment ), get_end( segment ), origin, absolute_tolerance_comparison_policy<length_t>(constants::zero<length_t>()) ) != oriented_left )  ||
			( detHiSegEnd == constants::zero<area_t>() && detLoSegStart == constants::zero<area_t>() && get_orientation( get_start( segment ), get_end( segment ), origin, absolute_tolerance_comparison_policy<length_t>(constants::zero<length_t>()) ) != oriented_right ) )
		{
			xPoints[0] = get_start( segment );
			xPoints[1] = get_start( segment );
			return true;
		}

		//! The segment falls outside of the range.
		return false;
	}

}//namespace geometrix;

#endif //GEOMETRIX_IS_SEGMENT_IN_RANGE_HPP
