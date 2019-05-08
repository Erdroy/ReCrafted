// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

/**
 * \brief Singleton class.
 * \tparam TType The singleton's handled object type
 *
 * \note To release a Singleton, please call 'Dispose' function 
 * \remarks Dispose calls the object's destructor and deletes it from the memory.
 */
template <class TType>
class Singleton
{
public:
    Singleton() = default;
    virtual ~Singleton() = default;

private:
    static TType*& GetInstanceRef()
    {
#if DEBUG
        static auto* instance = reinterpret_cast<TType*>(0x1);

        if (instance == reinterpret_cast<TType*>(0x1))
            instance = new TType();

        if (instance == nullptr)
            throw std::exception("This singleton call has been released.");
#else
        static auto* instance;

        if (instance == nullptr)
            instance = new TType();
#endif

        return instance;
    }

public:
    /**
     * \brief Disposes the singleton object.
     * Next call to GetInstance will throw an exception.
     */
    void Dispose() const
    {
        auto& instance = GetInstanceRef();
        delete instance;
        instance = nullptr;
    }

public:
    /**
     * \brief Gets (or creates) the singleton object.
     * \return The singleton object of type `TType`.
     */
    static TType* GetInstance()
    {
        return GetInstanceRef();
    }
};
