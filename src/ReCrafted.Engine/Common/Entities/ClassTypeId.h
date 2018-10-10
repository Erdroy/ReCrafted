// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef CLASSTYPEID_H
#define CLASSTYPEID_H

#include <atomic>

typedef unsigned short TypeId;

/**
* \brief Allows static type counting.
* \tparam TBase The base class.
*/
template<typename TBase>
struct ClassTypeId
{
private:
    static std::atomic<TypeId> m_lastTypeId;

public:
    /**
    * \brief Gets the type id of given class type.
    * \tparam TClass The class type.
    * \return The class type id.
    */
    template <typename TClass>
    static TypeId GetTypeId()
    {
        static const auto typeId = m_lastTypeId++;
        return typeId;
    }
};

template <typename TBase>
std::atomic<TypeId> ClassTypeId<TBase>::m_lastTypeId = { 0 };

#endif // CLASSTYPEID_H
