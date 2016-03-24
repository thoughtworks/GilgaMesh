cmake_minimum_required(VERSION 3.2)

include_directories(${CMAKE_SOURCE_DIR}/inc ${DEPLOY_PATH}/gtest/googlemock/include ${DEPLOY_PATH}/gtest/googletest/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_executable(RunIntegrationTests
               integration/GapTest.cpp)

target_link_libraries(RunIntegrationTests gmock gmock_main)