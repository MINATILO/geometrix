//
//! Copyright ? 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_ABSOLUTE_VALUE_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_ABSOLUTE_VALUE_HPP

#include <geometrix/algebra/functions/unary_function.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>

namespace geometrix {

    namespace tag
    {
        struct absolute_value{};
    }
    
    //! absolute value of a vector.
    template <typename T>
    struct un_fun
        <
            geometrix::tag::absolute_value
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_vector 
        > 
        : diversity_base<T>
    {
        typedef void                           is_vector;
        typedef void                           rank_1;
        typedef typename dimension_of<T>::type dimension_type;
		typedef typename reference_frame_of<T>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                           is_sequence;
        typedef void                           is_numeric_sequence;
        typedef void                           is_coordinate_sequence;

		template <std::size_t Index>
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef geometrix::tag::absolute_value tag;
            
            typedef typename type_at<T, Index>::type result_type;

            result_type operator()(tag, const T& a) const
            {          
				using std::abs;
                return abs(get<Index>( a )); 
            }
        };    
    };

	//! absolute value of a scalar.
	template <typename T>
	struct un_fun
		<
		    geometrix::tag::absolute_value
		  , T
		  , typename geometric_traits<typename remove_const_ref<T>::type>::is_scalar
		>
	{
		typedef void is_scalar;
		typedef void rank_0;
		typedef void is_homogeneous;

		struct context : boost::proto::callable_context < const context >
		{
			typedef geometrix::tag::absolute_value tag;

			typedef typename type_at<T>::type result_type;
			result_type operator()( tag, const T& a ) const
			{
				using std::abs;
				return abs(geometrix::get( a ));
			}
		};
	};

	//! absolute value of a matrix.
	template <typename T>
	struct un_fun
		<
		    geometrix::tag::absolute_value
		  , T
		  , typename geometric_traits<typename remove_const_ref<T>::type>::is_matrix
		>
		: diversity_base < T >
	{
		typedef void                                  is_matrix;
		typedef void                                  rank_2;
		typedef typename row_dimension_of<T>::type    row_dimension;
		typedef typename column_dimension_of<T>::type col_dimension;

		template <std::size_t Row, std::size_t Column>
		struct context : boost::proto::callable_context < const context<Row, Column> >
		{
			typedef geometrix::tag::absolute_value tag;

			typedef typename type_at<T, Row, Column>::type result_type;
			result_type operator()( tag, const T& a ) const
			{
				using std::abs;
				return abs(geometrix::get<Row, Column>( a ));
			}
		};
	};
    
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_ABSOLUTE_VALUE_HPP
