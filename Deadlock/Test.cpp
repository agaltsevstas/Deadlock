#include "Deadlock.h"
#include "LockGuard.h"
#include "Spinlock.h"

#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>

/*
 bash: https://github.com/Abd3lwahab/Test-Cases
 */

namespace details
{
    template<class T>
    concept IsDeadlock = requires(const T& deadlock) // Условие: проверка на контейнер
    {
        deadlock.Islock();
        deadlock();
    };
}

namespace assert_details
{
    template<details::IsDeadlock T>
    void assert_equals(const T& deadlock)
    {
        if (deadlock() == true)
            std::cerr << "failed!" << std::endl;
        else
            std::cerr << "passed!" << std::endl;
    }
}

#define ASSERT_DEADLOCK(result) \
    assert_details::assert_equals(result)


int main(int argc, char* argv[])
{
    if (argc != 2)
        throw std::runtime_error("Missing argument!");
    
    bool isDeadlock = false;
    if (strcmp(argv[1], "true") == 0)
        isDeadlock = true;
    else if (strcmp(argv[1], "false") == 0)
        isDeadlock = false;
    else
        throw std::runtime_error("Invalid argument!");
    
    Spinlock mutex1;
    Spinlock mutex2;
    Deadlock deadlock(mutex1, mutex2);
    
    auto function1 = [&]()
    {
        LockGuard lock1(mutex1);
        if (deadlock.Islock()) // Проверка на включенный deadlock
            deadlock.Create(); // Создание deadlock
        LockGuard lock2(mutex2);
    };
    
    auto function2 = [&]()
    {
        LockGuard lock1(mutex2);
        if (deadlock.Islock()) // Проверка на включенный deadlock
            deadlock.Create(); // Создание deadlock
        LockGuard lock2(mutex1);
    };
    
    /// Deadlock
    {
        deadlock.SetLock(isDeadlock); // Включение deadlock
        std::thread thread1(function1);
        std::thread thread2(function2);

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Время на deadlock
        
        ASSERT_DEADLOCK(deadlock); // Проверка на deadlock
        
        deadlock.Destroy(); // Разрушение deadlock
        
        thread1.join();
        thread2.join();
    }
    
    return 0;
}
