del /s /q libs_temp

robocopy libs libs_temp *.h *.lib *.dll *.def *.pdb /s

Compress-Archive libs_temp libs_prebuilt.zip