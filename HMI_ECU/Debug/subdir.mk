################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HMI_ECU.c \
../door_lock_HMI.c \
../keypad.c \
../lcd.c \
../timer1.c \
../uart.c \
../wd_timer.c 

OBJS += \
./HMI_ECU.o \
./door_lock_HMI.o \
./keypad.o \
./lcd.o \
./timer1.o \
./uart.o \
./wd_timer.o 

C_DEPS += \
./HMI_ECU.d \
./door_lock_HMI.d \
./keypad.d \
./lcd.d \
./timer1.d \
./uart.d \
./wd_timer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


