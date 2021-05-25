aux_source_directory(src GLAD_SRCS)
include_directories(include)
add_library(glad STATIC ${GLAD_SRCS})