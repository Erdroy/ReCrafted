del /s /q libs_temp

robocopy libs libs_temp *.h *.hpp *.lib *.dll *.def *.pdb .git /s

Compress-Archive libs_temp libs_prebuilt.zip