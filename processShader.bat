@echo off
echo processing shader
tools\\ShaderConverter.exe %1
tools\\xcs.exe -E %2 -T %3