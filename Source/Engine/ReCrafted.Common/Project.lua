project "ReCrafted.Common"
	location (path.join(SOURCE_DIR, "Engine/ReCrafted.Common"))
	targetname "ReCrafted.Common"
	
	kind "StaticLib"
	language "C++"
	flags { "NoManifest", "ShadowedVariables", "RelativeLinks", "NoPCH" }
	defines { "RC_API_EXPORT_CORE", "_CRT_SECURE_NO_WARNINGS", "JEMALLOC_EXPORT=", "JEMALLOC_STATIC", "NOMINMAX" }
	
	-- add source/header/shader files
	files {
		"./**.lua",
		"./**.cpp",
		"./**.h",
		"./**.hlsl",
		"./**.hlsli",
	}

	forceincludes { 
		path.join(SOURCE_DIR, "Engine/ReCrafted.Common/Common/Memory.h"),
		path.join(SOURCE_DIR, "Engine/ReCrafted.Common/Common/Allocator.h")
	}
	
    includedirs {
        "./",
    }

    -- default deps
    links { 
    }

	-- configs
    configuration { "Debug" }
		debugargs { "-debug" }
		defines { "DEBUG", "_ITERATOR_DEBUG_LEVEL=0" }
		runtime "Debug"
		symbols "On"
        links { }

    configuration { "Development" }
		debugargs { "-debug" }
		defines { "NDEBUG", "DEVELOPMENT" }
		flags { "No64BitChecks", "NoBufferSecurityCheck" }
		runtime "Release"
		symbols "On"
        links { }

	configuration { "Release" }
		debugargs { }
		defines { "NDEBUG", "RELEASE" }
		flags { "No64BitChecks", "NoBufferSecurityCheck", "LinkTimeOptimization" }
		runtime "Release"
        optimize "Speed"

	configuration { "x64" }
		targetdir (path.join(TARGET_DIR, "Bin64/"))
		debugdir (path.join(TARGET_DIR, "Bin64/"))
	
	configuration {}