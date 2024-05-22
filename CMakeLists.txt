cmake_minimum_required(VERSION 3.16)

project(CLog)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O3")
message("Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

message("Adding source files")
set(CLOG_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/CLog/src/registry.cpp
    ${CMAKE_SOURCE_DIR}/CLog/src/details/pattern/alignment_formatter.cpp
    ${CMAKE_SOURCE_DIR}/CLog/src/details/pattern/attribute.cpp
    ${CMAKE_SOURCE_DIR}/CLog/src/details/pattern/pattern.cpp
)

message("Creating CLog library")
add_library(CLog_lib STATIC ${CLOG_SOURCE_FILES})
target_include_directories(CLog_lib PRIVATE ${CMAKE_SOURCE_DIR}/CLog/src/)

message("Creating CLog main")
add_executable(CLog_main ${CMAKE_SOURCE_DIR}/Main/Main.cpp)
target_include_directories(CLog_main PRIVATE ${CMAKE_SOURCE_DIR}/CLog/src/)

message("Linking libraries")
target_link_libraries(CLog_lib PRIVATE Threads::Threads)
target_link_libraries(CLog_main PRIVATE CLog_lib)