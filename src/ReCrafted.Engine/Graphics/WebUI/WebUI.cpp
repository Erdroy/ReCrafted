// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUI.h"
#include "WebUIView.h"
#include "WebUIEngine.h"
#include "Core/Logger.h"

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

void WebUI::Resize(uint width, uint height)
{
    for (var&& view : m_views)
        view->Resize(width, height);
}

void WebUI::Update()
{
    m_engine->Update();

    for (var&& view : m_views)
        view->Update();
}

void WebUI::Render()
{
    m_engine->Render();

    //if (m_engine->NeedsViewsUpdate()) 
    {
        for (var&& view : m_views)
            view->Render();
    }
}

RefPtr<WebUIView> WebUI::CreateView()
{
    if (!m_engine->IsInitialized())
        return nullptr;

    var view = Object::CreateInstance<WebUIView>("ReCrafted.API.Graphics", "WebUIView");
    view->Init();
    m_views.Add(view);

    Logger::Log("Created new WebUIView");

    return view;
}
