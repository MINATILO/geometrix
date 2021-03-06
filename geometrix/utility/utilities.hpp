//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_UTILITIES_HPP
#define GEOMETRIX_UTILITIES_HPP
#pragma once

#include <geometrix/numeric/constants.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/tensor/numeric_sequence_compare.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/algebra/exterior_product.hpp>
#include <boost/concept_check.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/fusion/include/all.hpp>
#include <geometrix/algorithm/orientation/point_segment_orientation.hpp>

namespace geometrix {

    namespace result_of {

        template <typename A, typename B>
        struct angle_from_a_to_b
        {
        private:
            typedef decltype(typename type_at<A, 0>::type() - typename type_at<B, 0>::type()) xtype;
            typedef decltype(typename type_at<A, 1>::type() - typename type_at<B, 1>::type()) ytype;
        public:
            typedef decltype(atan2(std::declval<ytype>(), std::declval<xtype>())) type;
        };

    }//! namespace result_of;

    //! Function to get the angle from an origin to a target point in the 2D XY plane.
    template <typename CoordinateSequenceA, typename CoordinateSequenceB>
    inline typename result_of::angle_from_a_to_b<CoordinateSequenceA, CoordinateSequenceB>::type
        angle_from_a_to_b( const CoordinateSequenceA& A,
                           const CoordinateSequenceB& B,
                       typename boost::enable_if_c
                       <
                           geometric_traits<CoordinateSequenceA>::dimension_type::value == 2 &&
                           geometric_traits<CoordinateSequenceB>::dimension_type::value == 2
                       > ::type* = 0 )
    {
        using std::atan2;
        return atan2( get<1>( B ) - get<1>( A ), get<0>( B ) - get<0>( A ) );
    }

    namespace result_of {

        template <typename Vector>
        struct vector_angle
        {
        private:
            typedef decltype(typename type_at<Vector, 0>::type()) xtype;
            typedef decltype(typename type_at<Vector, 1>::type()) ytype;
        public:
            typedef decltype(atan2(std::declval<ytype>(), std::declval<xtype>())) type;
        };

    }//! namespace result_of;

    //! Return the angle in which the specified vector points.
    template <typename Vector>
    inline typename result_of::vector_angle<Vector>::type
        vector_angle(const Vector& v)
    {
        BOOST_CONCEPT_ASSERT((Vector2DConcept<Vector>));
        using std::atan2;
        return atan2(get<1>(v), get<0>(v));
    }

    //! Function to normalize an angle to within the interval [0,2*PI]
    template <typename CoordinateType>
    inline void normalize_angle_0_2pi(CoordinateType& angle)
    {
        //simplifies the angle to lay in the range of the interval 0 - 2*pi
        CoordinateType twoPI = constants::two_pi<CoordinateType>();
        if (angle > twoPI || angle < constants::zero<CoordinateType>())
        {
            using std::floor;
            auto n = floor(angle / twoPI);
            if (n != constants::zero<decltype(n)>())
                angle -= twoPI * n;
            if (angle > twoPI)
                angle -= twoPI;
            else if (angle < constants::zero<CoordinateType>())
                angle += twoPI;
        }
    }

    //! Function to normalize a copy of a given angle to within the interval [0,2*PI] and return the normalized value.
    template <typename CoordinateType>
    inline CoordinateType normalize_angle_0_2pi_copy(CoordinateType angle)
    {
        //simplifies the angle to lay in the range of the interval 0 - 2*pi
        normalize_angle_0_2pi(angle);
        return angle;
    }

    //! Function to normalize an angle to within the interval [-PI,PI]
    template <typename CoordinateType>
    inline void normalize_angle_minus_pi_to_pi( CoordinateType& angle )
    {
        //simplifies the angle to lay in the range of the interval 0 - 2*pi first and then into the -pi,pi range.
        normalize_angle_0_2pi(angle);

        CoordinateType pi = constants::pi<CoordinateType>();
        CoordinateType twoPI = constants::two_pi<CoordinateType>();
        if( angle > pi )
            angle -= twoPI;
        else if( angle <= geometrix::get(-pi) )
            angle += twoPI;
    }

    //! Function to normalize a copy of a given angle to within the interval [-PI,PI] and return the normalized value.
    template <typename CoordinateType>
    inline CoordinateType normalize_angle_minus_pi_to_pi_copy( CoordinateType angle )
    {
        //simplifies the angle to lay in the range of the interval 0 - 2*pi        
        normalize_angle_minus_pi_to_pi(angle);
        return angle;
    }

    //! Function to determine if 3 points are collinear in the 2D XY plane.
    //! From Computational Geometry in C by J. O'Rourke.
    template <typename PointA, typename PointB, typename PointC, typename NumberComparisonPolicy>
    inline bool is_collinear( const PointA& A,
                       const PointB& B,
                       const PointC& C,
                       const NumberComparisonPolicy& compare,
                       typename boost::enable_if_c
                       <
                           geometric_traits<PointA>::dimension_type::value == 2 &&
                           geometric_traits<PointB>::dimension_type::value == 2 &&
                           geometric_traits<PointC>::dimension_type::value == 2
                       > ::type* = 0 )
    {
        auto det = exterior_product_area( B-A, C-A );
        return compare.equals( det, constants::zero<decltype(det)>() );//Absolute tolerance checks are fine for Zero checks.
    }
    
    template <typename PointA, typename PointB, typename PointC, typename NumberComparisonPolicy>
    inline bool is_collinear( const PointA& A,
                       const PointB& B,
                       const PointC& C,
                       const NumberComparisonPolicy& compare,
                       typename boost::enable_if_c
                       <
                           geometric_traits<PointA>::dimension_type::value == 3 &&
                           geometric_traits<PointB>::dimension_type::value == 3 &&
                           geometric_traits<PointC>::dimension_type::value == 3
                       > ::type* = 0 )
    {
        using length_t = typename arithmetic_type_of<PointA>::type;
        using vector_t = vector<length_t, dimension_of<PointA>::value>;
        using cross_result_t = typename result_of::cross_product<vector_t, vector_t>::type;
		auto d = cross_result_t(( B - A ) ^ ( C - A ));
		return compare.equals( get<0>( d ), constants::zero<typename type_at<cross_result_t, 0>::type>() )
			&& compare.equals( get<1>( d ), constants::zero<typename type_at<cross_result_t, 0>::type>() )
			&& compare.equals( get<2>( d ), constants::zero<typename type_at<cross_result_t, 0>::type>() );
    }

    //! Function to determine if Point C is between points A-B
    //! From Computational Geometry in C by J. O'Rourke.
    template <typename PointA, typename PointB, typename PointC, typename NumberComparisonPolicy>
    inline bool is_between( const PointA& A,
                     const PointB& B,
                     const PointC& C,
                     bool includeBounds,
                     const NumberComparisonPolicy& compare,
                     typename boost::enable_if_c
                     <
                        geometric_traits<PointA>::dimension_type::value == 2 &&
                        geometric_traits<PointB>::dimension_type::value == 2 &&
                        geometric_traits<PointC>::dimension_type::value == 2
                     >::type* = 0 )
    {
        return is_collinear( A, B, C, compare ) && is_collinear_point_between( A, B, C, includeBounds, compare );
    }

    //! Function to determine if Point C is between points A-B where C is already determined to be collinear to A-B.
    //! From Computational Geometry in C by J. O'Rourke.
    template <typename PointA, typename PointB, typename PointC, typename NumberComparisonPolicy>
    inline bool is_collinear_point_between( const PointA& A,
        const PointB& B,
        const PointC& C,
        bool includeBounds,
        const NumberComparisonPolicy& compare,
        typename boost::enable_if_c
        <
        geometric_traits<PointA>::dimension_type::value == 2 &&
        geometric_traits<PointB>::dimension_type::value == 2 &&
        geometric_traits<PointC>::dimension_type::value == 2
        > ::type* = 0 )
    {
        GEOMETRIX_ASSERT( is_collinear( A, B, C, compare ) );

        //If AB not vertical, check between on x; else on y.
        bool ABVertical = compare.equals( get<0>( A ), get<0>( B ) );
        if( !ABVertical )
        {
            if( includeBounds )
            {
                return ((compare.less_than_or_equal( get<0>( A ), get<0>( C ) ) && compare.less_than_or_equal( get<0>( C ), get<0>( B ) )) ||
                    (compare.greater_than_or_equal( get<0>( A ), get<0>( C ) ) && compare.greater_than_or_equal( get<0>( C ), get<0>( B ) )));
            }
            else
            {
                return ((compare.less_than( get<0>( A ), get<0>( C ) ) && compare.less_than( get<0>( C ), get<0>( B ) )) ||
                    (compare.greater_than( get<0>( A ), get<0>( C ) ) && compare.greater_than( get<0>( C ), get<0>( B ) )));
            }
        }
        else
        {
            if( includeBounds )
            {
                return ((compare.less_than_or_equal( get<1>( A ), get<1>( C ) ) && compare.less_than_or_equal( get<1>( C ), get<1>( B ) )) ||
                    (compare.greater_than_or_equal( get<1>( A ), get<1>( C ) ) && compare.greater_than_or_equal( get<1>( C ), get<1>( B ) )));
            }
            else
            {
                return ((compare.less_than( get<1>( A ), get<1>( C ) ) && compare.less_than( get<1>( C ), get<1>( B ) )) ||
                    (compare.greater_than( get<1>( A ), get<1>( C ) ) && compare.greater_than( get<1>( C ), get<1>( B ) )));
            }
        }
    }

    //! \brief Function to determine if vector c falls in between vectors a and b.
    //! This can be used to check angle ranges without using atan2.
    template <typename Vector1, typename Vector2, typename Vector3, typename NumberComparisonPolicy>
    inline bool is_vector_between(const Vector1& a, const Vector2& b, const Vector3& c, bool includeBounds, const NumberComparisonPolicy& cmp)
    {
        using namespace geometrix;
        BOOST_CONCEPT_ASSERT((Vector2DConcept<Vector1>));
        BOOST_CONCEPT_ASSERT((Vector2DConcept<Vector2>));
        BOOST_CONCEPT_ASSERT((Vector2DConcept<Vector3>));
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy, double>));

        BOOST_AUTO(const detcb, exterior_product_area(c, b));

        //! If b is along c bounds included it's between.
        if (cmp.equals(detcb, constants::zero<decltype(detcb)>()) && cmp.greater_than_or_equal(dot_product(b, c), constants::zero<decltype(dot_product(b,c))>()))
            return includeBounds;

        BOOST_AUTO(const detac, exterior_product_area(a, c));

        //! If a is along c and includeBounds it's between.
        if (cmp.equals(detac, constants::zero<decltype(detac)>()) && cmp.greater_than_or_equal(dot_product(a, c), constants::zero<decltype(dot_product(a,c))>()))
            return includeBounds;

        BOOST_AUTO(const detab, exterior_product_area(a, b));

        //! If b is along a, c can only be between if it is along a and included and that's handled above.
        if (cmp.equals(detab, constants::zero<decltype(detab)>()) && cmp.greater_than_or_equal(dot_product(b, a), constants::zero<decltype(dot_product(b,a))>()))
            return false;

        //! If detab and detac have the same sign, then b and c are on the same side of a and can be compared directly.
        if (cmp.greater_than_or_equal(detac * detab, constants::zero<decltype(detac*detab)>()))
        {
            //! Both are on same side of a; compare to each other.
            return cmp.greater_than(detcb, constants::zero<decltype(detcb)>());
        }

        //! At this point b and c straddle a. A negative determinant means a large angle WRT a.
        //! If c's is positive it must be between a and b, else the opposite must be true.
        return cmp.greater_than(detac, constants::zero<decltype(detac)>());
    }

    template <typename Point, typename NumberComparisonPolicy>
    inline bool is_vertical( const Point& start,
                      const Point& end,
                      const NumberComparisonPolicy& compare )
    {
        return compare.equals( get<0>( start ), get<0>( end ) );
    }

    template <typename Segment, typename NumberComparisonPolicy>
    inline bool is_vertical( const Segment& s,
                      const NumberComparisonPolicy& compare )
    {
        return is_vertical( get_start( s ),
                            get_end( s ),
                            compare );
    }

    template <typename Point, typename NumberComparisonPolicy>
    inline bool is_horizontal( const Point& start, const Point& end, const NumberComparisonPolicy& compare )
    {
        return compare.equals( get<1>( start ),
                               get<1>( end ) );
    }

    template <typename Segment, typename NumberComparisonPolicy>
    inline bool is_horizontal( const Segment& s,const NumberComparisonPolicy& compare )
    {
        return is_horizontal( get_start( s ),
                              get_end( s ),
                              compare );
    }

    //! function to get the slope defined by two points
    template <typename Point>
    inline typename geometric_traits< Point >::arithmetic_type
        get_slope( const Point& s_start, const Point& s_end )
    {
        return arithmetic_promote(get<1>( s_end ) - get<1>( s_start )) / (get<0>( s_end )-get<0>( s_start ));
    }

    //! function to get the slope defined by a segment.
    template <typename Segment>
    inline typename geometric_traits< typename geometric_traits< Segment >::point_type >::arithmetic_type get_slope( const Segment& s )
    {
        return get_slope( get_start( s ), get_end ( s ) );
    }

    //! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
    template <typename Point, typename CoordinateType>
    inline CoordinateType y_of_x( const Point& s_start,
                           const Point& s_end,
                           CoordinateType x,
                           CoordinateType& slope )
    {
        BOOST_AUTO( x0, get<0>( s_start ) );
        BOOST_AUTO( x1, get<0>( s_end ) );
        BOOST_AUTO( y0, get<1>( s_start ) );
        BOOST_AUTO( y1, get<1>( s_end ) );
        slope = arithmetic_promote( (y1-y0) )/(x1-x0);
        return (x - x0) * slope + y0;
    }

    //! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
    template <typename Point, typename CoordinateType>
    inline CoordinateType y_of_x( const Point& s_start,
                           const Point& s_end,
                           CoordinateType x )
    {
        CoordinateType slope;
        return y_of_x( s_start, s_end, x, slope );
    }

    //! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
    template <typename Point, typename CoordinateType>
    inline CoordinateType x_of_y( const Point& s_start,
                           const Point& s_end,
                           CoordinateType y,
                           CoordinateType& slope )
    {
        CoordinateType y0, y1, x0, x1;

        x0 = get<0>( s_start );
        x1 = get<0>( s_end );
        y0 = get<1>( s_start );
        y1 = get<1>( s_end );

        slope = (y1-y0)/(x1-x0);

        CoordinateType x = (y - y0)/slope + x0;
        return x;
    }

    //! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
    template <typename Point, typename CoordinateType>
    inline CoordinateType x_of_y( const Point& s_start,
                           const Point& s_end,
                           CoordinateType y )
    {
        CoordinateType slope;
        return x_of_y( s_start, s_end, y, slope );
    }

    template <typename T1, typename T2>
    inline bool lexicographical_compare(const T1& t1, const T2& t2)
    {
        return t1 < t2;
    }

    template <typename T1, typename T2, typename U, typename ...Params>
    inline bool lexicographical_compare(const T1& t1, const T2& t2, const U& u, const Params&... p)
    {
        return !(t2 < t1) && (t1 < t2 || lexicographical_compare(u, p...));
    }

    namespace detail
    {
        template <std::size_t D>
        struct lexicographical
        {
            template <typename NumericSequence, typename NumberComparisonPolicy>
            static bool compare( const NumericSequence& lhs, const NumericSequence& rhs, const NumberComparisonPolicy& nCompare )
            {
                if( nCompare.less_than( get<dimension_of<NumericSequence>::value - D>( lhs ), get<dimension_of<NumericSequence>::value - D>( rhs ) ) )
                    return true;
                else if( nCompare.equals( get<dimension_of<NumericSequence>::value - D>( lhs ), get<dimension_of<NumericSequence>::value - D>( rhs ) ) )
                    return lexicographical<D-1>::compare( lhs, rhs, nCompare );
                else
                    return false;
            }
        };

        template <>
        struct lexicographical<0>
        {
            template <typename NumericSequence, typename NumberComparisonPolicy>
            static bool compare( const NumericSequence&, const NumericSequence&, const NumberComparisonPolicy& )
            {
                return false;//all were equal.
            }
        };
    }

    //! Lexicographical compare functor for Cartesian points. Sorts first in X and then in Y (then Z).
    template <typename NumberComparisonPolicy>
    class lexicographical_comparer
    {
        template <typename NumericSequence1, typename NumericSequence2>
        struct comparer
        {
            static bool compare( const NumericSequence1& lhs, const NumericSequence2& rhs, const NumberComparisonPolicy& nCompare )
            {
                return detail::lexicographical<dimension_of<NumericSequence1>::value>::compare( lhs, rhs, nCompare );
            }
        };

    public:

        lexicographical_comparer(){}
        lexicographical_comparer( const NumberComparisonPolicy& compare )
            : m_compare( compare )
        {}

        template <typename NumericSequence1, typename NumericSequence2>
        bool operator()( const NumericSequence1& p1, const NumericSequence2& p2 ) const
        {
            return comparer<NumericSequence1, NumericSequence2>::compare( p1, p2, m_compare );
        }

    private:

        NumberComparisonPolicy m_compare;

    };

    //! Test if lhs sequence is lexicographically less than rhs sequence.
    template <typename NumericSequence1, typename NumericSequence2, typename NumberComparisonPolicy>
    inline bool lexicographically_less_than(const NumericSequence1& lhs, const NumericSequence2& rhs, const NumberComparisonPolicy& cmp)
    {
        return lexicographical_comparer<NumberComparisonPolicy>(cmp)(lhs, rhs);
    }

    //! Lexicographical compare functor for points - reversed to sort in (Z), then Y then X.
    template <typename NumberComparisonPolicy>
    struct reverse_lexicographical_point_compare
    {
        reverse_lexicographical_point_compare(){}
        reverse_lexicographical_point_compare( const NumberComparisonPolicy& compare )
            : m_compare( compare )
        {}

        //! older compilers require disambiguation
        template <int> struct disambiguation_tag { disambiguation_tag(int) {} };

        template <typename Point>
        bool operator()( const Point& p1, const Point& p2, typename boost::enable_if< boost::is_same< typename geometric_traits<Point>::dimension_type, dimension<2> > >::type* = 0, disambiguation_tag<0> = 0 ) const
        {
            return ( m_compare.less_than( get<1>( p1 ), get<1>( p2 ) ) ) ||
                   ( m_compare.equals( get<1>( p1 ), get<1>( p2 ) ) && m_compare.less_than( get<0>( p1 ), get<0>( p2 ) ) );
        }

        template <typename Point>
        bool operator()( const Point& p1, const Point& p2, typename boost::disable_if< boost::is_same< typename geometric_traits<Point>::dimension_type, dimension<2> > >::type* = 0, disambiguation_tag<1> = 0 ) const
        {
            return ( m_compare.less_than( get<2>( p1 ), get<2>( p2 ) ) )||
                   ( m_compare.equals( get<2>( p1 ), get<2>( p2 ) ) && m_compare.less_than( get<1>( p1 ), get<1>( p2 ) ) ) ||
                   ( m_compare.equals( get<2>( p1 ), get<2>( p2 ) ) && m_compare.equals( get<1>( p1 ), get<1>( p2 ) ) && m_compare.less_than( get<0>( p1 ), get<0>( p2 ) ) );
        }

        NumberComparisonPolicy m_compare;

    };

    //! Lexicographical compare functor for Cartesian points. Sorts first in X and then in Y (then Z).
    template <typename NumberComparisonPolicy>
    struct lexicographical_segment_compare
    {
        lexicographical_segment_compare(){}
        lexicographical_segment_compare( const NumberComparisonPolicy& compare )
            : m_compare( compare )
            , m_pointCompare( compare )
        {}

        template <typename Segment>
        bool operator()( const Segment& s1, const Segment& s2 ) const
        {
            typedef typename geometric_traits< Segment >::point_type point_type;

            const point_type& start1 = get_start( s1 );
            const point_type& end1   = get_end( s2 );
            const point_type& start2 = get_start( s2 );
            const point_type& end2   = get_end( s2 );

            //Order the segments lexicographically first.
            const point_type* lower1;
            const point_type* upper1;
            const point_type* lower2;
            const point_type* upper2;

            if( m_pointCompare( start1, end1 ) )
            {
                lower1 = &start1;
                upper1 = &end1;
            }
            else
            {
                lower1 = &end1;
                upper1 = &start1;
            }

            if( m_pointCompare( start2, end2 ) )
            {
                lower2 = &start2;
                upper2 = &end2;
            }
            else
            {
                lower2 = &end2;
                upper2 = &start2;
            }

            return m_pointCompare( *lower1, *lower2 ) || ( numeric_sequence_equals( *lower1, *lower2, m_compare ) && m_pointCompare( *upper1, *upper2 ) );
        }

        NumberComparisonPolicy                            m_compare;
        lexicographical_comparer< NumberComparisonPolicy > m_pointCompare;

    };

    //! Functor to compare segments lexicographically as intervals A-C !< B-C if [A-B-C].
    template <typename NumberComparisonPolicy>
    struct segment_interval_compare
    {
        typedef lexicographical_comparer< NumberComparisonPolicy > lex_point_compare;

        segment_interval_compare(){}
        segment_interval_compare( const NumberComparisonPolicy& compare )
            : m_lexCompare( compare ){}

        template <typename Segment>
        bool operator()( const Segment& lhs, const Segment& rhs ) const
        {
            typedef typename geometric_traits<Segment>::point_type       point_type;

            const point_type* lhs_end;
            if( m_lexCompare( get_start( lhs ), get_end( lhs ) ) )
                lhs_end   = &get_end( lhs );
            else
                lhs_end   = &get_start( lhs );

            const point_type* rhs_start;
            if( m_lexCompare( get_start( rhs ), get_end( rhs ) ) )
                rhs_start = &get_start( rhs );
            else
                rhs_start = &get_end( rhs );

            //Now we have the segments in lexi order... we can compare the intervals.
            return m_lexCompare( *lhs_end, *rhs_start );
        }

        lex_point_compare m_lexCompare;

    };

    //! Given a set of segments take the geometrix difference of the set and the specified segments.
    //! precondition segments must all be collinear.
    template <typename Segment, typename SegmentIntervalSet, typename NumberComparisonPolicy>
    inline void collinear_segment_difference( SegmentIntervalSet& segments, const Segment& segment, const NumberComparisonPolicy& compare )
    {
        typedef lexicographical_comparer< NumberComparisonPolicy > lex_point_compare;
        typedef typename geometric_traits<Segment>::point_type    point_type;
        lex_point_compare lexCompare( compare );

        const point_type& C = get_start( segment );
        const point_type& D = get_end( segment );
        if (numeric_sequence_equals(C, D, compare))
            return;

        typename SegmentIntervalSet::iterator lb,ub;
        boost::tie( lb, ub ) = segments.equal_range( segment );

        std::vector< Segment > toInsert;
        while( lb != ub )
        {
            const Segment& overlappedSegment = *lb;

            const point_type& A = get_start( overlappedSegment );
            const point_type& B = get_end( overlappedSegment );

            //! must be collinear
            GEOMETRIX_ASSERT( is_collinear( C, D, A, compare ) && is_collinear( C, D, B, compare ) );

            bool CAD = is_between( C, D, A, true, compare );
            bool CBD = is_between( C, D, B, true, compare );

            if( CAD && CBD )
            {
                //remove this one.
                segments.erase( lb++ );
                continue;
            }

            bool AEqualC = numeric_sequence_equals( A, C, compare );
            bool AEqualD = numeric_sequence_equals( A, D, compare );
            bool BEqualC = numeric_sequence_equals( B, C, compare );
            bool BEqualD = numeric_sequence_equals( B, D, compare );

            bool ACB = is_between( A, B, C, true, compare );
            bool ADB = is_between( A, B, D, true, compare );
            if( ACB && ADB )
            {
                bool CDB = is_between( C, B, D, true, compare );
                if( CDB && !BEqualC )
                {
                    if( !AEqualC )
                        toInsert.push_back( construct< Segment >( A, C ) );
                    if( !BEqualD )
                        toInsert.push_back( construct< Segment >( D, B ) );
                    segments.erase( lb++ );
                    continue;
                }

                bool ADC = is_between(A, C, D, true, compare);
                if( ADC && !AEqualC )
                {
                    if( !AEqualD )
                        toInsert.push_back( construct< Segment >( A, D ) );
                    if( !BEqualC)
                        toInsert.push_back( construct< Segment >( C, B ) );
                    segments.erase( lb++ );
                    continue;
                }

                GEOMETRIX_ASSERT( false );
            }

            if( CAD && !(AEqualC || AEqualD) )
            {
                if( ADB && !BEqualD )
                {
                    toInsert.push_back( construct< Segment >( D, B ) );
                    //remove this one.
                    segments.erase( lb++ );
                    continue;
                }
                else if( ACB && !BEqualC )
                {
                    toInsert.push_back( construct< Segment >( C, B ) );
                    //remove this one.
                    segments.erase( lb++ );
                    continue;
                }
            }
            else if( CBD && !(BEqualC || BEqualD) )
            {
                if( ADB && !AEqualD)
                {
                    toInsert.push_back( construct< Segment >( A, D ) );
                    //remove this one.
                    segments.erase( lb++ );
                    continue;
                }
                else if( ACB && !AEqualC )
                {
                    toInsert.push_back( construct< Segment >( A, C ) );
                    //remove this one.
                    segments.erase( lb++ );
                    continue;
                }
            }

            ++lb;
        }

        segments.insert( toInsert.begin(), toInsert.end() );
    }

    //! Given a set of segments take the geometrix union of the set and the specified segments.
    //! precondition segments must all be collinear.
    template <typename Segment, typename SegmentIntervalSet, typename NumberComparisonPolicy>
    inline void collinear_segment_union( SegmentIntervalSet& segments, const Segment& segment, const NumberComparisonPolicy& compare )
    {
        typedef lexicographical_comparer< NumberComparisonPolicy > lex_point_compare;
        typedef typename geometric_traits<Segment>::point_type    point_type;
        lex_point_compare lexCompare( compare );

        typename SegmentIntervalSet::iterator lb,ub;
        boost::tie( lb, ub ) = segments.equal_range( segment );

        Segment unionSegment = construct<Segment>( get_start( segment ), get_end( segment ) );

        while( lb != ub )
        {
            const Segment& overlappedSegment = *lb;

            const point_type& A = get_start( overlappedSegment );
            const point_type& B = get_end( overlappedSegment );
            const point_type& C = get_start( unionSegment );
            const point_type& D = get_end( unionSegment );

            //! must be collinear
            GEOMETRIX_ASSERT( is_collinear( C, D, A, compare ) && is_collinear( C, D, B, compare ) );

            bool CAD = is_between( C, D, A, true, compare );
            bool CBD = is_between( C, D, B, true, compare );

            //Already contained in the union.
            if( CAD && CBD )
            {
                //remove this one.
                segments.erase( lb++ );
                continue;
            }

            bool ACB = is_between( A, B, C, true, compare );
            bool ADB = is_between( A, B, D, true, compare );

            //New segment contains existing union.. replace.
            if( ACB && ADB )
            {
                unionSegment = construct<Segment>( A, B );
                segments.erase( lb++ );
                continue;
            }

            if( CAD )
            {
                if( ADB )
                {
                    unionSegment = construct<Segment>( C, B );
                    segments.erase( lb++ );
                    continue;
                }
                if( ACB )
                {
                    unionSegment = construct<Segment>( D, B );
                    segments.erase( lb++ );
                    continue;
                }
            }
            else if( CBD )
            {
                if( ADB )
                {
                    unionSegment = construct<Segment>( A, C );
                    segments.erase( lb++ );
                    continue;
                }
                if( ACB )
                {
                    unionSegment = construct<Segment>( A, D );
                    segments.erase( lb++ );
                    continue;
                }
            }

            ++lb;
        }

        segments.insert( unionSegment );
    }

    //! sorting compare functor to sort coordinates by less than order.
    template <typename CoordinateType, typename NumberComparisonPolicy>
    struct coordinate_less_compare
    {
        coordinate_less_compare( const NumberComparisonPolicy& compare )
            : m_compare( compare )
        {}

        bool operator() ( const CoordinateType& lhs, const CoordinateType& rhs ) const
        {
            return m_compare.less_than( rhs, lhs );
        }

        NumberComparisonPolicy m_compare;
    };

    template <typename Compare>
    struct pair_first_compare
    {
        pair_first_compare( const Compare& compare )
            : m_compare( compare )
        {}

        template <typename T1, typename T2>
        bool operator()(const std::pair<T1,T2>& p1, const std::pair<T1,T2>& p2) const
        {
            return m_compare(p1.first, p2.first);
        }

        Compare m_compare;

    };

    template <typename Compare>
    struct pair_second_compare
    {
        pair_second_compare( const Compare& compare )
            : m_compare( compare )
        {}

        template <typename T1, typename T2>
        bool operator()(const std::pair<T1,T2>& p1, const std::pair<T1,T2>& p2) const
        {
            return m_compare(p1.second, p2.second);
        }

        Compare m_compare;

    };

    //! \struct dimension_compare
    //! \brief A predicate to compare two numeric sequences by the value at a specified dimension.
    template <std::size_t D, typename NumberComparisonPolicy>
    struct dimension_compare
    {
        dimension_compare( const NumberComparisonPolicy& compare )
            : m_compare( compare )
            , m_lexicographicalCompare( compare )
        {}

        template <typename NumericSequence>
        bool operator()( const NumericSequence& lhs, const NumericSequence& rhs ) const
        {
            //! Sequences are compared by dimension specified. In the case where coordinates at D are equal, the sequences are
            //! given a total order lexicographically.
            if( m_compare.less_than( get<D>( lhs ), get<D>( rhs ) ) )
                return true;
            else if( m_compare.equals( get<D>( lhs ), get<D>( rhs ) ) )
                return m_lexicographicalCompare( lhs, rhs );
            else
                return false;
        }

        NumberComparisonPolicy                          m_compare;
        lexicographical_comparer<NumberComparisonPolicy> m_lexicographicalCompare;
    };

    template <typename T>
    inline T min_copy(const T& a, const T& b)
    {
        return a < b ? a : b;
    }

    template <typename T>
    inline T max_copy(const T& a, const T& b)
    {
        return a > b ? a : b;
    }

    // clamp n to lie within the range [min, max]
    template <typename T>
    inline T clamp(T n, T min, T max)
    {
        if (n < min) return min;
        if (n > max) return max;
        return n;
    }

    template <typename T>
    inline int sign(const T& value)
    {
        return value >= constants::zero<T>() ? 1 : -1;
    }

}//namespace geometrix;

#endif //GEOMETRIX_UTILITIES_HPP
