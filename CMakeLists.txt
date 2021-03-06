cmake_minimum_required (VERSION 3.10)
project (glass VERSION 0.0.1)

cmake_policy(VERSION 2.6...3.18)

option(BUILD_SHARED_LIBS "Build the shared library" ON)
option(BUILD_DEBUG "Build debug version for gdb to debug" OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	if(BUILD_DEBUG)
		set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -g -D\"_DEBUG=1\"")
	else()
		set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
	endif()
endif()

include_directories(
	include
	third_party/assimp5.0.1/include
	third_party/glfw3.3.2/include
)

file(GLOB_RECURSE ALL_GLSL ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/glsl/*.glsl)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/shaders.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/glass/utils/shaders.h
  COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/glsl && python generate_hardcode.py
  DEPENDS ${ALL_GLSL}
)

set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "inject debug postfix to *.a/*.so/*.dll" FORCE)
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/assimp5.0.1)

set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW test programs" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW example programs" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW documents" FORCE)
add_subdirectory(third_party/glfw3.3.2)

file(GLOB_RECURSE GLASS_C_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE GLASS_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

if(BUILD_SHARED_LIBS)
	add_library(glass SHARED
		${GLASS_C_SRCS}
		${GLASS_CPP_SRCS}
		${CMAKE_CURRENT_SOURCE_DIR}/src/utils/shaders.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/include/glass/utils/shaders.h
	)
else()
	add_library(glass STATIC
		${GLASS_C_SRCS}
		${GLASS_CPP_SRCS}
		${CMAKE_CURRENT_SOURCE_DIR}/src/utils/shaders.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/include/glass/utils/shaders.h
	)
endif()

target_link_libraries(glass
	assimp
	zlibstatic
	glfw
)

install(TARGETS glass
        RUNTIME DESTINATION "bin"
        ARCHIVE DESTINATION "lib"
        LIBRARY DESTINATION "lib")

install(TARGETS assimp
        RUNTIME DESTINATION "bin"
        ARCHIVE DESTINATION "lib"
        LIBRARY DESTINATION "lib")

install(TARGETS glfw
        RUNTIME DESTINATION "bin"
        ARCHIVE DESTINATION "lib"
        LIBRARY DESTINATION "lib")

install(TARGETS zlibstatic DESTINATION "lib")

install(DIRECTORY include/glass DESTINATION include)