################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm

# All of the sources participating in the build are defined here
-include sources.mk
-include src/utility/subdir.mk
-include src/data/xmsg/common/subdir.mk
-include src/data/xmsg/subdir.mk
-include src/data/subdir.mk
-include src/config/subdir.mk
-include src/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := xmtps
BUILD_ARTIFACT_EXTENSION :=
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory pre-build && $(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: xmtps

# Tool invocations
xmtps: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/usr/lib -L/usr/local/lib -o "xmtps" $(OBJS) $(USER_OBJS) $(LIBS) -lboost_chrono -lboost_date_time -lboost_filesystem -lboost_iostreams -lboost_log -lboost_log_setup -lboost_program_options -lboost_regex -lboost_thread -lkmlbase -lkmldom -lkmlengine -lmysqlcppconn -lpthread -lvmime
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) xmtps
	-@echo ' '

pre-build:
	-if [ -f /var/lock/xmtps/lock.pid ]; then rm /var/lock/xmtps/lock.pid; fi
	-@echo ' '

post-build:
	-./../build/incrementBuild.sh; #cd ../doc;  doxygen doxy.cfg
	-@echo ' '

.PHONY: all clean dependents main-build pre-build post-build

-include ../makefile.targets
