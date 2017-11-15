// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef SPACEOBJECTMANAGER_H
#define SPACEOBJECTMANAGER_H

// includes
#include "Core/Singleton.h"
#include "Core/Containers/Array.h"
#include <thread>

class SpaceObjectChunk;

class SpaceObjectManager : public Singleton<SpaceObjectManager>
{
private:
	Array<std::thread*> m_workerThreads = {};
	volatile bool m_running = false;

public:
	virtual ~SpaceObjectManager() { }

private:
	void onDispose() override;

	void worker_function();

public:
	/**
	* \brief Initializes the SpaceObjectManager.
	*/
	void init();

	/**
	* \brief Updates the SpaceObjectManager.
	* This will invoke all chunk generation callbacks.
	*/
	void update();

public:
	/**
	 * \brief Enqueues chunk for generation.
	 * \param chunk The chunk that will be generated.
	 */
	void enqueue(SpaceObjectChunk* chunk);
};

#endif // SPACEOBJECTMANAGER_H
