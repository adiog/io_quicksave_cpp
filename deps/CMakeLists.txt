cmake_minimum_required(VERSION 3.6)
project(abseil)

#set(CMAKE_CXX_COMPILER /usr/bin/g++-5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-ggdb -O0 -fsanitize=address -I/usr/local/include/antlr4-runtime -L/usr/local/lib")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/abseil)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cctz/include)

FILE(GLOB_RECURSE CCTZ_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cctz/**.cc)
ADD_LIBRARY(cctz ${CCTZ_FILES})

FILE(GLOB_RECURSE ABSEIL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/abseil/**.cc)
ADD_LIBRARY(abseil ${ABSEIL_FILES})
TARGET_LINK_LIBRARIES(abseil cctz)