cmake_minimum_required(VERSION 3.5)
IF(CMAKE_SYSTEM_NAME STREQUAL "Android")
    SET(ANDROID 1)
ENDIF()

#output all files to the same directory, to make it easier
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Use c++ 23
if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 23) #20 for MSVC, 23 for latest code

set(SFML_DIR "${CMAKE_CURRENT_BINARY_DIR}\\_deps\\sfml-build")#changed from src to build
message("sfml directory: ${SFML_DIR}")
#set(sfml_include ${SFML_DIR}/include)

#set(CMAKE_BUILD_TYPE Debug)

# set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
# Shoutout to CMake guide: https://eliasdaler.github.io/using-cmake/

# increase the amount of i's to update cmakelists.txt:
# iiiiiiiiiiii
# ms builds:
# ii


IF(ANDROID)
    message ("android is defined")
    
    #set(sfml_lib ${SFML_DIR}/lib/${ANDROID_ABI})
    #set(PATH_TO_SFML ${ANDROID_NDK}/sources/third_party/sfml)
    set(PATH_TO_SFML ${SFML_DIR})
    #set(SFML_DIR ${PATH_TO_SFML}/lib/${ANDROID_ABI}/cmake/SFML)
    #add_library(sfml-activity SHARED IMPORTED)
    #set_target_properties(sfml-activity PROPERTIES IMPORTED_LOCATION ${sfml_lib}/libsfml-activity.so)

    # Export ANativeActivity_onCreate(),
    # Refer to: https://github.com/android-ndk/ndk/issues/381.
    
    #set(SFML_STATIC_LIBRARIES TRUE) #when linking static, we can't access the ANativeActivity_OnCreate function or the android functions
ELSE()
    # Enable additional warnings and debug mode
    # https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html
    # -pedantic removed; it complains about anonymous structs
    set (CMAKE_CXX_FLAGS "-g -Wextra -Wall -Wconversion -Wno-stringop-overread -Wno-unknown-pragmas -Wno-unused-parameter")#added D__GXX_TYPEINFO_EQUALITY_INLINE=0 for workaround test
        message ("android is not defined : ${CMAKE_SYSTEM_NAME}")
        #set(SFML_DIR "build\\_deps\\sfml-build")
        
    # If you want to link SFML statically
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(SFML_STATIC_LIBRARIES TRUE) #typeinfo problem
    endif()
    IF (WIN32)
        IF(EXISTS C:/msys64/mingw64/bin/gcc.exe)
            set(CMAKE_C_COMPILER C:/msys64/mingw64/bin/gcc.exe)
            set(CMAKE_CXX_COMPILER C:/msys64/mingw64/bin/g++.exe)
        ELSE()
            set(CMAKE_C_COMPILER D:/msys64/mingw64/bin/gcc.exe)
            set(CMAKE_CXX_COMPILER D:/msys64/mingw64/bin/g++.exe)
        ENDIF()

        #to change to clang:
        #set(CMAKE_C_COMPILER C:/msys64/mingw64/bin/clang.exe)
        #set(CMAKE_CXX_COMPILER C:/msys64/mingw64/bin/clang++.exe)
        #cl:
        #set(CMAKE_C_COMPILER "cl.exe")
        #set(CMAKE_CXX_COMPILER "cl.exe")
        # this makes sure that .dll-files are build into build/ folder
        #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
    ELSEIF (APPLE)
        # run 'where clang' and 'where clang++' to get paths
        set(CMAKE_C_COMPILER /usr/bin/clang)
        set(CMAKE_CXX_COMPILER /usr/bin/clang++)
    ELSEIF (UNIX AND NOT APPLE)
        # run 'which gcc' and 'which g++' to get paths
        set(CMAKE_C_COMPILER /usr/bin/gcc)
        set(CMAKE_CXX_COMPILER /usr/bin/g++)
    ENDIF()
ENDIF()


#find_path(GZIP_INCLUDE_DIRS "gzip/compress.hpp" REQUIRED)

# Get project name from folder name
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId}) 


# add .cpp-files

file(GLOB_RECURSE source_list "src/*.cpp")

IF (LOG_FILES_INCLUDED)
foreach(source_dir ${source_list})
    message(STATUS "[INFO] source file included: ${source_dir}")
endforeach()
ENDIF()
set(SOURCES ${source_list}) 

#add directories to add to #include statements for example we don't have to #include "include/math/..." but with this shortcut we can #include "math/..."
find_package(ZLIB REQUIRED)
include_directories(src)
include_directories(src/include)
include_directories( ${ZLIB_INCLUDE_DIRS} )

# 3rd party dependencies
add_subdirectory(lib)

#include more modern AL headers (when sfml updated openAL, they didn't update the headers)
include_directories(lib/headers)

# Find OpenAL
find_package(OpenAL REQUIRED)

# Here the .c/.cpp files get linked
IF(ANDROID)

    # Creates and names a library, sets it as either STATIC
    # or SHARED, and provides the relative paths to its source code.
    # You can define multiple libraries, and CMake builds them for you.
    # Gradle automatically packages shared libraries with your APK.
    #
    # In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
    # the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
    # is preferred for the same purpose.
    #
    # In order to load a library into your app from Java/Kotlin, you must call
    # System.loadLibrary() and pass the name of the library defined here;
    # for GameActivity/NativeActivity derived applications, the same library name must be
    # used in the AndroidManifest.xml file.
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
ELSE()
    add_executable(${PROJECT_NAME} ${SOURCES})
ENDIF()

#treat all warnings as errors
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)

# For files
set_source_files_properties(
  ${list_of_your_files}
  PROPERTIES
  COMPILE_FLAGS "..."
)

# Add compiler flags for generating debugging information
#add_compile_options(-lefence)
#target_compile_options(${PROJECT_NAME} PRIVATE /Zi)

#clang doesn't support precompiled headers, but uses an internal optimization which does something similar.
IF (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

# src/stdafxdynamic.h is not smart to include. it turns out that all precompiled header files are included in "cmake_pch.h"
#so when you change something in any of the files included by stdafxdynamic.h everything has to recompile
target_precompile_headers(${PROJECT_NAME} PUBLIC 
src/stdafx.h)
ENDIF()

target_compile_options(${PROJECT_NAME} PRIVATE -Wno-type-limits)

#find_package(SFML 2.6.1 COMPONENTS audio network graphics system window REQUIRED)
IF(SFML_STATIC_LIBRARIES)
    target_link_libraries(${PROJECT_NAME} PUBLIC -static-libgcc -static-libstdc++)
    set(CMAKE_EXE_LINKER_FLAGS " -static")

    set(ZLIB_USE_STATIC_LIBS "ON")
ENDIF()
target_link_libraries(${PROJECT_NAME} PUBLIC
ZLIB::ZLIB)

# Here we link 3rd party library ImGui-SFML
# No need to link SFML implicitly as ImGui-SFML does it for you!
# Refer to https://eliasdaler.github.io/using-cmake/
#target_link_libraries(${PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML)
#https://stackoverflow.com/questions/59573574/how-do-i-use-sfml-with-cmake-fetchcontent

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
# set(SFML_DIR "<sfml root prefix>/lib/cmake/SFML")

#add_executable(SFMLTest main.cpp)
#target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio)

#don't try to find_package sfml! we use fetchcontent_declare.
#find_package(SFML 2 COMPONENTS Graphics REQUIRED)

 IF(ANDROID)
    message("cmake toolchain file: ${CMAKE_TOOLCHAIN_FILE} android stl type: ${CMAKE_ANDROID_STL_TYPE} build type: ${CMAKE_BUILD_TYPE}")

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DNDEBUG)
    endif()
    
    #   add_library(openal SHARED IMPORTED) #TODO: is this necessary?
    #   #set_target_properties(openal PROPERTIES IMPORTED_LOCATION ${sfml_lib}/libopenal.so)
    #   #set_target_properties(openal PROPERTIES IMPORTED_LOCATION ${PATH_TO_SFML}/extlibs/lib/${ANDROID_ABI}/libopenal.so)
    #   #set_target_properties(openal PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-src/extlibs/lib/${ANDROID_ABI}/libopenal.so)
    #   set_target_properties(openal PROPERTIES IMPORTED_LOCATION ${SFML_DIR}/extlibs/libs-android/${ANDROID_ABI}/libopenal.so)
    #   message("location: ${SFML_DIR}/extlibs/libs-android/${ANDROID_ABI}/libopenal.so")
    #   target_link_libraries(${PROJECT_NAME} openal)
    #   #set_target_properties(openal PROPERTIES IMPORTED_LOCATION_DEBUG ${PATH_TO_SFML}/extlibs/lib/${ANDROID_ABI}/libopenal.so)
    #   #set_target_properties(openal PROPERTIES IMPORTED_LOCATION_RELEASE ${sfml_lib}/libopenal.so)
    #   #set_target_properties(openal PROPERTIES IMPORTED_LOCATION_DEBUG ${sfml_lib}/libopenal.so)
    #we aren't allowed to link OpenAL static, because it's in their terms. the user should be able to modify the OpenAL DLL

    # The ANativeActivity_onCreate function from SFML::Main needs to be exposed in
    # our libsfml-example.so file so that is can be loaded at runtime.
    
    # Find and link SFML

    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
    message("linking all in one...")
    target_link_libraries(${PROJECT_NAME} PUBLIC
    #openal
    -Wl,--whole-archive
    sfml-audio
    sfml-network
    sfml-graphics
    sfml-window
    sfml-system
    OpenAL::OpenAL
    android
    log
    -Wl,--no-whole-archive
    )

    # message("linking sfml..")
    # target_link_libraries(${PROJECT_NAME} PUBLIC
    # #openal
    # sfml-audio
    # sfml-network
    # sfml-graphics
    # sfml-window
    # sfml-system
    # )

    # # Specifies libraries CMake should link to your target library. You
    # # can link libraries from various origins, such as libraries defined in this
    # # build script, prebuilt third-party libraries, or Android system libraries.
    # target_link_libraries(${PROJECT_NAME} PUBLIC
    # # List libraries link to the target library
    # android
    # log)
    
    message("linking sfml-system whole archive")
    target_link_libraries(${PROJECT_NAME} PUBLIC
    -Wl,--whole-archive
    sfml-main
    -Wl,--no-whole-archive
    )
 ELSE()
    # Additional resources like images and fonts
    # We want the to copy from permanent folder "res" to temporary folder "build"
    # configure_file(res/OpenSans-Regular.ttf OpenSans-Regular.ttf COPYONLY)

    # Set "a" to be the output filename, does not really matter, shorter is quicker to type...
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "a")

    target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-audio
    sfml-network
    sfml-graphics
    sfml-system
    sfml-window
    sfml-main
    OpenAL::OpenAL)

    #copy OpenAL32.dll
    message(STATUS "win32: ${WIN32} build audio: ${SFML_BUILD_AUDIO}")
    message(STATUS "copy command: ${CMAKE_COMMAND} -E copy ${FETCHCONTENT_BASE_DIR}/sfml-src/extlibs/bin/x64/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>")
    if (WIN32 AND SFML_BUILD_AUDIO)
      #update openAL and add alsoft.ini to enable hrtf
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/lib/openAL/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
      )
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/lib/openAL/alsoft.ini $<TARGET_FILE_DIR:${PROJECT_NAME}>
      )
    endif()

ENDIF()