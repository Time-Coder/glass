cmake_minimum_required (VERSION 3.10)
project (glass)

cmake_policy(VERSION 2.6...3.18)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
	include
	include/third_party/glad
	include/third_party/assimp
	include/third_party/glfw
	include/third_party/stb_image
	# D:/MinGW-W64_posix_seh/include
)
# link_directories(
# 	D:/MinGW-W64_posix_seh/x86_64-w64-mingw32/lib
# )

aux_source_directory(src GLASS_SRCS)
aux_source_directory(src/utils GLASS_SRCS_UTILS)
aux_source_directory(src/utils/vec GLASS_SRCS_UTILS_VEC)

add_subdirectory(third_party/glad4.6)
add_subdirectory(third_party/stb_image)

set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "inject debug postfix to *.a/*.so/*.dll" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "build targets with shared libraries" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "" FORCE)
add_subdirectory(third_party/assimp5.0.1)

set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW test programs" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW example programs" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW documents" FORCE)
add_subdirectory(third_party/glfw3.3.2)

add_library(glass STATIC ${GLASS_SRCS} ${GLASS_SRCS_UTILS} ${GLASS_SRCS_UTILS_VEC})
install(TARGETS glass DESTINATION lib)
install(TARGETS assimp DESTINATION lib)
install(TARGETS zlibstatic DESTINATION lib)
install(TARGETS glfw DESTINATION lib)
install(TARGETS glad DESTINATION lib)
install(TARGETS stbimage DESTINATION lib)
install(DIRECTORY include DESTINATION .)