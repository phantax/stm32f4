#!/bin/bash

if [ -z ${OPENOCD_PATH+x} ]
then
    echo "Please set variable OPENOCD_PATH to your OpenOCD base directory"
    exit
fi

openocd -f ${OPENOCD_PATH}/tcl/board/stm32f4discovery.cfg

