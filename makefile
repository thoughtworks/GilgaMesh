#------------------------------------------------------------------------------
# Firmware build
#
# Selectable build options
#------------------------------------------------------------------------------
TARGET_BOARD         ?= BOARD_PCA10028

#------------------------------------------------------------------------------
# Define relative paths to SDK components
#------------------------------------------------------------------------------

SDK_BASE      := ./deploy/sdk/nrf_sdk_9_0
COMPONENTS    := $(SDK_BASE)/components
TEMPLATE_PATH := $(COMPONENTS)/toolchain/gcc
EHAL_PATH     := ./deploy/EHAL
LINKER_SCRIPT := ./linker/gcc_nrf51_s130_32kb.ld
OUTPUT_NAME   := MeshyMesh

#------------------------------------------------------------------------------
# Proceed cautiously beyond this point.  Little should change.
#------------------------------------------------------------------------------

export OUTPUT_NAME
#export GNU_INSTALL_ROOT
GNU_INSTALL_ROOT := ./deploy/gcc-arm-none-eabi/
GNU_VERSION      := 5.2.1
GNU_PREFIX       := arm-none-eabi
#ifeq ($(OS),Windows_NT)
#  include $(TEMPLATE_PATH)/Makefile.windows
#else
#  include $(TEMPLATE_PATH)/Makefile.posix
#endif

MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) )

# echo suspend
ifeq ("$(VERBOSE)","1")
  NO_ECHO :=
else
  NO_ECHO := @
endif

ifeq ($(MAKECMDGOALS),debug)
  BUILD_TYPE := debug
else
  BUILD_TYPE := release
endif

# Toolchain commands
CC       := "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc"
OBJCOPY  := "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy"
SIZE     := "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size"
MK       := mkdir
RM       := rm -rf

# function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

# source common to all targets

C_SOURCE_FILES += $(EHAL_PATH)/ARM/Nordic/nRF51/src/Vectors_nRF51.c
C_SOURCE_FILES += $(COMPONENTS)/libraries/timer/app_timer.c
C_SOURCE_FILES += $(COMPONENTS)/libraries/util/app_util_platform.c
C_SOURCE_FILES += $(COMPONENTS)/libraries/gpiote/app_gpiote.c
C_SOURCE_FILES += $(COMPONENTS)/drivers_nrf/gpiote/nrf_drv_gpiote.c
C_SOURCE_FILES += $(COMPONENTS)/drivers_nrf/common/nrf_drv_common.c
C_SOURCE_FILES += $(COMPONENTS)/ble/ble_radio_notification/ble_radio_notification.c
C_SOURCE_FILES += $(COMPONENTS)/drivers_nrf/hal/nrf_delay.c
C_SOURCE_FILES += $(COMPONENTS)/drivers_nrf/pstorage/pstorage.c
C_SOURCE_FILES += $(COMPONENTS)/softdevice/common/softdevice_handler/softdevice_handler.c
C_SOURCE_FILES += ./src/nrf/simple_uart.c
C_SOURCE_FILES += ./src/connection.c
C_SOURCE_FILES += ./src/conversion.c
C_SOURCE_FILES += ./src/gap.c
C_SOURCE_FILES += ./src/gatt.c
C_SOURCE_FILES += ./src/led.c
C_SOURCE_FILES += ./src/logger.c
C_SOURCE_FILES += ./src/terminal.c
C_SOURCE_FILES += ./src/command.c
C_SOURCE_FILES += ./src/timer.c
C_SOURCE_FILES += ./src/error.c
C_SOURCE_FILES += ./src/dfu.c
C_SOURCE_FILES += ./src/main.c

# includes common to all targets

INC_PATHS += -I./inc
INC_PATHS += -I./inc/nrf
INC_PATHS += -I./config

#arm GCC

#nordic nrf51
INC_PATHS += -I$(COMPONENTS)/ble/ble_radio_notification
INC_PATHS += -I$(COMPONENTS)/ble/ble_services/ble_dfu
INC_PATHS += -I$(COMPONENTS)/ble/common
INC_PATHS += -I$(COMPONENTS)/device
INC_PATHS += -I$(COMPONENTS)/libraries/timer
INC_PATHS += -I$(COMPONENTS)/libraries/button
INC_PATHS += -I$(COMPONENTS)/libraries/util
INC_PATHS += -I$(COMPONENTS)/libraries/gpiote
INC_PATHS += -I$(COMPONENTS)/libraries/bootloader_dfu
INC_PATHS += -I$(COMPONENTS)/softdevice/common/softdevice_handler
INC_PATHS += -I$(COMPONENTS)/softdevice/s130/headers
INC_PATHS += -I$(COMPONENTS)/toolchain
INC_PATHS += -I$(COMPONENTS)/toolchain/arm
INC_PATHS += -I$(COMPONENTS)/toolchain/gcc
INC_PATHS += -I$(COMPONENTS)/drivers_nrf/pstorage
INC_PATHS += -I$(COMPONENTS)/drivers_nrf/hal
INC_PATHS += -I$(COMPONENTS)/drivers_nrf/config
INC_PATHS += -I$(COMPONENTS)/drivers_nrf/common
INC_PATHS += -I$(COMPONENTS)/drivers_nrf/gpiote
INC_PATHS += -I$(COMPONENTS)/drivers_nrf/uart

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

ifeq ($(BUILD_TYPE),debug)
  DEBUG_FLAGS += -D DEBUG -g -O0
else
  DEBUG_FLAGS += -D NDEBUG -O3
endif

CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb
CFLAGS += -Og
CFLAGS += -fmessage-length=0
CFLAGS += -fsigned-char
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -flto
CFLAGS += -fno-move-loop-invariants
CFLAGS += -Wextra
CFLAGS += -g3
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += $(DEBUG_FLAGS)
#CFLAGS += $(DFU_DEBUG_FLAGS)
CFLAGS += -D$(TARGET_BOARD)
CFLAGS += -DNRF51
CFLAGS += -D__need___va_list
CFLAGS += -w
CFLAGS += -fabi-version=0
CFLAGS += -fno-exceptions
CFLAGS += -fno-rtti
CFLAGS += -fno-use-cxa-atexit
CFLAGS += -fno-threadsafe-statics

CFLAGS += -DDEST_BOARD_ID=0

# Helpful flags for minimizing bin file
#CFLAGS += -DENABLE_LOGGING #disabled to minimize
#CFLAGS += -v -Wall
#LDFLAGS += -v

LDFLAGS += -mcpu=cortex-m0
LDFLAGS += -mthumb
LDFLAGS += -Og
LDFLAGS += -fmessage-length=0
LDFLAGS += -fsigned-char
LDFLAGS += -ffunction-sections
LDFLAGS += -flto
LDFLAGS += -fno-move-loop-invariants
LDFLAGS += -Wextra
LDFLAGS += -g3
LDFLAGS += -T$(LINKER_SCRIPT)
LDFLAGS += -Xlinker
LDFLAGS += --gc-sections
LDFLAGS += -Wl,-Map,"_build/MeshyMesh.map"
LDFLAGS += --specs=nano.specs

LIBS += -L$(EHAL_PATH)/ARM/src
LIBS += -L$(EHAL_PATH)/ARM/Nordic/nRF51/CMSIS/Debug
LIBS += -lCMSIS

C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

TOOLCHAIN_BASE = $(basename $(notdir $(GNU_INSTALL_ROOT)))

TIMESTAMP := $(shell date +'%s')

vpath %.c $(C_PATHS)

OBJECTS = $(C_OBJECTS)

all: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_NAME).elf
	$(NO_ECHO)$(CC) $(LDFLAGS) $(LIBS) $(INC_PATHS) $(OBJECTS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

	@echo "*****************************************************"
	@echo "build project: $(OUTPUT_NAME)"
	@echo "build type:    $(BUILD_TYPE)"
	@echo "build with:    $(TOOLCHAIN_BASE)"
	@echo "build target:  $(TARGET_BOARD)"
	@echo "build products --"
	@echo "               $(OUTPUT_NAME).elf"
	@echo "               $(OUTPUT_NAME).hex"
	@echo "*****************************************************"

debug : all
release : all

# Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) -std=gnu99 $(CFLAGS) $(INC_PATHS) -c $< -o $@ > $(OUTPUT_BINARY_DIRECTORY)/$*.lst

# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_NAME).elf
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf

# Create binary .bin file from the .elf file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf
	@echo Preparing: $(OUTPUT_NAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).bin

# Create binary .hex file from the .elf file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf
	@echo Preparing: $(OUTPUT_NAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_NAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).bin

# Create binary .hex file from the .elf file
genhex: 
	@echo Preparing: $(OUTPUT_NAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).hex

echosize:
	-@echo ""
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf
	-@echo ""

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o
