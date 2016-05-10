cmake_minimum_required(VERSION 3.2)

SET(BOOTLOADER_DIR_NAME         bootloader)

get_filename_component(DEPLOY_PATH deploy ABSOLUTE)
get_filename_component(LINKER_PATH linker ABSOLUTE)
get_filename_component(BUILD_PATH _build/${CMAKE_BUILD_TYPE} ABSOLUTE)
get_filename_component(SRC_PATH src ABSOLUTE)
get_filename_component(INC_PATH inc ABSOLUTE)
get_filename_component(CONFIG_PATH config ABSOLUTE)
get_filename_component(BOOTLOADER_PATH ${BOOTLOADER_DIR_NAME} ABSOLUTE)

#set common variables
SET(SDK_BASE                ${DEPLOY_PATH}/sdk/nrf_sdk_11_0)
SET(COMPONENTS              ${SDK_BASE}/components)
SET(EXTERNAL                ${SDK_BASE}/external)
SET(EXTERNAL                ${SDK_BASE}/external)
SET(SOFTDEVICE              ${DEPLOY_PATH}/softdevice/s130_nrf51_2.0.1_API)
SET(EHAL_PATH               ${DEPLOY_PATH}/EHAL)

# set target hardware type
if(IS_PRODUCTION_BOARD)
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

