// ReCrafted (c) 2016-2018 Always Too Late

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
    virtual void initialize() = 0;
    virtual void run() = 0;
    virtual void shutdown() = 0;
};

#endif // APPLICATIONBASE_H
