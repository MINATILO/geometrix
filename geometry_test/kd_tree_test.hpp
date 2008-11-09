//
//! Copyright � 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_KD_TREE_TEST_HPP
#define _BOOST_GEOMETRY_KD_TREE_TEST_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "../geometry/point.hpp"
#include "../geometry/utilities.hpp"
#include "../geometry/point_sequence.hpp"
#include "../geometry/point_sequence_utilities.hpp"
#include "../geometry/random_generator.hpp"
#include "../geometry/euclidean_distance.hpp"
#include "../geometry/kd_tree.hpp"

template <typename PointSet>
struct point_visitor
{
    point_visitor( PointSet& points )
        :m_pSet( points )
    {}

    template <typename Point>
    void operator()( const Point& p ) const 
    {
        m_pSet.erase( p );
    }

    mutable PointSet& m_pSet;
};

//! Test a 2D case.
BOOST_AUTO_TEST_CASE( TestKDTree2D )
{
	using namespace boost::numeric::geometry;

	typedef point_double_2d CPoint2D;
	typedef point_double_3d CPoint3D;
	
    std::vector< CPoint2D > polygon;
    random_real_generator< boost::mt19937 > rnd(10.0);
    fraction_tolerance_comparison_policy<double> compare(1e-10);
    typedef std::set< CPoint2D, lexicographical_compare< fraction_tolerance_comparison_policy<double> > > PointSet;

    PointSet points( compare );
    for( size_t i=0;i < 1000; ++i )
    {
        double x = rnd();
        double y = rnd();
        if( x <= 5.0 && y <= 5.0 )
        {
            points.insert( CPoint2D( x, y ) );
        }
        polygon.push_back( CPoint2D( x, y ) ); 
    }
    
    kd_tree< CPoint2D > tree( polygon, compare, median_partitioning_strategy() );

    //! Specify a surface (square) with diagonal vector from 0,0, to 5,5 for the search range.
    orthogonal_range< CPoint2D > range( CPoint2D( 0.0, 0.0 ), CPoint2D( 5.0, 5.0 ) );

    //! Visit all the points inside the surface and remove them from the set.
    point_visitor< PointSet > visitor( points );
    tree.search( range, visitor, compare );   

    //! If it worked, points should be empty.
    BOOST_CHECK( points.empty() );
}

//! Test a 3D case.
BOOST_AUTO_TEST_CASE( TestKDTree3D )
{
	using namespace boost::numeric::geometry;

	typedef point_double_3d CPoint3D;
	
    std::vector< CPoint3D > polygon;
    random_real_generator< boost::mt19937 > rnd(10.0);
    fraction_tolerance_comparison_policy<double> compare(1e-10);
    typedef std::multiset< CPoint3D, lexicographical_compare< fraction_tolerance_comparison_policy<double> > > PointSet;

    PointSet points( compare );
    for( size_t i=0;i < 1000; ++i )
    {
        double x = rnd();
        double y = rnd();
        double z = rnd();
        if( x <= 5.0 && y <= 5.0 && z <= 5.0 )
        {
            points.insert( CPoint3D( x, y, z ) );
        }
        polygon.push_back( CPoint3D( x, y, z ) ); 
    }

    kd_tree< CPoint3D > tree( polygon, compare, median_partitioning_strategy() );

    //! Specify a volume (box) with diagonal vector from 0,0,0, to 5,5,5 for the search range.
    orthogonal_range< CPoint3D > range( CPoint3D( 0.0, 0.0, 0.0 ), CPoint3D( 5.0, 5.0, 5.0 ) );

    //! Visit all the points inside the volume and remove them from the set.
    point_visitor< PointSet > visitor( points );
    tree.search( range, visitor, compare );   

    //! If it worked, points should be empty.
    BOOST_CHECK( points.empty() );

    BOOST_FOREACH( const CPoint3D& p, points )
    {
        std::cout << p.get<0>() << ", " << p.get<1>() << ", " << p.get<2>() << std::endl;
    }
}

template <unsigned int N, typename Point, typename NumberComparisonPolicy>
struct n_nearest_neighbor_search
{
    BOOST_MPL_ASSERT_MSG(
        (N>0),
        N_MUST_BE_GREATER_THAN_ZERO,
        (n_nearest_neighbor_search<N,Point,NumberComparisonPolicy>) );

    n_nearest_neighbor_search( const Point& p, const NumberComparisonPolicy& compare )
        : m_compare( p, compare )
    {}

    //! method to visit the nearest after the search.
    template <typename Visitor>
    inline void visit_nearest( Visitor& v ) const
    {
        std::for_each( m_nNearest.begin(), m_nNearest.end(), v );
    }

    inline const std::vector<Point>& get_points() const { return m_nNearest; }

    //! Operator to test each point found in the range on the tree.
    template <typename Point>
    inline void operator()( const Point& p ) const 
    {
        if( m_nNearest.size() < N )
        {
            m_nNearest.push_back( p );

            //! if the size is now N.. sort it.
            if( m_nNearest.size() == N )
            {
                std::sort( m_nNearest.begin(), m_nNearest.end(), m_compare );
            }
        }
        else
        {
            //!Check if the value is less than the end.
            if( m_compare( p, m_nNearest.back() ) )
            {
                m_nNearest.pop_back();

                //! insert the new point 
                std::vector<Point>::iterator iter = std::lower_bound( m_nNearest.begin(), m_nNearest.end(), p, m_compare ); 
                m_nNearest.insert( iter, p );                
            }
        }
    }
    
    mutable std::vector< Point >                      m_nNearest;
    distance_compare< Point, NumberComparisonPolicy > m_compare;
    
};

struct point_printer
{
    point_printer( std::ostream& os )
        : m_os( os )
    {}

    template <typename Point>
    inline void operator()( const Point& p ) const 
    {
        m_os << p.get<0>() << ", " << p.get<1>() << ", " << p.get<2>() << std::endl;   
    }

    std::ostream& m_os;
};

//! Test a 3D case and report n nearest neighbors.
BOOST_AUTO_TEST_CASE( TestKDTreeNearest3D )
{
	using namespace boost::numeric::geometry;

	typedef point_double_3d CPoint3D;
	
    std::vector< CPoint3D > polygon;
    random_real_generator< boost::mt19937 > rnd(10.0);
    fraction_tolerance_comparison_policy<double> compare(1e-10);
    distance_compare< CPoint3D, fraction_tolerance_comparison_policy<double> > dCompare( CPoint3D(0.,0.,0.), compare );
    typedef std::multiset< CPoint3D, distance_compare< CPoint3D, fraction_tolerance_comparison_policy<double> > > PointSet;
    PointSet points( dCompare );
    for( size_t i=0;i < 1000; ++i )
    {
        double x = rnd();
        double y = rnd();
        double z = rnd();
        points.insert( CPoint3D( x, y, z ) );
        polygon.push_back( CPoint3D( x, y, z ) ); 
    }

    kd_tree< CPoint3D > tree( polygon, compare, median_partitioning_strategy() );

    //! Specify a volume (box) with diagonal vector over the entire range.
    //! TODO: This is a bit awkward... should create a search with no input range to assume all.
    orthogonal_range< CPoint3D > range( CPoint3D( 0.0, 0.0, 0.0 ), CPoint3D( 10.0, 10.0, 10.0 ) );

    //! Visit all the points inside the volume and remove them from the set.
    n_nearest_neighbor_search< 1, CPoint3D, fraction_tolerance_comparison_policy<double> > n_search( CPoint3D( 0.0, 0.0, 0.0 ), compare );
    tree.search( range, n_search, compare );   

    //! Print the points.
    n_search.visit_nearest( point_printer( std::cout ) );

    double distanceA = euclidean_distance( CPoint3D(0.,0.,0.), *n_search.get_points().begin() );
    double distanceB = euclidean_distance( CPoint3D(0.,0.,0.), *points.begin() );
    BOOST_CHECK( compare.equals( distanceA, distanceB ) );
}

#endif //_BOOST_GEOMETRY_KD_TREE_TEST_HPP
