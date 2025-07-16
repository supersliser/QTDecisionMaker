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
    
    # Ensure fmt library is available
    # When using vcpkg, fmt should be automatically handled, but we can add explicit handling if needed
    if(TARGET fmt::fmt)
        get_target_property(FMT_LOCATION fmt::fmt IMPORTED_LOCATION_RELEASE)
        if(FMT_LOCATION)
            message(STATUS "FMT library found at: ${FMT_LOCATION}")
        endif()
    endif()
endif()