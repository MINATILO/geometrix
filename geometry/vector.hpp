//
//! Copyright � 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_VECTOR_HPP
#define GENERATIVE_GEOMETRY_VECTOR_HPP
#pragma once

#include <geometry\vector_traits.hpp>
#include <geometry\math_functions.hpp>
#include <geometry\numeric_sequence.hpp>
#include <geometry\detail/vector_generator.hpp>
#include <geometry\neutral_reference_frame.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{

//! Define access interface
template <typename NumericType, unsigned int Dimension>
struct indexed_access_policy< vector< NumericType, Dimension > > 
    : boost::integral_constant< indexed_sequence_access_type,
                                GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE >{};

//! Concrete Vector Types for some of the more common coordinate types.
typedef vector<float, 2>       vector_float_2d;
typedef vector<float, 3>       vector_float_3d;
typedef vector<double, 2>      vector_double_2d;
typedef vector<double, 3>      vector_double_3d;

typedef vector<int, 2>         vector_int_2d;
typedef vector<int, 3>         vector_int_3d;
typedef vector<long long, 2>   vector_int64_2d;
typedef vector<long long, 3>   vector_int64_3d;

GENERATIVE_GEOMETRY_DEFINE_VECTOR_TRAITS( vector_float_2d, neutral_reference_frame_float_2d );
GENERATIVE_GEOMETRY_DEFINE_VECTOR_TRAITS( vector_float_3d, neutral_reference_frame_float_3d );
GENERATIVE_GEOMETRY_DEFINE_VECTOR_TRAITS( vector_double_2d, neutral_reference_frame_double_2d );
GENERATIVE_GEOMETRY_DEFINE_VECTOR_TRAITS( vector_double_3d, neutral_reference_frame_double_3d );

GENERATIVE_GEOMETRY_DEFINE_VECTOR_TRAITS( vector_int_2d, neutral_reference_frame_int_2d );
GENERATIVE_GEOMETRY_DEFINE_VECTOR_TRAITS( vector_int_3d, neutral_reference_frame_int_3d );
GENERATIVE_GEOMETRY_DEFINE_VECTOR_TRAITS( vector_int64_2d, neutral_reference_frame_int64_2d );
GENERATIVE_GEOMETRY_DEFINE_VECTOR_TRAITS( vector_int64_3d, neutral_reference_frame_int64_3d );

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_VECTOR_HPP
