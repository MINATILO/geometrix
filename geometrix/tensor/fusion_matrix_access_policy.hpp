//
//! Copyright ? 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_FUSION_MATRIX_ACCESS_POLICY_HPP
#define GEOMETRIX_FUSION_MATRIX_ACCESS_POLICY_HPP

#include <geometrix/tensor/tensor_access_policy.hpp>

namespace geometrix {

template <typename Matrix>
struct fusion_matrix_access_policy
{
    typedef typename remove_const_ref<Matrix>::type matrix_type;

    template <std::size_t Row, std::size_t Column>
    struct type_at
        : boost::fusion::result_of::at_c< row<matrix_type,Row>, Column >
    {
        BOOST_MPL_ASSERT_MSG
        (
            boost::mpl::bool_< ( row_dimension_of< Matrix >::value > Row && column_dimension_of< Matrix >::value > Column ) >::value
          , MATRIX_INDICES_ARE_OUT_OF_BOUNDS
          , (type_at<Row,Column>)
        );
    };

    template <std::size_t Row, std::size_t Column>
    static typename type_at<Row, Column>::type get( const Matrix& matrix ) 
    {
        return boost::fusion::at_c<Column>( boost::fusion::at_c<Row>(matrix) );
    }

    template <std::size_t Row, std::size_t Column>
    static void set( Matrix& matrix, typename boost::call_traits<typename type_at<Row,Column>::type>::param_type v ) 
    {
        boost::fusion::at_c<Column>( boost::fusion::at_c<Row>(matrix) ) = v;
    }
};

}//namespace geometrix;

#endif//GEOMETRIX_FUSION_MATRIX_ACCESS_POLICY_HPP
