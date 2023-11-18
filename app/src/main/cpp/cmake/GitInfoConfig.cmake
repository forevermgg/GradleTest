# https://cmake.org/cmake/help/latest/module/FindGit.html
find_package(Git)
if (GIT_FOUND)
    message("GIT_EXECUTABLE： " ${GIT_EXECUTABLE})
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_INFO_SHA
    )
endif()

if (GIT_INFO_SHA)
    string(REGEX REPLACE "[^0-9a-f]+" "" GIT_INFO_SHA "${GIT_INFO_SHA}")
endif()
message("GIT_INFO_SHA： " ${GIT_INFO_SHA})