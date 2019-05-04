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

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    # Compiling on host: Link local libraries dynamicly


    find_package(Protobuf REQUIRED)

    include_directories(${Protobuf_INCLUDE_DIRS})


else ()
    # Staticly link libs built for ARM

    set(Protobuf_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/ProtoBuffer/src")
    set(Protobuf_LIBRARIES ${Protobuf_INCLUDE_DIRS}/.libs/libprotobuf.a)

endif ()


find_package(Threads REQUIRED)
if (THREADS_HAVE_PTHREAD_ARG)
    message("have")
    target_compile_options(FlightComputer PUBLIC "-pthread")
endif ()
if (CMAKE_THREAD_LIBS_INIT)
    message("have2")
    target_link_libraries(FlightComputer "${CMAKE_THREAD_LIBS_INIT}")

endif ()


add_executable(FlightComputer ${SOURCES} ${PROTO_HDRS})

target_link_libraries(FlightComputer ${Protobuf_LIBRARIES})
