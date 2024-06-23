#pragma once

template <class TMutex>
class LockGuard
{
    LockGuard(const LockGuard&) = delete;
    
public:
    
    explicit LockGuard(TMutex& iMutex): _mutex(iMutex)
    {
        _mutex.Lock();
    }
    
    ~LockGuard()
    {
        _mutex.Unlock();
    }
    
private:
    TMutex& _mutex;
};
