# SIMPLE TEST USING OPENCV LIBRARY

# SUGESTED COMMANDS
# cmake . -Bbuild
# make -Cbuild
# ./main

cmake_minimum_required(VERSION 2.8)
project( display )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
find_package(OpenMVG REQUIRED)

include_directories(${OPENMVG_INCLUDE_DIRS})
add_executable(main test-lab-2.cpp)
target_link_libraries(main ${OPENMVG_LIBRARIES})


