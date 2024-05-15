cmake_minimum_required(VERSION 2.7...3.13 FATAL_ERROR)

project(Logger VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB HPP_FILES include/*.hpp)
file(GLOB CXX_FILES src/*.cpp)

find_path(LOGGER_INCLUDE_DIR Logger.hpp HINTS include)

if(LOGGER_INCLUDE_DIR)
  message(STATUS "Logger.hpp include directory: ${LOGGER_INCLUDE_DIR}")
else()
  message(FATAL "Logger.hpp include directory cannot found.")
endif()

set(LOGGER_INCLUDE_DIRS ${LOGGER_INCLUDE_DIR} CACHE PATH "Include directories for ${PROJECT_NAME}")

add_library(${PROJECT_NAME} SHARED
  ${CXX_FILES}
  ${HPP_FILES}
)

add_executable(${PROJECT_NAME}_simple_app
  main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${LOGGER_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME}_simple_app PRIVATE ${PROJECT_NAME})
