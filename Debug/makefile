################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include VoodooPS2Trackpad/subdir.mk
-include VoodooPS2Mouse/subdir.mk
-include VoodooPS2Keyboard/subdir.mk
-include VoodooPS2Daemon/subdir.mk
-include VoodooPS2Controller/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Release/synapticsconfigload.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Release/VoodooPS2synapticsPane.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Release/VoodooPS2Trackpad.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Release/VoodooPS2Trackpad.build/DerivedSources/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Release/VoodooPS2Mouse.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Release/VoodooPS2Mouse.build/DerivedSources/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Release/VoodooPS2Keyboard.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Release/VoodooPS2Keyboard.build/DerivedSources/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Release/VoodooPS2Daemon.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Release/VoodooPS2Controller.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Release/VoodooPS2Controller.build/DerivedSources/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Debug/synapticsconfigload.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Debug/VoodooPS2synapticsPane.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Debug/VoodooPS2Trackpad.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Debug/VoodooPS2Trackpad.build/DerivedSources/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Debug/VoodooPS2Mouse.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Debug/VoodooPS2Mouse.build/DerivedSources/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Debug/VoodooPS2Keyboard.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Debug/VoodooPS2Keyboard.build/DerivedSources/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Debug/VoodooPS2Daemon.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Debug/VoodooPS2Controller.build/Objects-normal/x86_64/subdir.mk
-include Build/Intermediates/VoodooPS2Controller.build/Debug/VoodooPS2Controller.build/DerivedSources/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: githubKextDevelop

# Tool invocations
githubKextDevelop: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	g++  -o "githubKextDevelop" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) githubKextDevelop
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
