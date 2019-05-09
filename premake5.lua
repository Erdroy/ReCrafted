ROOT_DIR = path.getabsolute(".")

OUTPUT_DIR = path.join(ROOT_DIR, "Projects/")
TARGET_DIR = path.join(ROOT_DIR, "Build/")

SOURCE_DIR = path.join(ROOT_DIR, "Source/")
LIBRARIES_DIR = path.join(SOURCE_DIR, "Libraries/")

-- generate solution
solution "ReCrafted"
	location (SOURCE_DIR)
	startproject "ReCrafted"

	configurations {
		"Debug",
		"Development",
		"Release",
	}
	
	platforms {
		"x64"
	}
	
	-- Project scripts
	group "Libraries"
		dofile "./Source/Libraries/Project.concurrentqueue.lua"
		dofile "./Source/Libraries/Project.cpp-base64.lua"
		dofile "./Source/Libraries/Project.fmt.lua"
		dofile "./Source/Libraries/Project.json.lua"
		dofile "./Source/Libraries/Project.sparsepp.lua"

	group "Engine"
		dofile "./Source/Engine/ReCrafted.API/Project.lua"
		dofile "./Source/Engine/ReCrafted.Engine/Project.lua"

	group "Game"
		dofile "./Source/Game/ReCrafted/Project.lua"
		dofile "./Source/Game/ReCrafted.Game/Project.lua"

	group "Tools"
		dofile "./Source/Tools/ReCrafted.VSDebugger/Project.lua"
		dofile "./Source/Tools/ReCrafted.Tools.Common/Project.lua"
		dofile "./Source/Tools/ReCrafted.Tools.APIGenerator/Project.lua"
		dofile "./Source/Tools/ReCrafted.Tools.ShaderCompiler/Project.lua"
		dofile "./Source/Tools/ReCrafted.Tools.ProjectManager/Project.lua"
		dofile "./Source/Tools/ReCrafted.Tools.LocaleConverter/Project.lua"
		dofile "./Source/Tools/ReCrafted.Tools.AtlasPacker/Project.lua"

-- generate C# Game/API only solution
solution "ReCrafted.Game"
	location (SOURCE_DIR)
	startproject "ReCrafted.Game"

	configurations {
		"Debug",
		"Development",
		"Release",
	}
	
	group "Engine"
		dofile "./Source/Engine/ReCrafted.API/Project.lua"
	
	group "Game"
		dofile "./Source/Game/ReCrafted.Game/Project.lua"
	