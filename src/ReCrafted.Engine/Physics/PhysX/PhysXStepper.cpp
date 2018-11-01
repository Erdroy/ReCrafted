// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysXStepper.h"

SINGLETON_IMPL(PhysXStepper)

void StepperTask::run()
{
    m_stepper->SubstepDone(this);
    release();
}

void StepperTaskSimulate::run()
{
    m_stepper->Simulate(mCont);
    //m_stepper->getSample().onSubstepStart(m_stepper->getSubStepSize());
}

PhysXStepper::PhysXStepper() :
      m_firstCompletionPending(false)
    , m_currentSubStep(0)
    , m_subStepCount(0)
{
    m_completion0.setStepper(this);
    m_completion1.setStepper(this);
    m_simulateTask.setStepper(this);

    m_sync = new shdfnd::Sync();
}

PxReal PhysXStepper::getSubStepSize() const
{
    return m_subStepSize;
}

void PhysXStepper::Initialize(const float subStepDelta, const int maxSubSteps)
{
    m_fixedSubStepSize = subStepDelta;
    m_maxSubSteps = maxSubSteps;
}

void PhysXStepper::OnDispose()
{
    delete m_sync;
}

bool PhysXStepper::Advance(PxScene* scene, const PxReal dt, void* scratchBlock, PxU32 scratchBlockSize)
{
    m_scratchBlock = scratchBlock;
    m_scratchBlockSize = scratchBlockSize;

    SubstepStrategy(dt, m_subStepCount, m_subStepSize);

    if (m_subStepCount == 0)
        return false;

    m_scene = scene;

    m_sync->reset();

    m_currentSubStep = 1;

    m_completion0.setContinuation(*m_scene->getTaskManager(), nullptr);

    // take first substep
    Substep(m_completion0);
    m_firstCompletionPending = true;

    return true;
}

void PhysXStepper::RenderDone()
{
    if (m_firstCompletionPending)
    {
        m_completion0.removeReference();
        m_firstCompletionPending = false;
    }
}

void PhysXStepper::Simulate(physx::PxBaseTask* ownerTask)
{
    PxSceneWriteLock writeLock(*m_scene);
    m_scene->simulate(m_subStepSize, ownerTask, m_scratchBlock, m_scratchBlockSize);
}

void PhysXStepper::Substep(StepperTask& completionTask)
{
    // setup any tasks that should run in parallel to simulate()
    //mSample->onSubstepSetup(m_subStepSize, &completionTask);

    // step
    m_simulateTask.setContinuation(&completionTask);
    m_simulateTask.removeReference();
}

void PhysXStepper::SubstepDone(StepperTask* ownerTask)
{
    //mSample->onSubstepPreFetchResult();

    PxSceneWriteLock writeLock(*m_scene);
    m_scene->fetchResults(true);

    //mSample->onSubstep(m_subStepSize);

    if (m_currentSubStep >= m_subStepCount)
    {
        m_sync->set();
    }
    else
    {
        StepperTask &s = ownerTask == &m_completion0 ? m_completion1 : m_completion0;
        s.setContinuation(*m_scene->getTaskManager(), nullptr);
        m_currentSubStep++;

        Substep(s);

        // after the first substep, completions run freely
        s.removeReference();
    }
}

void PhysXStepper::SubstepStrategy(const PxReal stepSize, PxU32& substepCount, PxReal& substepSize)
{
    if (m_accumulator > m_fixedSubStepSize)
        m_accumulator = 0.0f;

    // don't step less than the step size, just accumulate
    m_accumulator += stepSize;
    if (m_accumulator < m_fixedSubStepSize)
    {
        substepCount = 0;
        return;
    }

    substepSize = m_fixedSubStepSize;
    substepCount = PxMin(PxU32(m_accumulator / m_fixedSubStepSize), m_maxSubSteps);

    m_accumulator -= PxReal(substepCount)*substepSize;
}

void PhysXStepper::Reset()
{
    m_accumulator = 0.0f;
}

void PhysXStepper::Wait(PxScene* scene) const
{
    if (m_subStepCount && m_sync)
        m_sync->wait();
}
