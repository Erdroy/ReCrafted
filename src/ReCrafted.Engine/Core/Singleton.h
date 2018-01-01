// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SINGLETON_H
#define SINGLETON_H

// includes
#include "ReCrafted.h"

template<class T>
class Singleton : IDisposable
{
	static T* m_instance;

protected:
	Singleton() {}
	~Singleton() {}

private:
	virtual void onDispose() = 0;

public:
	void dispose() override
	{
		onDispose();
		SafeDelete(m_instance);
	}

public:
	static T* getInstance()
	{
		if (m_instance)
			return m_instance;

		m_instance = new T;
		return m_instance;
	}
};

#endif // SINGLETON_H
