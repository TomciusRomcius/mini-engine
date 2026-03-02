add_executable(engine main.cpp
        Application.cpp
        Application.h)
target_link_libraries(engine LINK_PUBLIC core)
include_directories(../core/src)
