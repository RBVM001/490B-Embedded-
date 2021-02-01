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

const char msg_minimumValue[] = "Minimum Value: ";
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

//incoming serial byte
int inByte = 0;

int valor = 0;               //variable for sending bytes to processing
int calibra[15][15];         //Calibration array for the min values of each of the 225 sensors.
int minsensor= 254;          //Variable for staring the min array
int multiplier = 254;
int pastmatrix[15][15];
	
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
	
//	SYSCTL_RCGCADC_R |= 0x02;
//  SYSCTL_RCGC2_R |= 0x00000010;   							// 1) activate clock for Port D
//  delay = SYSCTL_RCGC2_R;         							//  delay
//  GPIO_PORTD_DIR_R &= ~0x06;      							// 2) make PE1,PE2 input
//  GPIO_PORTD_AFSEL_R |= 0x06;    								// 3) enable alternate function on PD1
//  GPIO_PORTD_DEN_R &= ~0x06;      							// 4) disable digital I/O on PD1
//  GPIO_PORTD_AMSEL_R |= 0x06;     							// 5) enable analog function on PD1
//  SYSCTL_RCGC0_R |= 0x00020000;   							// 6) enable ADC1 (pg 458)
//  delay = SYSCTL_RCGC2_R;         
//  SYSCTL_RCGC0_R &= ~0x00000300;  							// 7) configure for 125K  (pg458)
//  ADC1_SSPRI_R = 0x0123;          							// 8) Sequencer 2 is highest priority (pg841)
//																								//  [3:0] - disable(0) enable(1) ASEN0-ASEN3
//  ADC1_EMUX_R |= 0x0F00;         							// 9) seq2 is always (continuosly sampling) (pg 833)
//																								// [15:12]- seq3, [11:8] - seq2, [7:4]-seq1, [3:0] - seq0
//  ADC1_SSMUX2_R |= 	0x0000021;									// 10) channel Ain2 (PE1, 2)  Ain1 (PE2, 1) pg801, pg875
//  ADC1_SSCTL2_R |= 0x0060;         						  // 11) disable TS0 D0, enable IE0 END0, pg876
//  ADC1_ACTSS_R |= 0x0004;         							// 12) enable sample sequencer 2 (pg821)
//	ADC1_IM_R = 0x00000004;  											// enable interrupt ss2, pg825
//	NVIC_PRI4_R = ( NVIC_PRI4_R & 0xFFFF0FFF ) | 0x00002000;	// Set interrupt priority to 1
//	NVIC_EN1_R          |=  0x00040000;           // pg 104, 134, 142	


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

void setup(){
	//Clear MUX data pins 
	int i,j,k;
	for ( i = 0; i < 4; i++){
		GPIO_PORTB_DATA_R &= ~columns[i];
		GPIO_PORTE_DATA_R &= ~rows[i];
	}
	//Set MUX data pins to 1 	
	GPIO_PORTB_DATA_R |= columns[4];
	GPIO_PORTB_DATA_R |= statusPin;
	GPIO_PORTB_DATA_R |= colPin;

	
	//Serial.begin(115200);
  
  //Serial.println("\n\Calibrating...\n");

 // Full of 0's of initial matrix
  for(j = 0; j < 15; j ++){ 
    writeMux(j);
    for(i = 0; i < 15; i ++)
      calibra[j][i] = 0;
  }
  
  // Calibration
  for(k = 0; k < 50; k++){  
    for(j = 0; j < 15; j ++){ 
      writeMux(j);
      for(i = 0; i < 15; i ++)
        calibra[j][i] = calibra[j][i] + readMux(i);
    }
  }
	
//Print averages
  for(j = 0; j < 15; j ++){ 
    writeMux(j);
    for(i = 0; i < 15; i ++){
      calibra[j][i] = calibra[j][i]/50;
      if(calibra[j][i] < minsensor)
        minsensor = calibra[j][i];
				UART0_TX( '0' );
				UART0_TX( '\t' );
    }
  UART0_TX( '\n' );
  }
  
	UART0_TX( '\n' );
	UART0_TX( '\n' );
	UART0_SendString( msg_minimumValue );
	UART0_TX( minsensor );
  UART0_TX( '\n' );
	establishContact();
	GPIO_PORTB_DATA_R &= ~colPin;
}

int readMux (char channel) {
	int i, val; 
	//improve on 
	  for ( i = 0; i < 4; i++){
		if (muxChannel[channel][i] == 0)
			GPIO_PORTE_DATA_R &= ~rows[i];
		else 
			GPIO_PORTE_DATA_R |= rows[i];
	}
  //read the value at the SIG pin
	val = 0;

	return val; 
}
void writeMux(char channel){
	int i;
	//improve on 
	  for ( i = 0; i < 4; i++){
		if (muxChannel[channel][i] == 0)
			GPIO_PORTB_DATA_R &= ~columns[i];
		else 
			GPIO_PORTB_DATA_R |= columns[i];
	}
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
	UART0_IBRD_R = 8;						// 115,200 baud integer portion
	UART0_FBRD_R = 44;						// 115,200 baud fraction portion
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
//			if( rx_data_flag ){	
//			GPIO_PORTF_DATA_R = (rx_data_byte << 1) & 0x0E;
//			rx_data_flag = 0;
//			// echo back received data
//		//	UART0_TX( rx_data_byte );
//				UART0_TX( '1' );
		//}
	//UART0_TX( '1' );
	
//Loop through and read all 16 values
  //Reports back Value at channel 6 is: 346
	int i, j, limsup;
  if (rx_data_byte > 0){
    inByte = rx_data_byte;
    
    if(inByte == 'A'){
    
      for( j = 14; j >= 0; j--){ 
        writeMux(j);
        
        for( i = 0; i < 15; i++){
            
          valor = readMux(i);
          
          //Saturation sensors
           limsup = 450;
          if(valor > limsup)
            valor = limsup;
            
          if(valor < calibra[j][i])
            valor = calibra[j][i];  
          
          valor = 0;//map(valor,minsensor, limsup,1,254); //will always send 0 to processing
          
          if(valor < 150)
            valor = 0;
          if(valor > 254)
            valor = 254;
          
          UART0_TX(valor);
					if ((GPIO_PORTB_DATA_R & colPin) == 0x08)
						GPIO_PORTB_DATA_R &= ~colPin;
					else 
						GPIO_PORTB_DATA_R |= colPin;
          //digitalWrite(COL_pin,!digitalRead(COL_pin));
					
        } 
      }
    }
        
  }
}

void DelayH(int d){unsigned long volatile time;    
  time = 727240*200/91;  // .1s or 100ms
  while(d*time){
		time--;
  }
}

void establishContact(){
	while ( rx_data_byte <= 0) { //no data
    UART0_TX('A');   // send a capital A
    DelayH(3);
  }
}

void initSole(){
	initColumns();
	initRows();
	setup(); 
}
