@echo off
setlocal

cd /d "%~dp0"

if not exist "RayTracing.exe" (
    echo RayTracing.exe was not found in this folder.
    echo Please build the project first.
    pause
    exit /b 1
)

if not exist "SDL3.dll" (
    echo SDL3.dll was not found in this folder.
    echo Keep SDL3.dll beside RayTracing.exe before running.
    pause
    exit /b 1
)

start "" "%~dp0RayTracing.exe"
