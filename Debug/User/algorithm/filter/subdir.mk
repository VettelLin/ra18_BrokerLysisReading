################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/algorithm/filter/ad_filter.c \
../User/algorithm/filter/u_filter.c 

OBJS += \
./User/algorithm/filter/ad_filter.o \
./User/algorithm/filter/u_filter.o 

C_DEPS += \
./User/algorithm/filter/ad_filter.d \
./User/algorithm/filter/u_filter.d 


# Each subdirectory must supply rules for building sources it contributes
User/algorithm/filter/%.o User/algorithm/filter/%.su User/algorithm/filter/%.cyclo: ../User/algorithm/filter/%.c User/algorithm/filter/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/ra18_BrokerLysisReading-main/User" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-algorithm-2f-filter

clean-User-2f-algorithm-2f-filter:
	-$(RM) ./User/algorithm/filter/ad_filter.cyclo ./User/algorithm/filter/ad_filter.d ./User/algorithm/filter/ad_filter.o ./User/algorithm/filter/ad_filter.su ./User/algorithm/filter/u_filter.cyclo ./User/algorithm/filter/u_filter.d ./User/algorithm/filter/u_filter.o ./User/algorithm/filter/u_filter.su

.PHONY: clean-User-2f-algorithm-2f-filter

