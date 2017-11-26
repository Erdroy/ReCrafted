externalproject "ReCrafted"
   location (path.join(ROOT_DIR, "src/ReCrafted"))
   uuid "673BEC15-F118-413D-9A7F-889B6174CF0A"
   kind "SharedLib"
   language "C#"

externalproject "ReCrafted.API"
   location (path.join(ROOT_DIR, "src/ReCrafted.API"))
   uuid "9D637328-3340-4344-B0AD-710749C1AD08"
   kind "SharedLib"
   language "C#"
externalproject "ReCrafted.VoxelEditor"
   location (path.join(ROOT_DIR, "src/ReCrafted.VoxelEditor"))
   uuid "F42BA546-F379-4D44-B404-A096EF0FC363"
   kind "WindowedApp"
   language "C#"
   
--[[
externalproject "ReCrafted.APIBuilder"
   location (path.join(ROOT_DIR, "src/ReCrafted.APIBuilder"))
   uuid "B5D814D8-240E-41CD-B88E-E05916A67636"
   kind "ConsoleApp"
   language "C#"
externalproject "ReCrafted.Pdb2MdbConverter"
   location (path.join(ROOT_DIR, "src/ReCrafted.APIBuilder"))
   uuid "045864EB-723B-4C3D-BB84-182C9FF862BA"
   kind "ConsoleApp"
   language "C#"
   
externalproject "ReCrafted.ShaderCompiler"
   location (path.join(ROOT_DIR, "src/ReCrafted.ShaderCompiler"))
   uuid "B584DB24-317F-4EF6-B8DD-207306327F0B"
   kind "ConsoleApp"
   language "C#"
   
externalproject "ReCrafted.VersionBuilder"
   location (path.join(ROOT_DIR, "src/ReCrafted.VersionBuilder"))
   uuid "E27EB8A3-DCF1-44C5-AFA0-2055B243D7A5"
   kind "ConsoleApp"
   language "C#"
]]