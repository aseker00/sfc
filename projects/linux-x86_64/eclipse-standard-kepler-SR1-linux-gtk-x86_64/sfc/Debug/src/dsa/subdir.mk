################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/shared/dev/users/amit/sfc/src/dsa/dsaLinkedQueue.cpp 

OBJS += \
./src/dsa/dsaLinkedQueue.o 

CPP_DEPS += \
./src/dsa/dsaLinkedQueue.d 


# Each subdirectory must supply rules for building sources it contributes
src/dsa/dsaLinkedQueue.o: /mnt/shared/dev/users/amit/sfc/src/dsa/dsaLinkedQueue.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


