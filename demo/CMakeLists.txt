cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(glass_demo)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -g")

aux_source_directory("." DEMO_SRCS)
include_directories("../glass/dist/release/include")
link_directories("../glass/dist/release/lib")

add_executable(main
	main.cpp
)

target_link_libraries(main
	glass
	assimp
	glfw3dll
	zlibstatic
	gdi32
	opengl32
)