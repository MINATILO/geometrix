//
//! Copyright � 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//! NOTE: This file is automatically generated. Any CHANGES made to this
//! file will be LOST on the next generation.
    namespace geometrix {
        template <typename N, unsigned int Rows, unsigned int Columns>
        struct construction_policy< matrix< N, Rows, Columns> >
        { 
            template <typename Matrix> 
            static matrix<N, Rows, Columns> construct( const Matrix& m )
            { 
                return detail::matrix_pod_constructor<Rows,Columns>::construct< matrix<N, Rows, Columns> >( m );
            }
        };
        template <typename N, unsigned int Rows, unsigned int Columns>
        struct geometric_traits< matrix<N,Rows,Columns> > 
        {
            typedef dimension<Rows> row_dimension;
            typedef dimension<Columns> col_dimension;
            typedef void is_matrix;
            typedef void is_homogeneous;
        }; 
        template <typename N, unsigned int Rows, unsigned int Columns, unsigned int Row>
        struct geometric_traits< row<matrix<N,Rows,Columns>,Row> > 
        {
            typedef boost::mpl::vector<N> storage_types;
            typedef dimension<Columns> dimension_type;
            typedef void is_sequence;
            typedef void is_numeric_sequence;
            typedef void is_vector;
            typedef void is_homogeneous;
        }; 
        template <typename N, unsigned int Rows, unsigned int Columns, unsigned int Column>
        struct geometric_traits< column<matrix<N,Rows,Columns>,Column> > 
        {
            typedef boost::mpl::vector<N> storage_types;
            typedef dimension<Rows> dimension_type; 
            typedef void is_sequence; 
            typedef void is_numeric_sequence; 
            typedef void is_vector; 
            typedef void is_homogeneous; 
        }; 
        template <typename N, unsigned int Rows, unsigned int Columns>
        struct tensor_traits< matrix<N, Rows, Columns> >
        {
            typedef index_operator_matrix_access_policy< matrix<N,Rows,Columns> > access_policy;
            typedef boost::mpl::int_<2> tensor_order;
            typedef void rank_2;
            typedef void is_tensor;
        };
        
    }
