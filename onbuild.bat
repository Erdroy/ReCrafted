@echo off

cd %1%

set /P NAME= < username.txt

echo Calculating build number...
set /P VAR= < project_meta/buildNum_%2-%NAME%.txt
set /A VAR += 1
echo %VAR% > project_meta/buildNum_%2-%NAME%.txt

echo %2% Build number: %VAR%

set /P VER= < version.txt

REM update game info file
echo Creating gameinfo file...
tools\\ReCrafted.VersionBuilder.exe "%VER%" build\\assets\\gameinfo.json

IF "%2%" == "Engine" (
REM run shader compiler
REM RD /S /Q build\assets\shaders
REM tools\\ReCrafted.ShaderCompiler.exe %1%
)

IF "%2%" == "Game" (
REM convert pdbs into mdbs

IF EXIST build/bin64 (
echo Converting 64bit PDB's into MDB's
echo Converting bin64/ReCrafted.Game.pdb into bin64/ReCrafted.Game.mdb
tools\\ReCrafted.Pdb2MdbConverter.exe build/bin64/ReCrafted.Game.dll

echo Converting bin64/ReCrafted.API.pdb into bin64/ReCrafted.API.mdb
tools\\ReCrafted.Pdb2MdbConverter.exe build/bin64/ReCrafted.API.dll
goto pdbexit
) ELSE (
goto pdbexit
)

:pdbexit
echo Converted all PDB's
goto exit
)

:exit
REM done!
echo Done!
SET ERRORLEVEL = 0
EXIT 0