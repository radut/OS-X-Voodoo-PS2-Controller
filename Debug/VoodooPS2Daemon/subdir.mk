################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../VoodooPS2Daemon/main.cpp 

OBJS += \
./VoodooPS2Daemon/main.o 

CPP_DEPS += \
./VoodooPS2Daemon/main.d 


# Each subdirectory must supply rules for building sources it contributes
VoodooPS2Daemon/%.o: ../VoodooPS2Daemon/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


