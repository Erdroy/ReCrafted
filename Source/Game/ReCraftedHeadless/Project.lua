project "ReCraftedHeadless"
	location (path.join(SOURCE_DIR, "Game/ReCraftedHeadless"))
	targetname "ReCraftedHeadless"
	
	kind "ConsoleApp"
	language "C++"
	flags { "NoManifest", "ShadowedVariables", "RelativeLinks", "NoPCH" }
	defines { "_CRT_SECURE_NO_WARNINGS", "JEMALLOC_EXPORT=", "JEMALLOC_STATIC", "NOMINMAX", "RC_HEADLESS" }
	
	-- add source/header/shader files
	files {
		"./**.lua",
		"./**.cpp",
		"./**.hpp",
		"./**.hlsl",
		"./**.hlsli",
	}

	forceincludes { 
		path.join(SOURCE_DIR, "Engine/ReCrafted.Common/Common/Memory.hpp"),
		path.join(SOURCE_DIR, "Engine/ReCrafted.Common/Common/Allocator.hpp")
	}
	
    includedirs {
        "./",
		path.join(SOURCE_DIR, "Engine/ReCrafted.Common"),
		path.join(SOURCE_DIR, "Engine/ReCrafted.Core"),
		--path.join(SOURCE_DIR, "Engine/ReCrafted.Graphics"),
		
        -- add all modules
		
		path.join(SOURCE_DIR, "Engine/ReCrafted.Game"),
    }
    
    -- default deps
    links { 
        "ReCrafted.Common",
        "ReCrafted.Core",
        --"ReCrafted.Graphics",
		
        "ReCrafted.Game",
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