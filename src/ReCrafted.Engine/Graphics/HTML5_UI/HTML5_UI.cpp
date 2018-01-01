// ReCrafted (c) 2016-2018 Always Too Late

#include "HTML5_UI.h"

#define HTML5_UI_ENABLED 0

#if HTML5_UI_ENABLED
#include "Core/Lock.h"
#include "Common/Display.h"
#include "Platform/Platform.h"

#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>

#include <stdio.h>
#include "Graphics/Rendering.h"
#include "Core/GameMain.h"

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#pragma comment(lib, "libcef.lib")
#pragma comment(lib, "libcef_dll_wrapper.lib")

class BrowserClient;
class RenderHandler;

Lock m_bufferIdLock = {};
bgfx::TextureHandle m_buffer;
byte* m_bufferData = nullptr;
int m_bufferSize = 0;

CefRefPtr<CefBrowser> browser;
CefRefPtr<BrowserClient> browserClient;
CefRefPtr<RenderHandler> renderHandler;

void createBuffers(uint16_t width, uint16_t height)
{
	const auto flags = 0
		| BGFX_TEXTURE_RT
		| BGFX_TEXTURE_MIN_POINT
		| BGFX_TEXTURE_MAG_POINT
		| BGFX_TEXTURE_MIP_POINT
		| BGFX_TEXTURE_U_CLAMP
		| BGFX_TEXTURE_V_CLAMP;

	m_bufferSize = width * height * 4;

	m_bufferData = new byte[m_bufferSize];

	memset(m_bufferData, 0, m_bufferSize);

	// initialize buffers
	m_buffer = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::BGRA8, flags, nullptr);
}

void destroyBuffers()
{
	delete[] m_bufferData;

	// bgfx is already down, so we don't have to destroy bufers.
	bgfx::destroy(m_buffer);
}

class RenderHandler : public CefRenderHandler
{
	IMPLEMENT_REFCOUNTING(RenderHandler);

public:
	bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override
	{
		rect = CefRect(0, 0, Display::get_Width(), Display::get_Height());
		return true;
	}

	bool GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override
	{
		return GetViewRect(browser, rect);
	}

	bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
		int viewX,
		int viewY,
		int& screenX,
		int& screenY) override
	{
		screenX = viewX;
		screenY = viewY;
		return true;
	}

	void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override
	{
		if (type != PET_VIEW || !GameMain::isRunning())
			return;

		// TODO: construct rectangle, instead of copying the whole texture
		// TODO: segmented copy

		auto size = width * height * 4;

		if(size != m_bufferSize)
		{
			// resize buffers
			destroyBuffers();
			createBuffers(width, height);
		}

		auto textureBuffer = m_bufferData;
		auto sourceBuffer = (byte*)buffer;

		// update texture
		memcpy(textureBuffer, sourceBuffer, size);

		auto ref = bgfx::makeRef(m_bufferData, size);
		bgfx::updateTexture2D(m_buffer, 0, 0, 0, 0, width, height, ref);
	}
};

class BrowserClient : public CefClient
{
	IMPLEMENT_REFCOUNTING(BrowserClient);

private:
	CefRefPtr<CefRenderHandler> m_renderHandler;

public:
	explicit BrowserClient(RenderHandler* renderHandler)
	{
		m_renderHandler = renderHandler;
	}

	CefRefPtr<CefRenderHandler> GetRenderHandler() override
	{
		return m_renderHandler;
	}
};

void HTML5UI::init()
{
	char resourcesDir[FILENAME_MAX];
	if (!GetCurrentDir(resourcesDir, sizeof(resourcesDir)))
		return;
	strcat(resourcesDir, "\\cef\\resources");

	CefMainArgs args(getHInstance());
	CefSettings settings = {};
	CefWindowInfo window_info = {};
	CefBrowserSettings browserSettings = {};

	CefString(&settings.resources_dir_path).FromASCII(resourcesDir);
	CefString(&settings.locales_dir_path).FromASCII("cef/locales");
	CefString(&settings.cache_path).FromASCII("cef/cache");
	CefString(&settings.log_file).FromASCII("cef/debug.log");
	CefString(&settings.user_agent).FromASCII("ReCrafted-CEF");
	CefString(&settings.user_agent).FromASCII("ReCrafted-CEF-1.0");

	settings.remote_debugging_port = 25000;
	settings.windowless_rendering_enabled = true;
	settings.background_color = 0x00000000;
	
	browserSettings.windowless_frame_rate = 120;
	browserSettings.plugins = STATE_DISABLED;
	browserSettings.webgl = STATE_ENABLED;

	CefEnableHighDPISupport();
	
	if (CefExecuteProcess(args, nullptr, nullptr) >= 0) // run CEF process, and exit when main process exits
		exit(0);

	if(!CefInitialize(args, settings, nullptr, nullptr))
		return;

	// create buffers
	createBuffers(Display::get_Height(), Display::get_Height());

	// initialize browser
	window_info.SetAsWindowless(static_cast<HWND>(Platform::getGameWindow()));
	
	renderHandler = new RenderHandler();
	browserClient = new BrowserClient(renderHandler);
	browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "about:blank", browserSettings, nullptr);
	
	auto host = browser->GetHost();
	host->WasResized();
	
	auto frame = browser->GetMainFrame();
	frame->LoadURL("file:///D:/CoherentGT/Samples/UI/C++/Minimal_HelloGT/uiresources/minRes/MainUI.html");
	
}

void HTML5UI::initChildren()
{
	CefMainArgs args(getHInstance());

	if (CefExecuteProcess(args, nullptr, nullptr) >= 0) // run CEF process, and exit when main process exits
		exit(0);
}

void HTML5UI::shutdown()
{
	destroyBuffers();
	
	// TODO: fix shutdown
	// https://bitbucket.org/chromiumembedded/cef/wiki/GeneralUsage#markdown-header-off-screen-rendering
	/*browser->GetHost()->CloseBrowser(false);

	CefShutdown();

	delete renderHandler;
	delete browserClient;*/
}

float m_last = 0;
void HTML5UI::draw()
{
	// draw texture
	Rendering::getInstance()->setState(false, false, true);
	Rendering::getInstance()->blit(0, m_buffer);
}

void HTML5UI::update()
{
	// TODO: push cef loop worker into another thread
	CefDoMessageLoopWork();
}

void HTML5UI::resized()
{
	auto host = browser->GetHost();
	host->WasResized();
}
#endif