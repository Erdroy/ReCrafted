// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PROFILER_H
#define PROFILER_H

#include "ReCrafted.h"
#include "Core/EngineComponent.h"

class Profiler : public EngineComponent<Profiler>
{
private:
    class ProfilerThread
    {
    public:
        std::thread::id threadId;
        const char* threadName;
    };

private:
    std::vector<ProfilerThread*> m_threads;

private:
    void DrawWindow();
    void DrawThread(ProfilerThread* thread);

protected:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;

public:
    static void InitThread(const char* threadName);
    static void FinishThread();

    static void NewFrame();

public:
    static void BeginProfile(const Char* name)
    {
    }

    static void BeginProfile(const char* name)
    {
    }

    static void EndProfile()
    {
    }
};

#endif // PROFILER_H
