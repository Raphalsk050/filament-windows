# Filament Build Guide (Windows)

This README explains how to set up all necessary dependencies and compile **Google Filament** on Windows using MSVC (Visual Studio) and Ninja, including an automated `build.bat` script.

---

## Prerequisites

Before building Filament, install and configure:

1. **Visual Studio 2019 (16.x)** or **Visual Studio 2022**

   * Include the **"Desktop development with C++"** workload.
   * Ensure the **Windows SDK** is installed.

2. **Git**

   ```bash
   git --version
   ```

3. **CMake ≥ 3.19**

   ```bash
   cmake --version
   ```

4. **Ninja ≥ 1.10**

   ```bash
   ninja --version
   ```

5. **Python 3** (in your `PATH`)

   ```bash
   python --version
   ```

---

## Cloning the Repository

Open a regular PowerShell or CMD and run:

```bash
git clone https://github.com/google/filament.git
cd filament
```

---

## Automated Build Script: `build.bat`

A provided batch script automates:

* Initializing the MSVC environment. (search for x64 Native Tools Command Prompt for VS 2022 in your windows search)
* Cleaning previous builds.
* Configuring CMake to use MSVC + Ninja.
* Building with Ninja.

Save the following as `build.bat` in the **root** of the cloned `filament` folder:

```bat
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
```

### Usage

From a **standard** Windows Command Prompt or PowerShell (no special VS prompt), simply run:

```bat
.\"build.bat"
```

The script will:

1. Detect and initialize the MSVC x64 environment.
2. Clean any previous `out/` directory.
3. Configure and generate Ninja build files.
4. Compile Filament.
5. Pause at the end, showing success or errors.

---

## Manual Build Steps (Optional)

If you prefer manual control, use these commands:

```bash
# 1. Open x64 Native Tools Command Prompt for VS
# 2. Navigate to repository root
mkdir out && cd out
cmake .. -G "Ninja" \
  -DCMAKE_BUILD_TYPE=Release \
  -DFILAMENT_BUILD_SAMPLES=ON \
  -DFILAMENT_BUILD_APPS=ON \
  -DFILAMENT_BUILD_TESTS=OFF
ninja -j %NUMBER_OF_PROCESSORS%
```

Or to generate a Visual Studio solution:

```bash
mkdir vs_build && cd vs_build
cmake .. -G "Visual Studio 16 2019" -A x64 \
  -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

---

## Notes

* Ensure **vswhere.exe** exists at the Installer path; it ships with Visual Studio.
* The batch script removes the need to manually open a developer prompt.
* Clang on Windows is not supported by Filament; only MSVC is supported.

Happy rendering! 🎉
