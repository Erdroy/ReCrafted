// ReCrafted © 2016-2017 Always Too Late

#include "GameMain.h"
#include "Common/Display.h"
#include "Graphics/Atlas.h"
#include "Scripting/Bindings.h"
#include "Scripting/Assembly.h"
#include "Scripting/Object.h"
#include "Graphics/DebugDraw.h"
#include "Graphics/HTML5_UI/HTML5_UI.h"

#define CHECK_SHUTDOWN if (!m_running) break;

#define RESET_FLAGS (BGFX_RESET_NONE | BGFX_RESET_VSYNC)

GameMain* GameMain::m_instance;

int m_cursorX = 0u;
int m_cursorY = 0u;
int m_cursorDeltaX = 0u;
int m_cursorDeltaY = 0u;
GameMain* gameMain_instance;

#if _WIN32
HCURSOR m_defaultCursor;
#endif

LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_SETCURSOR:
	{
		if(LOWORD(lparam) == HTCLIENT)
		{
			gameMain_instance->onCursorRequest();
		}
		break;
	}

	case WM_SIZE: // handle window resizing
	{
		auto width = LOWORD(lparam);
		auto height = HIWORD(lparam);
		gameMain_instance->onResize(width, height);
		break;
	}

	case WM_CLOSE: // handle window closing
	{
		gameMain_instance->shutdown();
		return 0;
	}

	// input handling
	case WM_KEYDOWN:
	{
		Input::getInstance()->emit(false, uint(wparam));
		return 0;
	}
	case WM_KEYUP:
	{
		Input::getInstance()->emit(true, uint(wparam));
		return 0;
	}
	case WM_CREATE:
	{
		// register for raw input
		// source: https://msdn.microsoft.com/pl-pl/library/windows/desktop/ms645546(v=vs.85).aspx#example_1
		RAWINPUTDEVICE Rid[1];

		Rid[0].usUsagePage = 0x01;
		Rid[0].usUsage = 0x02;
		Rid[0].dwFlags = 0;
		Rid[0].hwndTarget = static_cast<HWND>(Platform::getGameWindow());

		if (RegisterRawInputDevices(Rid, 1, sizeof Rid[0]) == FALSE) {
			// error, failed to register
			Logger::write("Failed to register mouse for RAW Input", LogLevel::Error);
			return 0;
		}
		return 0;
	}
	case WM_INPUT:
	{
		UINT dwSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

		static LPBYTE lpb[40] = {};
		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			return DefWindowProc(hWnd, msg, wparam, lparam);

		auto raw = reinterpret_cast<RAWINPUT*>(lpb);

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
			{
				m_cursorDeltaX += raw->data.mouse.lLastX;
				m_cursorDeltaY += raw->data.mouse.lLastY;

				// update cursor pos
				auto winHwnd = Platform::getGameWindow();

				if (winHwnd) {
					POINT cursorPos;
					GetCursorPos(&cursorPos);

					ScreenToClient(static_cast<HWND>(winHwnd), &cursorPos);

					m_cursorX = cursorPos.x;
					m_cursorY = cursorPos.y;
				}
			}

			if(raw->data.mouse.ulButtons & RI_MOUSE_LEFT_BUTTON_DOWN)
				Input::getInstance()->emit(false, INPUT_LBUTTON);
			
			if (raw->data.mouse.ulButtons & RI_MOUSE_LEFT_BUTTON_UP)
				Input::getInstance()->emit(true, INPUT_LBUTTON);

			if (raw->data.mouse.ulButtons & RI_MOUSE_MIDDLE_BUTTON_DOWN)
				Input::getInstance()->emit(false, INPUT_MBUTTON);

			if (raw->data.mouse.ulButtons & RI_MOUSE_MIDDLE_BUTTON_UP)
				Input::getInstance()->emit(true, INPUT_MBUTTON);

			if (raw->data.mouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_DOWN)
				Input::getInstance()->emit(false, INPUT_RBUTTON);

			if (raw->data.mouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_UP)
				Input::getInstance()->emit(true, INPUT_RBUTTON);
		}

		return DefWindowProc(hWnd, msg, wparam, lparam);
	}

	// default
	default:
		return DefWindowProc(hWnd, msg, wparam, lparam);
	}

	return DefWindowProc(hWnd, msg, wparam, lparam);
}

void GameMain::initScripting()
{
	// create root domain
	m_domain = Domain::createRoot();

	if(!Platform::fileExists("ReCrafted.Game.dll") || !Platform::fileExists("ReCrafted.API.dll"))
	{
		Logger::write("Could not find some of game assemblies.", LogLevel::Fatal);
		exit(-1);
		return;
	}

	// load base assemblies
	Assembly::Game = m_domain->loadAssembly("ReCrafted.Game.dll");
	Assembly::API = m_domain->loadAssembly("ReCrafted.API.dll");

	// apply bindings
	Bindings::bind();
	
	// create gamemain instance
	m_gamemain = Object::createInstance<Object>("ReCrafted.Game", "GameMain", Assembly::Game, false);

	// find methods
	m_init_method = m_gamemain->findMethod("ReCrafted.Game.GameMain::Initialize");
	m_update_method = m_gamemain->findMethod("ReCrafted.Game.GameMain::Update");
	m_simulate_method = m_gamemain->findMethod("ReCrafted.Game.GameMain::Simulate");
	m_drawui_method = m_gamemain->findMethod("ReCrafted.Game.GameMain::DrawUI");
	m_shutdown_method = m_gamemain->findMethod("ReCrafted.Game.GameMain::Shutdown");

	Logger::write("Scripting initialized", LogLevel::Info);
}

void GameMain::run()
{
	// set all needed instance handlers
	gameMain_instance = m_instance = this;

	m_input = new Input;
	m_time = new Time;

	auto instance = getHInstance();

	// TODO: load game config

	// initialize platform
	Platform::initialize();

#ifdef _WIN32
	// create gamewindow
	WNDCLASSEX wnd;
	memset(&wnd, 0, sizeof(wnd));
	wnd.cbSize = sizeof(wnd);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = wndProc;
	wnd.hInstance = instance;
	wnd.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wnd.hCursor = nullptr;
	wnd.lpszClassName = L"recrafted";
	wnd.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	RegisterClassEx(&wnd);
	
	m_defaultCursor = LoadCursor(nullptr, IDC_ARROW);

	auto window = CreateWindowW(L"recrafted", L"ReCrafted", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 1280, 720, NULL, NULL, instance, nullptr);
	Platform::setGameWindow(static_cast<void*>(window));

	ShowWindow(static_cast<HWND>(Platform::getGameWindow()), SW_MAXIMIZE);
#endif

	// initialize bgfx platform data
	bgfx::PlatformData pd;
	memset(&pd, 0, sizeof(pd));
	pd.nwh = Platform::getGameWindow();
	bgfx::setPlatformData(pd);

	// run
	m_running = true;

	// load game assets, initialize everything
	onLoad();

	// save start time
	auto lastTime = Platform::getMiliseconds();

	while (m_running) // main loop
	{
		// update time
		auto currentTime = Platform::getMiliseconds();
		Time::m_instance->m_deltaTime = ((currentTime - lastTime) / 1000.0);
		Time::m_instance->m_time = float(currentTime / 1000.0);
		lastTime = currentTime;

		// update input
		m_input->update(m_cursorX, m_cursorY, m_cursorDeltaX, m_cursorDeltaY);
		m_cursorDeltaX = 0;
		m_cursorDeltaY = 0;

		// process input
		MSG msg;
		msg.message = WM_NULL;
		while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// frame
		{
			onUpdate(); CHECK_SHUTDOWN
			onSimulate(); CHECK_SHUTDOWN // TODO: fixed-time step `onSimulate` call
			onDraw(); CHECK_SHUTDOWN
		}
	}

	// unload game assets
	onUnload();

	// destroy gamewindow
	DestroyWindow(static_cast<HWND>(Platform::getGameWindow()));
}

void GameMain::shutdown()
{
	// release all resources etc.
	SafeDispose(m_input);
	SafeDispose(m_time);

	m_running = false;
}

void GameMain::onLoad()
{
	// initialize logger
	m_logger = new Logger();
	m_logger->init();

	Logger::write("ReCrafted - startup", LogLevel::Info);

	uint width;
	uint height;

	// get game window size
	Platform::getGameWindowSize(&width, &height);

	Display::set_Width(width);
	Display::set_Height(height);

	Logger::write("Creating game renderer using Direct3D11 API", LogLevel::Info);

	// initialize scripting engine
	initScripting();

	// initialize bgfx
	bgfx::init(bgfx::RendererType::Direct3D11);
	bgfx::reset(Display::get_Width(), Display::get_Height(), RESET_FLAGS);

	bgfx::setDebug(BGFX_DEBUG_NONE);

	// Set view 0 clear state.
	bgfx::setViewClear(RENDERVIEW_BACKBUFFER, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030FF, 1.0f, 0);
	bgfx::setViewRect(RENDERVIEW_BACKBUFFER, 0, 0, Display::get_Width(), Display::get_Height());

	bgfx::setViewClear(RENDERVIEW_GBUFFER, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
	bgfx::setViewRect(RENDERVIEW_GBUFFER, 0, 0, Display::get_Width(), Display::get_Height());

	Logger::write("Renderer initialized", LogLevel::Info);

	// initialize rendering
	m_rendering = new Rendering;
	m_rendering->init();

	// initialize ui
	m_ui = new UI;
	m_ui->init();

	// initialize DebugDraw
	DebugDraw::init();

	// initialize HTML5 UI
	HTML5UI::init();

	Logger::write("Rendering pipeline initialized", LogLevel::Info);

	m_initialized = true;

	// initialize universe
	m_universe = new Universe();
	m_universe->init();

	// initialize entity pool
	m_entityPool = new EntityPool;
	m_entityPool->initialize();

	Logger::write("Game initialized", LogLevel::Info);

	m_init_method->invoke();
}

void GameMain::onUnload()
{
	if (!m_initialized)
		return;

	Logger::write("Shutting down...", LogLevel::Info);

	// invoke shutdown method
	m_shutdown_method->invoke();

	// destroy all objects
	Object::destroyall();
	
	// shutdown scripting engine
	m_domain->cleanup();

	// shutdown UI
	m_ui->dispose();
	SafeDispose(m_ui);

	// shutdown debug draw
	DebugDraw::shutdown();

	// shutdown HTML5 UI
	HTML5UI::shutdown();

	// shutdown bindings
	Bindings::shutdown();

	// release all resources etc.
	SafeDispose(m_rendering);
	SafeDispose(m_universe);
	SafeDispose(m_entityPool);

	// shutdown rendering
	bgfx::shutdown();

	SafeDispose(m_logger);
}

void GameMain::onResize(uint width, uint height)
{
	// check if game is already initialized
	if (!m_initialized)
		return; // do not allow to enter this method when game is not initialized yet.

	Display::set_Width(width);
	Display::set_Height(height);

	// reset bgfx state, this should force renderer to resize all the viewports etc.
	bgfx::setViewRect(RENDERVIEW_BACKBUFFER, 0, 0, Display::get_Width(), Display::get_Height());
	bgfx::setViewRect(RENDERVIEW_GBUFFER, 0, 0, Display::get_Width(), Display::get_Height());

	bgfx::reset(Display::get_Width(), Display::get_Height(), RESET_FLAGS);

	m_rendering->resize(width, height);
}

void GameMain::onUpdate()
{
	// update event, called every frame

	if (Input::isKeyDown(Key_F3))
	{
		bgfx::setDebug(BGFX_DEBUG_WIREFRAME);
		Logger::write("Switching to wireframe render mode", LogLevel::Info);
	}

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
	m_update_method->invoke();
    m_entityPool->update();

	if(m_lockCursor)
	{
		// lock position
		Input::setCursorPos(
			static_cast<int>(round(Display::get_Width() / 2.0f)),
			static_cast<int>(round(Display::get_Height() / 2.0f))
		);
	}
}

void GameMain::onSimulate()
{
	// simulation event, called every simulation tick(fixed time)

	m_universe->simulate();
	m_simulate_method->invoke();
    m_entityPool->simulate();
}

void GameMain::onDraw()
{
	// draw event, called every frame, must be ended with gpu backbuffer `present` or `swapbuffer` - bgfx::frame()
	bgfx::setViewRect(RENDERVIEW_BACKBUFFER, 0, 0, Display::get_Width(), Display::get_Height());
	bgfx::setViewRect(RENDERVIEW_GBUFFER, 0, 0, Display::get_Width(), Display::get_Height());

	bgfx::touch(RENDERVIEW_BACKBUFFER);

	// update state
	m_rendering->setState();

	m_rendering->beginRender(); // begin rendering the scene
	{
		// render shadows
		m_rendering->renderShadows();

		// draw all shadow casters
		m_universe->drawShadowCasters();

		// render static objects, eg.: static entities, voxels.
		m_rendering->renderStatic();
		m_universe->draw();

		// render all entities
		m_rendering->renderEntities();
	}
	m_rendering->endRender(); // end rendering the scene
	
	// debug draw
	DebugDraw::drawAll();
	
	// set UI state
	m_rendering->setState(false, false, true);

	// draw HTML5 UI
	HTML5UI::draw();

	// draw UI
	m_ui->beginDraw(); // begin draw UI
	{
		m_drawui_method->invoke();
	}
	m_ui->endDraw(); // end draw UI
	
	// next frame, wait vsync
	bgfx::frame();
}

void GameMain::onCursorRequest()
{
#if _WIN32
	if(m_showCursor)
	{
		SetCursor(m_defaultCursor);
	}
	else
	{
		SetCursor(nullptr);
	}
#endif
}

#undef CHECK_SHUTDOWN
