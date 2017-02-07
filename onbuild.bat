cd D:\ReCrafted

REM call clean.bat

set /P VAR= < buildNum.txt
set /A VAR += 1
echo %VAR% > buildNum.txt

set /P VER= < version.txt

tools\\ReCrafted.VersionBuilder.exe %VER% buildNum.txt assets\\gameinfo.json

SET ERRORLEVEL = 0
EXIT 0