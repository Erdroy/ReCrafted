// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUI.h"
#include "WebUIView.h"
#include "WebUIEngine.h"
#include "Core/Logger.h"

SINGLETON_IMPL(WebUI)

void WebUI::onInit()
{
    Logger::log("WebUI is being initialized...");

    m_engine = std::make_shared<WebUIEngine>();
    m_engine->init();

    Logger::log("WebUI initialized");
}

void WebUI::onDispose()
{
    for (var && view : m_views)
        Object::destroy(view);

    m_views.clear();

    SafeDispose(m_engine);

    Logger::log("WebUI disposed");
}

void WebUI::resize(uint width, uint height)
{
    for (var && view : m_views)
        view->resize(width, height);
}

void WebUI::update()
{
    m_engine->update();
}

void WebUI::render()
{
}

Ptr<WebUIView> WebUI::createView()
{
    var view = Object::createInstance<WebUIView>("ReCrafted.API.Graphics", "WebUIView");
    view->init();
    m_views.add(view);

    Logger::log("Created new WebUIView");

    return view;
}
