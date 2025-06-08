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
REM build.bat – Bootstraps MSVC environment and builds Filament using Ninja

REM 1) Must run from any Command Prompt (no need to open VS developer prompt manually)
REM    Script locates Visual Studio using vswhere and sets environment.

REM 2) Locate vswhere.exe
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
  echo [Error] vswhere.exe not found at "%VSWHERE%"
  echo Install the Visual Studio Installer or update this path.
  pause
  exit /b 1
)
for /f "tokens=*" %%i in ('""%VSWHERE%" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath"') do set "VS_INSTALL=%%i"
if not defined VS_INSTALL (
  echo [Error] Could not detect Visual Studio installation path.
  pause
  exit /b 1
)

REM 3) Initialize MSVC x64 environment
call "%VS_INSTALL%\VC\Auxiliary\Build\vcvars64.bat"
if %ERRORLEVEL% neq 0 (
  echo [Error] Failed to initialize MSVC environment.
  pause
  exit /b %ERRORLEVEL%
)

REM 4) Clean previous build
cd /d "%~dp0"
if exist out rd /s /q out

REM 5) Create build directory and enter it
set BUILD_DIR=%~dp0out
mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%"

REM 6) Configure CMake to use MSVC + Ninja
cmake .. -G "Ninja" ^
  -DCMAKE_C_COMPILER=cl.exe ^
  -DCMAKE_CXX_COMPILER=cl.exe ^
  -DCMAKE_LINKER=link.exe ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DFILAMENT_BUILD_SAMPLES=ON ^
  -DFILAMENT_BUILD_APPS=ON ^
  -DFILAMENT_BUILD_TESTS=OFF
if %ERRORLEVEL% neq 0 (
  echo [Error] CMake configuration failed!
  pause
  exit /b %ERRORLEVEL%
)

REM 7) Build with Ninja
ninja -j %NUMBER_OF_PROCESSORS%
if %ERRORLEVEL% neq 0 (
  echo [Error] Build failed!
  pause
  exit /b %ERRORLEVEL%
)

echo [Success] Filament built successfully in "%BUILD_DIR%\\bin" and "%BUILD_DIR%\\lib"
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
