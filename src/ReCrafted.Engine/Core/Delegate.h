// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef DELEGATE_H
#define DELEGATE_H

// includes
#include "ReCrafted.h"

interface IDelegateHandler {};

#define DelegateHandler public IDelegateHandler

typedef void(IDelegateHandler::*delegate_noparam)();

/**
 * \brief Delegate structure.
 */
struct Delegate
{
private:
	IDelegateHandler* m_instance = nullptr;
	delegate_noparam m_delegate = nullptr;

private:
	Delegate() {}

public:
	Delegate(IDelegateHandler* instance, delegate_noparam delegate) : m_instance(instance), m_delegate(delegate) { }

public:
	FORCEINLINE void Invoke() const 
	{
		if (this->m_instance == nullptr)
			return;

		(this->m_instance->*this->m_delegate)();
	}
};

#define MakeDelegate(func) Delegate((IDelegateHandler*)this, (delegate_noparam)&func)

#endif // DELEGATE_H
