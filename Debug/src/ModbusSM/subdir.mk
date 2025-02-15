################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ModbusSM/ModbusSmMaster.cpp \
../src/ModbusSM/ModbusSmMasterLinkLayer.cpp \
../src/ModbusSM/ModbusSmSlave.cpp \
../src/ModbusSM/ModbusSmSlaveLinkLayer.cpp 

CPP_DEPS += \
./src/ModbusSM/ModbusSmMaster.d \
./src/ModbusSM/ModbusSmMasterLinkLayer.d \
./src/ModbusSM/ModbusSmSlave.d \
./src/ModbusSM/ModbusSmSlaveLinkLayer.d 

OBJS += \
./src/ModbusSM/ModbusSmMaster.o \
./src/ModbusSM/ModbusSmMasterLinkLayer.o \
./src/ModbusSM/ModbusSmSlave.o \
./src/ModbusSM/ModbusSmSlaveLinkLayer.o 


# Each subdirectory must supply rules for building sources it contributes
src/ModbusSM/%.o: ../src/ModbusSM/%.cpp src/ModbusSM/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\EveDisplayTest\EveDisplayTest\src" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\EveDisplayTest\EveDisplayTest\src\CommunicationDevice" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\EveDisplayTest\EveDisplayTest\src\EveDisplay" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\EveDisplayTest\EveDisplayTest\src\Modbus" -I"D:\Projects\CurrentProjects\CautoProjects\EclipceProjects\EveDisplayTest\EveDisplayTest\src\ModbusSM" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-ModbusSM

clean-src-2f-ModbusSM:
	-$(RM) ./src/ModbusSM/ModbusSmMaster.d ./src/ModbusSM/ModbusSmMaster.o ./src/ModbusSM/ModbusSmMasterLinkLayer.d ./src/ModbusSM/ModbusSmMasterLinkLayer.o ./src/ModbusSM/ModbusSmSlave.d ./src/ModbusSM/ModbusSmSlave.o ./src/ModbusSM/ModbusSmSlaveLinkLayer.d ./src/ModbusSM/ModbusSmSlaveLinkLayer.o

.PHONY: clean-src-2f-ModbusSM

