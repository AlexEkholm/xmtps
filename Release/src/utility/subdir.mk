################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/utility/KmlUtil.cpp \
../src/utility/Log.cpp \
../src/utility/MysqlUtils.cpp \
../src/utility/PositionUtils.cpp \
../src/utility/SqlTableDescription.cpp 

OBJS += \
./src/utility/KmlUtil.o \
./src/utility/Log.o \
./src/utility/MysqlUtils.o \
./src/utility/PositionUtils.o \
./src/utility/SqlTableDescription.o 

CPP_DEPS += \
./src/utility/KmlUtil.d \
./src/utility/Log.d \
./src/utility/MysqlUtils.d \
./src/utility/PositionUtils.d \
./src/utility/SqlTableDescription.d 


# Each subdirectory must supply rules for building sources it contributes
src/utility/%.o: ../src/utility/%.cpp src/utility/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ --std=c++11 -DBOOST_ALL_DYN_LINK=1 -D'BUILD_DATETIME="$(shell date  '+%FT%R%:z')"' -D'BUILD_NUMBER=$(shell cat ../build/buildNo.txt)' -O2 -c -fmessage-length=0 -Wno-missing-field-initializers -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


