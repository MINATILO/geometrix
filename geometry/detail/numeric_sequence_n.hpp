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

    //! define the constructors via the preprocessor.
    template <typename T>
    boost::array<T,DIMENSION> make_array( BOOST_PP_ENUM_PARAMS(DIMENSION, const T& a) )        
    {
        boost::array<T,DIMENSION> numericSequence = { BOOST_PP_ENUM_PARAMS(DIMENSION, a) };
        return numericSequence;
    }

/////////////////////////////////////////////////////////////////////////////
//
// CLASS numeric_sequence
//! A template class for specifying numeric_sequence which can be compile time
//! and runtime indexed conforming to random access for both and can support
//! vector and scalar arithmetic.
//!
template <typename NumericType>
class numeric_sequence<NumericType,DIMENSION>
    //: boost::addable< numeric_sequence<NumericType, DIMENSION>                   // numeric_sequence + numeric_sequence
    //, boost::subtractable< numeric_sequence<NumericType, DIMENSION>              // numeric_sequence - numeric_sequence
    //, boost::dividable2< numeric_sequence<NumericType, DIMENSION>, NumericType   // numeric_sequence / T
    //, boost::multipliable2< numeric_sequence<NumericType, DIMENSION>, NumericType// numeric_sequence * T, T * numeric_sequence
    //> > > >
{
public:

	typedef NumericType                                                 numeric_type;
	typedef dimension_traits<DIMENSION>                                 dimension_type;
    typedef boost::array< NumericType, DIMENSION >                      numeric_array;
    typedef typename boost::range_iterator< numeric_array >::type       iterator;
    typedef typename boost::range_const_iterator< numeric_array >::type const_iterator;

    numeric_sequence(){}

    //! define the constructors via the preprocessor.
    numeric_sequence( BOOST_PP_ENUM_PARAMS(DIMENSION, const numeric_type& a) )
        : m_numericSequence( make_array( BOOST_PP_ENUM_PARAMS(DIMENSION, a) ) )
    {
    }

    numeric_sequence( const numeric_array& a )
        : m_numericSequence( a )
    {}

    template <unsigned int D>
    numeric_type get() const
    {
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< D >::value >= 0 && dimension_traits< D >::value < dimension_type::value )
		   , NUMERIC_SEQUENCE_GET_CALLED_WITH_INDEX_OUT_OF_BOUNDS
		   , ( dimension_traits< D > )
        );

        return boost::fusion::at_c<D>( m_numericSequence );
    }

    template <unsigned int D>
    numeric_type& get()
    {        
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< D >::value >= 0 && dimension_traits< D >::value < dimension_type::value )
		   , NUMERIC_SEQUENCE_GET_CALLED_WITH_INDEX_OUT_OF_BOUNDS
		   , ( dimension_traits< D > )
        );

        return boost::fusion::at_c<D>( m_numericSequence );
    }

    numeric_type get( size_t i ) const
    {        
        BOOST_ASSERT( i < dimension_type::value );
        return m_numericSequence[i];
    }

    numeric_type& get( size_t i )
    {        
        BOOST_ASSERT( i < dimension_type::value );
        return m_numericSequence[i];
    }

    numeric_type operator []( size_t i ) const
    {        
        BOOST_ASSERT( i < dimension_type::value );
        return m_numericSequence[i];
    }

    numeric_type& operator []( size_t i )
    {        
        BOOST_ASSERT( i < dimension_type::value );
        return m_numericSequence[i];
    }

    //! iterator interface for range operations.
    iterator       begin() { return boost::begin( m_numericSequence ); };
    const_iterator begin() const { return boost::begin( m_numericSequence ); };
    iterator       end() { return boost::end( m_numericSequence ); };
    const_iterator end() const { return boost::end( m_numericSequence ); };
    size_t         size() const { return dimension_type::value; }
    size_t         empty() const { return false; }

protected:

    //! Operator interface    
    numeric_sequence& operator+= ( const numeric_sequence& p )
    {
        boost::fusion::for_each( m_numericSequence, detail::pairwise_fusion_binary_operation< numeric_array, std::plus< numeric_type > >( p.m_numericSequence ) );
        return *this;
    }

    // numeric_sequence operator+(numeric_sequence, const numeric_sequence&) automatically
    // generated by addable.

    numeric_sequence& operator-= ( const numeric_sequence& p )
    {
        boost::fusion::for_each( m_numericSequence, detail::pairwise_fusion_binary_operation< numeric_array, std::minus< numeric_type > >( p.m_numericSequence ) );
        return *this;
    }
    // numeric_sequence operator-(numeric_sequence, const numeric_sequence&) automatically
    // generated by subtractable.

	numeric_sequence& operator*= ( const numeric_type& v )
    {
        boost::fusion::for_each( m_numericSequence, detail::assign_operation_result< numeric_type, std::multiplies< numeric_type > >( v ) );
        return *this;
    }
    // numeric_sequence operator*(numeric_sequence, const T&) and
    // numeric_sequence operator*(const T&, numeric_sequence) auto-generated
    // by multipliable.

	numeric_sequence& operator/= ( const numeric_type& v ) 
    {
        boost::fusion::for_each( m_numericSequence, detail::assign_operation_result< numeric_type, std::divides< numeric_type > >( v ) );
        return *this;
    }
    // numeric_sequence operator/(numeric_sequence, const T&) auto-generated
    // by dividable.

private:

    numeric_array m_numericSequence;

};
          
}}}//namespace boost::numeric::geometry

#undef DIMENSION
