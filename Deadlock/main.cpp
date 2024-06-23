#include "Deadlock.h"
#include "LockGuard.h"
#include "Spinlock.h"
#include "Test.h"

#include <iostream>
#include <mutex>
#include <thread>


int main()
{
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
        deadlock.SetLock(true); // Включение deadlock
        std::thread thread1(function1);
        std::thread thread2(function2);

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Время на deadlock
        
        ASSERT_DEADLOCK(deadlock); // Проверка на deadlock
        ASSERT_EQUALS(deadlock() == true, 1); // Проверка на deadlock
        
        deadlock.Destroy(); // Разрушение deadlock
        
        thread1.join();
        thread2.join();
    }
    /// No Deadlock
    {
        deadlock.SetLock(false); // Отключение deadlock
        std::thread thread1(function1);
        std::thread thread2(function2);

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Время на deadlock
        
        ASSERT_DEADLOCK(deadlock); // Проверка на deadlock
        ASSERT_EQUALS(deadlock() == false, 1); // Проверка на deadlock
        
        thread1.join();
        thread2.join();
    }

    
    return 0;
}
