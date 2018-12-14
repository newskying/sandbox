set(LIBRARY_OUTPUT_PATH ${Hello_SOURCE_DIR}/lib)
set(CMAKE_C_COMPILER g++)
set(SRC_LIST hello.c)

include_directories(${Hello_SOURCE_DIR}/include)
add_library(util STATIC ${SRC_LIST})

