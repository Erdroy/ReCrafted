// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIEngine.h"
#include "Core/Logger.h"

#include <cef_app.h>
#include <cef_client.h>

#ifdef _WIN32
#include <direct.h>
#include <dxgi.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#pragma comment(lib, "libcef.lib")

#ifdef _DEBUG
#pragma comment(lib, "libcef_dll_wrapper_debug.lib")
#else
#pragma comment(lib, "libcef_dll_wrapper.lib")
#endif

class HtmlApp : public CefApp, public CefBrowserProcessHandler
{
public:
    HtmlApp() {
    }

    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override
    {
        return this;
    }

    void OnBeforeCommandLineProcessing(
        CefString const& /*process_type*/,
        CefRefPtr<CefCommandLine> command_line) override
    {
        command_line->AppendSwitch("disable-gpu-vsync");
    }

    virtual void OnContextInitialized() override
    {
    }

private:

    IMPLEMENT_REFCOUNTING(HtmlApp);
};

CefRefPtr<HtmlApp> m_app = nullptr;

void WebUIEngine::init()
{
    char resourcesDir[FILENAME_MAX];
    if (!GetCurrentDir(resourcesDir, sizeof(resourcesDir)))
        return;

    strcat(resourcesDir, "\\cef\\resources");

    CefSettings settings = {};
    CefString(&settings.resources_dir_path).FromASCII(resourcesDir);
    CefString(&settings.locales_dir_path).FromASCII("cef/locales");
    CefString(&settings.cache_path).FromASCII("cef/cache");
    CefString(&settings.log_file).FromASCII("cef/debug.log");
    CefString(&settings.user_agent).FromASCII("ReCraftedBrowser");

    settings.log_severity = LOGSEVERITY_WARNING;
    settings.remote_debugging_port = 25000;
    settings.background_color = 0x00000000;
    settings.windowless_rendering_enabled = true;
    //settings.multi_threaded_message_loop = false;
    settings.external_message_pump = false;

    CefEnableHighDPISupport();

    const CefMainArgs args(getHInstance());
    cvar result = CefExecuteProcess(args, nullptr, nullptr);

    if (result >= 0) // run CEF process, and exit when main process exits
    {
        Logger::logError("Failed to initialize WebUI (CefExecuteProcess)");
        exit(0);
    }

    m_app = new HtmlApp();

    if (!CefInitialize(args, settings, m_app, nullptr))
    {
        Logger::logError("Failed to initialize WebUI (CefInitialize)");
        return;
    }

    Logger::log("WebUIEngine initialized");
}

void WebUIEngine::update()
{
    //CefDoMessageLoopWork();
}

void WebUIEngine::dispose()
{
    //CefShutdown();
}

void WebUIEngine::runChildren()
{
    CefMainArgs args(getHInstance());

    cvar result = CefExecuteProcess(args, nullptr, nullptr);

    if (result >= 0) // run CEF process, and exit when main process exits
        exit(0);
}
