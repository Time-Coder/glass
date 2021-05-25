aux_source_directory(src STB_IMAGE_SRCS)
include_directories(include)
add_library(stbimage STATIC ${STB_IMAGE_SRCS})