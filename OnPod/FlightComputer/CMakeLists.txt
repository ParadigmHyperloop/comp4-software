cmake_minimum_required(VERSION 3.13)
project(FlightComputer)

set(CMAKE_CXX_STANDARD 14)

include_directories(libs/EasyLogger)
include_directories(libs/ProtoBuffer)
include_directories(include)
include_directories(src)
include_directories(include/ProtoBuffer)

file(GLOB_RECURSE HEADERS "libs/*.h")
file(GLOB_RECURSE SOURCES "src/*.*")

include_directories(${CMAKE_CURRENT_BINARY_DIR})


if (${CMAKE_CXX_COMPILER} MATCHES ".arm")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc  -static-libstdc++")

    # Staticly link libs built for ARM
    set(Protobuf_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/libs/ProtoBuffer/src")
    include_directories(${Protobuf_INCLUDE_DIRS})
    set(Protobuf_LIBRARIES ${Protobuf_INCLUDE_DIRS}/.libs/libprotobuf.a)
else ()
    # Compiling on host: Link local libraries dynamicly
    find_package(Protobuf REQUIRED)
    include_directories(${Protobuf_INCLUDE_DIRS})
endif ()

find_package(Threads REQUIRED)
if (THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(FlightComputer PUBLIC "-pthread")
endif ()

add_executable(FlightComputer ${SOURCES}  ${PROTO_HDRS} src/Helpers/IPodValuesLogger.hpp src/Helpers/PodValuesLogger.cpp src/Helpers/PodValuesLogger.hpp)

target_link_libraries(FlightComputer ${Protobuf_LIBRARIES})

if (CMAKE_THREAD_LIBS_INIT)
    SET (MYLIB -Wl,--whole-archive ${CMAKE_THREAD_LIBS_INIT} -Wl,--no-whole-archive)
    target_link_libraries(FlightComputer ${MYLIB})
endif ()

