################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../VoodooPS2Trackpad/VoodooPS2ALPSGlidePoint.cpp \
../VoodooPS2Trackpad/VoodooPS2SentelicFSP.cpp \
../VoodooPS2Trackpad/VoodooPS2SynapticsTouchPad.cpp \
../VoodooPS2Trackpad/VoodooPS2TouchPadBase.cpp 

OBJS += \
./VoodooPS2Trackpad/VoodooPS2ALPSGlidePoint.o \
./VoodooPS2Trackpad/VoodooPS2SentelicFSP.o \
./VoodooPS2Trackpad/VoodooPS2SynapticsTouchPad.o \
./VoodooPS2Trackpad/VoodooPS2TouchPadBase.o 

CPP_DEPS += \
./VoodooPS2Trackpad/VoodooPS2ALPSGlidePoint.d \
./VoodooPS2Trackpad/VoodooPS2SentelicFSP.d \
./VoodooPS2Trackpad/VoodooPS2SynapticsTouchPad.d \
./VoodooPS2Trackpad/VoodooPS2TouchPadBase.d 


# Each subdirectory must supply rules for building sources it contributes
VoodooPS2Trackpad/%.o: ../VoodooPS2Trackpad/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


