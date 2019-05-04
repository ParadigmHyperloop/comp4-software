cmake_minimum_required(VERSION 3.13)
project(FlightComputer)

set(CMAKE_CXX_STANDARD 14)

include_directories(include/EasyLogger)
include_directories(include/FlightComputer)
include_directories(include/ProtoBuffer)
include_directories(include)

file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB SOURCES "src/*.c*")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

message(${CMAKE_CXX_COMPILER})

if (${CMAKE_CXX_COMPILER} MATCHES ".arm")
    # Staticly link libs built for ARM
    set(Protobuf_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/ProtoBuffer/src")
    include_directories(${Protobuf_INCLUDE_DIRS})

    set(Protobuf_LIBRARIES ${Protobuf_INCLUDE_DIRS}/.libs/libprotobuf.a)
else ()
    # Compiling on host: Link local libraries dynamicly
    find_package(Protobuf REQUIRED)
    include_directories(${Protobuf_INCLUDE_DIRS})
endif ()

find_package(Threads REQUIRED)
if (THREADS_HAVE_PTHREAD_ARG)
    message("have")
    target_compile_options(FlightComputer PUBLIC "-pthread")
endif ()

add_executable(FlightComputer ${SOURCES} ${PROTO_HDRS})

target_link_libraries(FlightComputer ${Protobuf_LIBRARIES})

if (CMAKE_THREAD_LIBS_INIT)
    SET (MYLIB -Wl,--whole-archive ${CMAKE_THREAD_LIBS_INIT} -Wl,--no-whole-archive)
    target_link_libraries(FlightComputer ${MYLIB})
endif ()

