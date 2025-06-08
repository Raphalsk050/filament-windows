@echo off
REM build.bat – Configures and builds Filament using MSVC cl.exe + Ninja

REM 1) Deve ser executado no “x64 Native Tools Command Prompt for VS”
if not defined VCToolsInstallDir (
  echo [Error] Run this script from the x64 Native Tools Command Prompt.
  pause
  exit /b 1
)

REM 2) Limpa build anterior
cd /d "%~dp0"
if exist out rd /s /q out

REM 3) Prepara diretório e PATH
set SCRIPT_DIR=%~dp0
set BUILD_DIR=%SCRIPT_DIR%out
mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%"

REM Garante que o cl.exe do MSVC venha primeiro no PATH
set PATH=%VCToolsInstallDir%bin\Hostx64\x64;%PATH%

REM 4) Chama o CMake com MSVC + Ninja
cmake .. -G "Ninja" ^
  -DCMAKE_C_COMPILER=cl.exe ^
  -DCMAKE_CXX_COMPILER=cl.exe ^
  -DCMAKE_LINKER=link.exe ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DFILAMENT_BUILD_SAMPLES=ON ^
  -DFILAMENT_BUILD_APPS=ON ^
  -DFILAMENT_BUILD_TESTS=OFF
if %ERRORLEVEL% neq 0 (
  echo.
  echo [Error] CMake configuration failed!
  pause
  exit /b %ERRORLEVEL%
)

REM 5) Compila
ninja -j %NUMBER_OF_PROCESSORS%
if %ERRORLEVEL% neq 0 (
  echo.
  echo [Error] Build failed!
  pause
  exit /b %ERRORLEVEL%
)

echo.
echo [Success] Filament built in "%BUILD_DIR%\bin" and "%BUILD_DIR%\lib".
pause