cmake_minimum_required(VERSION 3.2)
get_filename_component(DEPLOY_PATH deploy ABSOLUTE)
get_filename_component(LINKER_PATH linker ABSOLUTE)
get_filename_component(BUILD_PATH _build/${CMAKE_BUILD_TYPE} ABSOLUTE)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

project(MeshyMesh)
add_subdirectory(src)

#for now unit testing if only supported on Linux (tested on Ubuntu 16.04 MATE)
#this is because of problems compiling the google test framework on OSX (to be resolved)
if(LINUX)
    add_subdirectory(${DEPLOY_PATH}/gtest)
    add_subdirectory(tests)
endif()
