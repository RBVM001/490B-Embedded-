#include "Sender.h"
// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F

//------------UART1_Init------------
// Initialize the UART for 115200 baud rate (assuming 80 MHz UART clock),
// 8 bit word length, No parity, one stop bit, FIFOs enabled
// Input: none
// Output: none
void initSender(void){
//	const char msg_welcome[] = "\n\rBegin the sole reading!\n\r";
	volatile int32_t herf;
	// initialize UART0
  SYSCTL_RCGC1_R |= 0x02; // activate UART1
  SYSCTL_RCGC2_R |= 0x04; // activate port C
	while((SYSCTL_RCGC2_R&0x04) == 0){};
  UART1_CTL_R &= ~0x01;      						// disable UART p.918
  UART1_IBRD_R = 43;                    // IBRD = int(80,000,000 / (16 * 115200)) = int(86.80555556) p.914
  UART1_FBRD_R = 26;                    // FBRD = round(0.80555556 * 64) = 51.55555556 p.915
  UART1_LCRH_R = 0x76;                  // 8 bit word length (No  parity, one stop bit, FIFOs) p.916
  UART1_CTL_R |= 0x01;                  // enable UART p.918
  GPIO_PORTC_AFSEL_R |= 0x30;           // enable alt funct on PC4,PC5
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000; //p.650
  GPIO_PORTC_AMSEL_R &= ~0x30;          // disable analog functionality on PC4,PC5
  GPIO_PORTC_DEN_R |= 0x30;             // enable digital I/O on PC4, PC5
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

//------------UART1_Echo------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART1_Echo(unsigned char *buffer){
	int i=0;
  while(buffer[i]!='\0'){
    UART1_OutChar(buffer[i]);
    i++;
  }
}

//-----------------------UART_StepOut-----------------------
// Output a 32-bit number in ASCII unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Fixed format 4 digits, one space after, CR termination
void UART1_StepOut(unsigned long n){
		UART1_OutChar(n/1000+'0'); /* hundreds digit */
		n = n%1000;
    UART1_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    UART1_OutChar(n/10+'0'); /* tens digit */
    UART1_OutChar(n%10+'0'); /* ones digit */
	//	UART1_OutChar('#');  // send special character to indicate start of new value.
}


//------------UART1_GetName------------
// Output characters to UART1 as they are received from UART1
// Input: none
// Output: none
void senderGetName(unsigned char *buffer){
	unsigned char character;
	character = UART1_InChar();
	while(character != '\x0D'){
		*buffer = character;
    buffer = buffer + 1;
		character = UART1_InChar();
	}
	*buffer = '\0';
}

void sendAxis(unsigned long x, unsigned long y, unsigned long z) { 

	UART1_Echo("X:"); 
	UART1_StepOut(x); 
	UART1_OutChar(' '); 
	UART1_Echo("Y:"); 
	UART1_StepOut(y);
	UART1_OutChar(' '); 
	UART1_Echo("Z:"); 
	UART1_StepOut(z); 	
	UART1_Echo("\n"); 		
} 

void sendAvgAxis(unsigned long x, unsigned long y, unsigned z){
	UART1_Echo("XA:"); 
	UART1_StepOut(x); 
	UART1_OutChar(' '); 
	UART1_Echo("YA:"); 
	UART1_StepOut(y);
	UART1_OutChar(' '); 
	UART1_Echo("ZA:"); 
	UART1_StepOut(z); 	
	UART1_Echo("\n"); 		
}