// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once
#include <vector>
#include <deque>

#ifdef _WIN32 // windows
#ifndef PLATFORMWINDOWS_H
#define PLATFORMWINDOWS_H

// includes
#include "../Utils/Defines.h"
#include <cstdio>
#include <string.h>
#include <crtdbg.h>
#include <Windows.h>

#define PATHLENGTH 512

/// <summary>
/// File structure
/// </summary>
struct File
{
	friend class Platform;

private:
	FILE* m_file = nullptr;

public:

	char FileName[PATHLENGTH];
	size_t FileSize = 0u;

	~File()
	{
		// out of scope, close the file
		if (m_file) 
			close();
	}

	/// <summary>
	/// Read file to the buffer.
	/// </summary>
	/// <param name="buffer">The buffer pointer.</param>
	/// <param name="length">How much bytes read?</param>
	/// <param name="offset">The offset in bytes where should start.</param>
	void read(void* buffer, size_t length, size_t offset = 0) const
	{
		// set offset
		fseek(m_file, long(offset), SEEK_SET);

		// read
		fread(buffer, length, 1, m_file);

		// reset position to zero
		fseek(m_file, 0, SEEK_SET);
	}

	/// <summary>
	/// Read whole file to the buffer.
	/// </summary>
	/// <param name="buffer">The buffer pointer.</param>
	void read(void* buffer) const
	{
		read(buffer, FileSize);
	}

	void write(void* data, size_t data_lenght) const
	{
		fwrite(data, data_lenght, 1, m_file);
	}

	void flush() const
	{
		fflush(m_file);
	}

	/// <summary>
	/// Close the file. This will be called automatically when File strcture is out of scope.
	/// </summary>
	FORCEINLINE void close()
	{
		fclose(m_file);
		m_file = nullptr;
	}
};

/// <summary>
/// OpenMode
/// </summary>
struct OpenMode
{
	enum Enum
	{
		OpenRead,
		OpenWrite,
		OpenAppend,

		OpenReadWrite
	};
};

/// <summary>
/// Thread structure.
/// Contains some needed informations for threads.
/// </summary>
struct Thread
{
	uint64_t ThreadId;
	void* ThreadHandle;
};

typedef uint64_t(*ThreadFunction)(void* parameter);
typedef unsigned long(*QueueFunction)(void* parameter);

#define MAX_THREADS 16

DWORD WINAPI ThreadWorker(LPVOID lpParam);

/// <summary>
/// Platform class.
/// </summary>
class Platform
{
	friend class GameMain;

private:
	struct QueueEntry
	{
	public:
		bool isStarted = false;
		bool isEnded = false;
		QueueFunction functio = nullptr;
	};

private:
	static HWND m_gameWindow;

	// timer
	static LARGE_INTEGER m_frequency;
	static double m_start;
	static double m_freqCoeff;
	static HANDLE m_threadPool[MAX_THREADS];
	static unsigned char m_theadCount;
	static std::vector<QueueEntry> m_queue;

private:
	static void setGameWindow(HWND gameWindow)
	{
		m_gameWindow = gameWindow;
	}

public:
	/// <summary>
	/// Initialize platform
	/// </summary>
	FORCEINLINE static void initialize()
	{
		// initialize timer
		QueryPerformanceFrequency(&m_frequency);
		m_freqCoeff = double(m_frequency.QuadPart) / 1000.0;

		m_start = getMiliseconds();

		// create thread pool
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		int cpus = sysinfo.dwNumberOfProcessors;

		for(auto i =  0; i < cpus; i ++)
		{
			typedef unsigned int(*Func)(void* parameter);
			DWORD threadId = 0u;
			m_threadPool[i] = CreateThread(
				nullptr, 
				0,
				ThreadWorker,
				nullptr, 
				0, 
				&threadId
			);

			m_theadCount++;
		}
	}

	/// <summary>
	/// Queue job for automatic execution in threaded enviroment.
	/// </summary>
	FORCEINLINE static void queueJob(QueueFunction function)
	{
		// TODO: queueJob
		auto entry = QueueEntry();
		entry.functio = function;
		m_queue.push_back(entry);
	}

	/// <summary>
	/// Creates new thread.
	/// </summary>
	/// <returns>The new thread.</returns>
	FORCEINLINE static Thread createThread(ThreadFunction function, void* parameters)
	{
		DWORD threadId = 0;
		auto handle = CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(function), parameters, 0, &threadId);

		Thread thread = {};
		thread.ThreadId = threadId;
		thread.ThreadHandle = handle;

		return thread;
	}

	/// <summary>
	/// Kills specified thread.
	/// </summary>
	/// <param name="thread"></param>
	FORCEINLINE static void killThread(Thread* thread)
	{
		TerminateThread(thread->ThreadHandle, 0);
	}

	/// <summary>
	/// Get time in miliseconds.
	/// </summary>
	/// <returns>The time in miliseconds.</returns>
	FORCEINLINE static double getMiliseconds()
	{
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);
		return double(current.QuadPart) / m_freqCoeff - m_start;
	}

	/// <summary>
	/// Gets the main game window handle.
	/// </summary>
	/// <returns>The game window handle.</returns>
	FORCEINLINE static HWND getGameWindow()
	{
		return m_gameWindow;
	}

	/// <summary>
	/// Get size of game window.
	/// </summary>
	/// <param name="width">(out)The width.</param>
	/// <param name="height">(out)The height.</param>
	FORCEINLINE static void getGameWindowSize(unsigned int* width, unsigned int* height)
	{
		RECT windowRect;
		GetClientRect(m_gameWindow, &windowRect);

		*width = windowRect.right - windowRect.left;
		*height = windowRect.bottom - windowRect.top;
	}

	/// <summary>
	/// Set the cursor position.
	/// </summary>
	/// <param name="x">The x coord of target cursor position in pixels.</param>
	/// <param name="y">The y coord of target cursor position in pixels.</param>
	FORCEINLINE static void setCursorPosition(uint16_t x, uint16_t y)
	{
		POINT point = {};
		point.x = long(x);
		point.y = long(y);

		ClientToScreen(Platform::getGameWindow(), &point);
		SetCursorPos(point.x, point.y);
	}

	/// <summary>
	/// Check does file exists,
	/// </summary>
	/// <param name="fileName">The file path and name. Using working directory.</param>
	/// <returns>True when file exists.</returns>
	FORCEINLINE static bool fileExists(const char* fileName)
	{
		FILE* file;

		// check if file exists 
		// by just trying to open for read 
		// and checking the error
		auto error = fopen_s(&file, fileName, "r");
		
		// close the file now if exist to avoid some leaks or somethin
		if(error == 0)
			fclose(file);
		
		// return the value
		return error == 0;
	}

	/// <summary>
	/// Open file for operations.
	/// Make sure that the file exists.
	/// </summary>
	/// <param name="fileName">The file path and name. Using working directory.</param>
	/// <param name="fileOpenMode">The open mode.</param>
	/// <returns>File.</returns>
	FORCEINLINE static void openFile(File* file, const char* fileName, OpenMode::Enum fileOpenMode = OpenMode::OpenReadWrite)
	{
		strcpy_s(file->FileName, fileName);

		// select open mode
		const char* openMode = "??";
		switch(fileOpenMode)
		{
		case OpenMode::OpenRead:
			openMode = "r";
			break;
		case OpenMode::OpenWrite:
			openMode = "w";
			break;
		case OpenMode::OpenAppend:
			openMode = "w+";
			break;
		case OpenMode::OpenReadWrite:
			openMode = "r+";
			break;
		default: break;
		}

		auto error = fopen_s(&file->m_file, fileName, openMode);

		if(error != 0)
		{
			// file not found
			_ASSERT(false);
		}

		// measure file size
		fseek(file->m_file, 0, SEEK_END);
		file->FileSize = ftell(file->m_file);
		fseek(file->m_file, 0, SEEK_SET);
	}

	/// <summary>
	/// Get the current working directory.
	/// </summary>
	/// <param name="buffer">Output buffer.</param>
	FORCEINLINE static void getWorkingDirectory(char* buffer)
	{
		char i_buffer[MAX_PATH];
		GetModuleFileNameA(nullptr, i_buffer, MAX_PATH);

		// remove 'ReCrafted.exe', and copy
		auto length = strlen(i_buffer);
		for(auto i = length - 13u; i < length; i ++)
		{
			i_buffer[i] = '\0';
		}

		// copy
		strcpy_s(buffer, length, i_buffer);
	}
}; 

#endif // PLATFORMWINDOWS_H
#endif