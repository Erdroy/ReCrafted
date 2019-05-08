project "ReCrafted"
	location (path.join(SOURCE_DIR, "Game/ReCrafted"))
	targetname "ReCrafted"
	
	kind "WindowedApp"
	language "C++"
    entrypoint "WinMainCRTStartup"
	flags { "NoManifest", "ShadowedVariables", "RelativeLinks", "NoPCH" }
	defines { "_CRT_SECURE_NO_WARNINGS", "JEMALLOC_EXPORT=", "JEMALLOC_STATIC", "NOMINMAX" }
	
	-- add source/header/shader files
	files {
		"./**.lua",
		"./**.cpp",
		"./**.h",
		"./**.hlsl",
		"./**.hlsli",
	}

	forceincludes { 
		path.join(SOURCE_DIR, "Engine/ReCrafted.Engine/Common/Memory.h"),
		path.join(SOURCE_DIR, "Engine/ReCrafted.Engine/Common/Allocator.h")
	}
	
    includedirs {
        "./",
		path.join(SOURCE_DIR, "Engine/ReCrafted.Engine"),
        
        path.join(LIBRARIES_DIR, "sparsepp"),
        path.join(LIBRARIES_DIR, "concurrentqueue"),
        path.join(LIBRARIES_DIR, "fmt/include"),
    }
    
	libdirs {
		path.join(LIBRARIES_DIR, "ReCrafted.Mono/Build/Windows-x86_64"),
	}
	
    -- default deps
    links { 
        "ReCrafted.Engine",
    }
	
	-- add onbuild script and multi processor compilation
	configuration { "vs*"}
		buildoptions { "/MP" }
		postbuildcommands {
			"call " .. ROOT_DIR .. "/pm.bat PostBuild --skip-api",
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