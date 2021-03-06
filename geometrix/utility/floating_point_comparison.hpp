//
//! - Based on original code from: 
//!   ? Gennadiy Rozental 2001-2002 
//! Brandon Kohn 2008 - extended to use two different types.
//! 
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_FLOATING_POINT_COMPARISON_HPP
#define GEOMETRIX_DETAIL_FLOATING_POINT_COMPARISON_HPP

#include <boost/typeof/typeof.hpp>
#include <boost/units/unit.hpp>
#include <geometrix/numeric/numeric_traits.hpp>
#include <geometrix/numeric/constants.hpp>

namespace geometrix {
    
//! \brief Function to compute the absolute value of a numeric type.
template <typename NumericType>
inline NumericType absolute_value( const NumericType& v ) 
{
    return v < constants::zero<NumericType>() ? -v : v;
}

//! \brief Function to perform a division with care being taken to avoid over/underflow. 
//! Note: lhs and rhs are considered unsigned here.
namespace result_of {

    template <typename NumericType1, typename NumericType2>
    struct safe_division
    {
        typedef BOOST_TYPEOF_TPL( NumericType1() / NumericType2() ) type;
    };
}
template <typename NumericType1, typename NumericType2>
inline typename result_of::safe_division<NumericType1, NumericType2>::type safe_division( const NumericType1& lhs, const NumericType2& rhs )
{
    typedef typename result_of::safe_division<NumericType1, NumericType2>::type result_type;

    NumericType1 one1 = constants::one<NumericType1>();
    NumericType2 one2 = constants::one<NumericType2>();

    // Avoid overflow.
    if( rhs < one2 && 
        lhs * one1 > rhs * (std::numeric_limits<NumericType2>::max)() )
    {
        return (std::numeric_limits<result_type>::max)();
    }

    // Avoid underflow.
    if( lhs == constants::zero< NumericType1 >() ||
        ( rhs > one2 &&
          lhs * one1 < rhs * (std::numeric_limits<NumericType2>::min)() ) )
    {
        return constants::zero<result_type>();
    }

    return lhs/rhs;
}

//! \brief Check if a number equals zero to within a tolerance range (i.e. 0 +/- tolerance).
template<typename NumberType, typename ToleranceType>
inline bool equals_zero( const NumberType& v, const ToleranceType& tolerance )
{
    return absolute_value( v ) <= construct< NumberType >( absolute_value( tolerance ) );
}

struct floating_point_comparison_type
{
	enum strength 
	{
		//! From Boost.Test FPC tools.
		FPC_STRONG, //!< "Very close"   - equation 2' in docs, the default
		FPC_WEAK    //!< "Close enough" - equation 3' in docs.
	};
};

//! \brief A predicate type to compare if two numbers are equal to within a fuzzy tolerance bound.
template <typename ToleranceType>
class equals_within_tolerance_predicate
{
public:

    explicit equals_within_tolerance_predicate( const ToleranceType& tolerance, 
                                                floating_point_comparison_type::strength fpc_type = floating_point_comparison_type::FPC_STRONG ) 
    : m_fractionTolerance( absolute_value( tolerance ) )
    , m_fpcType( fpc_type == floating_point_comparison_type::FPC_STRONG )
    {}

    template <typename NumericType1, typename NumericType2>
    bool operator()( const NumericType1& lhs, const NumericType2& rhs ) const
    {
        BOOST_AUTO( diff, absolute_value( lhs - rhs ) );
        BOOST_AUTO( d1, safe_division( diff, absolute_value( rhs ) ) );
        BOOST_AUTO( d2, safe_division( diff, absolute_value( lhs ) ) );

        return m_fpcType ? (d1 <= m_fractionTolerance && d2 <= m_fractionTolerance) 
                         : (d1 <= m_fractionTolerance || d2 <= m_fractionTolerance);
    }

private:

    ToleranceType  m_fractionTolerance;
    bool           m_fpcType;
};

//! \brief Function to check if two numbers are equal to within a specified tolerance.
template<typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool equals_within_tolerance( const NumericType1& lhs, 
                              const NumericType2& rhs,
                              const ToleranceType& tolerance, 
                              floating_point_comparison_type::strength fpc_type = floating_point_comparison_type::FPC_STRONG )
{
    equals_within_tolerance_predicate<ToleranceType> pred( tolerance, fpc_type );
    return pred( lhs, rhs );
}

}//namespace geometrix;

#endif //GEOMETRIX_DETAIL_FLOATING_POINT_COMPARISON_HPP
