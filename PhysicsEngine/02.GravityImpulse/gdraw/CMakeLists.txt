cmake_minimum_required(VERSION 3.20)
project(gdraw)
set(CMAKE_CXX_STANDARD 20)

file( GLOB gdraw_SOURCES *.cpp )
add_library( ${PROJECT_NAME} ${gdraw_SOURCES} )
target_include_directories( ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} SDL3 gplatform gmath gassets)
target_link_libraries( ${PROJECT_NAME} gplatform SDL3::SDL3 gmath gassets gplatform)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG=0)
endif()