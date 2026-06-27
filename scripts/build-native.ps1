<#
.SYNOPSIS
    Configure and build the native desktop executable (Windows .exe).
.DESCRIPTION
    Wraps the CMake configure + build steps for the native target. Produces
    build\VisitorGameEngine.exe — the stdin game-select menu.

    Requires CMake and a C++17 toolchain. SDL2 + SDL2_image must be discoverable
    by CMake; on Windows the simplest path is vcpkg (see docs/PACKAGING.md).
.PARAMETER Backend
    SDL2 (default) or SFML.
.PARAMETER Toolchain
    Optional path to a CMake toolchain file (e.g. the vcpkg toolchain).
.EXAMPLE
    ./scripts/build-native.ps1
.EXAMPLE
    ./scripts/build-native.ps1 -Toolchain C:\vcpkg\scripts\buildsystems\vcpkg.cmake
#>
param(
    [ValidateSet("SDL2", "SFML")]
    [string]$Backend = "SDL2",
    [string]$Toolchain = ""
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot

$cmakeArgs = @("-B", "build", "-S", $root, "-DBACKEND=$Backend")
if ($Toolchain) { $cmakeArgs += "-DCMAKE_TOOLCHAIN_FILE=$Toolchain" }

Write-Host "Configuring ($Backend)..." -ForegroundColor Cyan
cmake @cmakeArgs

Write-Host "Building..." -ForegroundColor Cyan
cmake --build build --config Release -j 4

Write-Host "`nDone. Run the game menu with:" -ForegroundColor Green
Write-Host "  ./build/VisitorGameEngine        (or build/Release/VisitorGameEngine.exe on MSVC)"
