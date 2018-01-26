// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PLATFORM_H
#define PLATFORM_H

// includes
#include "Core/Guid.h"

#define PATHLENGTH 512

struct OpenMode
{
	enum _enum
	{
		OpenRead,
		OpenWrite,
		OpenAppend,

		OpenReadWrite
	};
};

struct File
{
	friend class Platform;

private:
	void* m_file = nullptr;

public:
	char FileName[PATHLENGTH];
	size_t FileSize = 0u;

	~File();

	void seek(long position) const;

	void read(void* buffer, size_t length, size_t offset = 0) const;

	void read(void* buffer) const;

	void write(void* data, const size_t dataLenght) const;

	void flush() const;

	void close() const;
};

class Platform
{
private:
	static void* m_gameWindow;

	// timer
	static unsigned char m_theadCount;
	static int m_cpuCount;

public:
	static void initialize();

	/**
	 * \brief Creates new Guid.
	 * \return The new unique guid.
	 */
	static Guid newGuid();

	/**
	* \brief Get time in miliseconds.
	* \return The time in miliseconds.
	*/
	static double getMiliseconds();

	/**
	 * \brief Gets the CPU count.
	 * \return The CPU count. 
	 */
	static int cpuCount();

	/**
	* \brief Gets the main game window handle.
	* \return The game window handle.
	*/
	static void* getGameWindow();

	/**
	 * \brief Sets  the main game window handle.
	 * \param gameWindow The game window handle. 
	 */
	static void setGameWindow(void* gameWindow);

	/**
	 * \brief Get size of game window.
	 * \param width (out)The width.
	 * \param height (out)The height.
	 */
	static void getGameWindowSize(unsigned int* width, unsigned int* height);

	/**
	 * \brief Set the cursor position.
	 * \param x The x coord of target cursor position in pixels.
	 * \param y The y coord of target cursor position in pixels.
	 */
	static void setCursorPosition(uint16_t x, uint16_t y);

	/**
	 * \brief Checks if file exists.
	 * \param fileName The file path and name. Using working directory.
	 * \return True when file exists.
	 */
	static bool fileExists(const char* fileName);

	/**
	 * \brief Open file for operations.
	 * Make sure that the file exists.
	 * \param file The file path and name. Using working directory.
	 * \param fileName The open mode.
	 * \param fileOpenMode The file.
	 */
	static void openFile(File* file, const char* fileName, OpenMode::_enum fileOpenMode = OpenMode::OpenReadWrite);

	/**
	 * \brief Get the current working directory.
	 * \param buffer Output buffer.
	 */
	static void getWorkingDirectory(char* buffer);
};

#endif // PLATFORM_H
