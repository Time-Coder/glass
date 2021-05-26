cmake_minimum_required (VERSION 3.10)
project (glass VERSION 0.0.0.1)

cmake_policy(VERSION 2.6...3.18)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")
option(BUILD_SHARED_LIBS "Build the shared library" ON)

include_directories(
	include
	include/third_party/glad
	include/third_party/assimp
	include/third_party/glfw
	include/third_party/stb_image
)

aux_source_directory(src GLASS_SRCS)
aux_source_directory(src/utils GLASS_SRCS_UTILS)
aux_source_directory(src/utils/vec GLASS_SRCS_UTILS_VEC)

add_subdirectory(third_party/glad4.6)

set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "inject debug postfix to *.a/*.so/*.dll" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "" FORCE)
add_subdirectory(third_party/assimp5.0.1)

set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW test programs" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW example programs" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW documents" FORCE)
add_subdirectory(third_party/glfw3.3.2)

if(BUILD_SHARED_LIBS)
	add_library(glass SHARED ${GLASS_SRCS} ${GLASS_SRCS_UTILS} ${GLASS_SRCS_UTILS_VEC})
else()
	add_library(glass STATIC ${GLASS_SRCS} ${GLASS_SRCS_UTILS} ${GLASS_SRCS_UTILS_VEC})
endif()

link_directories(
	third_party
)

target_link_libraries(glass
	assimp
	zlibstatic
	glfw
	glad
)

install(TARGETS glass
        EXPORT glassTargets
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

install(TARGETS glad DESTINATION "lib")
install(TARGETS zlibstatic DESTINATION "lib")

install(DIRECTORY include DESTINATION .)