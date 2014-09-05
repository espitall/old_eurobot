SRCS = Source/croutine.c Source/event_groups.c Source/list.c Source/queue.c Source/tasks.c Source/timers.c Source/portable/MemMang/heap_4.c


ifeq ($(HOST),avr)
MODULES = xmega/clock
else ifeq ($(HOST), arm)
SRCS +=  Source/portable/GCC/ARM_CM4F/port.c Source/portable/GCC/ARM_CM4F/syscalls.c
endif

#SRCS += FreeRTOS-Plus-IO/Common/FreeRTOS_DriverInterface.c FreeRTOS-Plus-IO/Common/FreeRTOS_IOUtils.c FreeRTOS-Plus-IO/Common/IOUtils_CharQueueTxAndRx.c FreeRTOS-Plus-IO/Common/IOUtils_CircularBufferRx.c FreeRTOS-Plus-IO/Common/IOUtils_ZeroCopyTx.c FreeRTOS-Plus-IO/Device/xmega/FreeRTOS_xmega_DriverInterface.c FreeRTOS-Plus-IO/Device/xmega/FreeRTOS_xmega_uart.c FreeRTOS-Plus-IO/Device/xmega/FreeRTOS_xmega_spi.c

MODULE_INCLUDE_DIRS = Source/include Source/portable/GCC/ARM_CM4F/
#FreeRTOS-Plus-IO/Include
#Source/portable/GCC/ARM_CM4F

MODULES = CMSIS
