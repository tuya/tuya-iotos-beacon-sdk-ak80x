C_SRC = $(PROJECT_ROOT)/board/linux/hal/hal_flash.c $(PROJECT_ROOT)/board/linux/hal/hal_uart.c $(PROJECT_ROOT)/board/linux/hal/hal_gpio.c $(PROJECT_ROOT)/board/linux/hal/hal_clock.c $(PROJECT_ROOT)/board/linux/hal/hal_irq.c $(PROJECT_ROOT)/board/linux/hal/hal_storage_256.c
USER_OBJ = ./board/hal/hal_flash.o ./board/hal/hal_uart.o ./board/hal/hal_gpio.o ./board/hal/hal_clock.o ./board/hal/hal_irq.o ./board/hal/hal_storage_256.o
INC = -I$(PROJECT_ROOT)/board/linux/hal
 
C_SRCS += ${C_SRC}
OBJS += ${USER_OBJ}
INCS += ${INC}
 
./board/hal/%.o: $(PROJECT_ROOT)/board/linux/hal/%.c
	@echo 'Building file: $<'
	$(CC) $(CFLAGS) ${INCS} -o $@ -c $< ${CMACROS}
	@echo 'Finished building: $<'
	@echo ' '
