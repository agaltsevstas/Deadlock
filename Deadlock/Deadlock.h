#pragma once

#include <optional>
#include <thread>
#include <tuple>


template<typename... TArgs>
class Deadlock final
{
    using Mutexs = std::tuple<TArgs&...>;
    
public:
    Deadlock(TArgs& ...args) noexcept :
    _flag(false),
    _mutexs(args...) {}
    
    void SetLock(bool flag) noexcept
    {
        _flag = flag;
    }
    
    bool Islock() const noexcept
    {
        return _flag;
    }
    
    void Create()
    {
        if (_flag)
        {
            auto Check = [this](auto&& mutex) noexcept
            {
                if (mutex.GetThread_ID() != std::this_thread::get_id() &&
                    mutex.GetThread_ID() != std::thread::id())
                {
                    _flag = false;
                }
            };
            
            while (_flag)
            {
                CallApply(Check);
            }
        }
    }
    
    void Destroy()
    {
        auto MutexUnlock = [](auto&& mutex) noexcept
        {
            mutex.StrongUnlock();
        };
        
        CallApply(MutexUnlock);
    }
    
    bool operator()() const noexcept
    {
        std::optional<std::thread::id> thread_id;
        auto Check = [&thread_id](auto&& mutex) noexcept -> bool
        {
            if (thread_id.has_value() && *thread_id != mutex.GetThread_ID())
                return true;
            else
                thread_id = mutex.GetThread_ID();
            
            return false;
        };
        
        return CallApply(Check);
    }
    
private:
    template<typename TFunction>
    decltype(auto) CallApply(TFunction&& function) const noexcept
    {
        return std::apply([&function](auto&& ...args) noexcept -> decltype(auto)
        {
            return (function(std::forward<decltype(args)>(args)), ...);
        }, _mutexs);
    }
    
private:
    bool _flag = false;
    Mutexs _mutexs;
};
