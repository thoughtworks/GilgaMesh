cmake_minimum_required(VERSION 3.2)
get_filename_component(DEPLOY_PATH deploy ABSOLUTE)
get_filename_component(LINKER_PATH linker ABSOLUTE)
get_filename_component(BUILD_PATH _build/${CMAKE_BUILD_TYPE} ABSOLUTE)

#add a variable to tell Linux from OSX
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

project(MeshyMesh)

if(IS_TEST_BUILD AND LINUX) #build tests on linux
    add_subdirectory(tests)
else() #build for real hardware
    add_subdirectory(src)
endif()
