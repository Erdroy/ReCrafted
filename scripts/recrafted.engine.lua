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
	defines { "_CRT_SECURE_NO_WARNINGS" }
	location (OUTPUT_DIR)
	targetname "ReCrafted"

	forceinclude { 
		"src/ReCrafted.Engine/Core/Memory.h" 
	}
	
	-- add include directories
	includedirs {
		path.join(ROOT_DIR, "src/ReCrafted.Engine"),
		path.join(LIBS_DIR, "mono/inc"),
		path.join(LIBS_DIR, "freetype/include"),
		path.join(LIBS_DIR, "fmod/inc"),
		path.join(LIBS_DIR, "rpmalloc/include"),
		path.join(LIBS_DIR, "base64/include"),
		path.join(LIBS_DIR, "json/include"),
		path.join(LIBS_DIR, "tsl/include"),
		path.join(LIBS_DIR, "sparsepp/include"),
		path.join(LIBS_DIR, "concurrentqueue/include"),
		path.join(LIBS_DIR, "ultralight/include"),
	}
	
	-- add source/header/shader files
	files {
		path.join(ROOT_DIR, "src/ReCrafted.Engine/**"),
	}
	
	-- add onbuild script and multi processor compilation
	configuration { "vs*"}
		buildoptions { "/MP" }
		postbuildcommands {
			"call " .. ROOT_DIR .. "/pm.bat PostBuild --skip-shaders --skip-api",
		}
	
	links { "d3d11", "dxguid", "dxgi", "d3dcompiler", "Rpcrt4", "Ultralight", "UltralightCore", "WebCore", "mono"}
		
	configuration { "Debug" }
		defines { "DEBUG", "_ITERATOR_DEBUG_LEVEL=0" }
		runtime "Debug"
		symbols "On"
		links { "rpmallocd", "freetype28MTd" }

	configuration { "Release" }
		defines { "NDEBUG" }
		flags { "OptimizeSpeed", "No64BitChecks", "NoBufferSecurityCheck" }
		runtime "Release"
		links { "rpmalloc", "freetype28MT" }

	configuration { "x64" }
		-- set target dir
		targetdir (path.join(TARGET_DIR, "bin64/"))
		debugdir (path.join(TARGET_DIR, "bin64/"))
	
	configuration { "x64", "vs*"}
		-- add bgfx lib path - bgfx always contains all needed libs after being compiled
		libdirs {
			path.join(LIBS_DIR, "mono/lib"),
			path.join(LIBS_DIR, "freetype/x64"),
			path.join(LIBS_DIR, "fmod/lib"),
			path.join(LIBS_DIR, "rpmalloc/lib"),
			path.join(LIBS_DIR, "ultralight/lib"),
		}
		linkoptions { "/ignore:4099" }
	
	-- TODO: add linux and macosx
	
	configuration {}