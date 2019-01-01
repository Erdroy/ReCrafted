// ReCrafted (c) 2016-2019 Always Too Late

#include "SceneManager.h"

void SceneManager::InitRuntime()
{
    API_FILE("Scene/SceneManager.Gen.cs");
    {
        API_COMMENT("SceneManager class.");
        API_CLASS(PUBLIC, ABSTRACT, "ReCrafted.API.Scene", "SceneManager");
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
