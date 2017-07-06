@echo off

cd %1%

set /P NAME= < username.txt

echo Calculating build number...
set /P VAR= < buildNum_%NAME%.txt
set /A VAR += 1
echo %VAR% > buildNum_%NAME%.txt

echo Build number: %VAR%

set /P VER= < version.txt

echo Creating gameinfo file...
tools\\ReCrafted.VersionBuilder.exe %VER% build\\assets\\gameinfo.json

RD /S /Q build\assets\shaders
tools\\ReCrafted.ShaderCompiler.exe %1%

echo Done!
SET ERRORLEVEL = 0
EXIT 0