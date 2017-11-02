// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef ARRAY_H
#define ARRAY_H

#include "Core/Defines.h"
#include <vector>
#include <algorithm>

template<class T>
class Array
{
private:
    std::vector<T> m_vector;
    uint32_t m_count = 0u;

public:
    Array()
    {
        m_vector = {};
    }

    explicit Array(uint32_t size)
    {
        m_vector = {};
        m_vector.reserve(size);
    }

    explicit Array(T* data, uint32_t size)
    {
        m_vector = {};
        m_vector.reserve(size);

        m_count = size;

        // copy data
        memcpy(m_vector.data(), data, size);
    }

public:
    FORCEINLINE void add(T item)
    {
        m_vector.push_back(item);

        m_count++;
    }

    FORCEINLINE bool remove(T item)
    {
		_ASSERT(m_count > 0u);

        auto index = std::find(m_vector.begin(), m_vector.end(), item);
        
        if (index != m_vector.end()) {
            m_vector.erase(index);
            m_count--;
            return true;
        }

        return false;
    }

    FORCEINLINE bool remove(void* itemPointer)
    {
		_ASSERT(m_count > 0u);

        for(auto i = 0u; i < m_count; i ++)
        {
            if(m_vector[i].get() == itemPointer)
            {
                m_vector.erase(m_vector.begin() + i);
                return true;
            }
        }

        return false;
    }

    FORCEINLINE void removeAt(int index)
    {
		_ASSERT(index >= 0 && index < static_cast<int>(m_count));

        m_vector.erase(m_vector.begin() + index);
        m_count--;
    }

    FORCEINLINE bool contains(T item)
    {
        if (std::find(m_vector.begin(), m_vector.end(), item) != m_vector.end()) {
            return true;
        }

        return false;
    }

    FORCEINLINE void sort()
    {
        std::sort(m_vector.begin(), m_vector.end());
    }

    template<class X>
    FORCEINLINE void sort()
    {
        std::sort(m_vector.begin(), m_vector.end(), X());
    }

    FORCEINLINE uint32_t count() const
    {
        return m_count;
    }

    FORCEINLINE uint32_t size() const
    {
        return static_cast<uint32_t>(m_vector.size());
    }

    FORCEINLINE typename std::vector<T>::iterator begin()
    {
        return m_vector.begin();
    }

    FORCEINLINE typename std::vector<T>::const_iterator begin() const
    {
        return m_vector.begin();
    }

    FORCEINLINE typename std::vector<T>::iterator end()
    {
        return m_vector.end();
    }

    FORCEINLINE typename std::vector<T>::const_iterator end()const
    {
        return m_vector.end();
    }

    FORCEINLINE T& last()
    {
        _ASSERT(m_count > 0u);
        return m_vector[m_count - 1];
    }

    FORCEINLINE const T& last() const
    {
        _ASSERT(m_count > 0u);
        return m_vector[m_count - 1];
    }

    FORCEINLINE T& first()
    {
        _ASSERT(m_count > 0u);
        return m_vector[0];
    }

    FORCEINLINE const T& first() const
    {
        _ASSERT(m_count > 0u);
        return m_vector[0];
    }

    FORCEINLINE T& at(int index)
    {
        _ASSERT(index >= 0 && index < static_cast<int>(m_count));

        return m_vector[index];
    }

    FORCEINLINE T* data()
    {
        return m_vector.data();
    }

    FORCEINLINE void clear()
    {
        m_count = 0;
        m_vector.clear();
    }

    FORCEINLINE void release()
    {
        m_count = 0;
        m_vector.clear();
        m_vector.shrink_to_fit();
    }

    FORCEINLINE T& operator[](int index)
    {
        _ASSERT(index >= 0 && index < static_cast<int>(m_count));
        return m_vector[index];
    }

    FORCEINLINE const T& operator[](int index) const
    {
        _ASSERT(index >= 0 && index < static_cast<int>(m_count));
        return m_vector[index];
    }
};

#endif // ARRAY_H
