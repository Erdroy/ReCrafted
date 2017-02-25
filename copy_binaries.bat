RD /S /Q lib64\debug
RD /S /Q lib64\release

xcopy "src\bgfx\.build\win64_vs2015\bin\bgfxDebug.lib" "lib64\debug\bgfxDebug.lib*"
xcopy "src\bgfx\.build\win64_vs2015\bin\bxDebug.lib" "lib64\debug\bxDebug.lib*"

xcopy "src\bgfx\.build\win64_vs2015\bin\bgfxRelease.lib" "lib64\release\bgfxRelease.lib*"
xcopy "src\bgfx\.build\win64_vs2015\bin\bxRelease.lib" "lib64\release\bxRelease.lib*"