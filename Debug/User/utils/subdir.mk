################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/utils/l_crc.c \
../User/utils/l_math.c \
../User/utils/l_md5.c \
../User/utils/l_net.c \
../User/utils/l_number.c \
../User/utils/l_string.c 

OBJS += \
./User/utils/l_crc.o \
./User/utils/l_math.o \
./User/utils/l_md5.o \
./User/utils/l_net.o \
./User/utils/l_number.o \
./User/utils/l_string.o 

C_DEPS += \
./User/utils/l_crc.d \
./User/utils/l_math.d \
./User/utils/l_md5.d \
./User/utils/l_net.d \
./User/utils/l_number.d \
./User/utils/l_string.d 


# Each subdirectory must supply rules for building sources it contributes
User/utils/%.o User/utils/%.su User/utils/%.cyclo: ../User/utils/%.c User/utils/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/ra18_BrokerLysisReading-main/User" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-utils

clean-User-2f-utils:
	-$(RM) ./User/utils/l_crc.cyclo ./User/utils/l_crc.d ./User/utils/l_crc.o ./User/utils/l_crc.su ./User/utils/l_math.cyclo ./User/utils/l_math.d ./User/utils/l_math.o ./User/utils/l_math.su ./User/utils/l_md5.cyclo ./User/utils/l_md5.d ./User/utils/l_md5.o ./User/utils/l_md5.su ./User/utils/l_net.cyclo ./User/utils/l_net.d ./User/utils/l_net.o ./User/utils/l_net.su ./User/utils/l_number.cyclo ./User/utils/l_number.d ./User/utils/l_number.o ./User/utils/l_number.su ./User/utils/l_string.cyclo ./User/utils/l_string.d ./User/utils/l_string.o ./User/utils/l_string.su

.PHONY: clean-User-2f-utils

