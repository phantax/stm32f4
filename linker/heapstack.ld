__stack = ORIGIN(CCMRAM) + LENGTH(CCMRAM);
__Main_Stack_Size = 16384;

PROVIDE ( _Heap_Begin = ORIGIN(CCMRAM) ) ;
PROVIDE ( _Heap_Limit = __stack - __Main_Stack_Size ) ;
