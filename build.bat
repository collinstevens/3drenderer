@echo off

call clean.bat

call vars.bat

if not exist build\ mkdir build\

cl /Wall /I third_party\%sdl%\include /Fdbuild\ /Fobuild\ /FC /Zi src\main.c shell32.lib SDL2main.lib SDL2.lib /link /LIBPATH:third_party\%sdl%\lib\x64 /out:build\renderer.exe /SUBSYSTEM:CONSOLE

copy third_party\%sdl%\lib\x64\SDL2.dll build > nul