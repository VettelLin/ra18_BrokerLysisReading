################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/app/u_can.c \
../User/app/u_cmd.c \
../User/app/u_main.c 

OBJS += \
./User/app/u_can.o \
./User/app/u_cmd.o \
./User/app/u_main.o 

C_DEPS += \
./User/app/u_can.d \
./User/app/u_cmd.d \
./User/app/u_main.d 


# Each subdirectory must supply rules for building sources it contributes
User/app/%.o User/app/%.su User/app/%.cyclo: ../User/app/%.c User/app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/ra18_BrokerLysisReading-main/User" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-app

clean-User-2f-app:
	-$(RM) ./User/app/u_can.cyclo ./User/app/u_can.d ./User/app/u_can.o ./User/app/u_can.su ./User/app/u_cmd.cyclo ./User/app/u_cmd.d ./User/app/u_cmd.o ./User/app/u_cmd.su ./User/app/u_main.cyclo ./User/app/u_main.d ./User/app/u_main.o ./User/app/u_main.su

.PHONY: clean-User-2f-app

