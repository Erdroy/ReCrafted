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