// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef IRESOURCE_H
#define IRESOURCE_H

// includes
#include "ReCrafted.h"
#include "Core/Containers/Array.h"
#include "Core/Lock.h"

/**
 * \brief IResource class. Base class for all graphics resources.
 * Implements instance counting. MUST override 'dispose' function.
 * Also must implement the IResource logic, using (public:) IRESOURCE_IMPL(ClassName) 
 * or please manually call addRef and release functions!
 */
interface IResource : public IDisposable
{
private:
    static uint m_resCount;

#if COMPILE_WITH_RESOURCE_LISTING
    static Array<IResource*> m_resources;
    static Lock m_resourcesLock;
#endif

protected:
    /**
     * \brief Adds instance.
     */
    void addRef()
    {
        m_resCount++;

#if COMPILE_WITH_RESOURCE_LISTING
        ScopeLock(m_resourcesLock);
        m_resources.add(this);
#endif
    }

    /**
     * \brief Removes instance.
     */
    void release()
    {
        m_resCount--;

#if COMPILE_WITH_RESOURCE_LISTING
        ScopeLock(m_resourcesLock);
        m_resources.remove(this);
#endif
    }

public:
    /**
     * \brief Checks for any left resource. Should be called after releasing (supposedly) all resources.
     */
    static void checkLeaks()
    {
        if(m_resCount > 0u)
            Logger::logWarning("Resource leaks detected! There is {0} unreleased resource objects!", m_resCount);
    }
};

#define IRESOURCE_IMPL(name)name(){addRef();}public:~name(){release();}

#endif // IRESOURCE_H
