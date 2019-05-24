################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/fileparser.c \
../src/hashmap.c \
../src/main.c \
../src/workerhelper.c 

OBJS += \
./src/fileparser.o \
./src/hashmap.o \
./src/main.o \
./src/workerhelper.o 

C_DEPS += \
./src/fileparser.d \
./src/hashmap.d \
./src/main.d \
./src/workerhelper.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/cristian/eclipse-workspace/WordCount/inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


