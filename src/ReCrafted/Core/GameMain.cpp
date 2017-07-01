// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "GameMain.h"
#include "../Scripting/ScriptingEngine.h"

#define GAMEBASE_CHECK_SHUTDOWN if (!m_running) break;

GameMain* GameMain::m_instance;

int m_cursorX = 0u;
int m_cursorY = 0u;
int m_cursorDeltaX = 0u;
int m_cursorDeltaY = 0u;
GameMain* gameMain_instance;

LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
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
		Rid[0].hwndTarget = Platform::getGameWindow();

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

					ScreenToClient(winHwnd, &cursorPos);

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
	wnd.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wnd.lpszClassName = L"recrafted";
	wnd.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	RegisterClassEx(&wnd);

	Platform::setGameWindow(CreateWindowW(L"recrafted", L"ReCrafted", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 1280, 720, NULL, NULL, instance, nullptr));

	ShowWindow(Platform::getGameWindow(), SW_MAXIMIZE);
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
		while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// frame
		{
			onUpdate(); GAMEBASE_CHECK_SHUTDOWN
			onSimulate(); GAMEBASE_CHECK_SHUTDOWN // TODO: fixed-time step `onSimulate` call
			onDraw(); GAMEBASE_CHECK_SHUTDOWN
		}
	}

	// unload game assets
	onUnload();

	// destroy gamewindow
	DestroyWindow(Platform::getGameWindow());
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

	// get game window size
	Platform::getGameWindowSize(&m_width, &m_height);

	Logger::write("Creating game renderer using Direct3D11 API", LogLevel::Info);

	// initialize scripting engine
	ScriptingEngine::run();

	// initialize bgfx
	bgfx::init(bgfx::RendererType::Direct3D11);
	bgfx::reset(m_width, m_height, BGFX_RESET_NONE);

	bgfx::setDebug(BGFX_DEBUG_NONE);

	// Set view 0 clear state.
	bgfx::setViewClear(RENDERVIEW_BACKBUFFER, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030FF, 1.0f, 0);
	bgfx::setViewRect(RENDERVIEW_BACKBUFFER, 0, 0, m_width, m_height);

	bgfx::setViewClear(RENDERVIEW_GBUFFER, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
	bgfx::setViewRect(RENDERVIEW_GBUFFER, 0, 0, m_width, m_height);

	Logger::write("Renderer initialized", LogLevel::Info);

	// initialize rendering
	m_rendering = new Rendering;
	m_rendering->init();

	// initialize ui
	m_ui = new UI;
	m_ui->init();

	Logger::write("Rendering pipeline initialized", LogLevel::Info);

	// initialize main camera for scene
	m_camera = Camera::createCamera(true, true);
	m_camera->set_position(Vector3(0.0f, 20.0f, -10.0f));

	m_initialized = true;

	// initialize universe
	m_universe = new Universe();
	m_universe->init();

	Logger::write("Game initialized", LogLevel::Info);
}

void GameMain::onUnload()
{
	if (!m_initialized)
		return;

	Logger::write("Shutting down...", LogLevel::Info);

	// shutdown scripting engine
	ScriptingEngine::shutdown();

	// release all resources etc.
	SafeDispose(m_rendering);
	SafeDispose(m_universe);

	// shutdown rendering
	bgfx::shutdown();

	SafeDispose(m_logger);
}

void GameMain::onResize(uint width, uint height)
{
	// check if game is already initialized
	if (!m_initialized)
		return; // do not allow to enter this method when game is not initialized yet.

	m_width = width;
	m_height = height;

	// reset bgfx state, this should force renderer to resize all the viewports etc.
	bgfx::setViewRect(RENDERVIEW_BACKBUFFER, 0, 0, m_width, m_height);
	bgfx::setViewRect(RENDERVIEW_GBUFFER, 0, 0, m_width, m_height);

	bgfx::reset(m_width, m_height, BGFX_RESET_NONE);

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

	ScriptingEngine::update();
}

void GameMain::onSimulate()
{
	// simulation event, called every simulation tick(fixed time)

	m_universe->simulate();

	ScriptingEngine::simulate();
}

void GameMain::onDraw()
{
	// draw event, called every frame, must be ended with gpu backbuffer `present` or `swapbuffer` - bgfx::frame()
	bgfx::setViewRect(RENDERVIEW_BACKBUFFER, 0, 0, m_width, m_height);
	bgfx::setViewRect(RENDERVIEW_GBUFFER, 0, 0, m_width, m_height);

	bgfx::touch(RENDERVIEW_BACKBUFFER);

	// update state
	m_rendering->setState(false, false);

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

	// draw UI
	m_ui->beginDraw(); // begin draw UI
	{
		m_ui->testDraw();
	}
	m_ui->endDraw(); // end draw UI
	
	// next frame, wait vsync
	bgfx::frame();
}

#undef GAMEBASE_CHECK_SHUTDOWN