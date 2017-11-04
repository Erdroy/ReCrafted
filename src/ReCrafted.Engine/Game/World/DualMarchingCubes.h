// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef DUALMARCHINGCUBES_H
#define DUALMARCHINGCUBES_H

// includes
#include "Graphics/Shader.h"
#include "Graphics/Mesh.h"

class DMC
{
private:
	Ptr<Mesh> m_mesh = nullptr;
	Ptr<Shader> m_shader = nullptr;

public:
	void generate();

	void draw();
};

#endif // DUALMARCHINGCUBES_H
