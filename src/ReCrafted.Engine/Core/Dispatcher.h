// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef DISPATCHER_H
#define DISPATCHER_H

// includes
#include "ReCrafted.h"
#include "EngineComponent.h"
#include "Action.h"

#include <concurrentqueue.h>

class Dispatcher : public EngineComponent<Dispatcher>
{
private:
    struct DispatchEntry
    {
        Action<void, void*> callback;
        void* userdata;
    };

private:
    moodycamel::ConcurrentQueue<DispatchEntry> m_dispatchQueue;

private:
    void Dispatch();

protected:
    void OnInit() override;
    void Update() override;
    void OnDispose() override;

public:
    static void Dispatch(Action<void, void*> callback, void* userdata = nullptr);
};

#endif // DISPATCHER_H
