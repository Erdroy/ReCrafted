// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Singleton.h"
#include "Physics/PhysX.h"

class MultiThreadStepper;

class StepperTask : public PxLightCpuTask
{
protected:
    MultiThreadStepper* m_stepper = nullptr;

public:
    void run() override;

public:
    void setStepper(MultiThreadStepper* stepper)
    {
        m_stepper = stepper;
    }

    MultiThreadStepper* getStepper()
    {
        return m_stepper;
    }

    const MultiThreadStepper* getStepper() const
    {
        return m_stepper;
    }

    const char* getName() const override
    {
        return "MT Stepper Task";
    }
};

class StepperTaskSimulate final : public StepperTask
{
public:
    StepperTaskSimulate() = default;

public:
    void run() override;
};

class MultiThreadStepper final : public Singleton<MultiThreadStepper>
{
public:
    /**
     * \brief The scratch memory block size that is used for scene simulation.
     * Must be 16-byte aligned.
     */
    static const PxU32 SCRATCH_BLOCK_SIZE = 1024 * 128;

private:
    bool m_firstCompletionPending;
    StepperTaskSimulate	m_simulateTask;
    StepperTask m_completion0;
    StepperTask m_completion1;

    PxScene* m_scene = nullptr;
    shdfnd::Sync* m_sync = nullptr;

    void* m_scratchBlock = nullptr;
    PxU32 m_scratchBlockSize = 0;

    PxU32 m_currentSubStep = 0;
    PxU32 m_subStepCount = 0;
    PxReal m_subStepSize = 0.0f;
    PxU32 m_maxSubSteps = 1;
    PxReal m_fixedSubStepSize = 1.0f / 60.0f;
    PxReal m_accumulator = 0.0f;

public:
    MultiThreadStepper();
    ~MultiThreadStepper();

public:
    void Initialize(float subStepDelta, int maxSubSteps = 1);

    void SubstepStrategy(PxReal stepSize, PxU32& substepCount, PxReal& substepSize);
    void Substep(StepperTask& completionTask);
    bool Advance(PxScene* scene, PxReal dt, void* scratchBlock, PxU32 scratchBlockSize);
    void SubstepDone(StepperTask* ownerTask);
    void RenderDone();
    void Simulate(PxBaseTask* ownerTask);
    void Reset();
    void Wait(PxScene* scene) const;

public:
    PxReal getSubStepSize() const;
};
