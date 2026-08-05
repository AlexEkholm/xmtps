################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/config/LocalConfiguration.cpp \
../src/config/LocalMysqlConfiguration.cpp 

OBJS += \
./src/config/LocalConfiguration.o \
./src/config/LocalMysqlConfiguration.o 

CPP_DEPS += \
./src/config/LocalConfiguration.d \
./src/config/LocalMysqlConfiguration.d 


# Each subdirectory must supply rules for building sources it contributes
src/config/%.o: ../src/config/%.cpp src/config/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ --std=c++11 -DBOOST_ALL_DYN_LINK=1 -D'BUILD_DATETIME="$(shell date  '+%FT%R%:z')"' -D'BUILD_NUMBER=$(shell cat ../build/buildNo.txt)' -O2 -c -fmessage-length=0 -Wno-missing-field-initializers -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


