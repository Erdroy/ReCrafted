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

struct ScopeLock
{
private:
	Lock* m_lock = nullptr;

private:
	ScopeLock(){}

public:
	ScopeLock(Lock* lock)
	{
		m_lock = lock;
		m_lock->lock();
	}

	~ScopeLock()
	{
		m_lock->unlock();
	}
};

#define lock_lock(x) ScopeLock x##_lock(&x);

#endif // LOCK_H
