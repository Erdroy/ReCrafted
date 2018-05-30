// ReCrafted (c) 2016-2018 Always Too Late

#include "Assembly.h"

RefPtr<Assembly> Assembly::API;
RefPtr<Assembly> Assembly::Game;

RefPtr<Class> Assembly::FindClass(const char* class_namespace, const char* class_name)
{
    RefPtr<Class> newClass(new Class);
    newClass->m_class = mono_class_from_name(m_image, class_namespace, class_name);
    newClass->m_assembly = this;
    return newClass;
}
