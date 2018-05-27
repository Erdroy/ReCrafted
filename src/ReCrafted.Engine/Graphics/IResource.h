// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef IRESOURCE_H
#define IRESOURCE_H

// includes
#include "ReCrafted.h"
#include "Core/Containers/Array.h"
#include "Core/Lock.h"
#include "Core/Logger.h"

/**
 * \brief IResource class. Base class for all graphics resources.
 * Implements instance counting. MUST override 'Dispose' function.
 * Also must implement the IResource logic, using (public:) IRESOURCE_IMPL(ClassName) 
 * or please manually call addRef and release functions!
 */
struct IResource : public IDisposable
{
private:
    static uint m_resCount;

protected:
    /**
     * \brief Adds instance.
     */
    void AddRef()
    {
        m_resCount++;
    }

    /**
     * \brief Removes instance.
     */
    void Release()
    {
        m_resCount--;
    }

public:
    /**
     * \brief Checks for any left resource. Should be called after releasing (supposedly) all resources.
     */
    static void CheckLeaks()
    {
        if (m_resCount > 0u)
            Logger::LogWarning("Resource leaks detected! There is {0} unreleased resource objects!", m_resCount);
    }
};

#define IRESOURCE_IMPL(name)name(){AddRef();}public:~name(){Release();}

#endif // IRESOURCE_H
