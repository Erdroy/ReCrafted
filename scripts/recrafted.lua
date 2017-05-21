project "ReCrafted"
	kind "WindowedApp"
	language "C++"
	flags {
		"WinMain", 
		"Unicode", 
		"NoImportLib",
		"NativeWChar",
		"StaticRuntime",
		"NoEditAndContinue", 
		"NoPCH",
		"NoRTTI",
	}
	defines { "_CRT_SECURE_NO_WARNINGS" }
	location (OUTPUT_DIR)

	-- add include directories
	includedirs {
		path.join(LIBS_DIR, "bx/include"),
		path.join(LIBS_DIR, "bimg/include"),
		path.join(LIBS_DIR, "bgfx/include"),
		path.join(LIBS_DIR, "json"),
	}
	
	-- add source/header/shader files
	files {
		path.join(ROOT_DIR, "src/ReCrafted/**"),
	}
	
	-- add onbuild script
	postbuildcommands {
		"call " .. ROOT_DIR .. "/onbuild.bat",
	}
	
	configuration { "Debug" }
		defines { "DEBUG", "_ITERATOR_DEBUG_LEVEL=0" }

	configuration { "Release" }
		flags { "OptimizeSpeed", "No64BitChecks", "NoBufferSecurityCheck" }

	configuration { "x64" }
		-- set target dir
		targetdir (path.join(TARGET_DIR, "bin64/"))
		debugdir (path.join(TARGET_DIR, "bin64/"))
		
	configuration { "x32" }
		-- set target dir
		targetdir (path.join(TARGET_DIR, "bin32/"))
		debugdir (path.join(TARGET_DIR, "bin32/"))
	
	configuration { "x64", "vs*"}
		-- add bgfx lib path - bgfx always contains all needed libs after being compiled
		libdirs { (path.join(LIBS_DIR, "bgfx/.build/win64_" .. _ACTION .. "/bin/")) }
	
	configuration { "x32", "vs*"}
		-- add bgfx lib path - bgfx always contains all needed libs after being compiled
		libdirs { (path.join(LIBS_DIR, "bgfx/.build/win32_" .. _ACTION .. "/bin/")) }
	
	-- TODO: add linux and macosx
	
	configuration {}