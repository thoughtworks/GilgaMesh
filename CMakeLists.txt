cmake_minimum_required(VERSION 3.2)
get_filename_component(DEPLOY_PATH deploy ABSOLUTE)
get_filename_component(LINKER_PATH linker ABSOLUTE)
get_filename_component(BUILD_PATH _build/${CMAKE_BUILD_TYPE} ABSOLUTE)
get_filename_component(SRC_PATH src ABSOLUTE)
get_filename_component(INC_PATH inc ABSOLUTE)
get_filename_component(CONFIG_PATH config ABSOLUTE)

#set common variables
SET(SDK_BASE                ${DEPLOY_PATH}/sdk/nrf_sdk_9_0)
SET(COMPONENTS              ${SDK_BASE}/components)
SET(EHAL_PATH               ${DEPLOY_PATH}/EHAL)

# set target hardware type
if(IS_PROD_BOARD)
    SET(TARGET_BOARD       BOARD_SEEED)
else()
    SET(TARGET_BOARD       BOARD_PCA10028)
endif()

project(MeshyMesh)

if(IS_TEST_BUILD AND UNIX) #build tests on linux or osx
    add_subdirectory(tests)
else() #build for real hardware
    add_subdirectory(src)
endif()
