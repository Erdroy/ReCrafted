// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

template <class T>
struct rc_allocator {
public:
    typedef T value_type;

public:
    rc_allocator() = default;
    template <class U> constexpr rc_allocator(const rc_allocator<U>&) noexcept { }

public:
    T * allocate(std::size_t n)
    {
        if (n > std::size_t(-1) / sizeof(T))
            throw std::bad_alloc();

        if (auto p = static_cast<T*>(rc_malloc(n * sizeof(T))))
            return p;

        throw std::bad_alloc();
    }

    void deallocate(T* p, std::size_t) noexcept
    {
        rc_free(p);
    }
};
template <class T, class U>
bool operator==(const rc_allocator<T>&, const rc_allocator<U>&)
{
    return true;
}

template <class T, class U>
bool operator!=(const rc_allocator<T>&, const rc_allocator<U>&)
{
    return false;
}

#endif // ALLOCATOR_H
