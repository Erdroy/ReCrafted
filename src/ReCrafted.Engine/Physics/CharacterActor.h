// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef CHARACTERACTOR_H
#define CHARACTERACTOR_H

#include "ReCrafted.h"
#include "Common/Actor.h"

class CharacterActor : public Actor<CharacterActor>
{
private:
    SCRIPTING_API_IMPL()

private:
    void OnAwake() override;
    void OnDestroy() override;

public:

};

#endif // CHARACTERACTOR_H
