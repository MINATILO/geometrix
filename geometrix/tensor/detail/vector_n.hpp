//
//! Copyright ? 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/concept_check.hpp>

#define DIMENSION BOOST_PP_ITERATION()

namespace geometrix {

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
#if defined( GEOMETRIX_DEFINE_VECTOR_OPERATORS )
    , boost::addable< vector<NumericType, DIMENSION>                   // vector + vector
    , boost::subtractable< vector<NumericType, DIMENSION>              // vector - vector
    , boost::dividable2< vector<NumericType, DIMENSION>, NumericType   // vector / T
    , boost::multipliable2< vector<NumericType, DIMENSION>, NumericType// vector * T, T * vector
    > > > >
#endif
{
public:

    typedef numeric_sequence< NumericType, DIMENSION > sequence_type;
    typedef typename sequence_type::numeric_type       coordinate_type;

    vector(){}

    //! define the constructors via the preprocessor.
    vector( BOOST_PP_ENUM_PARAMS(DIMENSION, const coordinate_type& a) )
        : sequence_type( BOOST_PP_ENUM_PARAMS(DIMENSION, a) )
    {
    }

    template <typename NumericSequence>
    vector( const NumericSequence& a )
        : sequence_type( a )
    {}

    template <typename Expr>
    vector& operator= (const Expr& expr)
    {
        BOOST_CONCEPT_ASSERT((VectorConcept<Expr>));
        sequence_type::operator =(expr);
        return *this;
    }

    //! Operator interface    
    template <typename Expr>
    vector& operator+= ( const Expr& p )
    {
        sequence_type::operator +=( p );
        return *this;
    }

    // numeric_sequence operator+(numeric_sequence, const numeric_sequence&) automatically
    // generated by addable.
    template <typename Expr>
    vector& operator-= ( const Expr& p )
    {
        sequence_type::operator -=( p );
        return *this;
    }

    // numeric_sequence operator-(numeric_sequence, const numeric_sequence&) automatically
    // generated by subtractable.
    template <typename Expr>
    vector& operator*= ( const Expr& v ) 
    {
        sequence_type::operator *=( v );
        return *this;
    }

    // numeric_sequence operator*(numeric_sequence, const T&) and
    // numeric_sequence operator*(const T&, numeric_sequence) auto-generated
    // by multipliable.
    template <typename Expr>
    vector& operator/= ( const Expr& v ) 
    {
        sequence_type::operator /=( v );
        return *this; 
    }
};

//! Helper macro to build access traits code.
#define GEOMETRIX_ACCESS_ARG_VECTOR_( z, i, T ) \
    geometrix::get<i>( args )                   \
/***/

template <typename T>
struct construction_policy< vector< T, DIMENSION> >
{    
    static vector<T, DIMENSION> construct( BOOST_PP_ENUM_PARAMS(DIMENSION, const T& a) )
    {
        return vector<T, DIMENSION>( BOOST_PP_ENUM_PARAMS(DIMENSION, a) );
    }

    template <typename NumericSequence>                                                                         
    static vector<T,DIMENSION> construct( const NumericSequence& args )
    {        
        return vector<T, DIMENSION>( BOOST_PP_ENUM( DIMENSION, GEOMETRIX_ACCESS_ARG_VECTOR_, NumericSequence ) );
    }
};

template <typename T>
struct assignment_policy < vector< T, DIMENSION> >
{
    static void assign(vector<T, DIMENSION>& v, BOOST_PP_ENUM_PARAMS(DIMENSION, const T& a))
    {
        BOOST_PP_REPEAT(DIMENSION, GEOMETRIX_SET_ARG_VALUE_AT_INDEX_, _);
    }

    template <typename NumericSequence>
    static void assign(vector<T, DIMENSION>& v, const NumericSequence& args)
    {
        BOOST_PP_REPEAT(DIMENSION, GEOMETRIX_SET_VALUE_AT_INDEX_, args);
    }
};

#undef GEOMETRIX_ACCESS_ARG_VECTOR_

}//namespace geometrix;

#undef DIMENSION
