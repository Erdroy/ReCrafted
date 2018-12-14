# ReCrafted
![alt text](https://github.com/Erdroy/ReCrafted/raw/master/images/repoheader.png)

## Building
1. Clone ReCrafted repository into suitable directory: `X:/ReCrafted/` (should contain: licenses, src, tools etc.)
2. Clone ReCraftedBinaries repository into `build` subdirectory: `X:/ReCrafted/build/` (should contain: assets, bin64, mono etc.).
3. Run command `.\pm SetupProject --username "YOUR USERNAME"` 
4. Run command `.\pm Update3rdParty`, or mannualy download zip from https://erdroy.com/ReCrafted3rdParty.zip and unzip all files from downloaded archive - `ReCrafted3rdParty.zip` into `X:/ReCrafted/lib/` (should contain directories like ).
5. Run command `.\pm CreateProjects -t vs2015` to create needed projects for Visual Studio 2015.
6. Done! Now you can open `ReCrafted.sln` to start project solution.

**Warning**: Please DO NOT upgrade projects at first startup.
![alt text](https://github.com/Erdroy/ReCrafted/raw/master/images/donotupgrademeufreak.png)

**Warning**: If you pull newly created files for C++ project, then you must run command `.\pm CreateProjects -t vs2015` and reload projects (window will popup in VS).

*Note: Game supports only Windows x86_64.*

## Project Manager commands
To run PM commands, you need to open PS or CMD in the project's root directory `X:/ReCrafted`, the best way to do this, is hold Left-Shift and click RMB in Explorer and then click 'Open PowerShell/CMD window here'.

1. `CreateProjects` - Creates solution and C++ project that can be used in Visual Studio or other IDE if available. (No arguments)
2. `SetupProject` - Setups project configuration and developer information. Run with `--username USERNAME`, where USERNAME is your nickname or full name. Remember to **always** have the same username.
3. `GenerateAPI` - Generates C# API. (No arguments)
4. `Pack3rdParty` - Packages needed files from `lib` directory to `ReCrafted3rdParty.zip` (No arguments)
5. `PackGameFull` - Creates clean game archive from current build. Run with `-o FILENAME` or `--output FILENAME` to specify where game build will be outputed.
6. `PackGamePatch` - Creates game patch from current build. Run with `-o FILENAME` or `--output FILENAME` to specify where game patch will be outputed. 
7. `CompileShaders` - Compiles all shaders from `src/ReCrafted.Engine/Shaders/**.hlsl`. (No arguments) 
8. `UpdateBuildInfo` - Internal command. Increments build count. (No arguments)
9. `PostBuild` - Internal command. Invokes UpdateBuildInfo and Compile Shaders. (No arguments)

## External tools
- [VisualStudio 2015/2017](https://www.visualstudio.com/)

## Links
- [Trello](https://trello.com/b/xGap0YZQ/recrafted)

*ReCrafted (c) 2016-2018 Always Too Late*
