#include "UART.h"
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

#define LEDs (*((volatile unsigned long *)0x40025038))	// PF[3:1]
unsigned char c;

void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode

void Delay(unsigned long ms){unsigned long volatile time;
  time = (727240*200/45)*ms;  // 0.1sec
  while(time){
		time--;
  }
}

void PortF_Init(void){
  uint32_t volatile delay;
	SYSCTL_RCGC2_R |= 0x20;							// 1) F clock
	delay = SYSCTL_RCGC2_R;							//    delay
	GPIO_PORTF_CR_R |= 0x0E;						// 2) allow changes to PF[3:1]
	GPIO_PORTF_DIR_R |= 0x0E;						//	  set PF[3:1] as outputs
	GPIO_PORTF_DEN_R |= 0x1E;						// 5)  enable digital pin PF[3:1]
}

int main(void){
		unsigned long i = 0;
	
  TExaS_Init();             // initialize grader, set system clock to 80 MHz
  UART1_Init();              // initialize UART
	UART_Init();
	PortF_Init();             // Initilaize Port F for LEDs
  EnableInterrupts();       // needed for TExaS
	
	
// The lines below were used to configure the HC-05 in AT mode	
// Baud rate had to be set to 34800 and LCRH to 0x70 when in AT mode
//	delay(100);
//	UART1_OutString("AT+NAME=CECS447\r\n");
//	delay(100);
//	UART1_OutString("AT+UART=57600,0,1\r\n");
//	delay(100);
//	UART1_OutString("AT+PSWD=7497\r\n");
//	delay(100);
//	UART1_OutString("AT+ROLE=0\r\n");
	while(1){
		c = UART_InChar();
		/*switch(c) {
			case '*' :
				for(i = 0; i < 120; i++){
					UART1_OutUDec(i);
					UART_OutUDec(i);
				}
				UART1_OutChar('\n');
				UART_OutChar('\n');
				break;
			case '#' :
				for(i = 0; i < 120; i++){
					UART1_OutUDec(i+20);
					UART_OutUDec(i+20);
				}
				UART1_OutChar('\n');
				UART_OutChar('\n');
				break;
			case '!' : 
				for(i = 0; i < 120 ; i++){
					UART1_OutUDec(i+40);
					UART_OutUDec(i+40);
				}
				UART1_OutChar('\n');
				UART_OutChar('\n');
				break;
			} */
			
			UART1_OutChar('*');
			UART_OutChar('*');
		for(i = 0; i < 120; i++){
				UART1_OutUDec(i);
				UART_OutUDec(i);
			}
			UART1_OutChar('\n');
			UART_OutChar('\n');
			Delay(1);
			UART1_OutChar('*');
			UART_OutChar('*');
		for(i = 0; i < 120; i++){
			UART1_OutUDec(i+20);
			UART_OutUDec(i+20);
		}
		UART1_OutChar('\n');
		UART_OutChar('\n');
		Delay(1);
		UART1_OutChar('*');
			UART_OutChar('*');
		for(i = 0; i < 120 ; i++){
			UART1_OutUDec(i+40);
			UART_OutUDec(i+40);
		}
		UART1_OutChar('\n');
				UART_OutChar('\n');
		
	}
}
