// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef LOCK_H
#define LOCK_H

// includes
#include <cstdint>

struct Lock
{
	uint64_t m_counter = 0u;
	void* m_semaphore = nullptr;

public:
	Lock();

	void lock();
	bool tryLock();
	void unlock();

	void dispose();
};

struct IScopeLock
{
private:
	Lock* m_lock = nullptr;

private:
    IScopeLock(){}

public:
    IScopeLock(Lock* lock)
	{
		m_lock = lock;
		m_lock->lock();
	}

	~IScopeLock()
	{
		m_lock->unlock();
	}
};

#define ScopeLock(x) IScopeLock x##_lock(&x);

#endif // LOCK_H
