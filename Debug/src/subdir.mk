################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/delay.c \
../src/lcd.c \
../src/lcd_driver.c \
../src/main.c 

OBJS += \
./src/delay.o \
./src/lcd.o \
./src/lcd_driver.o \
./src/main.o 

C_DEPS += \
./src/delay.d \
./src/lcd.d \
./src/lcd_driver.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mthumb -mfloat-abi=soft -I"C:/Users/gelharrs/workspace/Networking1/inc" -O0 -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


