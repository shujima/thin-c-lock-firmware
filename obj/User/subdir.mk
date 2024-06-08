################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/I2C.c \
../User/I2C_LED_IS31FL3731.c \
../User/I2C_RTC_BU9873.c \
../User/TC_Buttons.c \
../User/TC_Clock.c \
../User/TC_LED_Matrix.c \
../User/ch32v20x_it.c \
../User/main.c \
../User/system_ch32v20x.c 

OBJS += \
./User/I2C.o \
./User/I2C_LED_IS31FL3731.o \
./User/I2C_RTC_BU9873.o \
./User/TC_Buttons.o \
./User/TC_Clock.o \
./User/TC_LED_Matrix.o \
./User/ch32v20x_it.o \
./User/main.o \
./User/system_ch32v20x.o 

C_DEPS += \
./User/I2C.d \
./User/I2C_LED_IS31FL3731.d \
./User/I2C_RTC_BU9873.d \
./User/TC_Buttons.d \
./User/TC_Clock.d \
./User/TC_LED_Matrix.d \
./User/ch32v20x_it.d \
./User/main.d \
./User/system_ch32v20x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Develop\Hobby_ECAD\thin-c-lock-firmware\Debug" -I"C:\Develop\Hobby_ECAD\thin-c-lock-firmware\Core" -I"C:\Develop\Hobby_ECAD\thin-c-lock-firmware\User" -I"C:\Develop\Hobby_ECAD\thin-c-lock-firmware\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

