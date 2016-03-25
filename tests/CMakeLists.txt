cmake_minimum_required(VERSION 3.2)

# generate outputs macro
macro(GEN_OUTPUTS target)
    get_filename_component(name ${target} NAME_WE)
    #copy test executables
    add_custom_command(
        OUTPUT ${name}_copy_tests DEPENDS ${target}
        COMMAND mkdir -p ${BUILD_PATH}/tests
        COMMAND cp ${target} ${BUILD_PATH}/tests/${target}
    )
    add_custom_target(
        ${name}_outputs ALL DEPENDS ${name}_copy_tests
    )
endmacro(GEN_OUTPUTS)

include_directories(${CMAKE_SOURCE_DIR}/inc)

add_executable(RunUnitTests
               unit/GapTest.c
               unit/GattTest.c
               unit/RunTests.c)

target_link_libraries(RunUnitTests cmocka)
GEN_OUTPUTS(RunUnitTests)