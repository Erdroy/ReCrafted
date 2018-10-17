// ReCrafted (c) 2016-2018 Always Too Late

#include "AudioSound.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
    API_DEFINE_OBJECT_CREATOR("ReCrafted.API.Audio", "AudioSound", AudioSound)
}

void AudioSound::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Audio.AudioSound", &Internal::CreateAudioSound);

    API_FILE("Audio/AudioSound.Gen.cs");
    {
        API_COMMENT("AudioSound class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Audio", "AudioSound", "Object", PARTIAL, NOCONSTRUCTOR);
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
