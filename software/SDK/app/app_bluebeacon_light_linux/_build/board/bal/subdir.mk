C_SRC = $(PROJECT_ROOT)/board/linux/bal/bal_beacon.c
USER_OBJ = ./board/bal/bal_beacon.o
INC = -I$(PROJECT_ROOT)/board/linux/bal
 
C_SRCS += ${C_SRC}
OBJS += ${USER_OBJ}
INCS += ${INC}
 
./board/bal/%.o: $(PROJECT_ROOT)/board/linux/bal/%.c
	@echo 'Building file: $<'
	$(CC) $(CFLAGS) ${INCS} -o $@ -c $< ${CMACROS}
	@echo 'Finished building: $<'
	@echo ' '
