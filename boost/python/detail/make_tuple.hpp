# // Copyright David Abrahams 2002. Permission to copy, use,
# // modify, sell and distribute this software is granted provided this
# // copyright notice appears in all copies. This software is provided
# // "as is" without express or implied warranty, and with no claim as
# // to its suitability for any purpose.

#if !defined(BOOST_PP_IS_ITERATING)
# error Boost.Python - do not include this file!
#endif

#define N BOOST_PP_ITERATION()

#define BOOST_PYTHON_MAKE_TUPLE_ARG(N, ignored)         \
    PyTuple_SET_ITEM(                                   \
        result.ptr()                                    \
        , N                                             \
        , python::incref(python::object(a##N).ptr())    \
        );

    template <BOOST_PYTHON_UNARY_ENUM(N, class A)>
    tuple
    make_tuple(BOOST_PYTHON_BINARY_ENUM(N, A, const& a))
    {
        tuple result((detail::new_reference)::PyTuple_New(N));
        BOOST_PP_REPEAT(N, BOOST_PYTHON_MAKE_TUPLE_ARG, _)
        return result;
    }

#undef BOOST_PYTHON_MAKE_TUPLE_ARG

#undef N
