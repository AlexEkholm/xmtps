################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/data/xmsg/AuxiliarySensorData.cpp \
../src/data/xmsg/XCompositeSurfaceData.cpp \
../src/data/xmsg/XCompositeValve.cpp \
../src/data/xmsg/XEngIce.cpp \
../src/data/xmsg/XGPS.cpp \
../src/data/xmsg/XMfaMissionCfg.cpp \
../src/data/xmsg/XValve.cpp \
../src/data/xmsg/XWave.cpp 

CPP_DEPS += \
./src/data/xmsg/AuxiliarySensorData.d \
./src/data/xmsg/XCompositeSurfaceData.d \
./src/data/xmsg/XCompositeValve.d \
./src/data/xmsg/XEngIce.d \
./src/data/xmsg/XGPS.d \
./src/data/xmsg/XMfaMissionCfg.d \
./src/data/xmsg/XValve.d \
./src/data/xmsg/XWave.d 

OBJS += \
./src/data/xmsg/AuxiliarySensorData.o \
./src/data/xmsg/XCompositeSurfaceData.o \
./src/data/xmsg/XCompositeValve.o \
./src/data/xmsg/XEngIce.o \
./src/data/xmsg/XGPS.o \
./src/data/xmsg/XMfaMissionCfg.o \
./src/data/xmsg/XValve.o \
./src/data/xmsg/XWave.o 


# Each subdirectory must supply rules for building sources it contributes
src/data/xmsg/%.o: ../src/data/xmsg/%.cpp src/data/xmsg/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ --std=c++11 -DBOOST_ALL_DYN_LINK=1 -D'BUILD_DATETIME="$(shell date  '+%FT%R%:z')"' -D'BUILD_NUMBER=$(shell cat ../build/buildNo.txt)' -DBUILD_DEBUG=1 -O0 -g3 -Wextra -Wfloat-equal -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-data-2f-xmsg

clean-src-2f-data-2f-xmsg:
	-$(RM) ./src/data/xmsg/AuxiliarySensorData.d ./src/data/xmsg/AuxiliarySensorData.o ./src/data/xmsg/XCompositeSurfaceData.d ./src/data/xmsg/XCompositeSurfaceData.o ./src/data/xmsg/XCompositeValve.d ./src/data/xmsg/XCompositeValve.o ./src/data/xmsg/XEngIce.d ./src/data/xmsg/XEngIce.o ./src/data/xmsg/XGPS.d ./src/data/xmsg/XGPS.o ./src/data/xmsg/XMfaMissionCfg.d ./src/data/xmsg/XMfaMissionCfg.o ./src/data/xmsg/XValve.d ./src/data/xmsg/XValve.o ./src/data/xmsg/XWave.d ./src/data/xmsg/XWave.o

.PHONY: clean-src-2f-data-2f-xmsg

