################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/NodeTelem.pb.cc \
../src/PodCommand.pb.cc \
../src/PodTelem.pb.cc \
../src/States.pb.cc \
../src/easylogging++.cc 

CPP_SRCS += \
../src/Client.cpp \
../src/Commander.cpp \
../src/CoreControl.cpp \
../src/FlightComputer.cpp \
../src/NodeServer.cpp \
../src/Pod.cpp 

CC_DEPS += \
./src/NodeTelem.pb.d \
./src/PodCommand.pb.d \
./src/PodTelem.pb.d \
./src/States.pb.d \
./src/easylogging++.d 

OBJS += \
./src/Client.o \
./src/Commander.o \
./src/CoreControl.o \
./src/FlightComputer.o \
./src/NodeServer.o \
./src/NodeTelem.pb.o \
./src/Pod.o \
./src/PodCommand.pb.o \
./src/PodTelem.pb.o \
./src/States.pb.o \
./src/easylogging++.o 

CPP_DEPS += \
./src/Client.d \
./src/Commander.d \
./src/CoreControl.d \
./src/FlightComputer.d \
./src/NodeServer.d \
./src/Pod.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I.././include -I../include/FlightComputer -I../include/ProtoBuffer -O0 -g3 -Wall -c -fmessage-length=0  -lsocket -lnsl -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I.././include -I../include/FlightComputer -I../include/ProtoBuffer -O0 -g3 -Wall -c -fmessage-length=0  -lsocket -lnsl -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


