################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/RingBuffer.c \
../src/adc.c \
../src/delay.c \
../src/lcd.c \
../src/lcd_driver.c \
../src/main.c \
../src/syscalls.c \
../src/uart_driver.c 

OBJS += \
./src/RingBuffer.o \
./src/adc.o \
./src/delay.o \
./src/lcd.o \
./src/lcd_driver.o \
./src/main.o \
./src/syscalls.o \
./src/uart_driver.o 

C_DEPS += \
./src/RingBuffer.d \
./src/adc.d \
./src/delay.d \
./src/lcd.d \
./src/lcd_driver.d \
./src/main.d \
./src/syscalls.d \
./src/uart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DDEBUG -I"C:/Users/gelharrs/workspace/transistorStuff/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


