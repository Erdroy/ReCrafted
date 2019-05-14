// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

template <typename TType>
struct Array final
{
public:
    class iterator
    {
    public:
        typedef iterator self_type;
        typedef TType value_type;
        typedef TType& reference;
        typedef TType* pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        iterator(pointer ptr) : ptr_(ptr) { }
        self_type operator++() { self_type i = *this; ptr_++; return i; }
        self_type operator++(int junk) { ptr_++; return *this; }
        reference operator*() { return *ptr_; }
        pointer operator->() { return ptr_; }
        bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
        bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
    private:
        pointer ptr_;
    };

    class const_iterator
    {
    public:
        typedef const_iterator self_type;
        typedef TType value_type;
        typedef TType& reference;
        typedef TType* pointer;
        typedef int difference_type;
        typedef std::forward_iterator_tag iterator_category;
        const_iterator(pointer ptr) : ptr_(ptr) { }
        self_type operator++() { self_type i = *this; ptr_++; return i; }
        self_type operator++(int junk) { ptr_++; return *this; }
        const reference operator*() { return *ptr_; }
        const pointer operator->() { return ptr_; }
        bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
        bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
    private:
        pointer ptr_;
    };

private:
    TType* m_data;
    size_t m_count;

public:
    Array() = delete;

    Array(TType* data, const size_t count) noexcept : m_data(data), m_count(count) { }
    Array(Array<TType>& array) noexcept : m_data(array.m_data), m_count(array.m_count) {}
    Array(Array<TType>&& array) noexcept : m_data(array.m_data), m_count(array.m_count) {}

public:
    FORCE_INLINE iterator begin()
    {
        return iterator(m_data);
    }

    FORCE_INLINE const_iterator begin() const
    {
        return const_iterator(m_data);
    }

    FORCE_INLINE iterator end()
    {
        return iterator(m_data + m_count);
    }

    FORCE_INLINE const_iterator end() const
    {
        return const_iterator(m_data + m_count);
    }

    size_t size() const { return m_count; }

    TType& operator[](size_t index)
    {
        assert(index < m_count);
        return m_data[index];
    }

    const TType& operator[](size_t index) const
    {
        assert(index < m_count);
        return m_data[index];
    }

public:
    template <class X>
    FORCE_INLINE void Sort()
    {
        std::sort(begin(), end(), X());
    }

    FORCE_INLINE void Reverse()
    {
        std::reverse(begin(), end());
    }

    FORCE_INLINE TType& Last()
    {
        return end();
    }

    FORCE_INLINE const TType& Last() const
    {
        return end();
    }

    FORCE_INLINE TType& First()
    {
        return begin();
    }

    FORCE_INLINE const TType& First() const
    {
        return begin();
    }

    FORCE_INLINE TType& At(int index)
    {
        return m_data[index];
    }

    FORCE_INLINE TType* Data() const
    {
        return m_data;
    }

    FORCE_INLINE bool Empty() const
    {
        return m_count == 0u;
    }

    FORCE_INLINE size_t Count() const
    {
        return m_count;
    }
};

template <typename TType>
using ReadOnlyList = Array<TType>;
