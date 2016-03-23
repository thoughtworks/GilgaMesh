cmake_minimum_required(VERSION 3.2)
get_filename_component(DEPLOY_PATH deploy ABSOLUTE)
get_filename_component(LINKER_PATH linker ABSOLUTE)
get_filename_component(BUILD_PATH _build/${CMAKE_BUILD_TYPE} ABSOLUTE)

project(MeshyMesh)
add_subdirectory(src)
