################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/data/xmsg/common/XPresCodeCounts.cpp \
../src/data/xmsg/common/XValveCounts.cpp \
../src/data/xmsg/common/XWaveSpectralSample.cpp 

CPP_DEPS += \
./src/data/xmsg/common/XPresCodeCounts.d \
./src/data/xmsg/common/XValveCounts.d \
./src/data/xmsg/common/XWaveSpectralSample.d 

OBJS += \
./src/data/xmsg/common/XPresCodeCounts.o \
./src/data/xmsg/common/XValveCounts.o \
./src/data/xmsg/common/XWaveSpectralSample.o 


# Each subdirectory must supply rules for building sources it contributes
src/data/xmsg/common/%.o: ../src/data/xmsg/common/%.cpp src/data/xmsg/common/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ --std=c++11 -DBOOST_ALL_DYN_LINK=1 -D'BUILD_DATETIME="$(shell date  '+%FT%R%:z')"' -D'BUILD_NUMBER=$(shell cat ../build/buildNo.txt)' -O2 -g -Wextra -Wfloat-equal -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-data-2f-xmsg-2f-common

clean-src-2f-data-2f-xmsg-2f-common:
	-$(RM) ./src/data/xmsg/common/XPresCodeCounts.d ./src/data/xmsg/common/XPresCodeCounts.o ./src/data/xmsg/common/XValveCounts.d ./src/data/xmsg/common/XValveCounts.o ./src/data/xmsg/common/XWaveSpectralSample.d ./src/data/xmsg/common/XWaveSpectralSample.o

.PHONY: clean-src-2f-data-2f-xmsg-2f-common

