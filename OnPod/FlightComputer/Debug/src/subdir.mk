################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/FlightComputer1.cpp \
../src/PodInternalNetwork.cpp 

OBJS += \
./src/FlightComputer1.o \
./src/PodInternalNetwork.o 

CPP_DEPS += \
./src/FlightComputer1.d \
./src/PodInternalNetwork.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/lwaghorn/Development/comp4-software/FlightComputer/include" -O0 -g3 -Wall -c -fmessage-length=0  -lsocket -lnsl -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

