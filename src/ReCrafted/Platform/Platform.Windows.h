// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifdef WIN32 // windows
#ifndef PLATFORMWINDOWS_H
#define PLATFORMWINDOWS_H

// includes
#include "../Utils/Defines.h"
#include <cstdio>
#include <string.h>
#include <crtdbg.h>

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
			Close();
	}

	/// <summary>
	/// Read file to the buffer.
	/// </summary>
	/// <param name="buffer">The buffer pointer.</param>
	/// <param name="length">How much bytes read?</param>
	/// <param name="offset">The offset in bytes where should start.</param>
	void Read(void* buffer, size_t length, size_t offset = 0) const
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
	void Read(void* buffer) const
	{
		Read(buffer, FileSize);
	}

	/// <summary>
	/// Close the file. This will be called automatically when File strcture is out of scope.
	/// </summary>
	FORCEINLINE void Close()
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
/// Platform class.
/// </summary>
class Platform
{
public:

	/// <summary>
	/// Check does file exists,
	/// </summary>
	/// <param name="fileName">The file path and name. Using working directory.</param>
	/// <returns>True when file exists.</returns>
	FORCEINLINE static bool FileExists(const char* fileName)
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
	FORCEINLINE static void OpenFile(File* file, const char* fileName, OpenMode::Enum fileOpenMode = OpenMode::OpenReadWrite)
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
};

#endif // PLATFORMWINDOWS_H
#endif