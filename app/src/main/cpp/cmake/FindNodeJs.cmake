find_program(NODEJS_EXECUTABLE node)
mark_as_advanced(NODEJS_EXECUTABLE)

if(NODEJS_EXECUTABLE)
    execute_process(COMMAND ${NODEJS_EXECUTABLE} --version
            OUTPUT_VARIABLE NodeJs_VERSION
            ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(NodeJs_VERSION MATCHES "^v[0-9]")
        string(SUBSTRING ${NodeJs_VERSION} 1 -1 NodeJs_VERSION)
        message("NodeJs_VERSION" ${NodeJs_VERSION})
    else()
        unset(NodeJs_VERSION)
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NodeJs
        REQUIRED_VARS NODEJS_EXECUTABLE
        VERSION_VAR NodeJs_VERSION)

if(NOT TARGET NodeJs::NodeJs)
    add_executable(NodeJs::NodeJs IMPORTED)
    set_property(TARGET NodeJs::NodeJs PROPERTY IMPORTED_LOCATION ${NODEJS_EXECUTABLE})
endif()