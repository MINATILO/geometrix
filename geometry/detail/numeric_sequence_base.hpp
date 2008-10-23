//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_DETAIL_NUMERIC_SEQUENCE_BASE_HPP
#define _BOOST_GEOMETRY_DETAIL_NUMERIC_SEQUENCE_BASE_HPP
#pragma once

#include <boost/mpl/assert.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/detail/access.hpp>
#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/fusion/functional/adapter/fused.hpp>
#include <boost/fusion/functional/adapter/fused_procedure.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/config/no_tr1/utility.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/config.hpp>

namespace boost 
{ 
namespace numeric
{
namespace geometry
{
    template <typename F>
    boost::fusion::fused_procedure< F > make_fused_procedure( const F& f = F() )
    {
        return boost::fusion::fused_procedure< F >( f );
    }

    template <typename F>
    boost::fusion::fused< F > make_fused( const F& f = F() )
    {
        return boost::fusion::fused< F >( f );
    }

    template <typename NumericType, unsigned int D>
    class numeric_sequence;
}}}

namespace boost 
{
namespace fusion 
{
    //! Fusion sequence definitions.
    struct numeric_sequence_tag; //sequence tag for numeric sequence of dimension N.

    namespace traits
    {
        template<typename T, unsigned int D>
#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
        struct tag_of< boost::numeric::geometry::numeric_sequence<T,D>, void >
#else
        struct tag_of< boost::numeric::geometry::numeric_sequence<T,D> >
#endif
        {
            typedef numeric_sequence_tag type;
        };
    }

    struct random_access_traversal_tag;

    template <typename NumericSequence, int Pos>
    struct numeric_sequence_iterator
        : iterator_facade<numeric_sequence_iterator<NumericSequence, Pos>, random_access_traversal_tag>
    {
        BOOST_MPL_ASSERT_RELATION(Pos, >=, 0);
        BOOST_MPL_ASSERT_RELATION(Pos, <=, NumericSequence::static_size);

        typedef mpl::int_<Pos> index;
        typedef NumericSequence numeric_sequence_type;

        numeric_sequence_iterator(NumericSequence& v)
            : m_numeric(v) {}

        NumericSequence& m_numeric;

        template <typename Iterator>
        struct value_of
        {
            typedef typename Iterator::numeric_sequence_type numeric_sequence_type;
            typedef typename numeric_sequence_type::value_type type;
        };

        template <typename Iterator>
        struct deref
        {
            typedef typename Iterator::numeric_sequence_type numeric_sequence_type;
            typedef typename 
                mpl::if_<
                    is_const<numeric_sequence_type>
                  , typename numeric_sequence_type::const_reference
                  , typename numeric_sequence_type::reference
                >::type 
            type;

            static type
            call(Iterator const & it)
            {
                return it.m_numeric[Iterator::index::value];
            }
        };

        template <typename Iterator, typename N>
        struct advance
        {
            typedef typename Iterator::index index;
            typedef typename Iterator::numeric_sequence_type numeric_sequence_type;
            typedef numeric_sequence_iterator<numeric_sequence_type, index::value + N::value> type;

            static type
            call(Iterator const& i)
            {
                return type(i.m_numeric);
            }
        };

        template <typename Iterator>
        struct next : advance<Iterator, mpl::int_<1> > {};

        template <typename Iterator>
        struct prior : advance<Iterator, mpl::int_<-1> > {};

        template <typename I1, typename I2>
        struct distance : mpl::minus<typename I2::index, typename I1::index>
        {
            typedef typename
                mpl::minus<
                    typename I2::index, typename I1::index
                >::type 
            type;

            static type
            call(I1 const&, I2 const&)
            {
                return type();
            }
        };

    private:

        numeric_sequence_iterator<NumericSequence, Pos>& operator=(numeric_sequence_iterator<NumericSequence, Pos> const&);

    };

    namespace extension
    {
        template<typename T>
        struct value_at_impl;

        template <>
        struct value_at_impl<numeric_sequence_tag>
        {
            template <typename Sequence, typename N>
            struct apply 
            {
                typedef typename Sequence::value_type type;
            };
        };
    }

    namespace extension
    {
        template<typename T>
        struct size_impl;

        template<>
        struct size_impl<numeric_sequence_tag>
        {
            template<typename Sequence>
            struct apply : mpl::int_<Sequence::static_size> {};
        };
    }

    namespace extension
    {
        template<typename Tag>
        struct is_view_impl;

        template<>
        struct is_view_impl<numeric_sequence_tag>
        {
            template<typename T>
            struct apply : mpl::false_
            {};
        };
    }

    namespace extension
    {
        template<typename Tag>
        struct is_sequence_impl;

        template<>
        struct is_sequence_impl<numeric_sequence_tag>
        {
            template<typename Sequence>
            struct apply : mpl::true_ {};
        };
    }

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <>
        struct end_impl<numeric_sequence_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef numeric_sequence_iterator<Sequence, Sequence::static_size> type;
    
                static type
                call(Sequence& v)
                {
                    return type(v);
                }
            };
        };
    }

    namespace extension
    {
        template<typename T>
        struct category_of_impl;

        template<>
        struct category_of_impl<numeric_sequence_tag>
        {
            template<typename T>
            struct apply
            {
                typedef random_access_traversal_tag type;
            };
        };
    }

    namespace extension
    {
        template<typename T>
        struct begin_impl;

        template <>
        struct begin_impl<numeric_sequence_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef numeric_sequence_iterator<Sequence, 0> type;
    
                static type
                call(Sequence& v)
                {
                    return type(v);
                }
            };
        };
    }

    namespace extension
    {
        template<typename T>
        struct at_impl;

        template<>
        struct at_impl<numeric_sequence_tag>
        {
            template<typename Sequence, typename N>
            struct apply
            {
                typedef typename mpl::if_<
                    is_const<Sequence>, 
                    typename Sequence::const_reference, 
                    typename Sequence::reference>::type type;

                static type
                call(Sequence& seq)
                {
                    return seq[N::value];
                }
            };
        };
    }

}}//boost::fusion

namespace boost
{
namespace numeric
{
namespace geometry
{

template <typename NumericType, unsigned int Dimension>
class numeric_sequence 
{
    	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, NUMERIC_SEQUENCE_DIMENSION_NOT_DEFINED
		, (NumericType) );	
};

template <typename NumericType>
class numeric_sequence<NumericType,0>
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, NUMERIC_SEQUENCE_CANNOT_HAVE_ZERO_DIMENSION
		, (NumericType) );	
};

}}}//namespace boost::numeric::geometry

#endif //_BOOST_GEOMETRY_DETAIL_NUMERIC_SEQUENCE_BASE_HPP


