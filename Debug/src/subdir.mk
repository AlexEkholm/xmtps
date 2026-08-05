################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Main.cpp \
../src/stage1_MailRetrievalThread.cpp \
../src/stage2_MailProcessingThread.cpp \
../src/stage3_TelemetryDecoderThread.cpp \
../src/stage4b_PostProcessingThread.cpp 

CPP_DEPS += \
./src/Main.d \
./src/stage1_MailRetrievalThread.d \
./src/stage2_MailProcessingThread.d \
./src/stage3_TelemetryDecoderThread.d \
./src/stage4b_PostProcessingThread.d 

OBJS += \
./src/Main.o \
./src/stage1_MailRetrievalThread.o \
./src/stage2_MailProcessingThread.o \
./src/stage3_TelemetryDecoderThread.o \
./src/stage4b_PostProcessingThread.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ --std=c++11 -DBOOST_ALL_DYN_LINK=1 -D'BUILD_DATETIME="$(shell date  '+%FT%R%:z')"' -D'BUILD_NUMBER=$(shell cat ../build/buildNo.txt)' -DBUILD_DEBUG=1 -O0 -g3 -Wextra -Wfloat-equal -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/Main.d ./src/Main.o ./src/stage1_MailRetrievalThread.d ./src/stage1_MailRetrievalThread.o ./src/stage2_MailProcessingThread.d ./src/stage2_MailProcessingThread.o ./src/stage3_TelemetryDecoderThread.d ./src/stage3_TelemetryDecoderThread.o ./src/stage4b_PostProcessingThread.d ./src/stage4b_PostProcessingThread.o

.PHONY: clean-src

