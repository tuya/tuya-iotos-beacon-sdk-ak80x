-include sources.mk
-include objects.mk

#用所有中间文件生成目的文件，规则中可以用 $^替换掉 ${OBJECT}  
${BIN_TARGET}: ${OBJS} 
	@echo ' '
	@echo '> begin create app'
	$(CC) -o $@ ${OBJS} ${CMACROS} -pthread -lbluetooth -L${PROJECT_ROOT}/sdk/lib -ltuya_beacon_sdk_linux_v2.0.0

  
build: ${BIN_TARGET}
	@echo ' '
	@echo '> build finished'
	@echo ${BIN_TARGET}

clean:
	@echo '> clean all'
	rm -rf ${PROJECT_ROOT}/build/app/*.o
	rm -rf ${OBJS} ${BIN_TARGET}

run:
	./${BIN_TARGET}
