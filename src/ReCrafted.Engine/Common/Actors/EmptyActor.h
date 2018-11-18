// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef EMPTYACTOR_H
#define EMPTYACTOR_H

#include "ReCrafted.h"
#include "Common/Actors/Actor.h"

class EmptyActor final : public Actor<EmptyActor>
{
private:
    SCRIPTING_API_IMPL();

};

#endif // EMPTYACTOR_H
