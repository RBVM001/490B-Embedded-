#include "tm4c123gh6pm.h"
#include <stdlib.h>
// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F

//------------UART0_Init------------
// Initialize the UART for 57600 baud rate (assuming 80 MHz UART clock),
// 8 bit word length, 1 parity bit, odd, aprity one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART1_Init(void){
  SYSCTL_RCGC1_R |= 0x02; // activate UART0
  SYSCTL_RCGC2_R |= 0x02; // activate port B
	while((SYSCTL_RCGC2_R&0x02) == 0){};
  UART1_CTL_R &= ~0x01;      // disable UART
  UART1_IBRD_R = 86;                    // IBRD = int(80,000,000 / (16 * 57600)) = int(86.80555556)
  UART1_FBRD_R = 52;                    // FBRD = round(0.80555556 * 64) = 51.55555556
	//UART1_IBRD_R = 130;                 // IBRD and FBRD are configured to have a baud rate of 
  //UART1_FBRD_R = 13;									// 34800 for configuring in AT mode
  UART1_LCRH_R = 0x76;                  // 8 bit word length (odd parity, one stop bit, FIFOs)
  // UART1_LCRH_R = 0x70;               // LCRH must be set to 8-bit, no parity, one stop bit for AT mode 
  UART1_CTL_R |= 0x01;                  // enable UART
  GPIO_PORTB_AFSEL_R |= 0x03;           // enable alt funct on PB1,PB0
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTB_AMSEL_R &= ~0x03;          // disable analog functionality on PB1,PB0
  GPIO_PORTB_DEN_R |= 0x03;             // enable digital I/O on PB1,PB0
}

//------------UART0_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char UART1_InChar(void){
  while((UART1_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART1_DR_R&0xFF));
}

//------------UART0_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART1_OutChar(unsigned char data){
  while((UART1_FR_R&UART_FR_TXFF) != 0);
  UART1_DR_R = data;
}

//------------UART0_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART1_OutString(unsigned char buffer[]){
	int i=0;
  while(buffer[i]!='\0'){
    UART1_OutChar(buffer[i]);
    i++;
  }
}

//-----------------------UART_OutUDec-----------------------
// Output a 32-bit number in ASCII unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Fixed format 4 digits, one space after, CR termination
void UART1_OutUDec(unsigned long n){
		UART1_OutChar('#');  // send special character to indicate completion of Tx.
    UART1_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    UART1_OutChar(n/10+'0'); /* tens digit */
    UART1_OutChar(n%10+'0'); /* ones digit */
}

// UART 0
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
  UART0_IBRD_R = 86;                    // IBRD = int(80,000,000 / (16 * 19200)) = int(260.416666667)
  UART0_FBRD_R = 52;                    // FBRD = round(0.416666667 * 64) = 27
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs) cvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
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

//-----------------------UART_OutUDec-----------------------
// Output a 32-bit number in ASCII unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Fixed format 4 digits, one space after, CR termination
void UART_OutUDec(unsigned long n){
		UART_OutChar('#');  // send special character to indicate completion of Tx.
    UART_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    UART_OutChar(n/10+'0'); /* tens digit */
    UART_OutChar(n%10+'0'); /* ones digit */
}
