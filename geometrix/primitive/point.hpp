//
//! Copyright ? 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_HPP
#define GEOMETRIX_POINT_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/tensor/detail/point_generator.hpp>
#include <geometrix/space/neutral_reference_frame.hpp>
#include <geometrix/tensor/index_operator_vector_access_policy.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>

namespace geometrix {
    
//! Concrete Point Types for some of the more common coordinate types.
typedef point<float, 2>       point_float_2d;
typedef point<float, 3>       point_float_3d;
typedef point<double, 2>      point_double_2d;
typedef point<double, 3>      point_double_3d;
typedef point<double, 4>      point_double_4d;

typedef point<int, 2>         point_int_2d;
typedef point<int, 3>         point_int_3d;

}//namespace geometrix;

// GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_float_2d, (float), 2, float, double, neutral_reference_frame_2d, index_operator_vector_access_policy<point_float_2d> );
// GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_float_3d, (float), 3, float, double, neutral_reference_frame_3d, index_operator_vector_access_policy<point_float_3d> );
// GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_double_2d, (double), 2, double, double, neutral_reference_frame_2d, index_operator_vector_access_policy<point_double_2d> );
// GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_double_3d, (double), 3, double, double, neutral_reference_frame_3d, index_operator_vector_access_policy<point_double_3d> );
// GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_double_4d, (double), 4, double, double, neutral_reference_frame<4>, index_operator_vector_access_policy<point_double_4d> );
// 
// GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_int_2d, (int), 2, int, double, neutral_reference_frame_2d, index_operator_vector_access_policy<point_int_2d> );
// GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_int_3d, (int), 3, int, double, neutral_reference_frame_3d, index_operator_vector_access_policy<point_int_3d> );


namespace boost {
    namespace mpl {

        template<typename>
        struct sequence_tag;

        template<typename T, std::size_t D>
        struct sequence_tag<geometrix::point<T, D>>
        {
            typedef fusion::fusion_sequence_tag type;
        };

        template<typename T, std::size_t D>
        struct sequence_tag<const geometrix::point<T, D>>
        {
            typedef fusion::fusion_sequence_tag type;
        };
    }
}//! namespace boost::mpl;
 /***/

namespace geometrix {
    template<typename T, std::size_t D>
    struct tensor_traits< geometrix::point<T, D> >
        : index_operator_vector_access_policy<geometrix::point<T, D>>
    {
        typedef index_operator_vector_access_policy<geometrix::point<T, D>> access_policy;
        typedef boost::mpl::int_<1> tensor_order;
        typedef void GEOMETRIX_TENSOR_ORDER(1);
        typedef void is_tensor;
        typedef void make_fusion_sequence;
    };

    template<typename T, std::size_t D>
    struct geometric_traits< geometrix::point<T, D> >
    {
        using hyperplane_dimension = dimension<1>;
        typedef geometrix::point<T, D>                point_type;
        typedef void                                  is_point;
        typedef neutral_reference_frame<D>            reference_frame;
        typedef void                                  is_coordinate_sequence;
        typedef typename arithmetic_promotion_policy<T>::type arithmetic_type;
        typedef decltype(std::declval<arithmetic_type>() / std::declval<arithmetic_type>()) dimensionless_type;
        typedef void                                  is_numeric_sequence;
        typedef geometrix::point<T, D>                sequence_type;
        typedef GEOMETRIX_AS_MPL_VECTOR((T))          storage_types;
        typedef dimension<D>                          dimension_type;
        typedef void                                  is_sequence;
        typedef void GEOMETRIX_TYPE_DIVERSITY_TAG((T));
    };
}//! namespace geometrix;
 /***/

#endif //GEOMETRIX_POINT_HPP
