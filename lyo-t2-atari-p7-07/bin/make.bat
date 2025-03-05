@echo off

echo Lancement du make

SolutionGenerator.exe -make ..\

setlocal

set Build_DIR=%CD%\..\ide\vs\MoteurDX\Build\

mkdir "%Build_DIR%\Debug"
mkdir "%Build_DIR%\Release"

set DEBUG_DIR=%CD%\..\ide\vs\MoteurDX\Build\Debug\
set RELEASE_DIR=%CD%\..\ide\vs\MoteurDX\Build\Release\
set DLL_PATH=%CD%\..\res\sfml\bin\openal32.dll
if not exist "%DLL_PATH%" (
    echo Le fichier openal32.dll est introuvable.
    pause
    exit /b
)

echo Copie de openal32.dll dans Debug...
copy "%DLL_PATH%" "%DEBUG_DIR%" /y

echo Copie de openal32.dll dans Release...
copy "%DLL_PATH%" "%RELEASE_DIR%" /y

echo Make Completed !
pause