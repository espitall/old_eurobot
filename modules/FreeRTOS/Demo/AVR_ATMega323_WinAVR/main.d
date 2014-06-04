main.o main.d : main.c ../../Source/include/FreeRTOS.h \
 ../../Source/include/projdefs.h FreeRTOSConfig.h \
 ../../Source/include/portable.h \
 ../../Source/include/../portable/GCC/ATMega323/portmacro.h \
 ../../Source/include/mpu_wrappers.h ../../Source/include/task.h \
 ../../Source/include/list.h ../../Source/include/croutine.h \
 ../Common/include/PollQ.h ../Common/include/integer.h \
 ../Common/include/serial.h ../Common/include/comtest.h \
 ../Common/include/crflash.h ../Common/include/print.h \
 ../Common/include/partest.h regtest.h
