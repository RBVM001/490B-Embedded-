#include "Slave.h" 
#include <stdbool.h>
#include <stdint.h>

void initSlave(){
	volatile int32_t herf;
//	// initialize UART3
  SYSCTL_RCGCUART_R |= 0x08; // activate UART3 p.460
  SYSCTL_RCGCGPIO_R |= 0x04; // activate port C p.464
  UART3_CTL_R &= ~0x01;      						// disable UART p.918
  UART3_IBRD_R = 43;                    // IBRD = int(80,000,000 / (16 * 115200)) = int(86.80555556) p.914
  UART3_FBRD_R = 26;                    // FBRD = round(0.80555556 * 64) = 51.55555556 p.915
  UART3_LCRH_R = 0x76;                  // 8 bit word length (No  parity, one stop bit, FIFOs) p.916
  UART3_CTL_R |= 0x01;                  // enable UART p.918
  GPIO_PORTC_AFSEL_R |= 0xC0;           // enable alt funct on PD6,PD7
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0x00FFFFFF)+0x11000000; //p.650
  GPIO_PORTC_AMSEL_R &= ~0xC0;          // disable analog functionality on PD6,PD7
  GPIO_PORTC_DEN_R |= 0xC0;             // enable digital I/O on PD6,PD7
}

void signalSlaveMaster(void){
	while (slaveInChar()!= 254){
	slaveOutChar(253);
	}
	
}

void slaveOutChar(unsigned char data){
  while((UART3_FR_R&UART_FR_TXFF) != 0);
  UART3_DR_R = data;
}

//------------UART3_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART3_OutString(unsigned char buffer[]){
	int i=0;
  while(buffer[i]!='\0'){
    slaveOutChar(buffer[i]);
    i++;
  }
}
unsigned char slaveInChar(void){
  while((UART3_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART3_DR_R&0xFF));
}
//------------UART3_GetName------------
// Output characters to UART1 as they are received from UART1
// Input: none
// Output: none
void slaveGetName(unsigned char *buffer){
	unsigned char character;
	character = slaveInChar();
	while(character != '\x0D'){
		*buffer = character;
    buffer = buffer + 1;
		character = slaveInChar();
	}
	*buffer = '\0';
}

//------------UART3_Echo------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART3_Echo(unsigned char *buffer){
	int i=0;
  while(buffer[i]!='\0'){
    slaveOutChar(buffer[i]);
    i++;
  }
}