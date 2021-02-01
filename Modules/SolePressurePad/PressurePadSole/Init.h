#include "tm4c123gh6pm.h"
#include <stdint.h>
 #ifndef INIT_H
 #define INIT_H
 void initRows(void);
 void initColumns(void);
 void initSole(void); 
 void initWrite(void);
 void initUART(void);
 void writeMux(void);
 void UART0_TX( char c );
 void UART0_SendString( const char *string );
 void whileUART(void);
 #endif 
 