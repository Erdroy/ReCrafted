// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Rendering.h"

Rendering* Rendering::m_instance;


void Rendering::init()
{
	// load all shaders

}

void Rendering::beginRender()
{
}

void Rendering::endRender()
{
	
}

void Rendering::renderShadows()
{
}

void Rendering::renderStatic()
{
}

void Rendering::renderEntities()
{
}

void Rendering::dispose()
{


	// suicide
	delete this;
}
