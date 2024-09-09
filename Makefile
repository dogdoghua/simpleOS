CROSS_COMPILER_PREFIX = aarch64-linux-gnu
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles -I$(INC_DIR)

all: clean kernel8.img

assembly.o: assembly.S
	$(CROSS_COMPILER_PREFIX)-gcc $(CFLAGS) -c assembly.S -o assembly.o

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CROSS_COMPILER_PREFIX)-gcc $(CFLAGS) -c $< -o $@

kernel8.img: assembly.o $(OBJS)
	$(CROSS_COMPILER_PREFIX)-ld -nostdlib -nostartfiles assembly.o $(OBJS) -T linker.ld -o kernel8.elf
	$(CROSS_COMPILER_PREFIX)-objcopy -O binary kernel8.elf kernel8.img

clean: 
	rm kernel8.elf $(OBJ_DIR)/*.o > /dev/null 2> /dev/null || true

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -display none -serial null -serial stdio