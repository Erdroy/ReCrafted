// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "GameCore.h"
#include "../Utils/Defines.h"
#include <bx/timer.h>

GameCore* GameCore::m_instance;

void GameCore::onLoad()
{
	// initialize bgfx
	bgfx::init(bgfx::RendererType::Direct3D11);
	bgfx::reset(m_width, m_height, BGFX_RESET_VSYNC);

	bgfx::setDebug(BGFX_DEBUG_STATS);

	// Set view 0 clear state.
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030FF, 1.0f, 0);
	bgfx::setViewRect(0, 0, 0, m_width, m_height);

	bgfx::setState(0 | BGFX_STATE_DEFAULT);

	// initialize rendering
	m_rendering = new Rendering;
	m_rendering->init();

	// initialize main camera for scene
	m_camera = Camera::createCamera(true, true);
	m_camera->setPosition(vector3f(0.0f, 0.0f, -10.0f));

	m_initialized = true;

	m_timeOffset = bx::getHPCounter();
}

void GameCore::onUnload()
{
	if (!m_initialized)
		return;

	// release all resources etc.
	SafeDispose(m_rendering);

	bgfx::shutdown();
}

void GameCore::onResize(uint width, uint height)
{
	// check if game is already initialized
	if (!m_initialized)
		return; // do not allow to enter this method when game is not initialized yet.

	m_width = width;
	m_height = height;

	// reset bgfx state, this should force renderer to resize all the viewports etc.
	bgfx::setViewRect(0, 0, 0, m_width, m_height);
	bgfx::reset(m_width, m_height, BGFX_RESET_VSYNC);

	m_rendering->resize(width, height);
}

void GameCore::onUpdate()
{
	// update event, called every frame
	Time::m_instance->m_time = static_cast<float>((bx::getHPCounter() - m_timeOffset) / double(bx::getHPFrequency()));

	auto now = bx::getHPCounter();
	auto frameTime = now - m_lastTimeOffset;
	m_lastTimeOffset = now;

	auto freq = double(bx::getHPFrequency());
	auto toMs = 1000.0 / freq;

	Time::m_instance->m_deltaTime = double(frameTime)*toMs;

	// exit the game when `escape` key is pressed
	if (Input::isKeyDown(Key_Escape))
		shutdown();

}

void GameCore::onSimulate()
{
	// simulation event, called every simulation tick(fixed time)
	
}

void GameCore::onDraw()
{
	// draw event, called every frame, must be ended with gpu backbuffer `present` or `swapbuffer` - bgfx::frame()
	bgfx::setViewRect(0, 0, 0, m_width, m_height);
	bgfx::touch(0);

	m_rendering->beginRender(); // begin rendering the scene
	{
		// render shadows
		m_rendering->renderShadows();
		// TODO: draw all shadow casters
		// TODO: call World->DrawShadowCasters
		// TODO: call EntityPool->DrawShadowCasters

		// render static objects, eg.: static entities, voxels.
		m_rendering->renderStatic();
		// TODO: call World->Draw

		// render all entities
		m_rendering->renderEntities();
		// TODO: call EntityPool->Draw
	}
	m_rendering->endRender(); // end rendering the scene

	// next frame, wait vsync
	bgfx::frame();
}