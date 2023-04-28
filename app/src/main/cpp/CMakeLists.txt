# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)
# https://www.bookstack.cn/read/CMake-Cookbook/content-chapter7-7.2-chinese.md
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(colors)
include(PlatformCheck)
include(GitInfoConfig)
include(IncludeWhatYouUse)
include(TimeTrace)
include(FindNode)
include(FindNodeJs)
include(FindPython)
include(CMakeFindSublimeText2)
include(CMakeFindJavaCommon)
include(CMakePrintSystemInformation)
include(CMakePrintHelpers)
define_colors()

message("This is a normal message")
message("${Red}This is a red${ColourReset}")
message("${BoldRed}This is a bold red${ColourReset}")
message("${Green}This is a green${ColourReset}")
message("${BoldMagenta}This is bold${ColourReset}")

# https://www.kancloud.cn/csyangbinbin/cmake-cookbook1/2157928
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("Configuring on/for Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message("Configuring on/for macOS")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message("Configuring on/for Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "AIX")
    message("Configuring on/for IBM AIX")
else()
    message("Configuring on/for ${CMAKE_SYSTEM_NAME}")
endif()

find_package(PythonInterp REQUIRED)
# https://www.ljjyy.com/archives/2021/03/100654.html
execute_process(
        COMMAND
        ${PYTHON_EXECUTABLE} "-c" "print('Hello, world!')"
        RESULT_VARIABLE _status
        OUTPUT_VARIABLE _hello_world
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

message("RESULT_VARIABLE is: ${_status}")
message("OUTPUT_VARIABLE is: ${_hello_world}")

message("PYTHON WORKING_DIRECTORY:" "${CMAKE_CURRENT_SOURCE_DIR}/../../../../python")

execute_process(
        COMMAND python "${CMAKE_CURRENT_SOURCE_DIR}/../../../../python/test.py"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        RESULT_VARIABLE script_result
        OUTPUT_VARIABLE script_output)
message("script_result is: ${script_result}")
message("script_output is: ${script_output}")
IF(NOT "${script_result}" STREQUAL 0)
    MESSAGE(FATAL_ERROR "Failed to run python script")
ELSE ()
    # do whatever
ENDIF()

# brew install graphviz
# Declares and names the project.

project("cmaketest")

file(
        DOWNLOAD
        https://www.baidu.com/img/PCtm_d9c8750bed0b3c7d089fa7d55720d6cf.png
        ${PROJECT_SOURCE_DIR}/../thirdparty/test.png
        STATUS DOWNLOAD_STATUS
        SHOW_PROGRESS
)
message(DOWNLOAD_STATUS : ${DOWNLOAD_STATUS})

#[[file(
        DOWNLOAD
        https://fukuchi.org/works/qrencode/qrencode-3.4.4.tar.bz2
        ${PROJECT_SOURCE_DIR}/../thirdparty/qrencode.tar.bz
        STATUS DOWNLOAD_STATUS
        SHOW_PROGRESS
)]]

message(STATUS "CMAKE_COMMAND=${CMAKE_COMMAND}")
execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${PROJECT_SOURCE_DIR}/../thirdparty/qrencode.tar.bz
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../thirdparty/
)

execute_process(
        COMMAND ${CMAKE_COMMAND} -E rename qrencode-3.4.4 qrencode3.4.4
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../thirdparty/
)

# https://oomake.com/question/1816649
execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar cfv ${PROJECT_SOURCE_DIR}/../thirdparty/archive.zip --format=zip test.png
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../thirdparty/
)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        cmaketest

        # Sets the library as a shared library.
        SHARED
        android/jni_util.cc
        android/jni_util.h
        android/jni_weak_ref.cc
        android/jni_weak_ref.h
        android/log_level.h
        android/log_settings.h
        android/log_settings.cc
        android/log_settings_state.cc
        android/logging.h
        android/logging.cc
        android/scoped_java_ref.cc
        android/scoped_java_ref.h
        android/thread_local.cc
        android/thread_local.h
        android/string_conversion.cc
        android/string_conversion.h
        app.hpp
        app.cpp
        binding_context.hpp
        checked_mutex.hpp
        # Provides a relative path to your source file(s).
        connection_change_notifier.hpp
        connection_change_notifier.cpp
        java_binding_context.hpp
        java_binding_context.cpp
        java_class.cpp
        java_class.hpp
        java_class_global_def.hpp
        java_class_global_def.cpp
        Java_com_test_cmaketest_OsApp.cpp
        Java_com_test_cmaketest_SyncSession.cpp
        java_global_ref_by_copy.cpp
        java_global_ref_by_copy.hpp
        java_global_ref_by_move.cpp
        java_global_ref_by_move.hpp
        java_global_weak_ref.cpp
        java_global_weak_ref.hpp
        java_local_ref.hpp
        java_method.hpp
        java_method.cpp
        jni_utils.cpp
        jni_utils.hpp
        load_library.cpp
        managed.cc
        native-lib.cpp
        NativeObjectReference.cpp
        android/scheduler.hpp
        task_runner.h
        TestData.h
        TestData.cpp
        thread_pool.cpp
        thread_pool.hpp
        thread_pool.cpp
        U16String.cc
        uuid.hpp
        uuid.cpp
        waitable_event.h
        waitable_event.cc
        wrap_err.hpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(cmaketest PUBLIC "LINUX")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_definitions(cmaketest PUBLIC "MACOS")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(cmaketest PUBLIC "WINDOWS")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    target_compile_definitions(cmaketest PUBLIC "Android")
endif()

# if(cmaketest)
    # For asan and ubsan, we need to copy some files from the NDK into our APK.
    get_filename_component(HINT_PATH ${ANDROID_C_COMPILER} DIRECTORY)
    set(ARCH_STR ${CMAKE_ANDROID_ARCH})
    if ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "arm64-v8a")
        set(ARCH_STR "aarch64")
    elseif ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "armeabi")
        set(ARCH_STR "arm")
    elseif ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "armeabi-v7a")
        set(ARCH_STR "arm")
    elseif ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "x86")
        set(ARCH_STR "i686")
    elseif ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "x86_64")
        set(ARCH_STR "x86_64")
    endif()

    if(${SANITIZE} STREQUAL "hwasan")
        message("Using hwasan")

        target_compile_options(cmaketest PUBLIC -fsanitize=hwaddress -fno-omit-frame-pointer)
        target_link_options(cmaketest PUBLIC -fsanitize=hwaddress)
    elseif(${SANITIZE} STREQUAL "asan")
        message("Using asan")

        target_compile_options(cmaketest PUBLIC -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(cmaketest PUBLIC -fsanitize=address)

        # Grab libclang_rt.asan-${ARCH_STR}-android.so from the NDK.
        file(GLOB ASAN_GLOB "${HINT_PATH}/../lib64/clang/*/lib/linux")
        find_file(ASAN
                NAMES libclang_rt.asan-${ARCH_STR}-android.so
                PATHS ${ASAN_GLOB})
        get_filename_component(ASAN_NAME ${ASAN} NAME)
        set(ASAN_NAME ${CMAKE_SOURCE_DIR}/../../asan/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/${ASAN_NAME})
        add_custom_command(
                TARGET cmaketest PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${ASAN} ${ASAN_NAME})

        # Grab the asan wrapper script from the NDK.
        find_file(WRAP
                NAMES asan.sh
                HINTS ${HINT_PATH}/../../../../../wrap.sh)
        set(WRAP_NAME ${CMAKE_SOURCE_DIR}/../../asan/resources/lib/${CMAKE_ANDROID_ARCH_ABI}/wrap.sh)
        add_custom_command(
                TARGET cmaketest PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${WRAP} ${WRAP_NAME})
    elseif(${SANITIZE} STREQUAL "ubsan")
        message("Using ubsan")

        target_compile_options(cmaketest PUBLIC -fsanitize=undefined -fno-sanitize-recover=undefined)
        target_link_options(cmaketest PUBLIC -fsanitize=undefined -fno-sanitize-recover=undefined)

        # Grab libclang_rt.ubsan_standalone-${ARCH_STR}-android.so from the NDK.
        file(GLOB UBSAN_GLOB "${HINT_PATH}/../lib64/clang/*/lib/linux")
        find_file(UBSAN
                NAMES libclang_rt.ubsan_standalone-${ARCH_STR}-android.so
                PATHS ${UBSAN_GLOB})
        get_filename_component(UBSAN_NAME ${UBSAN} NAME)
        set(UBSAN_NAME ${CMAKE_SOURCE_DIR}/../../ubsan/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/${UBSAN_NAME})
        add_custom_command(
                TARGET cmaketest PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${UBSAN} ${UBSAN_NAME})
    endif()
# endif()

target_link_libraries( # Specifies the target library.
        cmaketest

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})