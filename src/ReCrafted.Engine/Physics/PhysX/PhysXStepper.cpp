// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysXStepper.h"

SINGLETON_IMPL(PhysXStepper)

void StepperTask::run()
{
    m_stepper->substepDone(this);
    release();
}

void StepperTaskSimulate::run()
{
    m_stepper->simulate(mCont);
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

void PhysXStepper::OnDispose()
{
    delete m_sync;
}

void PhysXStepper::substep(StepperTask& completionTask)
{
    // setup any tasks that should run in parallel to simulate()
    //mSample->onSubstepSetup(m_subStepSize, &completionTask);

    // step
    m_simulateTask.setContinuation(&completionTask);
    m_simulateTask.removeReference();
}

bool PhysXStepper::advance(PxScene* scene, const PxReal dt, void* scratchBlock, PxU32 scratchBlockSize)
{
    m_scratchBlock = scratchBlock;
    m_scratchBlockSize = scratchBlockSize;

    substepStrategy(dt, m_subStepCount, m_subStepSize);

    if (m_subStepCount == 0)
        return false;

    m_scene = scene;

    m_sync->reset();

    m_currentSubStep = 1;

    m_completion0.setContinuation(*m_scene->getTaskManager(), nullptr);

    // take first substep
    substep(m_completion0);
    m_firstCompletionPending = true;

    return true;
}

void PhysXStepper::substepDone(StepperTask* ownerTask)
{
    //mSample->onSubstepPreFetchResult();

    {
        PxSceneWriteLock writeLock(*m_scene);
        m_scene->fetchResults(true);
    }

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

        substep(s);

        // after the first substep, completions run freely
        s.removeReference();
    }
}

void PhysXStepper::renderDone()
{
    if (m_firstCompletionPending)
    {
        m_completion0.removeReference();
        m_firstCompletionPending = false;
    }
}

void PhysXStepper::simulate(physx::PxBaseTask* ownerTask)
{
    PxSceneWriteLock writeLock(*m_scene);
    m_scene->simulate(m_subStepSize, ownerTask, m_scratchBlock, m_scratchBlockSize);
}

void PhysXStepper::wait(PxScene* scene)
{
    if (m_subStepCount && m_sync)
        m_sync->wait();
}

void PhysXStepper::substepStrategy(const PxReal stepSize, PxU32& substepCount, PxReal& substepSize)
{
    if (mAccumulator > mFixedSubStepSize)
        mAccumulator = 0.0f;

    // Don't step less than the step size, just accumulate
    mAccumulator += stepSize;
    if (mAccumulator < mFixedSubStepSize)
    {
        substepCount = 0;
        return;
    }

    substepSize = mFixedSubStepSize;
    substepCount = PxMin(PxU32(mAccumulator / mFixedSubStepSize), mMaxSubSteps);

    mAccumulator -= PxReal(substepCount) * substepSize;
}