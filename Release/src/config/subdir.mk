################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/config/LocalConfiguration.cpp \
../src/config/LocalMysqlConfiguration.cpp 

CPP_DEPS += \
./src/config/LocalConfiguration.d \
./src/config/LocalMysqlConfiguration.d 

OBJS += \
./src/config/LocalConfiguration.o \
./src/config/LocalMysqlConfiguration.o 


# Each subdirectory must supply rules for building sources it contributes
src/config/%.o: ../src/config/%.cpp src/config/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ --std=c++11 -DBOOST_ALL_DYN_LINK=1 -D'BUILD_DATETIME="$(shell date  '+%FT%R%:z')"' -D'BUILD_NUMBER=$(shell cat ../build/buildNo.txt)' -O2 -g -Wextra -Wfloat-equal -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-config

clean-src-2f-config:
	-$(RM) ./src/config/LocalConfiguration.d ./src/config/LocalConfiguration.o ./src/config/LocalMysqlConfiguration.d ./src/config/LocalMysqlConfiguration.o

.PHONY: clean-src-2f-config

