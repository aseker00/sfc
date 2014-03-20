################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/shared/dev/users/amit/sfc/src/fwk/utest/fwkUtestStringTest.cpp 

OBJS += \
./src/fwk/utest/fwkUtestStringTest.o 

CPP_DEPS += \
./src/fwk/utest/fwkUtestStringTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/fwk/utest/fwkUtestStringTest.o: /mnt/shared/dev/users/amit/sfc/src/fwk/utest/fwkUtestStringTest.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


