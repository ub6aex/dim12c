PRJ_NAME   = dim12c
SRCDIR     = src
BUILD_DIR  = build
SRC        = $(wildcard $(SRCDIR)/*.c)
ASRC       = $(wildcard $(SRCDIR)/*.s)
CC         = arm-none-eabi-gcc
AS         = arm-none-eabi-gcc
OBJCOPY    = arm-none-eabi-objcopy
OBJDUMP    = arm-none-eabi-objdump
SIZE       = arm-none-eabi-size
PROGRAMMER = openocd
PGFLAGS    = -f interface/stlink.cfg -f target/stm32f0x.cfg -c "program $(BUILD_DIR)/$(PRJ_NAME).elf verify reset exit"
DEVICE     = STM32F030
OPT       ?= -Og
LDSCRIPT   = LinkerScript.ld
CFLAGS     = -fdata-sections -ffunction-sections -g3 -Wall -mcpu=cortex-m0 -mlittle-endian -mthumb -I inc/ -D $(DEVICE) $(OPT)
ASFLAGS    =  $(CFLAGS)
LDFLAGS    = -T $(LDSCRIPT) -Wl,--gc-sections --specs=nano.specs --specs=nosys.specs

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

.PHONY: all clean flash

all: $(BUILD_DIR)/$(PRJ_NAME).elf $(BUILD_DIR)/$(PRJ_NAME).hex $(BUILD_DIR)/$(PRJ_NAME).bin

-include $(wildcard $(BUILD_DIR)/*.d)

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRC:.c=.o)))
vpath %.c $(sort $(dir $(SRC)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASRC:.s=.o)))
vpath %.s $(sort $(dir $(ASRC)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(PRJ_NAME).elf: $(OBJECTS) Makefile
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(TOOLCHAIN)$(OBJCOPY) -O ihex $(BUILD_DIR)/$(PRJ_NAME).elf $(BUILD_DIR)/$(PRJ_NAME).hex
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(TOOLCHAIN)$(OBJCOPY) -O binary $(BUILD_DIR)/$(PRJ_NAME).elf $(BUILD_DIR)/$(PRJ_NAME).bin

$(BUILD_DIR):
	mkdir $@

clean:
	-rm -fR $(BUILD_DIR)

flash: all
	$(PROGRAMMER) $(PGFLAGS)
