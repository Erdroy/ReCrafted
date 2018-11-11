ROOT_DIR = path.getabsolute(".")
OUTPUT_DIR = ROOT_DIR
TARGET_DIR = path.join(ROOT_DIR, "build/")
LIBS_DIR = path.join(ROOT_DIR, "lib/")

-- generate solution
solution "ReCrafted"
	configurations {
		"Debug",
		"Development",
		"Release"
	}
	
	-- define platforms
	if _ACTION == "xcode4" then
		platforms {
			"Universal",
		}
	else
		platforms {
			--"x32",
			"x64"
		}
	end
	
	location (OUTPUT_DIR)
	
	startproject "ReCrafted"

dofile "scripts/recrafted.engine.lua"
dofile "scripts/external.lua"