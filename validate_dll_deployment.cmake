# CMake script to validate DLL deployment for Windows installer
# This script can be run after building to check if all required DLLs are present

if(WIN32)
    message(STATUS "Validating Windows DLL deployment...")
    
    # List of required DLLs that should be present in the installation
    set(REQUIRED_DLLS
        "fmt.dll"
        "libpng16.dll" 
        "harfbuzz.dll"
        "freetype.dll"
    )
    
    # Additional DLLs that might be needed
    set(OPTIONAL_DLLS
        "zlib1.dll"
        "bz2.dll"
        "brotlicommon.dll"
        "brotlidec.dll"
        "brotlienc.dll"
    )
    
    # Check if CMAKE_INSTALL_PREFIX is set
    if(NOT CMAKE_INSTALL_PREFIX)
        set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")
    endif()
    
    set(BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
    
    message(STATUS "Checking for DLLs in: ${BIN_DIR}")
    
    # Check for required DLLs
    set(MISSING_DLLS "")
    foreach(DLL ${REQUIRED_DLLS})
        set(DLL_PATH "${BIN_DIR}/${DLL}")
        if(EXISTS "${DLL_PATH}")
            message(STATUS "✓ Found required DLL: ${DLL}")
        else()
            message(WARNING "✗ Missing required DLL: ${DLL}")
            list(APPEND MISSING_DLLS ${DLL})
        endif()
    endforeach()
    
    # Check for optional DLLs
    foreach(DLL ${OPTIONAL_DLLS})
        set(DLL_PATH "${BIN_DIR}/${DLL}")
        if(EXISTS "${DLL_PATH}")
            message(STATUS "✓ Found optional DLL: ${DLL}")
        else()
            message(STATUS "- Optional DLL not found: ${DLL}")
        endif()
    endforeach()
    
    # Check for Qt DLLs (these should be handled by windeployqt)
    set(QT_DLLS
        "Qt6Core.dll"
        "Qt6Gui.dll"
        "Qt6Widgets.dll"
        "Qt6Sql.dll"
    )
    
    foreach(DLL ${QT_DLLS})
        set(DLL_PATH "${BIN_DIR}/${DLL}")
        if(EXISTS "${DLL_PATH}")
            message(STATUS "✓ Found Qt DLL: ${DLL}")
        else()
            message(WARNING "✗ Missing Qt DLL: ${DLL} (should be handled by windeployqt)")
        endif()
    endforeach()
    
    # List all DLLs actually present
    file(GLOB ALL_DLLS "${BIN_DIR}/*.dll")
    if(ALL_DLLS)
        message(STATUS "All DLLs found in bin directory:")
        foreach(DLL_PATH ${ALL_DLLS})
            get_filename_component(DLL_NAME ${DLL_PATH} NAME)
            message(STATUS "  - ${DLL_NAME}")
        endforeach()
    else()
        message(WARNING "No DLLs found in ${BIN_DIR}")
    endif()
    
    # Summary
    if(MISSING_DLLS)
        list(LENGTH MISSING_DLLS NUM_MISSING)
        message(STATUS "❌ Deployment validation FAILED: ${NUM_MISSING} required DLL(s) missing")
        foreach(DLL ${MISSING_DLLS})
            message(STATUS "  Missing: ${DLL}")
        endforeach()
        message(FATAL_ERROR "Required DLLs are missing from the deployment")
    else()
        message(STATUS "✅ Deployment validation PASSED: All required DLLs are present")
    endif()
    
else()
    message(STATUS "DLL validation is only relevant for Windows builds")
endif()