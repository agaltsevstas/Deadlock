#pragma once

#include <cassert>


namespace details
{
    template<class T>
    concept IsDeadlock = requires(T&& deadlock) // Условие: проверка на контейнер
    {
        deadlock.Create();
        deadlock.Islock();
        deadlock();
        deadlock.Destroy();
    };

    template<typename T>
    concept Arithmetic = std::is_arithmetic<T>::value;

    template <class T, class U>
    concept Convertible_To = std::convertible_to<T, U>;
}

namespace assert_details 
{
    template<details::Arithmetic T, typename U>
    void assert_equals_impl(T&& result, U&& expected)
    {
        if (expected != result)
            std::cerr << "Assertion failed, Result: " << result << std::endl;
    }

    template<details::IsDeadlock T>
    void assert_equals_impl(T&& deadlock)
    {
        if (deadlock() == true)
            std::cerr << "Deadlock!" << std::endl;
    }

    template<typename T>
    void assert_equals(T&& result)
    {
        assert_equals_impl(std::forward<T>(result));
    }

    template<class T, class U>
    requires details::Convertible_To<T, U>
    void assert_equals(T&& result, U&& expected)
    {
        assert_equals_impl(std::forward<T>(result), std::forward<U>(expected));
    }
}

#define ASSERT_DEADLOCK(result) \
    assert_details::assert_equals((result))

#define ASSERT_EQUALS(result, expected) \
    assert_details::assert_equals((result), (expected))
