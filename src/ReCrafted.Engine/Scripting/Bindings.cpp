// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Bindings.h"
#include "Object.h"

#include "Common/Input.h"
#include "Common/Display.h"
#include "Core/GameMain.h"
#include "Core/Logger.h"
#include "Graphics/UI/UI.h"
#include "Graphics/Camera.h"
#include "Graphics/Font.h"

#include <map>

std::map<std::string, objectinstancer> m_typeMap = {};

void Bindings::bind()
{
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

void Bindings::bindObject(char* typePtr, objectinstancer method)
{
	/*if(m_typeMap[typePtr] != nullptr)
	{
		Logger::write("Binding Object type already exists.", LogLevel::Warning);
		return;
	}*/

	m_typeMap[std::string(typePtr)] = method;
}

Ptr<Object> Bindings::instantiate(char* typePtr)
{
	auto instancer = m_typeMap[std::string(typePtr)];
	return instancer();
}

void Bindings::shutdown()
{

}
