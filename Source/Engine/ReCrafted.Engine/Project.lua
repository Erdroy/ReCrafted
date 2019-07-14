project "ReCrafted.Engine"
	location (path.join(SOURCE_DIR, "Engine/ReCrafted.Engine"))
	targetname "ReCrafted.Engine"
	
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
		path.join(SOURCE_DIR, "Engine/ReCrafted.Engine/Common/Memory.h"),
		path.join(SOURCE_DIR, "Engine/ReCrafted.Engine/Common/Allocator.h")
	}
	
    includedirs {
        "./",
        -- add all modules
        path.join(LIBRARIES_DIR, "upng"),
        path.join(LIBRARIES_DIR, "sparsepp"),
        path.join(LIBRARIES_DIR, "concurrentqueue"),
        path.join(LIBRARIES_DIR, "fmt/include"),
        path.join(LIBRARIES_DIR, "json/include"),
        path.join(LIBRARIES_DIR, "cpp-base64"),
		
        path.join(LIBRARIES_DIR, "ReCrafted.Mono/Include"),
        path.join(LIBRARIES_DIR, "ReCrafted.PhysX/Include"),
    }

	libdirs {
		path.join(LIBRARIES_DIR, "ReCrafted.Mono/Build/Windows-x86_64"),
	}
	
    -- default deps
    links { 
        "cpp-base64",
        "upng",

        "d3d11", "dxguid", "dxgi", "d3dcompiler", 
        "Rpcrt4",
		
		-- Mono
		"mono-2.0-sgen",
		
		-- PhysX
		"PhysX_static_64", 
		"PhysXCharacterKinematic_static_64", 
		"PhysXCommon_static_64",
		"PhysXCooking_static_64", 
		"PhysXExtensions_static_64", 
		"PhysXFoundation_static_64", 
		"PhysXPvdSDK_static_64", 
		"PhysXVehicle_static_64", 
	}
	
    -- exclude hlsl files from build
    filter { "files:**.hlsl" }
        flags { "ExcludeFromBuild" }
        
	-- add multi processor compilation
	configuration { "vs*"}
		buildoptions { "/MP" }

	-- configs
    configuration { "Debug" }
		libdirs { 
			path.join(LIBRARIES_DIR, "ReCrafted.PhysX/Build/Windows-x86_64/Debug") 
		}
		objdir (path.join(ROOT_DIR, "Temp/ReCrafted.Engine/debug"))
		debugargs { "-debug" }
		defines { "DEBUG", "_ITERATOR_DEBUG_LEVEL=0" }
		runtime "Debug"
		symbols "On"
        links { }

    configuration { "Development" }
		libdirs { 
			path.join(LIBRARIES_DIR, "ReCrafted.PhysX/Build/Windows-x86_64/Checked") 
		}
		objdir (path.join(ROOT_DIR, "Temp/ReCrafted.Engine/developement"))
		debugargs { "-debug" }
		defines { "NDEBUG", "DEVELOPMENT" }
		flags { "No64BitChecks", "NoBufferSecurityCheck" }
		runtime "Release"
		symbols "On"
        links { }

	configuration { "Release" }
		libdirs { 
			path.join(LIBRARIES_DIR, "ReCrafted.PhysX/Build/Windows-x86_64/Release") 
		}
		objdir (path.join(ROOT_DIR, "Temp/ReCrafted.Engine/release"))
		debugargs { }
		defines { "NDEBUG", "RELEASE" }
		flags { "No64BitChecks", "NoBufferSecurityCheck", "LinkTimeOptimization" }
		runtime "Release"
        optimize "Speed"

	configuration { "x64" }
		targetdir (path.join(TARGET_DIR, "Bin64/"))
		debugdir (path.join(TARGET_DIR, "Bin64/"))
	
	configuration {}