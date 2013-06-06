//
//! Copyright � 2008-2011
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
#include <geometrix/algebra/algebra.hpp>
#include <boost/concept_check.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace geometrix {

    //! Function to get the angle from an origin to a target point in the 2D XY plane.
    template <typename CoordinateSequence>
    inline typename geometric_traits<CoordinateSequence>::arithmetic_type
        angle_to_point( const CoordinateSequence& A,
                        const CoordinateSequence& B,
                        typename boost::enable_if<
                            boost::is_same<
                                typename geometric_traits<CoordinateSequence>::dimension_type,
                                dimension<2> 
                            >
                        >::type* = 0 )
    {
        return math::atan2( get<1>( B ) - get<1>( A ), 
                            get<0>( B ) - get<0>( A ) );
    }

    //! Function to normalize an angle to within the interval [0,2*PI]
    template <typename CoordinateType>
    inline void normalize_angle( CoordinateType& angle )
    {
        //simplifies the angle to lay in the range of the interval 0 - 2*pi
        CoordinateType pi = constants<CoordinateType>::pi();        
        CoordinateType twoPI = CoordinateType( 2 ) * pi;
        if ( angle > twoPI || angle < CoordinateType( 0 ) )
        {
            CoordinateType n = math::floor( angle / twoPI );
            if ( n != CoordinateType( 0 ) )
                angle -= twoPI * n;
            if ( angle > twoPI )
                angle -= twoPI;
            else if ( angle < CoordinateType( 0 ) )
                angle += twoPI;            
        }
    }

    //! Function to determine if 3 points are collinear in the 2D XY plane.
    //! From Computational Geometry in C by J. O'Rourke.
    template <typename Point, typename NumberComparisonPolicy>
    inline bool is_collinear( const Point& A, 
                       const Point& B,
                       const Point& C,
                       const NumberComparisonPolicy& compare,
                       typename boost::enable_if<
                                boost::is_same<
                                    typename geometric_traits<Point>::dimension_type,
                                    dimension<2> 
                                >
                              >::type* = 0 )
    {
        using namespace algebra;
        BOOST_AUTO(det, exterior_product_area( B-A, C-A ) );
        return compare.equals( det, 0 );//Absolute tolerance checks are fine for Zero checks.
    }

    //! Function to determine if Point C is between points A-B
    //! From Computational Geometry in C by J. O'Rourke.
    template <typename Point, typename NumberComparisonPolicy>
    inline bool is_between( const Point& A,
                     const Point& B,
                     const Point& C,
                     bool includeBounds,
                     const NumberComparisonPolicy& compare,
                     typename boost::enable_if<
                        boost::is_same<
                            typename geometric_traits<Point>::dimension_type,
                            dimension<2>
                        >
                     >::type* = 0 )
    {
        if( !is_collinear( A, B, C, compare ) )
            return false;
     
        //If AB not vertical, check between on x; else on y.
        bool ABVertical = compare.equals( get<0>( A ), get<0>( B ) );
        if( !ABVertical )
        {
            if(includeBounds)
            {
                return ( ( compare.less_than_or_equal( get<0>( A ), get<0>( C ) ) && compare.less_than_or_equal( get<0>( C ), get<0>( B ) ) ) ||
                         ( compare.greater_than_or_equal( get<0>( A ), get<0>( C ) ) && compare.greater_than_or_equal( get<0>( C ), get<0>( B ) ) ) );
            }
            else
            {
                return ( ( compare.less_than( get<0>( A ), get<0>( C ) ) && compare.less_than( get<0>( C ), get<0>( B ) ) ) ||
                         ( compare.greater_than( get<0>( A ), get<0>( C ) ) && compare.greater_than( get<0>( C ), get<0>( B ) ) ) );
            }
        }
        else
        {
            if(includeBounds)
            {
                return ( ( compare.less_than_or_equal( get<1>( A ), get<1>( C ) ) && compare.less_than_or_equal( get<1>( C ), get<1>( B ) ) ) ||
                         ( compare.greater_than_or_equal( get<1>( A ), get<1>( C ) ) && compare.greater_than_or_equal( get<1>( C ), get<1>( B ) ) ) );
            }
            else
            {
                return ( ( compare.less_than( get<1>( A ), get<1>( C ) ) && compare.less_than( get<1>( C ), get<1>( B ) ) ) ||
                         ( compare.greater_than( get<1>( A ), get<1>( C ) ) && compare.greater_than( get<1>( C ), get<1>( B ) ) ) );
            }
        }
    }

    //! Orientation test to check if point C is left, collinear, or right of the line formed by A-B.
    enum orientation_type
    {
        oriented_right     = -1,
        oriented_collinear = 0,
        oriented_left      = 1
    };
    template <typename Point, typename NumberComparisonPolicy>
    inline orientation_type get_orientation( const Point& A,
                                      const Point& B,
                                      const Point& C,
                                      const NumberComparisonPolicy& compare )
    {
        using namespace algebra;
        BOOST_AUTO( cross, exterior_product_area( B-A, C-A ) );
        if( compare.less_than( cross, 0 ) )
            return oriented_right;
        else if( compare.greater_than( cross, 0 ) )
            return oriented_left;
        else
            return oriented_collinear;
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
    template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
    inline CoordinateType y_of_x( const Point& s_start,
                           const Point& s_end,
                           CoordinateType x, 
                           CoordinateType& slope, 
                           const NumberComparisonPolicy& compare )
    {
        BOOST_AUTO( x0, get<0>( s_start ) );
        BOOST_AUTO( x1, get<0>( s_end ) );
        BOOST_AUTO( y0, get<1>( s_start ) );
        BOOST_AUTO( y1, get<1>( s_end ) );
        slope = arithmetic_promote( (y1-y0) )/(x1-x0);
        return (x - x0) * slope + y0;
    }

    //! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
    template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
    inline CoordinateType y_of_x( const Point& s_start,
                           const Point& s_end,
                           CoordinateType x,
                           const NumberComparisonPolicy& compare )
    {
        CoordinateType slope;
        return y_of_x( s_start, s_end, x, slope, compare );
    }

    //! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
    template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
    inline CoordinateType x_of_y( const Point& s_start,
                           const Point& s_end,
                           CoordinateType y,
                           CoordinateType& slope,
                           const NumberComparisonPolicy& compare )
    {
        typedef Point point_type;
        CoordinateType y0, y1, x0, x1;

        x0 = get<0>( s_start );
        x1 = get<0>( s_end );
        y0 = get<1>( s_start );
        y1 = get<1>( s_end );
                
        slope = (y1-y0)/(x1-x0);

        CoordinateType x = (y - y0)/slope + x0;     
        BOOST_ASSERT( compare.equals( y, y_of_x( s_start, s_end, x, compare )  ) );

        return x;
    }

    //! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
    template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
    inline CoordinateType x_of_y( const Point& s_start,
                           const Point& s_end, 
                           CoordinateType y,
                           const NumberComparisonPolicy& compare )
    {
        CoordinateType slope;
        return x_of_y( s_start, s_end, y, slope, compare  );
    }

    namespace detail
    {
        template <unsigned int D>
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
            static bool compare( const NumericSequence& lhs, const NumericSequence& rhs, const NumberComparisonPolicy& nCompare )
            {
                return false;//all were equal.                 
            }
        };
    }

    //! Lexicographical compare functor for Cartesian points. Sorts first in X and then in Y (then Z).
    template <typename NumberComparisonPolicy>
    class lexicographical_compare
    {   
        template <typename NumericSequence>
        struct comparer
        {
            static bool compare( const NumericSequence& lhs, const NumericSequence& rhs, const NumberComparisonPolicy& nCompare )
            {
                return detail::lexicographical<dimension_of<NumericSequence>::value>::compare( lhs, rhs, nCompare );
            }
        };

    public:

        lexicographical_compare(){}
        lexicographical_compare( const NumberComparisonPolicy& compare )
            : m_compare( compare )
        {}

        template <typename NumericSequence>
        bool operator()( const NumericSequence& p1, const NumericSequence& p2 ) const
        {
            return comparer<NumericSequence>::compare( p1,p2, m_compare );
        }

    private:
        
        NumberComparisonPolicy m_compare;

    };

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
        lexicographical_compare< NumberComparisonPolicy > m_pointCompare;

    };

    //! Functor to compare segments lexicographically as intervals A-C !< B-C if [A-B-C].
    template <typename NumberComparisonPolicy>
    struct segment_interval_compare
    {    
        typedef lexicographical_compare< NumberComparisonPolicy > lex_point_compare;

        segment_interval_compare(){}
        segment_interval_compare( const NumberComparisonPolicy& compare )
            : m_lexCompare( compare ){}

        template <typename Segment>
        bool operator()( const Segment& lhs, const Segment& rhs ) const
        {
            typedef typename geometric_traits<Segment>::point_type       point_type;

            const point_type* lhs_start;
            const point_type* lhs_end;
            const point_type* rhs_start;
            const point_type* rhs_end;
           
            if( m_lexCompare( get_start( lhs ), get_end( lhs ) ) )
            {
                lhs_start = &get_start( lhs );
                lhs_end   = &get_end( lhs ); 
            }
            else
            {
                lhs_start = &get_end( lhs );
                lhs_end   = &get_start( lhs );
            }

            if( m_lexCompare( get_start( rhs ), get_end( rhs ) ) )
            {
                rhs_start = &get_start( rhs );
                rhs_end   = &get_end( rhs );
            }
            else
            {
                rhs_start = &get_end( rhs );
                rhs_end   = &get_start( rhs );
            }

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
        typedef lexicographical_compare< NumberComparisonPolicy > lex_point_compare;
        typedef typename geometric_traits<Segment>::point_type    point_type;
        lex_point_compare lexCompare( compare );

        const point_type& C = get_start( segment );
        const point_type& D = get_end( segment );

        typename SegmentIntervalSet::iterator lb,ub;
        boost::tie( lb, ub ) = segments.equal_range( segment );

        std::vector< Segment > toInsert;
        while( lb != ub )
        {        
            const Segment& overlappedSegment = *lb;

            const point_type& A = get_start( overlappedSegment );
            const point_type& B = get_end( overlappedSegment );

            //! must be collinear
            BOOST_ASSERT( is_collinear( C, D, A, compare ) && is_collinear( C, D, B, compare ) );
            
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
                bool ADC = is_between( A, C, D, true, compare );
                if( CDB && !(BEqualC || BEqualD) )
                {
                    toInsert.push_back( construct< Segment >( A, C ) );
                    toInsert.push_back( construct< Segment >( D, B ) );
                    segments.erase( lb++ );
                    continue;
                }
                else if( ADC && !(AEqualC || AEqualD) )
                {
                    toInsert.push_back( construct< Segment >( A, D ) );
                    toInsert.push_back( construct< Segment >( C, B ) );
                    segments.erase( lb++ );
                    continue;
                }

                BOOST_ASSERT( false );
                
            }   

            if( CAD && !(AEqualC || AEqualD) )
            {         
                if( ADB && !(BEqualD) )
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
        typedef lexicographical_compare< NumberComparisonPolicy > lex_point_compare;
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
            BOOST_ASSERT( is_collinear( C, D, A, compare ) && is_collinear( C, D, B, compare ) );

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

    //! sorting compare functor to sort points clockwise or counter-clockwise around a point 
    template <typename Point, typename NumberComparisonPolicy>
    struct orientation_compare
    {
        typedef Point point_type;
        typedef typename geometric_traits< point_type >::coordinate_type coordinate_type;

        orientation_compare( const point_type& center, orientation_type winding, const NumberComparisonPolicy& compare )
            : m_winding( winding )
            , m_center( center )
            , m_compare( compare )
        {}

        bool operator() ( const point_type& lhs, const point_type& rhs ) const
        {            
            if( numeric_sequence_equals( lhs, rhs, m_compare ) )
                return false;

            orientation_type c_lhs = get_orientation( m_center, rhs, lhs, m_compare );

            if( c_lhs == oriented_collinear && m_winding != oriented_collinear )
            {
                //if the points are on opposite sides of the center 
                coordinate_type rhsDeltaY = get<1>( rhs ) - get<1>( m_center );
                coordinate_type lhsDeltaY = get<1>( lhs ) - get<1>( m_center );
                coordinate_type rhsDeltaX = get<0>( rhs ) - get<0>( m_center );
                coordinate_type lhsDeltaX = get<0>( lhs ) - get<0>( m_center );

                coordinate_type zero( 0 );

                //Check if opposite directions
                if( m_compare.less_than( lhsDeltaY * rhsDeltaY, zero ) || m_compare.less_than( lhsDeltaX * rhsDeltaX, zero ) )
                {
                    //opposite - compare quadrants?
                    bool rhsYNegative = m_compare.less_than( rhsDeltaY, zero );
                    bool lhsYNegative = m_compare.less_than( lhsDeltaY, zero );
                    bool rhsXNegative = m_compare.less_than( rhsDeltaX, zero );
                    bool lhsXNegative = m_compare.less_than( lhsDeltaX, zero );

                    int lhsQuadrant;
                    int rhsQuadrant;

                    if( !rhsYNegative && !rhsXNegative )
                        rhsQuadrant = 0;
                    
                    if( !lhsYNegative && !lhsXNegative )
                        lhsQuadrant = 0;
                    
                    if( !rhsYNegative && rhsXNegative )
                        rhsQuadrant = 1;
                    
                    if( !lhsYNegative && lhsXNegative )
                        lhsQuadrant = 1;
                    
                    if( rhsYNegative && rhsXNegative )
                        rhsQuadrant = 2;
                    
                    if( lhsYNegative && lhsXNegative )
                        lhsQuadrant = 2;
                    
                    if( rhsYNegative && !rhsXNegative )
                        rhsQuadrant = 3;
                    
                    if( lhsYNegative && !lhsXNegative )
                        lhsQuadrant = 3;                    

                    return ( lhsQuadrant > rhsQuadrant && m_winding == oriented_left );
                }
                else
                {
                    return false;
                }
            }

            return ( c_lhs == m_winding );
        }

        point_type             m_center;
        orientation_type       m_winding;
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
    template <unsigned int D, typename NumberComparisonPolicy>
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
        lexicographical_compare<NumberComparisonPolicy> m_lexicographicalCompare;
    }; 

}//namespace geometrix;

#endif //GEOMETRIX_UTILITIES_HPP