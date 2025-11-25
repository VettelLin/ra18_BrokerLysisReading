################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ra18/busmsg.c \
../User/ra18/can_msg.c \
../User/ra18/heating_dock.c \
../User/ra18/iamp_msg.c 

OBJS += \
./User/ra18/busmsg.o \
./User/ra18/can_msg.o \
./User/ra18/heating_dock.o \
./User/ra18/iamp_msg.o 

C_DEPS += \
./User/ra18/busmsg.d \
./User/ra18/can_msg.d \
./User/ra18/heating_dock.d \
./User/ra18/iamp_msg.d 


# Each subdirectory must supply rules for building sources it contributes
User/ra18/%.o User/ra18/%.su User/ra18/%.cyclo: ../User/ra18/%.c User/ra18/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/ra18_BrokerLysisReading-main/User" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-ra18

clean-User-2f-ra18:
	-$(RM) ./User/ra18/busmsg.cyclo ./User/ra18/busmsg.d ./User/ra18/busmsg.o ./User/ra18/busmsg.su ./User/ra18/can_msg.cyclo ./User/ra18/can_msg.d ./User/ra18/can_msg.o ./User/ra18/can_msg.su ./User/ra18/heating_dock.cyclo ./User/ra18/heating_dock.d ./User/ra18/heating_dock.o ./User/ra18/heating_dock.su ./User/ra18/iamp_msg.cyclo ./User/ra18/iamp_msg.d ./User/ra18/iamp_msg.o ./User/ra18/iamp_msg.su

.PHONY: clean-User-2f-ra18

