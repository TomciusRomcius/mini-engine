add_executable(engine main.cpp)
target_link_libraries(engine LINK_PUBLIC core)
include_directories(../core/src)