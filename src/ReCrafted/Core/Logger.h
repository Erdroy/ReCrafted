// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef LOGGER_H
#define LOGGER_H

// includes
#include "../recraftedPrerequisites.h"
#include "../Platform/Platform.h"

#ifndef LOGGER_MAXSIZE
	#define LOGGER_MAXSIZE 8192
#endif

struct LogLevel
{
	enum Enum
	{
		Info,
		Debug,
		Warning,
		Error,
		Fatal,

		Count
	};
};

class Logger
{
	friend class GameCore;

private:
	static Logger* m_instance;

private:
	File m_logFile = {};
	
private:
	Logger() { m_instance = this; }

private:
	void init();
	void dispose();

public:
	FORCEINLINE static void write(const char* messageA, const char* messageB, const char* messageC, LogLevel::Enum logLevel = LogLevel::Debug)
	{
#ifndef _DEBUG
		if (logLevel == LogLevel::Debug)
			return;
#endif
		static char buffer[LOGGER_MAXSIZE] = {};

		strcpy_s(buffer, messageA);
		strcat_s(buffer, messageB);
		strcat_s(buffer, messageC);

		write(buffer, logLevel);
	}

	FORCEINLINE static void write(const char* messageA, const char* messageB, LogLevel::Enum logLevel = LogLevel::Debug)
	{
		write(messageA, messageB, "", logLevel);
	}

	FORCEINLINE static void write(const char* message, LogLevel::Enum logLevel = LogLevel::Debug)
	{
#ifndef _DEBUG
		if (logLevel == LogLevel::Debug)
			return;
#endif

		static char buffer[LOGGER_MAXSIZE] = {};
		static char timeBuffer[64] = {};
		buffer[0] = '\0';
		timeBuffer[0] = '\0';

		// create date
		time_t rawtime;
		time(&rawtime);
		struct tm timeinfo;
		localtime_s(&timeinfo, &rawtime);
		strftime(reinterpret_cast<char*>(timeBuffer), 64, "%T ", &timeinfo);

		// write date
		strcat_s(buffer, timeBuffer);

		switch(logLevel)
		{
		case LogLevel::Info: strcat_s(buffer, "[INFO] "); break;
		case LogLevel::Debug: strcat_s(buffer, "[DEBUG] "); break;
		case LogLevel::Warning: strcat_s(buffer, "[WARNING] "); break;
		case LogLevel::Error: strcat_s(buffer, "[ERROR] "); break;
		case LogLevel::Fatal: strcat_s(buffer, "[FATAL] "); break;
		case LogLevel::Count: break;
		default: break;
		}

		strcat_s(buffer, message);
		strcat_s(buffer, "\n");

		m_instance->m_logFile.write(static_cast<void*>(buffer), size_t(strlen(buffer)));
		m_instance->m_logFile.flush();
	}
};

#endif // LOGGER_H