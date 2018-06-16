// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Core/Math/Vector3.h"
#include "Core/Containers/Array.h"

class DebugDraw : public EngineComponent<DebugDraw>
{
    friend class Graphics;

private:
    Array<Vector3> m_lines = {};

private:
    void OnInit() override;
    void OnLoad() override;
    void Update() override;
    void Render();
    void OnDispose() override;

public:
    static void DrawBox(const Vector3& position, const Vector3& size);
};

#endif // DEBUGDRAW_H
