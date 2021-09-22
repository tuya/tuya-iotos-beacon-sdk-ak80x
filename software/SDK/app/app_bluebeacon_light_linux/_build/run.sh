#!/bin/bash
OPTION=$1
PARAM1=$2
APP_DIR=..

function build(){
    echo " "
	echo "> [build-step3]---subdir.mk--of--app----------------------"
	{
        MAKEFILE_DIR=./app
        MAKEFILE_FILE=$MAKEFILE_DIR/subdir.mk
        if [ -d "$MAKEFILE_DIR" ]; then
            rm $MAKEFILE_DIR/*.*
        else
            mkdir -p $MAKEFILE_DIR
        fi

		C_DIR=`find .. | grep ".*\.c$" | sed 's:/[a-z|A-Z|0-9|_]*\.c$::g' | sort | uniq`
        I_DIR=`find .. | grep ".*\.h$" | sed 's:/[a-z|A-Z|0-9|_]*\.h$::g' | sed 's:\(.*\):-I\1:g' | sort | uniq`
		O_DIR=./app
        
        C_FILES=$(find .. | grep "\.c$" | sed 's:^./:'$APP_DIR'/:g')
		O_FILES=$(find .. -type f -printf "%f\n" | grep ".c$" | sed 's:\.c:\.o:g' | sed 's:^:'$O_DIR'/:g')


        echo "C_SRC = "$C_FILES >> $MAKEFILE_FILE 
        echo "USER_OBJ = "$O_FILES >> $MAKEFILE_FILE
        echo "INC = "$I_DIR >> $MAKEFILE_FILE
        echo " " >> $MAKEFILE_FILE   

        echo 'C_SRCS += ${C_SRC}' >> $MAKEFILE_FILE
        echo 'OBJS += ${USER_OBJ}' >> $MAKEFILE_FILE
        echo 'INCS += ${INC}' >> $MAKEFILE_FILE
      
        for SRC_PATH in $C_DIR
        do
            echo " " >> $MAKEFILE_FILE
            echo './app/%.o: '$SRC_PATH'/%.c' >> $MAKEFILE_FILE
            echo "	@echo 'Building file: $<'" >> $MAKEFILE_FILE
            echo "	@echo 'Invoking: TC32 Compiler'" >> $MAKEFILE_FILE
            echo '	$(CC) $(CFLAGS) ${INCS} -o $@ -c $< ${CMACROS}' >> $MAKEFILE_FILE
            echo "	@echo 'Finished building: $<'" >> $MAKEFILE_FILE
            echo "	@echo ' '" >> $MAKEFILE_FILE
        done

		echo "makefile is: "$MAKEFILE_FILE
		cat $MAKEFILE_FILE
    }

    echo " "
	echo "> [build-step4]-------------------------------------------"
	{
        make build       
    }
}


echo "bash run.sh $option[build/clean/run/auto] $param1[null]"

if [ "$OPTION" == "build" ]; then
    build
elif [ "$OPTION" == "clean" ]; then
    make clean
elif [ "$OPTION" == "run" ]; then
    make run
elif [ "$OPTION" == "auto" ]; then
    make clean
    build
    make run
else
    echo "error, try bash run.sh help"
fi

