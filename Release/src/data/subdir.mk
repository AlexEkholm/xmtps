################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/data/XFloat.cpp \
../src/data/XFloatDiveData.cpp \
../src/data/XFloatMailMessage.cpp \
../src/data/XFloatMetadata.cpp 

CPP_DEPS += \
./src/data/XFloat.d \
./src/data/XFloatDiveData.d \
./src/data/XFloatMailMessage.d \
./src/data/XFloatMetadata.d 

OBJS += \
./src/data/XFloat.o \
./src/data/XFloatDiveData.o \
./src/data/XFloatMailMessage.o \
./src/data/XFloatMetadata.o 


# Each subdirectory must supply rules for building sources it contributes
src/data/%.o: ../src/data/%.cpp src/data/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ --std=c++11 -DBOOST_ALL_DYN_LINK=1 -D'BUILD_DATETIME="$(shell date  '+%FT%R%:z')"' -D'BUILD_NUMBER=$(shell cat ../build/buildNo.txt)' -O2 -g -Wextra -Wfloat-equal -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-data

clean-src-2f-data:
	-$(RM) ./src/data/XFloat.d ./src/data/XFloat.o ./src/data/XFloatDiveData.d ./src/data/XFloatDiveData.o ./src/data/XFloatMailMessage.d ./src/data/XFloatMailMessage.o ./src/data/XFloatMetadata.d ./src/data/XFloatMetadata.o

.PHONY: clean-src-2f-data

