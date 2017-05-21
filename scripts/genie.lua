ROOT_DIR = path.getabsolute("../")
OUTPUT_DIR = path.join(ROOT_DIR, ".projects/", _ACTION)
TARGET_DIR = path.join(ROOT_DIR, "build/")
LIBS_DIR = path.join(ROOT_DIR, "libs/")

-- check deps


-- generate solution
solution "ReCrafted"
	configurations {
		"Debug",
		"Release"
	}
	
	-- define platforms
	if _ACTION == "xcode4" then
		platforms {
			"Universal",
		}
	else
		platforms {
			"x32",
			"x64",
			"Native",
		}
	end
	
	location (OUTPUT_DIR)
	
	startproject "ReCrafted"

dofile "recrafted.lua"