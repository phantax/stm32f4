# The project's name
# _____________________________________________________________________________

PROJ_NAME  = stm32


# Path to the STM32F4Cube package 
# _____________________________________________________________________________

CUBE_DIR   = /home/walz/Code/STM32CubeF4/STM32Cube_FW_F4_V1.8.0


# The list of source files (wildcards may be used, but NO REVERSE PATHS)
# _____________________________________________________________________________

SRC_FILES  = ./src/*.c
SRC_FILES += ./system/src/newlib/*.c

SRC_FILES += $(CUBE_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/*.c
SRC_FILES += $(CUBE_DIR)/Drivers/BSP/STM32F4-Discovery/stm32f4_discovery.c
SRC_FILES += $(CUBE_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.s


# The list of include directories
# _____________________________________________________________________________

INC_DIRS   = ./inc
INC_DIRS  += ./system/include

INC_DIRS  += $(CUBE_DIR)/Drivers/BSP/STM32F4-Discovery
INC_DIRS  += $(CUBE_DIR)/Drivers/CMSIS/Include
INC_DIRS  += $(CUBE_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Include
INC_DIRS  += $(CUBE_DIR)/Drivers/STM32F4xx_HAL_Driver/Inc


# Preprocessor macro defines
# _____________________________________________________________________________

MACROS   = STM32F407xx
MACROS  += printf=iprintf
MACROS  += USE_FULL_ASSERT
MACROS  += USE_HAL_DRIVER


# Compiler prefix
# _____________________________________________________________________________

C_PREFIX = arm-none-eabi-


# The linker script
# _____________________________________________________________________________

LD_SCRIPTS  = $(CUBE_DIR)/Projects/STM32F4-Discovery/Templates/SW4STM32/STM32F4-Discovery/STM32F407VGTx_FLASH.ld
LD_SCRIPTS += linker/heapstack.ld


# =============================================================================

BUILD_DIR  = build

# _____________________________________________________________________________

CC       = $(C_PREFIX)gcc
OBJCOPY  = $(C_PREFIX)objcopy
OBJDUMP  = $(C_PREFIX)objdump
SIZE	 = $(C_PREFIX)size

# _____________________________________________________________________________

FLAGS    = -mlittle-endian
FLAGS   += -mthumb
FLAGS   += -mcpu=cortex-m4
#FLAGS   += -mthumb-interwork
#FLAGS   += -mfloat-abi=hard
#FLAGS   += -mfpu=fpv4-sp-d16

# _____________________________________________________________________________

CFLAGS   = $(FLAGS)
CFLAGS  += -c
CFLAGS  += -g3
CFLAGS  += -O0 
CFLAGS  += -std=gnu99

CFLAGS  += -fdata-sections
CFLAGS  += -ffunction-sections
CFLAGS  += -fno-common
CFLAGS  += -funroll-loops

CFLAGS  += -Wall
CFLAGS  += -Wshadow
CFLAGS  += -Wdeclaration-after-statement
CFLAGS  += -Wcast-align 
CFLAGS  += -Wbad-function-cast
CFLAGS  += -Wstrict-prototypes 
CFLAGS  += -Wmissing-prototypes
CFLAGS  += -Wsign-compare
CFLAGS  += -Wno-unused-parameter

CFLAGS  += $(foreach d, $(INC_DIRS), -I$d)
CFLAGS  += $(foreach d, $(MACROS), -D$d)

# _____________________________________________________________________________

LDFLAGS  = $(FLAGS)
LDFLAGS += $(foreach d, $(LD_SCRIPTS), -T$d)
#LDFLAGS += --specs=nosys.specs
#LDFLAGS += -nostartfiles
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(PROJ_NAME).map
LDFLAGS += -Xlinker --gc-sections

# _____________________________________________________________________________

OBJ_BASE_DIR  = $(BUILD_DIR)/obj
PREP_BASE_DIR = $(BUILD_DIR)/prep
ASM_BASE_DIR  = $(BUILD_DIR)/asm

EXECUTABLE    = $(BUILD_DIR)/$(PROJ_NAME).elf
BINARY        = $(BUILD_DIR)/$(PROJ_NAME).bin

SOURCES       = $(foreach d, $(SRC_FILES), $(shell ls -1 $d))
OBJECTS       = $(addprefix $(OBJ_BASE_DIR)/, $(addsuffix .o, $(SOURCES)))
PREPS         = $(addprefix $(PREP_BASE_DIR)/, $(SOURCES))
ASMS          = $(patsubst $(OBJ_BASE_DIR)/%, $(ASM_BASE_DIR)/%.s, $(OBJECTS))


all: bin size

exec: $(EXECUTABLE) 

bin: $(BINARY) 

prep: $(PREPS) 

obj: $(OBJECTS)

asm: $(ASMS)



$(EXECUTABLE): obj
	@echo "\033[01;33m==> Linking files -> '$@':\033[00;00m"
	@mkdir -p $(dir $@)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo ""

$(BINARY): $(EXECUTABLE)
	@echo "\033[01;33m==> Creating binary '$(EXECUTABLE)' -> '$@':\033[00;00m"
	@mkdir -p $(dir $@)
	$(OBJCOPY) -O binary $(EXECUTABLE) $@
	
list: $(EXECUTABLE)
	@echo "\033[01;33m==> Creating listing for '$(EXECUTABLE)' :\033[00;00m"
	$(OBJDUMP) --source --all-headers --demangle --line-number $(EXECUTABLE) > $(EXECUTABLE).lst
	
size : $(EXECUTABLE)
	@echo "\033[01;33m==> Printing size for '$(EXECUTABLE)':\033[00;00m"
	$(SIZE) --format=berkley $(EXECUTABLE)
	
$(OBJ_BASE_DIR)/%.o: %
	@echo "\033[01;32m==> Compiling '$<' -> '$@':\033[00;00m"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@
	@echo ""
	
$(PREP_BASE_DIR)/%: %
	@echo "\033[01;32m==> Preprocessing '$<' -> '$@':\033[00;00m"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -E > $@
	@echo ""
	
$(ASM_BASE_DIR)/%.s: $(OBJ_BASE_DIR)/%
	@echo "\033[01;32m==> Disassembling '$<' -> '$@':\033[00;00m"
	@mkdir -p $(dir $@)
	$(OBJDUMP) -d -S $< > $@
	@echo ""
	
clean:
	@echo "\033[01;31m==> Cleaning directories:\033[00;00m"
	rm -rf $(BUILD_DIR)/
	@echo ""

