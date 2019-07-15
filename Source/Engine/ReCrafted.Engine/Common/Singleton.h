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
        static TType* instance;

        if (instance == nullptr)
            instance = new TType();

#ifdef DEBUG
        if (IsReleased())
            throw std::exception("This singleton has been disposed.");
#endif

        return instance;
    }

public:
    /// <summary>
    ///     Disposes the singleton object. Next call to GetInstance will throw an exception.
    /// </summary>
    void Dispose() const
    {
        auto& instance = GetInstanceRef();
        IsReleased(true);
        delete instance;
        instance = nullptr;
    }

public:
    /// <summary>
    ///     Gets or sets release state.
    /// </summary>
    static bool IsReleased(const bool set = false)
    {
        static auto isReleased = false;

        if (set)
            isReleased = true;

        return isReleased;

    }

    /// <summary>
    ///     Gets (or creates) the singleton object.
    /// </summary>
    /// <returns>The singleton object of type `TType`.</returns>
    static TType* GetInstance()
    {
        if (IsReleased())
            return nullptr;

        return GetInstanceRef();
    }
};
