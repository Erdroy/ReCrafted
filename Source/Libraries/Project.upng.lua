project "upng"
	location (LIBRARIES_DIR)
	targetname "upng"
	kind "StaticLib"
	language "C++"
	
	-- add source/header/shader files
	files {
		"./upng/upng.c",
		"./upng/upng.h",
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