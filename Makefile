ifeq ($(OS),Windows_NT)
    RM := rmdir /s /q
    FIX_BLANK := 
else
    RM := rm -rf
endif

CC=arm-none-eabi-gcc
MACH=cortex-m4
SOURCE_DIR=src
INCLUDE_DIR=inc
BUILD_DIR=build
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -O0 -Wall -g -I $(INCLUDE_DIR)
LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=nano.specs -T stm32_ls.ld -Wl,-Map=$(BUILD_DIR)/final.map
LDFLAGS_SH= -mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=rdimon.specs -lc -lrdimon -T stm32_ls.ld -Wl,-Map=$(BUILD_DIR)/final.map

all:$(BUILD_DIR)/main.o $(BUILD_DIR)/led.o $(BUILD_DIR)/stm32_startup.o $(BUILD_DIR)/syscalls.o $(BUILD_DIR)/final.elf

# recipe for semihosting
semi:$(BUILD_DIR)/main.o $(BUILD_DIR)/led.o $(BUILD_DIR)/stm32_startup.o $(BUILD_DIR)/syscalls.o $(BUILD_DIR)/final_shosting.elf

clean:
	$(RM) $(BUILD_DIR)
	
$(BUILD_DIR)/main.o:$(SOURCE_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/led.o:$(SOURCE_DIR)/led.c
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/stm32_startup.o:$(SOURCE_DIR)/stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/syscalls.o:$(SOURCE_DIR)/syscalls.c
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/final.elf:$(BUILD_DIR)/main.o $(BUILD_DIR)/led.o $(BUILD_DIR)/syscalls.o $(BUILD_DIR)/stm32_startup.o
	$(CC) $(LDFLAGS) -o $@ $^

# to test semhosting feature
$(BUILD_DIR)/final_shosting.elf:$(BUILD_DIR)/main.o $(BUILD_DIR)/led.o $(BUILD_DIR)/stm32_startup.o
	$(CC) $(LDFLAGS_SH) -o $@ $^

load:
	openocd -f board/st_nucleo_f4.cfg