// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

/**
 * \brief Singleton class.
 * \tparam T The singleton's handled object type
 *
 * \note PLEASE remember to add SINGLETON_IMPL(NAME_OF_CLASS) to the source file!
 * To release a Singleton, please call 'Dispose' function.
 */
template <class T>
class Singleton
{
public:
    virtual ~Singleton() = default;

public:
    static T* GetInstance()
    {
        static T instance;
        return &instance;
    }
};
