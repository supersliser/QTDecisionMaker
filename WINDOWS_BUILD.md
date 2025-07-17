# Building Windows Installer

This document describes how to build a Windows installer for QT Decision Maker.

## Prerequisites

1. **Windows 10/11** with Visual Studio 2019 or later
2. **CMake 3.16 or later**
3. **Git**
4. **vcpkg** (will be automatically cloned by build script)
5. **NSIS** (Nullsoft Scriptable Install System) for creating the installer

## Building the Installer

### Option 1: Using GitHub Actions (Recommended)

The project is configured with GitHub Actions that automatically build the Windows installer when code is pushed to the `production` branch. The installer will be available as an artifact in the Actions run.

### Option 2: Manual Build

1. **Clone the repository:**
   ```bash
   git clone https://github.com/supersliser/QTDecisionMaker.git
   cd QTDecisionMaker
   ```

2. **Set up vcpkg:**
   ```cmd
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   cd ..
   ```

3. **Install Qt6:**
   - Download and install Qt6 from https://www.qt.io/download
   - Or use vcpkg: `vcpkg install qtbase[widgets,sql]:x64-windows`

4. **Configure CMake:**
   ```cmd
   cmake -B build ^
     -DCMAKE_BUILD_TYPE=Release ^
     -DCMAKE_PREFIX_PATH=C:\Qt\6.5.0\msvc2022_64 ^
     -DCMAKE_TOOLCHAIN_FILE=vcpkg\scripts\buildsystems\vcpkg.cmake ^
     -DBUILD_TESTING=OFF
   ```

5. **Build the application:**
   ```cmd
   cmake --build build --config Release --target QTDecisionMaker
   ```

6. **Create the installer:**
   ```cmd
   cd build
   cpack -G NSIS -C Release
   ```

## Installer Features

The Windows installer includes:

- **Complete application bundle** with all necessary Qt and FMT dependencies
- **Desktop shortcut** for easy access
- **Start Menu entry** in the Programs folder
- **Uninstaller** accessible from Add/Remove Programs
- **Version information** embedded in the executable
- **Proper dependency management** ensuring the application runs on target systems

## Dependencies Included

- **Qt6 Libraries:** Core, Widgets, GUI, SQL components
- **FMT Library:** For formatted text output
- **Image Support Libraries:** libpng16.dll for PNG image handling
- **Text Rendering Libraries:** harfbuzz.dll and freetype.dll for advanced text rendering
- **Compression Libraries:** zlib1.dll, bz2.dll, and brotli libraries for various compression formats
- **Visual C++ Redistributables:** Automatically detected and included
- **Windows Runtime:** Required system libraries

All dependencies are automatically detected and bundled using vcpkg and the enhanced deployment script.

## Installer Configuration

The installer is configured using CPack with the NSIS generator. Key configuration files:

- `CMakeLists.txt` - Main build configuration with CPack settings
- `windows_deploy.cmake` - Windows-specific deployment logic
- `app.rc` - Windows resource file with version information
- `vcpkg.json` - Dependency management configuration

## Troubleshooting

### Common Issues

1. **Qt6 not found:**
   - Ensure Qt6 is properly installed
   - Set CMAKE_PREFIX_PATH to your Qt installation directory
   - Verify vcpkg toolchain is correctly specified

2. **FMT library missing:**
   - Ensure vcpkg is properly set up
   - Run `vcpkg install fmt:x64-windows` if needed

3. **Missing DLL errors (fmt.dll, libpng16.dll, harfbuzz.dll, freetype.dll):**
   - These are automatically handled by the enhanced deployment script
   - Ensure vcpkg dependencies are properly installed: `vcpkg install fmt libpng harfbuzz freetype:x64-windows`
   - The deployment script will automatically locate and bundle these DLLs

4. **NSIS not found:**
   - Download and install NSIS from https://nsis.sourceforge.io/
   - Add NSIS to your system PATH

5. **Installer creation fails:**
   - Check that all dependencies are properly bundled
   - Verify the build completed successfully
   - Review CPack output for error messages

## Customization

To customize the installer:

1. **Change installer appearance:** Modify CPACK_NSIS_* variables in CMakeLists.txt
2. **Add/remove shortcuts:** Update CPACK_NSIS_CREATE_ICONS_EXTRA
3. **Modify version info:** Edit app.rc resource file
4. **Include additional files:** Use additional install() commands

For more information, see the [CMake CPack documentation](https://cmake.org/cmake/help/latest/module/CPack.html).