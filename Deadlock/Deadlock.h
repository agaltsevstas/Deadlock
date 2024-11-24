#pragma once

#include <optional>
#include <thread>
#include <tuple>


template<typename... TArgs>
class Deadlock final
{
    using Mutexs = std::tuple<TArgs&...>;
    
public:
    Deadlock(TArgs& ...args):
    _flag(false),
    _mutexs(args...) {}
    
    void SetLock(bool flag)
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
            auto Check = [this](const auto& mutex)
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
        auto MutexUnlock = [](auto& mutex)
        {
            mutex.StrongUnlock();
        };
        
        CallApply(MutexUnlock);
    }
    
    bool operator()() const
    {
        std::optional<std::thread::id> thread_id;
        auto Check = [&thread_id](const auto& mutex) -> bool
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
    decltype(auto) CallApply(TFunction&& function) const
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
