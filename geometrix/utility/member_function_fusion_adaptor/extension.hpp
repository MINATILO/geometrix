//
//! Copyright ? 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_EXTENSION_HPP
#define GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_EXTENSION_HPP


namespace boost 
{
namespace fusion 
{
namespace extension
{
    
    template <typename T, int N>
    struct member_function{};

    template <typename T>
    struct member_function_size{};

    template <typename T, int N>
    struct member_function<T const, N>
        : member_function<T, N>
    {};
    
    template <typename T>
    struct member_function_size<T const>
        : member_function_size<T>
    {};

}}}

#endif //GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_EXTENSION_HPP
