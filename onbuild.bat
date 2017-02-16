cd %1%

echo Calculating build number...
set /P VAR= < buildNum_%USERNAME%.txt
set /A VAR += 1
echo %VAR% > buildNum_%USERNAME%.txt

echo Build number: %VAR%

set /P VER= < version.txt

echo Creating gameinfo file...
tools\\ReCrafted.VersionBuilder.exe %VER% build\\assets\\gameinfo.json

tools\\ReCrafted.ShaderCompiler2.exe %1%

echo Done!
SET ERRORLEVEL = 0
EXIT 0