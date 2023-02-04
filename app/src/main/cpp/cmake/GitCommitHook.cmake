execute_process(COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/../tools/private/pre-commit"
        "${CMAKE_CURRENT_SOURCE_DIR}/../.git/hooks/pre-commit"
        )