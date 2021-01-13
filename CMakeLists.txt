cmake_minimum_required(VERSION 3.10)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

project(emulator)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Set up SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})


set(INCLUDE_DIR "${SRC_DIR}")
include_directories(${INCLUDE_DIR})

set(SOURCES ${SRC_DIR}/main.cpp ${SRC_DIR}/cpu/LR35902.cpp ${SRC_DIR}/Display.cpp ${SRC_DIR}/MemoryBus.cpp ${SRC_DIR}/cpu/instructions/interpreter/cb_interpreter.cpp ${SRC_DIR}/cpu/instructions/interpreter/interpreter.cpp ${SRC_DIR}/Cartridge.cpp ${SRC_DIR}/Render.cpp ${SRC_DIR}/Timer.cpp ${SRC_DIR}/Interrupt.cpp src/file-utils.h src/file-utils.cpp src/Gameboy.cpp src/Gameboy.h)
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SDL2 Libraries
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# Glad
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

#set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

#set(TEST_SOURCES ${TEST_DIR}/tests-main.cpp ${TEST_DIR}/tests-files.cpp ${TEST_DIR}/tests-cartridge.cpp ${SRC_DIR}/file-utils.h ${SRC_DIR}/file-utils.cpp ${SRC_DIR}/Cartridge.cpp)

#add_executable(${PROJECT_NAME}-tests ${TEST_SOURCES})
#target_include_directories(${PROJECT_NAME}-tests PRIVATE ${INCLUDE_DIR})
#set_property(TARGET ${PROJECT_NAME}-tests PROPERTY CXX_STANDARD 17)

#set(CATCH_DIR "${LIB_DIR}/catch")
#add_library("catch" "${CATCH_DIR}/catch.hpp")
#target_include_directories("catch" PRIVATE "${CATCH_DIR}")
#target_include_directories(${PROJECT_NAME}-tests PRIVATE "${CATCH_DIR}")
#target_include_directories(${PROJECT_NAME}-tests PRIVATE "${GLAD_DIR}/include")
#target_link_libraries(${PROJECT_NAME}-tests "glad" "${CMAKE_DL_LIBS}")
#target_link_libraries(${PROJECT_NAME}-tests "catch" "${CMAKE_DL_LIBS}")