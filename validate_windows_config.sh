#!/bin/bash
# Simple validation script for Windows installer configuration

echo "=== CMake Configuration Validation ==="

# Check if CMakeLists.txt has basic syntax errors
echo "Checking CMakeLists.txt syntax..."
if grep -q "include(CPack)" CMakeLists.txt; then
    echo "✓ CPack included"
else
    echo "✗ CPack not included"
    exit 1
fi

if grep -q "CPACK_GENERATOR.*NSIS" CMakeLists.txt; then
    echo "✓ NSIS generator configured"
else
    echo "✗ NSIS generator not configured"
    exit 1
fi

if grep -q "qt_generate_deploy_app_script" CMakeLists.txt; then
    echo "✓ Qt deployment configured"
else
    echo "✗ Qt deployment not configured"
    exit 1
fi

# Check vcpkg.json
echo "Checking vcpkg.json..."
if grep -q "qtbase" vcpkg.json; then
    echo "✓ Qt dependency in vcpkg"
else
    echo "✗ Qt dependency missing in vcpkg"
    exit 1
fi

if grep -q "fmt" vcpkg.json; then
    echo "✓ FMT dependency in vcpkg"
else
    echo "✗ FMT dependency missing in vcpkg"
    exit 1
fi

# Check Windows resource file
echo "Checking Windows resource file..."
if [ -f "app.rc" ]; then
    echo "✓ Windows resource file exists"
    if grep -q "QT Decision Maker" app.rc; then
        echo "✓ Resource file has proper product name"
    else
        echo "✗ Resource file missing product name"
        exit 1
    fi
else
    echo "✗ Windows resource file missing"
    exit 1
fi

# Check GitHub Actions workflow
echo "Checking GitHub Actions workflow..."
if grep -q "cpack -G NSIS" .github/workflows/cmake-windows.yml; then
    echo "✓ Installer build step in workflow"
else
    echo "✗ Installer build step missing in workflow"
    exit 1
fi

if grep -q "windows-installer" .github/workflows/cmake-windows.yml; then
    echo "✓ Installer artifact upload configured"
else
    echo "✗ Installer artifact upload missing"
    exit 1
fi

echo ""
echo "=== Validation Complete ==="
echo "All checks passed! Windows installer configuration looks good."