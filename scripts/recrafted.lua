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

	-- add include directories
	includedirs {
		path.join(ROOT_DIR, "src/ReCrafted.Engine"),
		path.join(LIBS_DIR, "bx/include"),
		path.join(LIBS_DIR, "bimg/include"),
		path.join(LIBS_DIR, "bgfx/include"),
		path.join(LIBS_DIR, "json"),
		path.join(LIBS_DIR, "mono/inc"),
		path.join(LIBS_DIR, "freetype/include"),
		path.join(LIBS_DIR, "fmod/include"),
	}
	
	-- add source/header/shader files
	files {
		path.join(ROOT_DIR, "src/ReCrafted.Engine/**"),
	}
	
	-- add onbuild script and multi processor compilation
	configuration { "vs*"}
		buildoptions { "/MP" }
		postbuildcommands {
			"call " .. ROOT_DIR .. "/onbuild.bat " .. ROOT_DIR .. "/ Engine",
		}
	
	configuration { "Debug" }
		defines { "DEBUG", "_ITERATOR_DEBUG_LEVEL=0" }
		runtime "Debug"
		symbols "On"

	configuration { "Release" }
		flags { "OptimizeSpeed", "No64BitChecks", "NoBufferSecurityCheck" }
		runtime "Release"

	configuration { "x64" }
		-- set target dir
		targetdir (path.join(TARGET_DIR, "bin64/"))
		debugdir (path.join(TARGET_DIR, "bin64/"))
		
	--configuration { "x32" }
	--	-- set target dir
	--	targetdir (path.join(TARGET_DIR, "bin32/"))
	--	debugdir (path.join(TARGET_DIR, "bin32/"))
	
	configuration { "x64", "vs*"}
		-- add bgfx lib path - bgfx always contains all needed libs after being compiled
		libdirs { 
			path.join(LIBS_DIR, "bgfx/.build/win64_" .. _ACTION .. "/bin/"),
			path.join(LIBS_DIR, "mono/lib"),
			path.join(LIBS_DIR, "freetype/x64"),
			path.join(LIBS_DIR, "fmod/lib"),
		}
		linkoptions { "/ignore:4099" }
	
	--configuration { "x32", "vs*"}
	--	-- add bgfx lib path - bgfx always contains all needed libs after being compiled
	--	libdirs { 
	--		path.join(LIBS_DIR, "bgfx/.build/win32_" .. _ACTION .. "/bin/"),
	--		path.join(LIBS_DIR, "mono/lib"),
	--		path.join(LIBS_DIR, "freetype/win32"),
	--		path.join(LIBS_DIR, "fmod/lib"),
	--	}
	--	linkoptions { "/ignore:4099" }
	
	configuration { "vs*", "Debug"}
		libdirs { 
			path.join(LIBS_DIR, "freetype/debug"),
		}
	
	-- TODO: add linux and macosx
	
	configuration {}