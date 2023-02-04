find_program (NODEJS_EXECUTABLE NAMES node nodejs
        HINTS
        $ENV{NODE_DIR}
        PATH_SUFFIXES bin
        DOC "Node.js interpreter")

message("NODEJS_EXECUTABLE:" ${NODEJS_EXECUTABLE})

if (NODEJS_EXECUTABLE)
    execute_process(COMMAND ${NODEJS_EXECUTABLE} --version
            OUTPUT_VARIABLE _VERSION
            RESULT_VARIABLE _NODE_VERSION_RESULT)
    message("NODEJS_EXECUTABLE:" ${_VERSION})
    message("NODEJS_EXECUTABLE:" ${_NODE_VERSION_RESULT})
    execute_process(COMMAND ${NODEJS_EXECUTABLE} -e "console.log(process.versions.v8)"
            OUTPUT_VARIABLE _V8_VERSION
            RESULT_VARIABLE _V8_RESULT)
    message("NODEJS_EXECUTABLE:" ${_V8_VERSION})
    message("NODEJS_EXECUTABLE:" ${_V8_RESULT})
    if (NOT ${_NODE_VERSION_RESULT} AND NOT ${_V8_RESULT})
        message("NODEJS_EXECUTABLE:NOT")
        string (REPLACE "v" "" NODE_VERSION_STRING "${_VERSION}")
        string (REPLACE "." ";" _VERSION_LIST "${NODE_VERSION_STRING}")
        list (GET _VERSION_LIST 0 NODE_VERSION_MAJOR)
        list (GET _VERSION_LIST 1 NODE_VERSION_MINOR)
        list (GET _VERSION_LIST 2 NODE_VERSION_PATCH)
        message("NODE_VERSION_MAJOR:" ${NODE_VERSION_MAJOR} " NODE_VERSION_MINOR:" ${NODE_VERSION_MINOR} " NODE_VERSION_PATCH:" ${NODE_VERSION_PATCH})
        set (V8_VERSION_STRING ${_V8_VERSION})
        string (REPLACE "." ";" _V8_VERSION_LIST "${_V8_VERSION}")
        string (REPLACE "." "" V8_DEFINE_STRING "${_V8_VERSION}")
        string (STRIP ${V8_DEFINE_STRING} V8_DEFINE_STRING)
        list (GET _V8_VERSION_LIST 0 V8_VERSION_MAJOR)
        list (GET _V8_VERSION_LIST 1 V8_VERSION_MINOR)
        list (GET _V8_VERSION_LIST 2 V8_VERSION_PATCH)
        message("V8_VERSION_MAJOR:" ${V8_VERSION_MAJOR} " V8_VERSION_MINOR:" ${V8_VERSION_MINOR} " V8_VERSION_PATCH:" ${V8_VERSION_PATCH})
        # we end up with a nasty newline so strip everything that isn't a number
        string (REGEX MATCH "^[0-9]*" V8_VERSION_PATCH ${V8_VERSION_PATCH})
        message("V8_VERSION_PATCH:" ${V8_VERSION_PATCH})

        string (REGEX REPLACE "\n" "" NODE_VERSION_STRING ${NODE_VERSION_STRING})
        string (REGEX REPLACE "\n" "" V8_VERSION_STRING ${V8_VERSION_STRING})

        message("NODE_VERSION_STRING:" ${NODE_VERSION_STRING})
        message("V8_VERSION_STRING:" ${V8_VERSION_STRING})

        mark_as_advanced (NODEJS_EXECUTABLE)
    else ()
        message (FATAL_ERROR "Check Node Failed")
    endif ()
endif ()