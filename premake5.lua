ROOT_DIR = path.getabsolute(".")

OUTPUT_DIR = path.join(ROOT_DIR, "Projects/")
TARGET_DIR = path.join(ROOT_DIR, "Build/")

SOURCE_DIR = path.join(ROOT_DIR, "Source/")
LIBS_DIR = path.join(SOURCE_DIR, "Libraries/")

-- generate solution
solution "ReCrafted"
	location (SOURCE_DIR)
	startproject "ReCrafted"

	configurations {
		"Debug",
		"Development",
		"Release",
	}
	
	-- define platforms
	if _ACTION == "xcode4" then
		platforms {
			"Universal",
		}
	else
		platforms {
			"x64"
		}
	end
    
	-- add onbuild script and multi processor compilation
	configuration { "vs*"}
		buildoptions { "/MP" }
		postbuildcommands {
			"call " .. ROOT_DIR .. "/pm.bat PostBuild",
		}

-- Project scripts
group "Engine"
dofile "./Source/Engine/ReCrafted.Common/Project.lua"
dofile "./Source/Engine/ReCrafted.Core/Project.lua"
dofile "./Source/Engine/ReCrafted.Graphics/Project.lua"

group "Game"
dofile "./Source/Game/ReCrafted.Game/Project.lua"
dofile "./Source/Game/ReCrafted/Project.lua"
dofile "./Source/Game/ReCraftedHeadless/Project.lua"

group "Tools"
dofile "./Source/Tools/ReCrafted.ProjectManager/Project.lua"
dofile "./Source/Tools/ReCrafted.LocaleConverter/Project.lua"
dofile "./Source/Tools/ReCrafted.ShaderCompiler/Project.lua"
dofile "./Source/Tools/ReCrafted.AtlasPacker/Project.lua"
