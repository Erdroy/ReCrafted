project "ReCrafted.Engine"
	kind "WindowedApp"
	language "C++"
	flags {
		"WinMain", 
		"Unicode", 
		"NoImportLib",
		"NativeWChar",
		"NoEditAndContinue", 
		"NoPCH",
		"StaticRuntime",
		"NoRTTI",
	}
	defines { "_CRT_SECURE_NO_WARNINGS", "JEMALLOC_EXPORT=", "JEMALLOC_STATIC" }
	location (OUTPUT_DIR)
	targetname "ReCrafted"
	
	-- add source/header/shader files
	files {
		path.join(ROOT_DIR, "src/ReCrafted.Engine/**"),
		
		path.join(LIBS_DIR, "lodepng/src/**"),
		path.join(LIBS_DIR, "fmt/src/**"),
		path.join(LIBS_DIR, "fastnoise/src/**"),
		path.join(LIBS_DIR, "upng/src/**"),
	}

	forceincludes { 
		path.join(ROOT_DIR, "Core/Memory.h"),
		path.join(ROOT_DIR, "Core/Allocator.h")
	}
	
	-- add include directories
	includedirs {
		path.join(ROOT_DIR, "src/ReCrafted.Engine"),
		path.join(LIBS_DIR, "mono/inc"),
		path.join(LIBS_DIR, "freetype/include"),
		path.join(LIBS_DIR, "fmod/inc"),
		path.join(LIBS_DIR, "base64/include"),
		path.join(LIBS_DIR, "json/include"),
		path.join(LIBS_DIR, "tsl/include"),
		path.join(LIBS_DIR, "sparsepp/include"),
		path.join(LIBS_DIR, "concurrentqueue/include"),
		path.join(LIBS_DIR, "ultralight/include"),
		path.join(LIBS_DIR, "tbb/include"),
		path.join(LIBS_DIR, "jemalloc/include"),
		path.join(LIBS_DIR, "msvc_compat/include"),
		path.join(LIBS_DIR, "lodepng/include"),
		path.join(LIBS_DIR, "fmt/include"),
		path.join(LIBS_DIR, "fastnoise/include"),
		path.join(LIBS_DIR, "upng/include"),
		path.join(LIBS_DIR, "directxtex/include"),
		path.join(LIBS_DIR, "physx/include"),
	}
	
	-- add onbuild script and multi processor compilation
	configuration { "vs*"}
		buildoptions { "/MP" }
		postbuildcommands {
			"call " .. ROOT_DIR .. "/pm.bat PostBuild",
		}
	
	links { "d3d11", "dxguid", "dxgi", "d3dcompiler", "Rpcrt4", "Ultralight", "UltralightCore", "mono-2.0-sgen", "fmod64_vc" }
		
	configuration { "Debug" }
		debugargs { "-debug" }
		defines { "DEBUG", "_ITERATOR_DEBUG_LEVEL=0" }
		runtime "Debug"
		symbols "On"
		links { "jemallocd", "tbb_debug", "DirectXTexd", "freetype28MTd", "PxFoundationDEBUG_x64", "PhysX3CommonDEBUG_x64", "SceneQueryDEBUG", "PhysX3DEBUG_x64", "PhysX3CookingDEBUG_x64", "PhysX3ExtensionsDEBUG", "PhysX3VehicleDEBUG", "PxPvdSDKDEBUG_x64" }

	configuration { "Development" }
		debugargs { "-debug" }
		defines { "NDEBUG", "DEVELOPMENT" }
		flags { "No64BitChecks", "NoBufferSecurityCheck" }
		runtime "Release"
		symbols "On"
		links { "jemalloc", "tbb", "DirectXTex", "freetype28MT", "PxFoundation_x64", "PhysX3Common_x64", "SceneQuery", "PhysX3_x64", "PhysX3Cooking_x64", "PhysX3Extensions", "PhysX3Vehicle", "PxPvdSDK_x64" }
		
	configuration { "Release" }
		debugargs { "-debug" }
		defines { "NDEBUG", "RELEASE" }
		flags { "OptimizeSpeed", "No64BitChecks", "NoBufferSecurityCheck" }
		runtime "Release"
		links { "jemalloc", "tbb", "DirectXTex", "freetype28MT", "PxFoundation_x64", "PhysX3Common_x64", "SceneQuery", "PhysX3_x64", "PhysX3Cooking_x64", "PhysX3Extensions", "PhysX3Vehicle", "PxPvdSDK_x64" }

	configuration { "x64" }
		-- set target dir
		targetdir (path.join(TARGET_DIR, "bin64/"))
		debugdir (path.join(TARGET_DIR, "bin64/"))
	
	configuration { "x64", "vs*"}
		-- add bgfx lib path - bgfx always contains all needed libs after being compiled
		libdirs {
			path.join(LIBS_DIR, "mono/lib"),
			path.join(LIBS_DIR, "freetype/lib"),
			path.join(LIBS_DIR, "fmod/lib"),
			path.join(LIBS_DIR, "ultralight/lib"),
			path.join(LIBS_DIR, "tbb/lib"),
			path.join(LIBS_DIR, "jemalloc/lib"),
			path.join(LIBS_DIR, "directxtex/lib"),
			path.join(LIBS_DIR, "physx/lib")
		}
		linkoptions { "/ignore:4099" }
	
	-- TODO: add linux and macosx
	
	configuration {}