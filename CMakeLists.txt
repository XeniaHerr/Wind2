cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(CMAKE_CXX_STANDARD 23)


project(Wind2 VERSION 0.1 LANGUAGES CXX)

include_directories(include)




add_subdirectory(dummy)
add_subdirectory(src)
