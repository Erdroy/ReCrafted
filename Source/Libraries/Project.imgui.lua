project "imgui"
	location (LIBRARIES_DIR)
	targetname "imgui_static"
	kind "StaticLib"
	language "C++"
	
	-- add source/header/shader files
	files {
		"./imgui/imgui_draw.cpp",
		"./imgui/imgui.cpp",
		"./imgui/imgui_widgets.cpp",
		"./imgui/imgui.h",
		"./imgui/imgui_internal.h",
		"./imgui/imstb_rectpack.h",
		"./imgui/imstb_textedit.h",
		"./imgui/imstb_truetype.h",
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