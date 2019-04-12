// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "Core/SubSystems/SubSystem.h"

class Time final : public SubSystem<Time>
{
protected:
    void Initialize() override;
    void Shutdown() override;
};
