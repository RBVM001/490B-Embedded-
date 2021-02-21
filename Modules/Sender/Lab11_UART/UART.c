#include "tm4c123gh6pm.h"
#include "UART.h"
#include <stdlib.h>

//------------UART_Init------------
// Initialize the UART for 19200 baud rate (assuming 80 MHz UART clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART_Init(void){
  SYSCTL_RCGC1_R |= 0x01; // activate UART0
  SYSCTL_RCGC2_R |= 0x01; // activate port A
	while((SYSCTL_RCGC2_R&0x01) == 0){};
  UART0_CTL_R &= ~0x01;      // disable UART
  UART0_IBRD_R = 260;                    // IBRD = int(80,000,000 / (16 * 19200)) = int(260.416666667)
  UART0_FBRD_R = 27;                    // FBRD = round(0.416666667 * 64) = 27
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART0_LCRH_R = 0x70;
  UART0_CTL_R |= 0x01;                  // enable UART
  GPIO_PORTA_AFSEL_R |= 0x03;           // enable alt funct on PA1,PA0
                                        // configure PA1,PA0 as UART0
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTA_AMSEL_R &= ~0x03;          // disable analog functionality on PA1,PA0
  GPIO_PORTA_DEN_R |= 0x03;             // enable digital I/O on PA1,PA0
}

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char UART_InChar(void){
  while((UART0_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART0_DR_R&0xFF));
}

//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(unsigned char data){
  while((UART0_FR_R&UART_FR_TXFF) != 0);
  UART0_DR_R = data;
}

//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(unsigned char buffer[]){
	int i=0;
  while(buffer[i]!='\0'){
    UART_OutChar(buffer[i]);
    i++;
  }
}

