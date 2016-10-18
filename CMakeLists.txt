cmake_minimum_required(VERSION 3.2)

get_filename_component(DEPLOY_PATH deploy ABSOLUTE)
get_filename_component(LINKER_PATH linker ABSOLUTE)
get_filename_component(BUILD_PATH _build/${CMAKE_BUILD_TYPE} ABSOLUTE)
get_filename_component(SRC_PATH src ABSOLUTE)
get_filename_component(INC_PATH inc ABSOLUTE)
get_filename_component(CONFIG_PATH config ABSOLUTE)

#set common variables
SET(SDK_BASE                ${DEPLOY_PATH}/sdk/nrf_sdk_12_1)
SET(COMPONENTS              ${SDK_BASE}/components)
SET(EXTERNAL                ${SDK_BASE}/external)
SET(EXTERNAL                ${SDK_BASE}/external)
SET(EHAL_PATH               ${DEPLOY_PATH}/EHAL)

if(NOT ${TARGET_ARCH} MATCHES "NRF52")
  SET(SOFTDEVICE              ${DEPLOY_PATH}/softdevice/s130_nrf52_3.0.0_API)
  SET(TARGET_BOARD       BOARD_PCA10040)
else() #build for NRF51 by default
  SET(SOFTDEVICE              ${DEPLOY_PATH}/softdevice/s130_nrf51_2.0.1_API)
  SET(TARGET_BOARD       BOARD_PCA10028)
endif()

project(ThoughtMesh)
if(IS_TEST_BUILD AND UNIX) #build tests on linux or osx
    add_subdirectory(tests)
else() #build for real hardware
    add_subdirectory(src)
endif()

