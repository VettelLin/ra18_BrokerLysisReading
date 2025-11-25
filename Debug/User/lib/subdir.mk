################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/lib/l_can.c \
../User/lib/l_cmd.c \
../User/lib/l_delay.c \
../User/lib/l_gpio.c \
../User/lib/l_timer.c \
../User/lib/l_uart.c 

OBJS += \
./User/lib/l_can.o \
./User/lib/l_cmd.o \
./User/lib/l_delay.o \
./User/lib/l_gpio.o \
./User/lib/l_timer.o \
./User/lib/l_uart.o 

C_DEPS += \
./User/lib/l_can.d \
./User/lib/l_cmd.d \
./User/lib/l_delay.d \
./User/lib/l_gpio.d \
./User/lib/l_timer.d \
./User/lib/l_uart.d 


# Each subdirectory must supply rules for building sources it contributes
User/lib/%.o User/lib/%.su User/lib/%.cyclo: ../User/lib/%.c User/lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/ra18_BrokerLysisReading-main/User" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-lib

clean-User-2f-lib:
	-$(RM) ./User/lib/l_can.cyclo ./User/lib/l_can.d ./User/lib/l_can.o ./User/lib/l_can.su ./User/lib/l_cmd.cyclo ./User/lib/l_cmd.d ./User/lib/l_cmd.o ./User/lib/l_cmd.su ./User/lib/l_delay.cyclo ./User/lib/l_delay.d ./User/lib/l_delay.o ./User/lib/l_delay.su ./User/lib/l_gpio.cyclo ./User/lib/l_gpio.d ./User/lib/l_gpio.o ./User/lib/l_gpio.su ./User/lib/l_timer.cyclo ./User/lib/l_timer.d ./User/lib/l_timer.o ./User/lib/l_timer.su ./User/lib/l_uart.cyclo ./User/lib/l_uart.d ./User/lib/l_uart.o ./User/lib/l_uart.su

.PHONY: clean-User-2f-lib

