project "ReCrafted.Browser"
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
	targetname "ReCrafted.Browser"

	-- add include directories
	includedirs {
		path.join(ROOT_DIR, "src/ReCrafted.Browser"),
		path.join(LIBS_DIR, "cef/include"),
		path.join(LIBS_DIR, "cef"),
	}
	
	-- add source/header/shader files
	files {
		path.join(ROOT_DIR, "src/ReCrafted.Browser/**"),
	}
	
	-- add onbuild script and multi processor compilation
	configuration { "vs*"}
		buildoptions { "/MP" }
		postbuildcommands {
			"call " .. ROOT_DIR .. "/onbuild.bat " .. ROOT_DIR .. "/ Engine",
		}
	
	links { "libcef" }
		
	configuration { "Debug" }
		defines { "DEBUG", "_ITERATOR_DEBUG_LEVEL=0" }
		runtime "Debug"
		symbols "On"
		links { "libcef_dll_wrapper_debug" }

	configuration { "Release" }
		defines { "NDEBUG" }
		flags { "OptimizeSpeed", "No64BitChecks", "NoBufferSecurityCheck" }
		runtime "Release"
		links { "libcef_dll_wrapper" }

	configuration { "x64" }
		-- set target dir
		targetdir (path.join(TARGET_DIR, "bin64/"))
		debugdir (path.join(TARGET_DIR, "bin64/"))
		
	configuration { "x64", "vs*"}
		-- add bgfx lib path - bgfx always contains all needed libs after being compiled
		libdirs { 
			path.join(LIBS_DIR, "cef/lib"),
		}
		linkoptions { "/ignore:4099" }
	
	-- TODO: add linux and macosx
	
	configuration {}