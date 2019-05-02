cmake_minimum_required(VERSION 3.13)
project(FlightComputer)

set(CMAKE_CXX_STANDARD 14)

include_directories(include/EasyLogger)
include_directories(include/FlightComputer)
include_directories(include/ProtoBuffer)
include_directories(include)

file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB SOURCES "src/*.c*")





find_package(Threads REQUIRED)
if (THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(FlightComputer PUBLIC "-pthread")
endif ()
if (CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(FlightComputer "${CMAKE_THREAD_LIBS_INIT}")
endif ()


find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})



add_executable(FlightComputer ${SOURCES}  ${PROTO_HDRS})


target_link_libraries(FlightComputer ${Protobuf_LIBRARIES})
