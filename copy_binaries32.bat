RD /S /Q lib\debug
RD /S /Q lib\release

xcopy "src\bgfx\.build\win32_vs2015\bin\bgfxDebug.lib" "lib\debug\bgfxDebug.lib*"
xcopy "src\bgfx\.build\win32_vs2015\bin\bxDebug.lib" "lib\debug\bxDebug.lib*"

xcopy "src\bgfx\.build\win32_vs2015\bin\bgfxRelease.lib" "lib\release\bgfxRelease.lib*"
xcopy "src\bgfx\.build\win32_vs2015\bin\bxRelease.lib" "lib\release\bxRelease.lib*"

xcopy "src\bgfx\.build\win32_vs2015\bin\bgfxDebug.pdb" "lib\debug\bgfxDebug.pdb*"
xcopy "src\bgfx\.build\win32_vs2015\bin\bxDebug.pdb" "lib\debug\bxDebug.pdb*"

xcopy "src\bgfx\.build\win32_vs2015\bin\bgfxRelease.pdb" "lib\release\bgfxRelease.pdb*"
xcopy "src\bgfx\.build\win32_vs2015\bin\bxRelease.pdb" "lib\release\bxRelease.pdb*"