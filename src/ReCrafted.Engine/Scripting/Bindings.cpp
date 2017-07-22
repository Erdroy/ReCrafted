// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Bindings.h"
#include "Object.h"

#include "Common/Input.h"
#include "Common/Display.h"
#include "Core/GameMain.h"
#include "Core/Logger.h"
#include "Graphics/UI/UI.h"
#include "Graphics/Camera.h"
#include <Graphics/Font.h>

#define INSTANCES_COUNT 65535

objectinstancer* m_instancers = nullptr;

void Bindings::bind()
{
	m_instancers = new objectinstancer[INSTANCES_COUNT];

	for (auto i = 0; i < INSTANCES_COUNT; i++)
		m_instancers[i] = nullptr;

	Object::initRuntime();

	Logger::initRuntime();
	Input::initRuntime();
	GameMain::initRuntime();
	Time::initRuntime();
	Display::initRuntime();
	UI::initRuntime();
	Camera::initRuntime();
	Texture2D::initRuntime();
	Font::initRuntime();
}

void Bindings::bindObject(int type, objectinstancer method)
{
	if(m_instancers[type] != nullptr)
	{
		Logger::write("Binding Object type already exists.", LogLevel::Warning);
		return;
	}

	m_instancers[type] = method;
}

Ptr<Object> Bindings::instantiate(int type)
{
	auto instancer = m_instancers[type];
	return instancer();
}

void Bindings::shutdown()
{
	delete[] m_instancers;
}
