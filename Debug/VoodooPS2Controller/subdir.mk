################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../VoodooPS2Controller/AppleACPIPS2Nub.cpp \
../VoodooPS2Controller/ApplePS2Device.cpp \
../VoodooPS2Controller/ApplePS2KeyboardDevice.cpp \
../VoodooPS2Controller/ApplePS2MouseDevice.cpp \
../VoodooPS2Controller/VoodooPS2Controller.cpp 

OBJS += \
./VoodooPS2Controller/AppleACPIPS2Nub.o \
./VoodooPS2Controller/ApplePS2Device.o \
./VoodooPS2Controller/ApplePS2KeyboardDevice.o \
./VoodooPS2Controller/ApplePS2MouseDevice.o \
./VoodooPS2Controller/VoodooPS2Controller.o 

CPP_DEPS += \
./VoodooPS2Controller/AppleACPIPS2Nub.d \
./VoodooPS2Controller/ApplePS2Device.d \
./VoodooPS2Controller/ApplePS2KeyboardDevice.d \
./VoodooPS2Controller/ApplePS2MouseDevice.d \
./VoodooPS2Controller/VoodooPS2Controller.d 


# Each subdirectory must supply rules for building sources it contributes
VoodooPS2Controller/%.o: ../VoodooPS2Controller/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


