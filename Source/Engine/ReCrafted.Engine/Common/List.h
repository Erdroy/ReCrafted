// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

template <typename TType>
class List final
{
private:
    std::vector<TType> m_vector;

public:
    List() = default;
    ~List() = default;

    explicit List(uint32_t size)
    {
        // Reserve space
        m_vector.reserve(size);
    }

    explicit List(TType* data, uint32_t size)
    {
        // Reserve and copy data
        m_vector.reserve(size);
        memcpy(m_vector.data(), data, size);
    }

public:
    /// <summary>
    ///     Returns read-only array of this list.
    /// </summary>
    /// <returns>The array.</returns>
    FORCE_INLINE Array<TType> ToArray()
    {
        return { m_vector.data(), m_vector.size() };
    }

public:
    /// <summary>
    ///     Adds given item to the list.
    /// </summary>
    /// <param name="item">The item.</param>
    FORCE_INLINE void Add(TType item)
    {
        m_vector.push_back(item);
    }

    /// <summary>
    ///     Removes given item from the list.
    /// </summary>
    /// <param name="item">The item.</param>
    /// <returns>True when item was removed successfully.</returns>
    FORCE_INLINE bool Remove(TType item)
    {
        const auto index = std::find(m_vector.begin(), m_vector.end(), item);

        if (index != m_vector.end())
        {
            m_vector.erase(index);
            return true;
        }

        return false;
    }

    FORCE_INLINE void Erase(typename std::vector<TType>::const_iterator where)
    {
        m_vector.erase(where);
    }

    FORCE_INLINE void Erase(typename std::vector<TType>::const_iterator where, typename std::vector<TType>::const_iterator last)
    {
        m_vector.erase(where, last);
    }

    FORCE_INLINE void RemoveAt(int index)
    {
        m_vector.erase(m_vector.begin() + index);
    } 
    
    FORCE_INLINE void Reserve(uint32_t size)
    {
        m_vector.reserve(size);
    }

    FORCE_INLINE void Resize(uint32_t size)
    {
        m_vector.resize(size);
    }

    FORCE_INLINE Array<TType> ToArray() const
    {
        return Array<TType>(m_vector.data(), m_vector.size());
    }

    /// <summary>
    ///     Checks if this list contains given item.
    /// </summary>
    /// <param name="item">The item to check if it exists in this list.</param>
    /// <returns>True when item was found.</returns>
    /// <remarks>Using this, assumes that the T has comparison operator overloaded (if class/struct)</remarks>
    FORCE_INLINE bool Contains(const TType& item) const
    {
        return static_cast<bool>(std::find(m_vector.begin(), m_vector.end(), item) != m_vector.end());
    }

    /// <summary>
    ///     Gets an index of given item in this list, returns -1 when item was not found.
    /// </summary>
    /// <param name="item">The item to check it's index in this list.</param>
    /// <returns>Returns index of the item or -1 when it does not exists in this list.</returns>
    /// <remarks>Using this, assumes that the T has comparison operator overloaded (if class/struct)</remarks>
    FORCE_INLINE int IndexOf(TType& item)
    {
        const auto iterator = std::find(m_vector.begin(), m_vector.end(), item);

        if (iterator == m_vector.end())
            return -1;

        return static_cast<int>(std::distance(m_vector.begin(), iterator));
    }

    FORCE_INLINE void Copy(const List<TType>& array)
    {
        for (auto&& elem : array)
            Add(elem);
    }

    FORCE_INLINE void Sort()
    {
        std::sort(m_vector.begin(), m_vector.end());
    }

    template <class X>
    FORCE_INLINE void Sort()
    {
        std::sort(m_vector.begin(), m_vector.end(), X());
    }

    FORCE_INLINE void Reverse()
    {
        std::reverse(m_vector.begin(), m_vector.end());
    }

    FORCE_INLINE bool Empty() const
    {
        return m_vector.empty();
    }

    FORCE_INLINE size_t Count() const
    {
        return m_vector.size();
    }

    FORCE_INLINE size_t Size() const
    {
        return m_vector.size();
    }

    FORCE_INLINE typename std::vector<TType>::iterator begin()
    {
        return m_vector.begin();
    }

    FORCE_INLINE typename std::vector<TType>::const_iterator begin() const
    {
        return m_vector.begin();
    }

    FORCE_INLINE typename std::vector<TType>::iterator end()
    {
        return m_vector.end();
    }

    FORCE_INLINE typename std::vector<TType>::const_iterator end() const
    {
        return m_vector.end();
    }

    FORCE_INLINE TType& Last()
    {
        return m_vector[m_vector.size() - 1];
    }

    FORCE_INLINE const TType& Last() const
    {
        return m_vector.end();
    }

    FORCE_INLINE TType& First()
    {
        return m_vector.begin();
    }

    FORCE_INLINE const TType& First() const
    {
        return m_vector.begin();
    }

    FORCE_INLINE TType& At(int index)
    {
        return m_vector.at(index);
    }

    FORCE_INLINE TType* Data()
    {
        return m_vector.data();
    }

    FORCE_INLINE void Clear()
    {
        m_vector.clear();
    }

    FORCE_INLINE void Release()
    {
        m_vector.clear();
        m_vector.shrink_to_fit();
    }

    FORCE_INLINE TType& operator[](int index)
    {
        return m_vector.at(index);
    }

    FORCE_INLINE const TType& operator[](int index) const
    {
        return m_vector.at(index);
    }
};
