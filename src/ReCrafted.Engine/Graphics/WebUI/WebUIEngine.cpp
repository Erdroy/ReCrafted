// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIEngine.h"
#include "Core/Logger.h"
#include "Platform/Platform.h"

#include <cef_app.h>
#include <cef_client.h>
#include <include/cef_version.h>

#include <thread>

#ifdef _WIN32
#include <direct.h>
#include <dxgi.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

SINGLETON_IMPL(WebUIEngine)

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

class QuitTask : public CefTask
{
public:
    QuitTask() { }
    void Execute() override {
        CefQuitMessageLoop();
    }
    IMPLEMENT_REFCOUNTING(QuitTask);
};

CefRefPtr<HtmlApp> m_app = nullptr;
Ptr<std::thread> m_cefThread = nullptr;

void WebUIEngine::runCEF()
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
    settings.multi_threaded_message_loop = false;

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

    // run message loop
    CefRunMessageLoop();
    
    // shutdown cef now
    CefShutdown();
}

void WebUIEngine::init()
{
    m_cefThread = std::make_shared<std::thread>([this] {
        runCEF();
    });
    
    m_initialized = true;
    Logger::log("WebUIEngine initialized CEF_VERSION: {0} (Chromium: {1}.{2}.{3}.{4})", 
        CEF_VERSION, 
        CHROME_VERSION_MAJOR, 
        CHROME_VERSION_MINOR, 
        CHROME_VERSION_BUILD, 
        CHROME_VERSION_PATCH
    );
}

void WebUIEngine::onDispose()
{
    if (m_cefThread)
    {
        // post Quit task to CEF to say - kill yourself.
        CefRefPtr<CefTask> task(new QuitTask());
        CefPostTask(TID_UI, task.get());
        m_cefThread->join();
        m_cefThread.reset();
    }
}

void WebUIEngine::runChildren()
{
    // this can run on main thread, as children process will exit after running
    // TODO: ReCrafted.Browser.exe - separate application for browser processes
    CefMainArgs args(getHInstance());

    cvar result = CefExecuteProcess(args, nullptr, nullptr);

    if (result >= 0) // run CEF process, and exit when main process exits
        exit(0);
}

bool WebUIEngine::isInitialized()
{
    if (!m_instance)
        return false;

    return m_instance->m_initialized;
}
