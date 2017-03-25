// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "GameCore.h"
#include "../Utils/Defines.h"

GameCore* GameCore::m_instance;

void GameCore::onLoad()
{
	// initialize logger
	m_logger = new Logger();
	m_logger->init();

	Logger::write("ReCrafted - startup", LogLevel::Info);

	// get game window size
	Platform::getGameWindowSize(&m_width, &m_height);

	Logger::write("Creating game renderer using Direct3D11 API", LogLevel::Info);

	// initialize bgfx
	bgfx::init(bgfx::RendererType::Direct3D11);
	bgfx::reset(m_width, m_height, BGFX_RESET_VSYNC);

	bgfx::setDebug(BGFX_DEBUG_TEXT);

	// Set view 0 clear state.
	bgfx::setViewClear(RENDERVIEW_BACKBUFFER, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030FF, 1.0f, 0);
	bgfx::setViewRect(RENDERVIEW_BACKBUFFER, 0, 0, m_width, m_height);

	bgfx::setViewClear(RENDERVIEW_GBUFFER, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
	bgfx::setViewRect(RENDERVIEW_GBUFFER, 0, 0, m_width, m_height);

	Logger::write("Renderer initialized", LogLevel::Info);

	// initialize rendering
	m_rendering = new Rendering;
	m_rendering->init();

	Logger::write("Rendering pipeline initialized", LogLevel::Info);

	// initialize main camera for scene
	m_camera = Camera::createCamera(true, true);
	m_camera->set_position(Vector3(0.0f, 20.0f, -10.0f));

	m_initialized = true;

	// update state
	m_rendering->setState(false, false);

	// initialize universe
	m_universe = new Universe();
	m_universe->init();

	Logger::write("Game initialized", LogLevel::Info);
}

void GameCore::onUnload()
{
	if (!m_initialized)
		return;

	Logger::write("Shutting down...", LogLevel::Info);

	// release all resources etc.
	SafeDispose(m_rendering);
	SafeDispose(m_universe);
	SafeDispose(m_logger);

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
	bgfx::setViewRect(RENDERVIEW_BACKBUFFER, 0, 0, m_width, m_height);
	bgfx::setViewRect(RENDERVIEW_GBUFFER, 0, 0, m_width, m_height);

	bgfx::reset(m_width, m_height, BGFX_RESET_VSYNC);

	m_rendering->resize(width, height);
}

void GameCore::onUpdate()
{
	// update event, called every frame

	// exit the game when `escape` key is pressed
	if (Input::isKeyDown(Key_Escape))
		shutdown();

	if (Input::isKeyDown(Key_F4))
	{
		bgfx::setDebug(BGFX_DEBUG_NONE);
		Logger::write("Switching to default render mode", LogLevel::Info);
	}

	if (Input::isKeyDown(Key_F5))
	{
		bgfx::setDebug(BGFX_DEBUG_STATS);
		Logger::write("Switching to debug stats render mode", LogLevel::Info);
	}

	if (Input::isKeyDown(Key_F6))
	{
		bgfx::setDebug(BGFX_DEBUG_TEXT);
		Logger::write("Switching to debug text render mode", LogLevel::Info);
	}

	m_universe->update();
}

void GameCore::onSimulate()
{
	// simulation event, called every simulation tick(fixed time)
	
	m_universe->simulate();
}

void GameCore::onDraw()
{
	// draw event, called every frame, must be ended with gpu backbuffer `present` or `swapbuffer` - bgfx::frame()
	bgfx::setViewRect(RENDERVIEW_BACKBUFFER, 0, 0, m_width, m_height);
	bgfx::setViewRect(RENDERVIEW_GBUFFER, 0, 0, m_width, m_height);
	bgfx::touch(RENDERVIEW_BACKBUFFER);
	bgfx::touch(RENDERVIEW_GBUFFER);

	m_rendering->beginRender(); // begin rendering the scene
	{
		// render shadows
		m_rendering->renderShadows();
		// draw all shadow casters
		m_universe->drawShadowCasters();
		// TODO: call EntityPool->DrawShadowCasters

		// render static objects, eg.: static entities, voxels.
		m_rendering->renderStatic();
		m_universe->draw();

		// render all entities
		m_rendering->renderEntities();
		// TODO: call EntityPool->Draw
	}
	m_rendering->endRender(); // end rendering the scene

	// next frame, wait vsync
	bgfx::frame();
}