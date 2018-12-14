set(EXECUTABLE_OUTPUT_PATH ${Hello_SOURCE_DIR}/bin)
set(SRC_LIST main.cpp)

include_directories(${Hello_SOURCE_DIR}/include)
link_directories(${Hello_SOURCE_DIR}/lib)

add_executable(hello ${SRC_LIST})
target_link_libraries(hello util)

