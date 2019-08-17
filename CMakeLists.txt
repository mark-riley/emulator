cmake_minimum_required(VERSION 3.10)
project(emulator)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(SOURCES ${SRC_DIR}/main.cpp ${SRC_DIR}/LR35902.cpp ${SRC_DIR}/Display.cpp ${SRC_DIR}/MemoryBus.cpp ${SRC_DIR}/cb_instructions.cpp ${SRC_DIR}/instructions.cpp ${SRC_DIR}/Cartridge.cpp ${SRC_DIR}/Render.cpp ${SRC_DIR}/Timer.cpp ${SRC_DIR}/Interrupt.cpp)
set(TEST_SOURCES ${TEST_DIR}/tests-main.cpp ${TEST_DIR}/tests-suite.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    include_directories($(SDL2_INCLUDE_DIR))
    link_libraries(${SDL2_LIBRARY})
endif()

set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

add_executable(${PROJECT_NAME}-tests ${TEST_SOURCES})
target_include_directories(${PROJECT_NAME}-tests PRIVATE ${INCLUDE_DIR})
set_property(TARGET ${PROJECT_NAME}-tests PROPERTY CXX_STANDARD 17)

set(CATCH_DIR "${LIB_DIR}/catch")
#add_library("catch" "${CATCH_DIR}/catch.hpp")
#target_include_directories("catch" PRIVATE "${CATCH_DIR}")
target_include_directories(${PROJECT_NAME}-tests PRIVATE "${CATCH_DIR}")
target_include_directories(${PROJECT_NAME}-tests PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME}-tests "glad" "${CMAKE_DL_LIBS}")
#target_link_libraries(${PROJECT_NAME}-tests "catch" "${CMAKE_DL_LIBS}")