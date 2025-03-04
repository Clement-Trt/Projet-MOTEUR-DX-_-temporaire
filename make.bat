Lancement du make

set DEBUG_DIR=%CD%\lyo-t2-atari-p7-07\ide\vs\MoteurDX\Build\Debug\
set RELEASE_DIR=%CD%\lyo-t2-atari-p7-07\ide\vs\MoteurDX\Build\Release\
set DLL_PATH=%CD%\lyo-t2-atari-p7-07\res\sfml\bin\openal32.dll
if not exist "%DLL_PATH%" (
    echo Le fichier openal32.dll est introuvable.
    pause
    exit /b
)

echo Copie de openal32.dll dans Debug...
copy "%DLL_PATH%" "%DEBUG_DIR%" /y

echo Copie de openal32.dll dans Release...
copy "%DLL_PATH%" "%RELEASE_DIR%" /y

SolutionGenerator.exe -make lyo-t2-atari-p7-07
pause