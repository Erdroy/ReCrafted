// ReCrafted (c) 2016-2018 Always Too Late

#include "Assembly.h"

Ref<Assembly> Assembly::API;
Ref<Assembly> Assembly::Game;

Ref<Class> Assembly::FindClass(const char* class_namespace, const char* class_name)
{
    Ref<Class> newClass(new Class);
    newClass->m_class = mono_class_from_name(m_image, class_namespace, class_name);
    newClass->m_assembly = this;
    return newClass;
}
