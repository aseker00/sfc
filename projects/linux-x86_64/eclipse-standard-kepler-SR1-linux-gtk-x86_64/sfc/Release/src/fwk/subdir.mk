################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/shared/dev/users/amit/sfc/src/fwk/fwkName.cpp 

OBJS += \
./src/fwk/fwkName.o 

CPP_DEPS += \
./src/fwk/fwkName.d 


# Each subdirectory must supply rules for building sources it contributes
src/fwk/fwkName.o: /mnt/shared/dev/users/amit/sfc/src/fwk/fwkName.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


