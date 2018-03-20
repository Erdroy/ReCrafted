// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SPACEOBJECTMANAGER_H
#define SPACEOBJECTMANAGER_H

// includes
#include "ReCrafted.h"
#include "Core/Delegate.h"
#include "Core/Containers/Array.h"

#include <thread>

class SpaceObjectOctreeNode;
class SpaceObjectChunk;

struct ProcessMode
{
	enum _enum
	{
		Populate,
		Depopulate,
        Rebuild
	};
};

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
	* This will invoke all node generation callbacks.
	*/
	void update();

public:
	/**
	 * \brief Enqueues node for generation.
	 */
	static void enqueue(SpaceObjectOctreeNode* node, ProcessMode::_enum mode, Delegate<void> callback);
};

#endif // SPACEOBJECTMANAGER_H
