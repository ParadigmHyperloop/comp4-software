################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/ProtoStructs.pb.cc 

CPP_SRCS += \
../src/FlightComputer.cpp \
../src/MainControlLoop.cpp \
../src/MemoryAccess.cpp \
../src/NodeServer.cpp \
../src/PodInternalNetwork.cpp \
../src/nodeSim.cpp 

CC_DEPS += \
./src/ProtoStructs.pb.d 

OBJS += \
./src/FlightComputer.o \
./src/MainControlLoop.o \
./src/MemoryAccess.o \
./src/NodeServer.o \
./src/PodInternalNetwork.o \
./src/ProtoStructs.pb.o \
./src/nodeSim.o 

CPP_DEPS += \
./src/FlightComputer.d \
./src/MainControlLoop.d \
./src/MemoryAccess.d \
./src/NodeServer.d \
./src/PodInternalNetwork.d \
./src/nodeSim.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I.././include -I../include/FlightComputer -O0 -g3 -Wall -c -fmessage-length=0  -lsocket -lnsl -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I.././include -I../include/FlightComputer -O0 -g3 -Wall -c -fmessage-length=0  -lsocket -lnsl -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


