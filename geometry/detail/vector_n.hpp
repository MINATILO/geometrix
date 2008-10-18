//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

namespace boost
{
namespace numeric
{
namespace geometry
{

#define DIMENSION BOOST_PP_ITERATION()

/////////////////////////////////////////////////////////////////////////////
//
// CLASS vector
//! A template class for specifying vector which can be compile time
//! and runtime indexed conforming to random access for both and can support
//! vector and scalar arithmetic.
//!
template <typename NumericType>
class vector<NumericType, DIMENSION>
    : public numeric_sequence< NumericType, DIMENSION >
    , boost::addable< vector<NumericType, DIMENSION>                   // vector + vector
    , boost::subtractable< vector<NumericType, DIMENSION>              // vector - vector
    , boost::dividable2< vector<NumericType, DIMENSION>, NumericType   // vector / T
    , boost::multipliable2< vector<NumericType, DIMENSION>, NumericType// vector * T, T * vector
    //, boost::additive< vector<NumericType, DIMENSION>, detail::numeric_sequence< NumericType, DIMENSION >     // vector + numeric_sequence
    > > > >// >
{
public:

    typedef numeric_sequence< NumericType, DIMENSION > sequence_type;
    typedef typename sequence_type::numeric_type       coordinate_type;
	

    vector(){}

    //! define the constructors via the preprocessor.
    vector( BOOST_PP_ENUM_PARAMS(DIMENSION, const numeric_type& a) )
        : sequence_type( BOOST_PP_ENUM_PARAMS(DIMENSION, a) )
    {
    }

    vector( const typename sequence_type::numeric_array& a )
        : sequence_type( a )
    {}

    //! Operator interface    
    vector operator+= ( const sequence_type& p )
    {
        sequence_type::operator +=( p );
        return *this;
    }

    // numeric_sequence operator+(numeric_sequence, const numeric_sequence&) automatically
    // generated by addable.

    vector operator-= ( const sequence_type& p )
    {
        sequence_type::operator -=( p );
        return *this;
    }
    // numeric_sequence operator-(numeric_sequence, const numeric_sequence&) automatically
    // generated by subtractable.

	vector operator*= ( const coordinate_type& v ) 
    {
        sequence_type::operator *=( v );
        return *this;
    }
    // numeric_sequence operator*(numeric_sequence, const T&) and
    // numeric_sequence operator*(const T&, numeric_sequence) auto-generated
    // by multipliable.

	vector operator/= ( const coordinate_type& v ) 
    {
        sequence_type::operator /=( v );
        return *this; 
    }

};
          
}}}//namespace boost::numeric::geometry

#undef DIMENSION
