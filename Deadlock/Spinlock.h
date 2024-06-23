#pragma once

#include <iostream>
#include <mutex>
#include <thread>


class Spinlock final
{
    Spinlock(const Spinlock&) = delete;
    Spinlock(Spinlock&&) = delete;
    Spinlock& operator=(const Spinlock&) = delete;
    Spinlock& operator=(Spinlock&&) = delete;
    
public:
    Spinlock() = default;
    ~Spinlock() = default;
    
    void Lock() noexcept
    {
        if (_flag)
        {
            while (_flag);
        }
        else
        {
            _thread_id = std::this_thread::get_id();
            _flag = true;
        }
    }
    
    bool Try_lock() noexcept
    {
        auto lock = _flag.load();
        if (!lock)
            _flag = true;
        
        return !lock;
    }
    
    void Unlock() noexcept
    {
        if (_thread_id == std::this_thread::get_id())
        {
            _flag = false;
            _thread_id = std::thread::id();
        }
    }
    
private:
    std::thread::id GetThread_ID() const noexcept
    {
        return _thread_id;
    }
    
    void StrongUnlock() noexcept
    {
        _flag = false;
        _thread_id = std::thread::id();
    }
    
private:
    std::thread::id _thread_id;
    std::atomic_bool _flag = false;
    
    template <typename... TArgs> friend class Deadlock;
};
