// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

// includes
#include <cstdint>

struct Lock
{
    uint64_t m_counter = 0u;
    void* m_semaphore = nullptr;

public:
    Lock();
    ~Lock();

public:
    void LockNow();
    bool TryLock();
    void UnlockNow();

    void Enter();
};

struct IScopeLock
{
private:
    Lock* m_lock = nullptr;

private:
    IScopeLock()
    {
    }

public:
    IScopeLock(Lock* lock)
    {
        m_lock = lock;
        m_lock->LockNow();
    }

    ~IScopeLock()
    {
        m_lock->UnlockNow();
    }
};

#define ScopeLock(x) IScopeLock x##_lock(&x);
