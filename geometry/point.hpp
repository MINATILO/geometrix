//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POINT_HPP
#define _BOOST_GEOMETRY_POINT_HPP
#pragma once

#include "point_traits.hpp"
#include <boost/mpl/assert.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/utility.hpp>
#include <boost/operators.hpp>

namespace boost
{
namespace numeric
{
namespace geometry
{

//! Enum for common dimension names in cartesian coordinates.
enum cartesian_dimension
{
	e_x = 0,
	e_y = 1,
	e_z = 2
};

//! Enum for common dimension names in polar/spherical coordinates.
enum polar_dimension
{
	e_r = 0,
	e_theta = 1,
	e_phi = 2
};

/////////////////////////////////////////////////////////////////////////////
//
// CLASS point
//! A template class for specifying point types with a specialized coordinate
//! type and dimension. A constraint on point dimensions is enforced to avoid
//! instantiating points with unsupported dimensionality.
//!
template <typename Coordinate, unsigned int Dimension>
class point
{
public:	

	typedef Coordinate               coordinate_type;
	typedef dimension_tag<Dimension> dimension_type;
	typedef point<coordinate_type,dimension_type::value> UnsupportedPointDimension;

	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, POINT_DIMENSION_NOT_SUPPORTED
		, (UnsupportedPointDimension) );	
};

//! 2D point specialization.
template <typename Coordinate>
class point<Coordinate, 2> : boost::addable< point<Coordinate,2>    // point + point
            , boost::subtractable< point<Coordinate,2>              // point - point
            , boost::dividable2< point<Coordinate,2>, Coordinate    // point / T
            , boost::multipliable2< point<Coordinate,2>, Coordinate // point * T, T * point
            > > > >
{
public:

	typedef Coordinate        coordinate_type;
	typedef dimension_tag<2>  dimension_type;

	//! Default Constructor
	explicit point( coordinate_type x = 0, coordinate_type y = 0 )
        : m_x( x ), 
          m_y( y )
	{}

	~point(){}

	//! Access the X coordinate
	inline coordinate_type get_x() const { return m_x; }

	//! Access the Y coordinate
	inline coordinate_type get_y() const { return m_y; }

	//! Mutate the X coordinate
	inline void            set_x( coordinate_type x ) { m_x = x; }

	//! Mutate the Y coordinate
	inline void            set_y( coordinate_type y ) { m_y = y; }

	//! dimensional index accessor
	inline coordinate_type get_coordinate( cartesian_dimension dimension ) const 
	{
		assert( dimension >= e_x && dimension <= e_y );
		if( dimension == e_x )
            return m_x;
        else
            return m_y;
	}

	//! dimensional index accessor
	inline coordinate_type operator []( cartesian_dimension dimension ) const { return get_coordinate( dimension ); }

	//! dimensional index accessor
	inline coordinate_type operator ()( cartesian_dimension dimension ) const { return get_coordinate( dimension ); }

	//! dimensional index mutator
	inline void            set_coordinate( cartesian_dimension dimension, coordinate_type value ) 
	{
		assert( dimension != e_z );
		if( dimension == e_x )
            m_x = value;
        else if( dimension == e_y )
            m_y = value;
	}

	//! Operator interface
	inline point operator+= ( const point& p ){ m_x += p.m_x; m_y += p.m_y; return *this; }
    // point operator+(point, const point&) automatically
    // generated by addable.

    inline point operator-= ( const point& p ){ m_x -= p.m_x; m_y -= p.m_y; return *this; }
    // point operator-(point, const point&) automatically
    // generated by subtractable.

	inline point operator*= ( const coordinate_type& v ) { m_x *= v; m_y *= v; return *this; }
    // point operator*(point, const T&) and
    // point operator*(const T&, point) auto-generated
    // by multipliable.

	inline point operator/= ( const coordinate_type& v ) { m_x /= v; m_y /= v; return *this; }
    // point operator/(point, const T&) auto-generated
    // by dividable.

private:

	coordinate_type m_x;
    coordinate_type m_y;

};

//! 3D point specialization.
template <typename Coordinate>
class point<Coordinate, 3> : boost::addable< point<Coordinate,3>    // point + point
            , boost::subtractable< point<Coordinate,3>              // point - point
            , boost::dividable2< point<Coordinate,3>, Coordinate    // point / T
            , boost::multipliable2< point<Coordinate,3>, Coordinate // point * T, T * point
            > > > >
{
public:

	typedef Coordinate        coordinate_type;
	typedef dimension_tag<3>  dimension_type;

	//! Default Constructor
	explicit point( coordinate_type x = 0, coordinate_type y = 0, coordinate_type z = 0 )
    : m_coordinates( dimension_type::value )
	{		
		m_coordinates( 0 ) = x;
		m_coordinates( 1 ) = y;
		m_coordinates( 2 ) = z;
	}

	~point(){}

	//! Access the X coordinate
	inline coordinate_type get_x() const { return m_coordinates(0); }

	//! Access the Y coordinate
	inline coordinate_type get_y() const { return m_coordinates(1); }

	//! Access the Z coordinate (if enabled)
	inline coordinate_type get_z() const { return m_coordinates(2); }
	
	//! Mutate the X coordinate
	inline void            set_x( coordinate_type x ) { m_coordinates( 0 ) = x; }

	//! Mutate the Y coordinate
	inline void            set_y( coordinate_type y ) { m_coordinates( 1 ) = y; }

	//! Mutate the Z coordinate
	inline void            set_z( coordinate_type z ) { m_coordinates( 2 ) = z; }

	//! dimensional index accessor
	inline coordinate_type get_coordinate( cartesian_dimension dimension ) const { return m_coordinates( dimension ); }

	//! dimensional index accessor
	inline coordinate_type operator []( cartesian_dimension dimension ) const { return m_coordinates( dimension ); }

	//! dimensional index accessor
	inline coordinate_type operator ()( cartesian_dimension dimension ) const { return m_coordinates( dimension ); }

	//! dimensional index mutator
	inline void            set_coordinate( cartesian_dimension dimension, coordinate_type value ) { m_coordinates( dimension ) = value; }

    //! Operator interface
	inline point operator+= ( const point& p ){ ( m_coordinates += p.m_coordinates ); return *this; }
    // point operator+(point, const point&) automatically
    // generated by addable.

    inline point operator-= ( const point& p ){ ( m_coordinates -= p.m_coordinates ); return *this; }
    // point operator-(point, const point&) automatically
    // generated by subtractable.

	inline point operator*= ( const coordinate_type& v ) { ( m_coordinates *= v ); return *this; }
    // point operator*(point, const T&) and
    // point operator*(const T&, point) auto-generated
    // by multipliable.

	inline point operator/= ( const coordinate_type& v ) { ( m_coordinates /= v ); return *this; }
    // point operator/(point, const T&) auto-generated
    // by dividable.

private:

	boost::numeric::ublas::vector<coordinate_type> m_coordinates;

};
                  
//! Concrete Point Types for some of the more common coordinate types.
typedef point<float, 2>       point_float_2d;
typedef point<float, 3>       point_float_3d;
typedef point<double, 2>      point_double_2d;
typedef point<double, 3>      point_double_3d;

//! Specialize the coordinate accessors for cartesian coords.
#define BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( Point )                                 \
template <>                                                                           \
struct cartesian_access_traits< Point >                                               \
{                                                                                     \
    typedef Point                                     point_type;                     \
    typedef point_traits<point_type>::coordinate_type coordinate_type;                \
    typedef point_traits<point_type>::dimension_type  dimension_type;                 \
                                                                                      \
    static inline coordinate_type get_x( const point_type& p ) { return p.get_x(); }  \
    static inline coordinate_type get_y( const point_type& p ) { return p.get_y(); }  \
    template <class T>                                                                \
    static typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_tag<3> >, typename point_traits<T>::coordinate_type >::type get_z( const T& p ) { return p.get_z(); } \
    template <typename T>                                                             \
    static inline typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_tag<2> >, point_type >::type construct( const coordinate_type& x, const coordinate_type& y ) { return point_type( x, y ); } \
    template <class T>                                                                \
    static inline typename boost::disable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_tag<2> >, point_type >::type construct( const coordinate_type& x, const coordinate_type& y, const coordinate_type& z ) { return point_type( x, y, z ); } \
};

//! Specialize the coordinate accessors for polar coords.
#define BOOST_DEFINE_POLAR_ACCESS_TRAITS( Point )                                     \
template <>                                                                           \
struct polar_access_traits< Point >                                                   \
{                                                                                     \
    typedef Point                                     point_type;                     \
    typedef point_traits<point_type>::coordinate_type coordinate_type;                \
    typedef point_traits<point_type>::dimension_type  dimension_type;                 \
                                                                                      \
    static inline coordinate_type get_theta( const point_type& p ) { return atan( p.get_y() / p.get_x() ); }  \
    template <class T>                                                                \
    static typename boost::disable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_tag<3> >, typename point_traits<T>::coordinate_type >::type get_radius( const T& p ) { return sqrt( p.get_x()*p.get_x() + p.get_y() * p.get_y() ); } \
    template <class T>                                                                \
    static typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_tag<3> >, typename point_traits<T>::coordinate_type >::type get_radius( const T& p ) { return sqrt( p.get_x()*p.get_x() + p.get_y() * p.get_y() + p.get_z() * p.get_z() ); } \
    template <class T>                                                                \
    static typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_tag<3> >, typename point_traits<T>::coordinate_type >::type get_phi( const T& p ) { return acos( p.get_z()/ get_radius( p ) ); } \
    template <typename T>                                                             \
    static inline typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_tag<2> >, point_type >::type construct( const coordinate_type& r, const coordinate_type& t ) { return point_type( r*cos(t), r*sin(t) ); }                                                                                        \
    template <class T>                                                                \
    static inline typename boost::disable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_tag<2> >, point_type >::type construct( const coordinate_type& r, const coordinate_type& t, const coordinate_type& phi ) { return point_type( r*cos(t)*sin(phi), r*sin(t)*sin(phi), r*cos(phi) ); } \
};

BOOST_DEFINE_POINT_TRAITS( point_float_2d );
BOOST_DEFINE_POINT_TRAITS( point_float_3d );
BOOST_DEFINE_POINT_TRAITS( point_double_2d );
BOOST_DEFINE_POINT_TRAITS( point_double_3d );

BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_float_2d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_float_3d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_double_2d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_double_3d );

BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_float_2d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_float_3d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_double_2d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_double_3d );

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POINT_HPP
