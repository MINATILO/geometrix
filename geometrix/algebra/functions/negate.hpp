//
//! Copyright ? 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_NEGATE_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_NEGATE_HPP

#include <geometrix/algebra/functions/unary_function.hpp>
#include <boost/units/quantity.hpp>
#include <type_traits>

namespace geometrix {

	template <typename T, typename EnableIf = void>
	struct should_expression_negate : boost::false_type {};

	template <typename T>
	struct should_expression_negate<T, typename std::enable_if<is_vector<typename std::decay<T>::type>::value>::type> : std::true_type {};

	template <typename T>
	struct should_expression_negate<T, typename std::enable_if<is_matrix<typename std::decay<T>::type>::value>::type> : std::true_type {};
	
    namespace tag
    {
        struct negate{};
    }
	    
    //! negate a vector.
    template <typename T>
    struct un_fun
        < 
            boost::proto::tag::negate
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
            typedef boost::proto::tag::negate tag;
            using result_type = typename std::decay<typename type_at<T, Index>::type>::type;

            result_type operator()(tag, const T& a) const
            {            
                return -geometrix::get<Index>(a);
            }
        };
    };

    //! negate a scalar.
    template <typename T>
    struct un_fun
        < 
            boost::proto::tag::negate
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_scalar 
        > 
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;

        struct context : boost::proto::callable_context< const context >
        {            
            typedef boost::proto::tag::negate tag;
        
			using result_type = typename std::decay<typename type_at<T>::type>::type; 

			result_type operator()(tag, const T& a) const
            {
				return -geometrix::get(a);
            }
        };
    };

    //! negate a matrix.
    template <typename T>
    struct un_fun
        <
            boost::proto::tag::negate
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_matrix 
        >  
        : diversity_base<T>
    {
        typedef void                                  is_matrix;
        typedef void                                  rank_2;
        typedef typename row_dimension_of<T>::type    row_dimension;
        typedef typename column_dimension_of<T>::type col_dimension;

		template <std::size_t Row, std::size_t Column>
        struct context : boost::proto::callable_context< const context<Row, Column> >
        {            
            typedef boost::proto::tag::negate tag;
        
			using result_type = typename std::decay<typename type_at<T, Row, Column>::type>::type;

            result_type operator()(tag, const T& a) const
            {            
                return -geometrix::get<Row, Column>( a );
            }
        };
    };

}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_NEGATE_HPP
