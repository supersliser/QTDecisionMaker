# Windows deployment configuration for QTDecisionMaker
# This script helps ensure all necessary dependencies are included in the Windows installer

if(WIN32)
    # Set up windeployqt for Qt deployment
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS ${Qt6_DIR}/../../../bin)
    
    if(WINDEPLOYQT_EXECUTABLE)
        message(STATUS "Found windeployqt: ${WINDEPLOYQT_EXECUTABLE}")
        
        # Custom target to run windeployqt
        add_custom_target(deploy_qt
            COMMAND ${WINDEPLOYQT_EXECUTABLE} --verbose 2 --release --no-translations --no-system-d3d-compiler --no-opengl-sw $<TARGET_FILE:QTDecisionMaker>
            DEPENDS QTDecisionMaker
            COMMENT "Deploying Qt libraries for QTDecisionMaker"
        )
        
        # Install Qt dependencies
        install(CODE "
            execute_process(
                COMMAND ${WINDEPLOYQT_EXECUTABLE} --verbose 2 --release --no-translations --no-system-d3d-compiler --no-opengl-sw \${CMAKE_INSTALL_PREFIX}/bin/QTDecisionMaker.exe
                WORKING_DIRECTORY \${CMAKE_INSTALL_PREFIX}
            )
        " COMPONENT Runtime)
    else()
        message(WARNING "windeployqt not found. Qt dependencies may not be properly deployed.")
    endif()
    
    # Function to find and install vcpkg DLLs
    function(install_vcpkg_dll DLL_NAME)
        if(CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg")
            # Extract vcpkg root from toolchain file path
            get_filename_component(VCPKG_ROOT "${CMAKE_TOOLCHAIN_FILE}" DIRECTORY)
            get_filename_component(VCPKG_ROOT "${VCPKG_ROOT}" DIRECTORY)
            get_filename_component(VCPKG_ROOT "${VCPKG_ROOT}" DIRECTORY)
            
            # Determine architecture-specific path
            if(CMAKE_SIZEOF_VOID_P EQUAL 8)
                set(VCPKG_ARCH "x64-windows")
            else()
                set(VCPKG_ARCH "x86-windows")
            endif()
            
            # Look for the DLL in vcpkg installed directory
            set(VCPKG_BIN_DIR "${VCPKG_ROOT}/installed/${VCPKG_ARCH}/bin")
            find_file(${DLL_NAME}_PATH 
                NAMES ${DLL_NAME}
                PATHS ${VCPKG_BIN_DIR}
                NO_DEFAULT_PATH
            )
            
            if(${DLL_NAME}_PATH)
                message(STATUS "Found ${DLL_NAME} at: ${${DLL_NAME}_PATH}")
                install(FILES ${${DLL_NAME}_PATH} 
                    DESTINATION ${CMAKE_INSTALL_BINDIR} 
                    COMPONENT Runtime)
            else()
                message(WARNING "${DLL_NAME} not found in vcpkg installation at ${VCPKG_BIN_DIR}")
            endif()
        endif()
    endfunction()
    
    # Install specific DLLs that are commonly missing
    install_vcpkg_dll("fmt.dll")
    install_vcpkg_dll("fmtd.dll")  # Debug version
    install_vcpkg_dll("libpng16.dll")
    install_vcpkg_dll("libpng16d.dll")  # Debug version
    install_vcpkg_dll("harfbuzz.dll")
    install_vcpkg_dll("harfbuzzd.dll")  # Debug version
    install_vcpkg_dll("freetype.dll")
    install_vcpkg_dll("freetyped.dll")  # Debug version
    install_vcpkg_dll("zlib1.dll")  # Often needed by libpng
    install_vcpkg_dll("zlibd1.dll")  # Debug version
    install_vcpkg_dll("bz2.dll")  # Sometimes needed by freetype
    install_vcpkg_dll("bz2d.dll")  # Debug version
    
    # Install additional common dependencies that might be needed
    install_vcpkg_dll("brotlicommon.dll")
    install_vcpkg_dll("brotlidec.dll")
    install_vcpkg_dll("brotlienc.dll")
    
    # Also install runtime libraries by searching in common paths
    install(CODE "
        # Find and install all vcpkg DLLs that our executable depends on
        if(CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES \"vcpkg\")
            get_filename_component(VCPKG_ROOT \"${CMAKE_TOOLCHAIN_FILE}\" DIRECTORY)
            get_filename_component(VCPKG_ROOT \"\${VCPKG_ROOT}\" DIRECTORY)
            get_filename_component(VCPKG_ROOT \"\${VCPKG_ROOT}\" DIRECTORY)
            
            if(CMAKE_SIZEOF_VOID_P EQUAL 8)
                set(VCPKG_ARCH \"x64-windows\")
            else()
                set(VCPKG_ARCH \"x86-windows\")
            endif()
            
            set(VCPKG_BIN_DIR \"\${VCPKG_ROOT}/installed/\${VCPKG_ARCH}/bin\")
            
            # Copy all DLLs from vcpkg that might be needed
            file(GLOB VCPKG_DLLS \"\${VCPKG_BIN_DIR}/*.dll\")
            foreach(DLL_FILE \${VCPKG_DLLS})
                get_filename_component(DLL_NAME \${DLL_FILE} NAME)
                # Only copy DLLs we know might be needed
                if(DLL_NAME MATCHES \"^(fmt|libpng|harfbuzz|freetype|zlib|bz2|brotli).*\\.dll$\")
                    message(STATUS \"Installing vcpkg DLL: \${DLL_NAME}\")
                    file(COPY \${DLL_FILE} DESTINATION \${CMAKE_INSTALL_PREFIX}/bin)
                endif()
            endforeach()
        endif()
    " COMPONENT Runtime)
    
    # Ensure fmt library is available
    if(TARGET fmt::fmt)
        get_target_property(FMT_LOCATION fmt::fmt IMPORTED_LOCATION_RELEASE)
        if(FMT_LOCATION)
            message(STATUS "FMT library found at: ${FMT_LOCATION}")
        endif()
    endif()
endif()