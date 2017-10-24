// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

//#include <functional>
//#include <type_traits>
/*
template<class X = void>
struct holder
{
    typedef void type;
};

template<class F, class T, class X = void>
struct is_callable_with_trait
        : std::false_type
{};

template<class F, class T>
struct is_callable_with_trait<F, T, typename holder<
        decltype(std::declval<F>()(std::declval<T>()))
>::type>
        : std::true_type
{};



template<typename FOO, typename ARG>
auto chain(FOO&& foo, ARG && arg) -> decltype(foo(arg))
//    -> typename std::enable_if<
//        is_callable_with_trait<decltype(foo),decltype(arg)>::value,
//        decltype(foo(arg))>::type
{
    return foo(arg);
}

template<typename FOO, typename ARG>
auto operator>>(FOO&& foo, ARG && arg) -> decltype(foo(arg))
//     -> typename std::enable_if<
//          is_callable_with_trait<decltype(foo),decltype(arg)>::value,
//          decltype(foo(arg))>::type
{
    return foo(arg);
}
*/
#include <gtest/gtest.h>

template<typename T>
struct Wrapper {
    explicit Wrapper(T value) : value(value) {};
    operator T() {return value;}
    T value;
};

template<typename T>
Wrapper<T> FORWARD(T x)
{
    return Wrapper<T>(x);
}

template<typename T>
T operator>>(Wrapper<T> arg, T(*foo)(T))
{
    return foo(arg.value);
}

template<typename T>
Wrapper<T> operator>>(Wrapper<T> arg, Wrapper<T(*)(T)> foo)
{
    return Wrapper<T>((*foo.value)(arg.value));
}

int add(int x)
{
    return x + 4;
}

int multiply(int x)
{
    return x * 7;
}


TEST(ChainCallTestSuite, ChainCallTestCase) {
    int ret = FORWARD(add(2)) >> FORWARD(multiply);
    ASSERT_EQ(ret, 42);

    int ret_3 = FORWARD(add(2)) >> FORWARD(multiply) >> FORWARD(add);
    ASSERT_EQ(ret_3, 46);
}

//TEST(ChainCallTestSuite, ChainCallTestCase2) {
//    int ret_explicit = chain(multiply, add(2));
//    ASSERT_EQ(ret_explicit, 42);
//}