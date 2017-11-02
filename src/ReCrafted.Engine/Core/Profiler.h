// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef PROFILER_H
#define PROFILER_H

// includes
#include "Platform/Platform.h"
#include "Logger.h"
#include "Containers/Array.h"

/// <summary>
/// Profiler class.
/// </summary>
class Profiler
{
private:
private:
	static Array<double> m_stack;

public:
	/// <summary>
	/// Begin profile.
	/// </summary>
	FORCEINLINE static void beginProfile()
	{
		auto start = Platform::getMiliseconds();
		m_stack.add(start);
	}

	/// <summary>
	/// End profile and report message to visual studio console.
	/// </summary>
	/// <param name="fmtd_message">The formated message, like: 'Profile done, action took %0.7f miliseconds.'</param>
	FORCEINLINE static void endProfile(const char* fmtd_message)
	{
		if (m_stack.size() == 0u)
			return;

		auto start = m_stack.last();
		auto end = Platform::getMiliseconds();
		auto time = end - start;

		char buffer[512] = {};
		sprintf_s(buffer, fmtd_message, time);
		
		Logger::write(buffer, LogLevel::Debug);

		// clear
		m_stack.removeAt(m_stack.count() - 1);
	}
};

#endif // PROFILER_H
