################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../VoodooPS2Keyboard/VoodooPS2Keyboard.cpp 

OBJS += \
./VoodooPS2Keyboard/VoodooPS2Keyboard.o 

CPP_DEPS += \
./VoodooPS2Keyboard/VoodooPS2Keyboard.d 


# Each subdirectory must supply rules for building sources it contributes
VoodooPS2Keyboard/%.o: ../VoodooPS2Keyboard/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


