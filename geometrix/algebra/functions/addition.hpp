//
//! Copyright ? 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_ADDITION_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_ADDITION_HPP

#include <geometrix/algebra/functions/binary_function.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>

namespace geometrix {
        
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::plus, is_sequence, is_vector );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::plus, is_matrix, is_matrix );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::plus, is_scalar, is_scalar);
        
    //! Sum of Scalar and Scalar
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::plus
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
            typedef boost::proto::tag::plus tag_t;
            typedef typename result_of::plus
                    <
                        typename type_at<Left>::type
                      , typename type_at<Right>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return do_plus(get( l ), get( r ));
            }
        };
    };
        
    //! Addition of Vectors
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::plus
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
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
            typedef boost::proto::tag::plus tag_t;
            typedef typename result_of::plus
                    <
                        typename type_at<Left,Index>::type
                      , typename type_at<Right,Index>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return do_plus(get<Index>( l ), get<Index>( r ));
            }
        };
    };
        
    //! Addition of a Vector to a Point
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::plus
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_point
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                     is_point;
        typedef typename dimension_of<Right>::type       dimension_type;
        typedef void                                     rank_1;
        typedef typename reference_frame_of<Right>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                                     is_sequence;
        typedef void                                     is_numeric_sequence;
        typedef void                                     is_coordinate_sequence;

        template <std::size_t Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index, Callable> >
        {            
            typedef boost::proto::tag::plus tag_t;
            typedef typename result_of::plus
                    <
                        typename type_at<Left,Index>::type
                      , typename type_at<Right,Index>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return do_plus(get<Index>( l ), get<Index>( r ));
            }
        };
    };
        
    //! Addition of Matrices
    template <typename Left, typename Right>
    struct bin_fun
        <
            boost::proto::tag::plus
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_matrix 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                      is_matrix;
        typedef void                                      rank_2;
        typedef typename row_dimension_of<Right>::type    row_dimension;
        typedef typename column_dimension_of<Right>::type col_dimension;

		template <std::size_t Row, std::size_t Column, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Row, Column, Callable> >
        {            
            typedef boost::proto::tag::plus tag_t;
            typedef typename result_of::plus
                    <
                        typename type_at<Left,Row,Column>::type
                      , typename type_at<Right,Row,Column>::type
                    >::type result_type;
            
            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return do_plus(get<Row, Column>( l ), get<Row, Column>( r ));
            }
        };
    };
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_ADDITION_HPP
