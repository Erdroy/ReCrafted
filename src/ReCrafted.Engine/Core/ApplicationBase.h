// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef APPLICATIONBASE_H
#define APPLICATIONBASE_H

/**
* \brief Application base class.
*/
class ApplicationBase
{
protected:
    ~ApplicationBase() = default;

public:
    virtual void Initialize() = 0;
    virtual void Run() = 0;
    virtual void Shutdown() = 0;
};

#endif // APPLICATIONBASE_H
