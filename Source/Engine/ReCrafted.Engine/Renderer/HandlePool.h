// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include <concurrentqueue.h>

/**
 * \brief HandlePool<> class. Provides static renderer handle pooling system.
 * Can be accessed by name, eg.: HandlePool<Texture2DHandle, Texture2DDescription>::AllocateHandle().
 * 
 * \tparam THandle The handle type. Must inherit from Renderer::ObjectHandle or at least implement `uint32_t idx`
 * as public structure member.
 * 
 * \tparam THandleDesc The handle description. This can be any proper type.
 * If not needed, use Renderer::EmptyDescription.
 * 
 * \note Best practice is to use typdef, eg.:
 * `typedef HandlePool<WindowHandle, WindowDescription> WindowHandlePool`
 * and then, initialize it once using: `WindowHandlePool::Instance()->Initialize(...)`
 */
template<typename THandle, typename THandleDesc>
class HandlePool
{
    const size_t dynamicHandleAlloc = 16;

private:
    std::vector<THandle> m_handles;
    std::vector<THandleDesc> m_handleDescriptions;
    moodycamel::ConcurrentQueue<THandle> m_freeHandles;

    bool m_allowDynamicAllocation = false;
    uint32_t m_lastHandle = 0u;

private:
    void AllocateHandles(const size_t handleCount)
    {
        for (auto i = 0u; i < handleCount; i++)
        {
            THandle handle;
            handle.idx = ++m_lastHandle; // Handle index (idx) CANNOT BE 0!

            m_handles.emplace_back(handle);
            m_handleDescriptions.emplace_back(THandleDesc());
            m_freeHandles.enqueue(m_handles.back());
        }
    }

public: 
    /**
     * \brief Default constructor.
     */
    HandlePool() = default;

public:
    /**
     * \brief Initializes this handle pool.
     * \param initialHandles The initial amount of handles that can be acquired.
     * \param allowDynamicAllocation Sets whatever the pool should be able to allocate more handles
     * when running out of it.
     * 
     * \note When dynamic allocation (allowDynamicAllocation) is false,
     * then `initialHandles` is the maximal amount of handles that can be acquired.
     */
    void Initialize(const size_t initialHandles, const bool allowDynamicAllocation = false)
    {
        m_allowDynamicAllocation = allowDynamicAllocation;
        AllocateHandles(initialHandles);
    }

    /**
     * \brief Gets next handle index that will be used in the next handle allocation.
     * \return The handle index.
     * 
     * \note Returned value is always non-zero! Zero idx handle is INVALID_HANDLE!
     */
    uint32_t GetNextHandleIndex() const
    {
        return m_lastHandle + 1;
    }

    /**
     * \brief Releases all handles allocated by this pool.
     * Cannot use it anymore. Not really required to call.
     */
    void Release()
    {
        m_handles.clear();
        m_handleDescriptions.clear();
    }

public:
    /**
     * \brief Allocates free handle of current type (THandle).
     * \return The allocated handle.
     * 
     * \note When dynamic allocation is enabled, this will always return valid handle.
     */
    FORCEINLINE static THandle AllocateHandle()
    {
        THandle handle;
        if (!Instance()->m_freeHandles.try_dequeue(handle))
        {
            _ASSERT_(Instance()->m_allowDynamicAllocation,
                "HandleAllocator is out of handles and dynamic allocation is disabled!");

            Instance()->AllocateHandles(Instance()->dynamicHandleAlloc);

            ASSERT(Instance()->m_freeHandles.try_dequeue(handle));
        }

        return handle;
    }

    /**
     * \brief Gets given handle description.
     * \param handle The handle.
     * \return The handle description.
     * 
     * \note This is a reference, and can be modified.
     */
    FORCEINLINE static THandleDesc& GetHandleDescription(const THandle& handle)
    {
        return Instance()->m_handleDescriptions.at(handle.idx);
    }

    /**
     * \brief Returns given handle to the pool.
     * \param handle The handle that will be freed and returned to the pool.
     * 
     * \note This also clears handle description.
     */
    FORCEINLINE static void FreeHandle(const THandle& handle)
    {
        Instance()->m_freeHandles.enqueue(handle);

        // Clear description
        GetHandleDescription(handle) = THandleDesc();
    }

    /**
     * \brief Gets instance of given handle pool.
     * \return The handle pool pointer.
     */
    static HandlePool<THandle, THandleDesc>* Instance()
    {
        static HandlePool<THandle, THandleDesc> instance;
        return &instance;
    }
};
