C_SRC = ../src/main.c ../src/app_dps.c
USER_OBJ = ./app/main.o ./app/app_dps.o
INC = -I../inc
 
C_SRCS += ${C_SRC}
OBJS += ${USER_OBJ}
INCS += ${INC}
 
./app/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	$(CC) $(CFLAGS) ${INCS} -o $@ -c $< ${CMACROS}
	@echo 'Finished building: $<'
	@echo ' '
