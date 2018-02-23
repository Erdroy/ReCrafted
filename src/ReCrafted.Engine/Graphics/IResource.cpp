// ReCrafted (c) 2016-2018 Always Too Late

#include "IResource.h"

uint IResource::m_resCount;

#if COMPILE_WITH_RESOURCE_LISTING
Array<IResource*> IResource::m_resources;
Lock IResource::m_resourcesLock;
#endif