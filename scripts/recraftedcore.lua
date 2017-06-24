project "ReCraftedCore"
	kind "SharedLib"
	language "C#"
	location (OUTPUT_DIR)
	
	-- add source/header/shader files
	files {
		path.join(ROOT_DIR, "src/ReCraftedCore/**"),
	}
	configuration { "Debug*" }
		defines { "DEBUG" }
		flags   { "Symbols" }
	
	configuration { "Release*" }
		defines { "RELEASE" }
		
	configuration { "x64" }
		-- set target dir
		targetdir (path.join(TARGET_DIR, "bin64/"))
		debugdir (path.join(TARGET_DIR, "bin64/"))
		
	configuration { "x32" }
		-- set target dir
		targetdir (path.join(TARGET_DIR, "bin32/"))
		debugdir (path.join(TARGET_DIR, "bin32/"))