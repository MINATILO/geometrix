//
//! Copyright ? 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_MULTIPLICATION_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_MULTIPLICATION_HPP

#include <geometrix/algebra/functions/binary_function.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>

namespace geometrix {
        
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::multiplies, is_scalar, is_vector );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::multiplies, is_vector, is_scalar );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::multiplies, is_scalar, is_matrix );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::multiplies, is_matrix, is_scalar );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::multiplies, is_scalar, is_scalar );

    //! Product of Scalar and Scalar
    template <typename Left, typename Right>
    struct bin_fun
        <
            boost::proto::tag::multiplies
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_scalar
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_scalar 
        >
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;

        struct context : boost::proto::callable_context< const context >
        {            
            typedef boost::proto::tag::multiplies tag_t;

            using left_t = Left;
            using right_t = Right;

            using left_eval_t = typename type_at<Left>::type;
            using right_eval_t = typename type_at<Right>::type;

            typedef typename result_of::multiplies
                    <
                        typename type_at<Left>::type
                      , typename type_at<Right>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return do_multiplies(get( l ), get( r ));
            }
        };
    };
        
    //! Product of Scalar and Vector
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::multiplies
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_scalar
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : diversity_base<Right>
    {
        typedef void                                     is_vector;
        typedef void                                     rank_1;
        typedef typename dimension_of<Right>::type       dimension_type;
        typedef typename reference_frame_of<Right>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                                     is_sequence;
        typedef void                                     is_numeric_sequence;
        typedef void                                     is_coordinate_sequence;

		template <std::size_t Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index, Callable> >
        {            
            typedef boost::proto::tag::multiplies tag_t;            
            typedef typename result_of::multiplies
                    <
                        typename type_at<Left>::type
                      , typename type_at<Right, Index>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return do_multiplies(get( l ), get<Index>( r ));
            }
        };
    };

    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::multiplies
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_scalar
        >
        : diversity_base<Left>
    {
        typedef void                                     is_vector;
        typedef void                                     rank_1;
        typedef typename dimension_of<Left>::type        dimension_type;
        typedef typename reference_frame_of<Left>::type  reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                                     is_sequence;
        typedef void                                     is_numeric_sequence;
        typedef void                                     is_coordinate_sequence;

		template <std::size_t Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index, Callable> >
        {            
            typedef boost::proto::tag::multiplies tag_t;            
            typedef typename result_of::multiplies
                <
                    typename type_at<Left, Index>::type
                  , typename type_at<Right>::type
                >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return do_multiplies(get<Index>( l ), get( r ));
            }
        };
    };
        
    //! Product of Scalar and Matrix
    template <typename Left, typename Right>
    struct bin_fun
        <
            boost::proto::tag::multiplies
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_scalar
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_matrix 
        >
        : diversity_base<Right>
    {
        typedef void                                      is_matrix;
        typedef void                                      rank_2;
        typedef typename row_dimension_of<Right>::type    row_dimension;
        typedef typename column_dimension_of<Right>::type col_dimension;

		template <std::size_t Row, std::size_t Column, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Row, Column, Callable> >
        {            
            typedef boost::proto::tag::multiplies tag_t;
            typedef typename result_of::multiplies
                    <
                        typename type_at<Left>::type
                      , typename type_at<Right, Row, Column>::type
                    >::type result_type;
            
            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return do_multiplies(get( l ), get<Row, Column>( r ));
            }
        };
    };

    template <typename Left, typename Right>
    struct bin_fun
        <
            boost::proto::tag::multiplies
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_scalar
        >
        : diversity_base<Left>
    {
        typedef void                                      is_matrix;
        typedef void                                      rank_2;
        typedef typename row_dimension_of<Left>::type     row_dimension;
        typedef typename column_dimension_of<Left>::type  col_dimension;

		template <std::size_t Row, std::size_t Column, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Row, Column, Callable> >
        {            
            typedef boost::proto::tag::multiplies tag_t;
            typedef typename result_of::multiplies
                <
                    typename type_at<Left, Row, Column>::type
                  , typename type_at<Right>::type
                >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return do_multiplies(get<Row, Column>( r ), get( l ));
            }
        };
    };
    
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_MULTIPLICATION_HPP
