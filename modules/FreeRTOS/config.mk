SRCS = Source/croutine.c Source/event_groups.c Source/list.c Source/portable/GCC/ATXMega128a1u/port.c Source/queue.c Source/tasks.c Source/timers.c Source/portable/MemMang/heap_1.c

SRCS += FreeRTOS-Plus-IO/Common/FreeRTOS_DriverInterface.c FreeRTOS-Plus-IO/Common/FreeRTOS_IOUtils.c FreeRTOS-Plus-IO/Common/IOUtils_CharQueueTxAndRx.c FreeRTOS-Plus-IO/Common/IOUtils_CircularBufferRx.c FreeRTOS-Plus-IO/Common/IOUtils_ZeroCopyTx.c FreeRTOS-Plus-IO/Device/xmega/FreeRTOS_xmega_DriverInterface.c FreeRTOS-Plus-IO/Device/xmega/FreeRTOS_xmega_uart.c

MODULE_INCLUDE_DIRS = Source/include Source/portable/GCC/ATXMega128a1u FreeRTOS-Plus-IO/Include

MODULES = xmega/clock
