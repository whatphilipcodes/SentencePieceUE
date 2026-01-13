@echo off
setlocal

REM --- Configuration ---
set "PLUGIN_ROOT=%~dp0..\..\.."
set "SOURCE_DIR=%PLUGIN_ROOT%\Source\ThirdParty\sentencepiece_src"
set "BUILD_DIR=%PLUGIN_ROOT%\Intermediate\ThirdParty\Build_Win64"
set "INSTALL_DIR=%PLUGIN_ROOT%\Source\ThirdParty\Lib\Win64"

REM --- Clean and Create Dirs ---
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
mkdir "%BUILD_DIR%"
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

REM --- CMake Generation ---
REM We force /MD (Multi-threaded DLL) to match Unreal's runtime.
REM We disable Shared libs to get a static .lib.
REM We enable builtin protobuf to avoid external dependencies.
cd "%BUILD_DIR%"
cmake "%SOURCE_DIR%" ^
    -G "Visual Studio 17 2022" ^
    -A x64 ^
    -DCMAKE_CXX_FLAGS_RELEASE="/MD /O2" ^
    -DCMAKE_CXX_FLAGS_DEBUG="/MDd /Zi" ^
    -DCMAKE_CONFIGURATION_TYPES="Release" ^
    -DSPM_ENABLE_SHARED=OFF ^
    -DSPM_USE_BUILTIN_PROTOBUF=ON ^
    -DSPM_ENABLE_TCMALLOC=OFF

REM --- Build ---
cmake --build . --config Release --parallel

REM --- Install/Copy ---
REM Copy the static lib to our plugin folder
copy "src\Release\sentencepiece.lib" "%INSTALL_DIR%\"
copy "src\Release\sentencepiece-train.lib" "%INSTALL_DIR%\"

echo.
echo Build Complete. Check %INSTALL_DIR%
pause