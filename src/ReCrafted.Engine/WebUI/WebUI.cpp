// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUI.h"
#include "WebUIView.h"
#include "WebUIEngine.h"
#include "Core/Logger.h"
#include "Content/ContentManager.h"
#include "Common/Profiler/Profiler.h"

SINGLETON_IMPL(WebUI)

void WebUI::OnInit()
{
    Logger::Log("WebUI is being initialized...");

    m_engine = WebUIEngine::GetInstance();
    m_engine->Init();

    Logger::Log("WebUI initialized");
}

void WebUI::OnDispose()
{
    for (var&& view : m_views)
        Object::Destroy(view);

    m_views.Clear();

    SafeDispose(m_engine);

    Logger::Log("WebUI disposed");
}

void WebUI::Resize(const uint width, const uint height)
{
    for (crvar view : m_views)
    {
        if(view->IsFullscreen())
            view->Resize(width, height);
    }
}

void WebUI::Update()
{
    Profiler::BeginProfile(__FUNCTION__);

    m_engine->Update();

    for (crvar view : m_views)
        view->Update();

    Profiler::EndProfile();
}

void WebUI::Render()
{
    Profiler::BeginProfile(__FUNCTION__);

    m_engine->Render();

    for (crvar view : m_views)
        view->Render();

    Profiler::EndProfile();
}

void WebUI::RenderViews()
{
    Profiler::BeginProfile(__FUNCTION__);
    for (crvar view : m_views)
        view->RenderView();
    Profiler::EndProfile();
}

WebUIView* WebUI::CreateView()
{
    if (!WebUIEngine::IsInitialized())
        return nullptr;

    var view = Object::CreateInstance<WebUIView>("ReCrafted.API.Graphics", "WebUIView");

    view->Init();
    m_views.Add(view);

    Logger::Log("Created new WebUIView");

    return view;
}
