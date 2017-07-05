// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Logger.h"
#include "GameInfo.h"

Logger* Logger::m_instance;

void Logger::init()
{
	if(!GameInfo::containsArgument(TEXT("-nolo")))
	{
		m_canWrite = true;

		// open log file
		Platform::openFile(&m_logFile, "log.txt", OpenMode::OpenWrite);
	}
}

void Logger::dispose()
{
	if(m_canWrite)
	{
		// close log file
		m_logFile.close();
	}

	delete this;
}
