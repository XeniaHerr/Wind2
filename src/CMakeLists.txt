

add_executable(TEST test.cpp)



add_library(WIND2_LIB STATIC client.cpp
                            topic.cpp
                            monitor.cpp
                            WindowManagerModel.cpp)
