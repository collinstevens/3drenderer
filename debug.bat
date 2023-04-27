@echo off

if not exist build\renderer.exe echo Program doesn't exist

if exist build\renderer.exe call devenv build\renderer.exe