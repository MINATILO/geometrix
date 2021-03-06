//
//! Copyright ? 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHMN_ROTATION_HPP
#define GEOMETRIX_ALGORITHMN_ROTATION_HPP

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/tensor/matrix.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/algebra/exterior_product.hpp>
#include <geometrix/arithmetic/vector/magnitude.hpp>
#include <boost/iterator/transform_iterator.hpp>

namespace geometrix 
{
	//! Make a rotation matrix for the angle between two unit vectors.
	template <typename Vector1, typename Vector2>
	inline matrix<typename geometric_traits<Vector1>::dimensionless_type, 2, 2> make_rotation_matrix(const Vector1& v1, const Vector2& v2)
	{
		using dimensionless_t = typename geometric_traits<Vector1>::dimensionless_type;
		auto cosa = construct<dimensionless_t>(dot_product(v1, v2));
		auto sina = construct<dimensionless_t>(exterior_product_area(v1, v2));
		matrix<dimensionless_t, 2, 2> m{ { { cosa, get(-sina) }
		        , { sina,  cosa } } };

		return m;
	}

	template <typename Point, typename ArithmeticType>
	inline Point rotate_point(const Point& p, const matrix<ArithmeticType, 2, 2>& rot, const Point& rotationOrigin)
	{
		return construct<Point>((rot * (p - rotationOrigin)) + as_vector(rotationOrigin));
	}

	template <typename Point, typename Vector>
	inline Point rotate_point(const Point& p, const Vector& v1, const Vector& v2, const Point& rotationOrigin)
	{
		return rotate_point(p, make_rotation_matrix(v1, v2), rotationOrigin);
	}

	template <typename Vector, typename ArithmeticType>
	inline Vector rotate_vector(const Vector& v, const matrix<ArithmeticType, 2, 2>& rot)
	{
		return construct<Vector>(rot * v);
	}

	template <typename Vector>
	inline Vector rotate_vector(const Vector& v, const Vector& v1, const Vector& v2)
	{
		return rotate_vector(v, make_rotation_matrix(v1, v2));
	}
	
	template <typename PointSequence, typename Vector, typename Point>
	inline PointSequence rotate_points(const PointSequence& poly, const Vector& v1, const Vector& v2, const Point& rotationOrigin)
	{
		auto rot = make_rotation_matrix(v1, v2);
		auto rotatePoint = [&rot, &rotationOrigin](const Point& p) -> Point
		{
			return construct<Point>((rot * (p - rotationOrigin)) + as_vector(rotationOrigin));
		};

		return PointSequence(boost::make_transform_iterator(poly.begin(), rotatePoint), boost::make_transform_iterator(poly.end(), rotatePoint));
	}

	template <typename PointSequence, typename Matrix, typename Vector, typename Point>
	inline PointSequence rotate_translate_points(const PointSequence& poly, const Matrix& rot, const Vector& translation, const Point& rotationOrigin)
	{		
		auto rotatePoint = [&rot, &translation, &rotationOrigin](const Point& p) -> Point
		{
			return construct<Point>(((rot * (p - rotationOrigin)) + as_vector(rotationOrigin)) + translation);
		};

		return PointSequence(boost::make_transform_iterator(poly.begin(), rotatePoint), boost::make_transform_iterator(poly.end(), rotatePoint));
	}

	template <typename PointSequence, typename Vector1, typename Vector2, typename TransVector, typename Point>
	inline PointSequence rotate_translate_points(const PointSequence& poly, const Vector1& v1, const Vector2& v2, const TransVector& translation, const Point& rotationOrigin)
	{
		auto rot = make_rotation_matrix(v1, v2);
		return rotate_translate_points(poly, rot, translation, rotationOrigin);
	}
	
}//! namespace geometrix;

#endif //GEOMETRIX_ALGORITHMN_ROTATION_HPP
