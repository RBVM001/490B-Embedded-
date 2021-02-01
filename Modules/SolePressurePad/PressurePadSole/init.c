#include "init.h"
#include <stdbool.h>
#include <stdint.h>
int8_t rx_data_byte, rx_data_flag;

void UART0_SendString( const char * );
void UART0_TX( char c );

//Defines for the muxes 
#define RS0 0x08 //S0 - PE3
#define RS1 0x04 //S1 - PE2
#define RS2 0x02 //S2 - PE1
#define RS3 0x01 //S3 - PE0
#define RSZ 0x02 //Z  - PD1

#define CS0 0x80 //S0 - PB6
#define CS1 0x40 //S1 - PB5 
#define CS2 0x20 //S2 - PB4
#define CS3 0x02 //S3 - PB1 
#define CSZ 0x01 //Z  - PB0

char columns [5] = {CS0,CS1,CS2,CS3,CSZ};
char rows [5] = {RS0, RS1, RS2, RS3, RSZ};

//Status and Column pins default for arduino mini pro 
#define statusPin 0x04 // PB2
#define colPin  0x08   // PB3

const bool muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

	
void initRows(){volatile unsigned long delay;
//PD1 - Z,PE0 - S3,PE1 - S2,PE2 - S1,PE3 - S0
	SYSCTL_RCGC2_R |= 0x00000010;     // 1) E
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTE_CR_R |= 0x0F;           // allow changes to PE0-PE3  
  GPIO_PORTE_AMSEL_R &= ~(0x0F);        // 3) disable analog function
  GPIO_PORTE_PCTL_R &= ~(0x00001111);   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R |= 0x0F;             // 5) PE0-PE3 output   
  GPIO_PORTE_AFSEL_R &= ~(0x0F);        // 6) no alternate function      
  GPIO_PORTE_DEN_R |= 0x0F;            // 7) enable digital pins PE0-PE3
	
	SYSCTL_RCGC2_R |= 0x00000008;     // 1) D
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTD_CR_R |= 0x02;           // allow changes to PD1
  GPIO_PORTD_AMSEL_R &= ~(0x02);        // 3) disable analog function
  GPIO_PORTD_PCTL_R &= ~(0x00000010);   // 4) GPIO clear bit PCTL  
  GPIO_PORTD_DIR_R |= 0x02;             // 5) PD1 output   
  GPIO_PORTD_AFSEL_R &= ~(0x02);        // 6) no alternate function      
  GPIO_PORTD_DEN_R |= 0x02;            // 7) enable digital pins PD1


}
	
void initColumns(){volatile unsigned long delay;
//PB0 - Z,PB1 - S3,PB4 - S2,PB5 - S1,PB6 - S0
	SYSCTL_RCGC2_R |= 0x00000002;     // 1) B
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTB_CR_R |= 0xFF;           // allow changes to PB0-PB7  
  GPIO_PORTB_AMSEL_R &= ~(0xFF);        // 3) disable analog function
  GPIO_PORTB_PCTL_R &= ~(0x11111111);   // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R |= 0xFF;             // 5) PB0-PB7 output   
  GPIO_PORTB_AFSEL_R &= ~(0xFF);        // 6) no alternate function      
  GPIO_PORTB_DEN_R |= 0xFF;            // 7) enable digital pins PB0-PB7
	
}

void initWrite(){
	//Clear MUX data pins 
	int i;
	for ( i = 0; i < 4; i++){
		GPIO_PORTB_DATA_R &= ~columns[i];
		GPIO_PORTE_DATA_R &= ~rows[i];
	}
	//Set MUX data pins to 1 
	GPIO_PORTB_DATA_R |= statusPin;
	GPIO_PORTB_DATA_R |= colPin;
	GPIO_PORTB_DATA_R |= columns[4];
	
	//Serial.begin(115200);
  
  //Serial.println("\n\Calibrating...\n");

	
}
void writeMux(){
	//initColumns();
}

void initUART(){
	const char msg_welcome[] = "\n\rBegin the sole reading!\n\r";
	const int number = 5; 
	volatile int32_t herf;
	// initialize PORTF
	SYSCTL_RCGCGPIO_R |= 0x20;	// enable clock to Port F
	herf = SYSCTL_RCGCGPIO_R;
	GPIO_PORTF_DIR_R |= 0x0E;		// PF3,2,1 output
	GPIO_PORTF_DEN_R |= 0x0E;		// PF3,2,1 digital
	
	// initialize UART0
	SYSCTL_RCGCGPIO_R |= 0x01;		// enable clock to PORTA
	herf = SYSCTL_RCGCGPIO_R;
	GPIO_PORTA_DEN_R = 0x03;			// PA1,0 are digital
	GPIO_PORTA_AFSEL_R = 0x03;		// PA1,0 alternate function enable
	GPIO_PORTA_PCTL_R = 0x00000011;			// PA1,0 configured for UART
	
	SYSCTL_RCGCUART_R |= 0x01;		// enable clock to UART0
	herf = SYSCTL_RCGCGPIO_R;
	UART0_CTL_R = 0;							// disable UART0
	UART0_IBRD_R = 8;						// 9600 baud integer portion
	UART0_FBRD_R = 44;						// 9600 baud fraction portion
	UART0_CC_R = 0;								// UART0 timed using System clock
	UART0_LCRH_R = 0x60;					// 8-bit, no parity, 1-stop, no FIFO
	UART0_IM_R |= 0x0010;					// enable TX, RX interrupt
	UART0_CTL_R = 0x301;					// enable UART0, TX, RX
	NVIC_PRI1_R |= 3 << 13;		// set interrupt priority to 3
	NVIC_EN0_R |= 0x00000020;	// enable IRQ5 for UART0
	
	herf = NVIC_EN0_R;
	UART0_SendString( msg_welcome );
}

void UART0_Handler(void){
	volatile int32_t readback;
	if(UART0_MIS_R & 0x0010){
		rx_data_byte = UART0_DR_R;
		rx_data_flag = 1;
		UART0_ICR_R = 0x0010;
		readback = UART0_ICR_R;
	}
	else{
		UART0_ICR_R = UART0_MIS_R;	// clear all interrutp flags
		readback = UART0_ICR_R;			// force flags clear
	}	
}

void UART0_SendString( const char *string ){
	int i; 
	for( i = 0; string[i] != 0; i++ ){
		UART0_TX( string[i] );
	}
}

void UART0_TX( char c ){
	while( (UART0_FR_R & 0x20 ) != 0 );
	UART0_DR_R = c;
}

void whileUART(){
			if( rx_data_flag ){	
			GPIO_PORTF_DATA_R = (rx_data_byte << 1) & 0x0E;
			rx_data_flag = 0;
			// echo back received data
		//	UART0_TX( rx_data_byte );
				UART0_TX( '1' );
		}
}


void initSole(){
	initColumns();
	initRows();
	initWrite(); 
}