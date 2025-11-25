################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/algorithm/control/pid.c 

OBJS += \
./User/algorithm/control/pid.o 

C_DEPS += \
./User/algorithm/control/pid.d 


# Each subdirectory must supply rules for building sources it contributes
User/algorithm/control/%.o User/algorithm/control/%.su User/algorithm/control/%.cyclo: ../User/algorithm/control/%.c User/algorithm/control/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/ra18_BrokerLysisReading-main/User" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-algorithm-2f-control

clean-User-2f-algorithm-2f-control:
	-$(RM) ./User/algorithm/control/pid.cyclo ./User/algorithm/control/pid.d ./User/algorithm/control/pid.o ./User/algorithm/control/pid.su

.PHONY: clean-User-2f-algorithm-2f-control

