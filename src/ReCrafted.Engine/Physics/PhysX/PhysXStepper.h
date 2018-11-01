// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSXSTEPPER_H
#define PHYSXSTEPPER_H

#include "PhysX.h"
#include "Core/Singleton.h"

#include <foundation/PsSync.h>

class PhysXStepper;

class StepperTask : public PxLightCpuTask
{
protected:
    PhysXStepper* m_stepper = nullptr;

public:
    void run() override;

public:
    void setStepper(PhysXStepper* stepper)
    {
        m_stepper = stepper;
    }

    PhysXStepper* getStepper()
    {
        return m_stepper;
    }

    const PhysXStepper*	getStepper() const
    {
        return m_stepper;
    }

    const char* getName() const override
    {
        return "Stepper Task";
    }
};

class StepperTaskSimulate : public StepperTask
{

public:
    StepperTaskSimulate() = default;

public:
    void run() override;
};

class PhysXStepper : public Singleton<PhysXStepper>
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

    PxU32 m_currentSubStep;
    PxU32 m_subStepCount;
    PxReal m_subStepSize{};
    void* m_scratchBlock = nullptr;
    PxU32 m_scratchBlockSize{};

    PxReal mAccumulator{};
    PxReal mFixedSubStepSize = 1.0f / 60.0f;
    PxU32 mMaxSubSteps = 1;

public:
    PhysXStepper();
    ~PhysXStepper() = default;

public:
    void OnDispose() override;

    void substepStrategy(PxReal stepSize, PxU32& substepCount, PxReal& substepSize);
    void substep(StepperTask& completionTask);
    bool advance(PxScene* scene, PxReal dt, void* scratchBlock, PxU32 scratchBlockSize);
    void substepDone(StepperTask* ownerTask);
    void renderDone();
    void simulate(PxBaseTask* ownerTask);
    void wait(PxScene* scene);

public:
    PxReal getSubStepSize() const;
};

#endif // PHYSXSTEPPER_H
