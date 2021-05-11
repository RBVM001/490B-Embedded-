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
#define RSZ 0x10 //Z  - PE4

#define CS0 0x40 //S0 - PB6
#define CS1 0x20 //S1 - PB5 
#define CS2 0x10 //S2 - PB4
#define CS3 0x02 //S3 - PB1 
#define CSZ 0x80 //Z  - PB7

//Look up table for rows and columns
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
//PE4 - Z,PE0 - S3,PE1 - S2,PE2 - S1,PE3 - S0
	SYSCTL_RCGC2_R |= 0x00000010;     // 1) E
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTE_CR_R |= 0x0F;           // allow changes to PE0-PE3  
  GPIO_PORTE_AMSEL_R &= ~(0x0F);        // 3) disable analog function
  GPIO_PORTE_PCTL_R &= ~(0x00001111);   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R |= 0x0F;             // 5) PE0-PE3 output   
  GPIO_PORTE_AFSEL_R &= ~(0x0F);        // 6) no alternate function      
  GPIO_PORTE_DEN_R |= 0x0F;            // 7) enable digital pins PE0-PE3
	
  SYSCTL_RCGCADC_R |= 0x0001;   // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= 0x10;    // 2) activate clock for Port E
  while((SYSCTL_PRGPIO_R&0x10) != 0x10){};  // 3 for stabilization
  GPIO_PORTE_DIR_R &= ~0x10;    // 4) make PE4 input
  GPIO_PORTE_AFSEL_R |= 0x10;   // 5) enable alternate function on PE4
  GPIO_PORTE_DEN_R &= ~0x10;    // 6) disable digital I/O on PE4
  GPIO_PORTE_AMSEL_R |= 0x10;   // 7) enable analog functionality on PE4
// while((SYSCTL_PRADC_R&0x0001) != 0x0001){}; // good code, but not implemented in simulator
  ADC0_PC_R &= ~0xF;
  ADC0_PC_R |= 0x1;             // 8) configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123;        // 9) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;      // 10) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;       // 11) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;
  ADC0_SSMUX3_R += 9;           // 12) set channel
  ADC0_SSCTL3_R = 0x0006;       // 13) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;         // 14) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;       // 15) enable sample sequencer 3

}
	
void initColumns(){volatile unsigned long delay;
//PB0 - Z,PB1 - S3,PB4 - S2,PB5 - S1,PB6 - S0
	SYSCTL_RCGC2_R |= 0x00000002;     // 1) B
  delay = SYSCTL_RCGC2_R;          // delay   
  GPIO_PORTB_CR_R |= 0xFE;           // allow changes to PB0-PB7  
  GPIO_PORTB_AMSEL_R &= ~(0xFE);        // 3) disable analog function
  GPIO_PORTB_PCTL_R &= ~(0x11111110);   // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R |= 0xFE;             // 5) PB0-PB7 output   
  GPIO_PORTB_AFSEL_R &= ~(0xFE);        // 6) no alternate function      
  GPIO_PORTB_DEN_R |= 0xFE;            // 7) enable digital pins PB0-PB7
	
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
 
	val = ADC0_InSeq3();
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
	volatile int32_t herf;
	// initialize UART0
	SYSCTL_RCGCGPIO_R |= 0x01;		// enable clock to PORTA
	herf = SYSCTL_RCGCGPIO_R;
	GPIO_PORTA_DEN_R = 0x03;			// PA1,0 are digital
	GPIO_PORTA_AFSEL_R = 0x03;		// PA1,0 alternate function enable
	GPIO_PORTA_PCTL_R = 0x00000011;			// PA1,0 configured for UART
	
	SYSCTL_RCGCUART_R |= 0x01;		// enable clock to UART0
	herf = SYSCTL_RCGCGPIO_R;
	UART0_CTL_R = 0;							// disable UART0
	UART0_IBRD_R = 43;						// 115,200 baud integer portion
	UART0_FBRD_R = 26;						// 115,200 baud fraction portion
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
		UART0_ICR_R = UART0_MIS_R;	// clear all interrutpt flags
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
           limsup = 1600;
          if(valor > limsup)
            valor = limsup;
            
          if(valor < calibra[j][i])
            valor = calibra[j][i];  
          
					valor = map(valor,minsensor, limsup,1,254);         
          if(valor < 150)
            valor = 0;
          if(valor > 254)
            valor = 254;
          
					
          UART0_TX(valor);
					if ((GPIO_PORTB_DATA_R & colPin) == 0x08)
						GPIO_PORTB_DATA_R &= ~colPin;
					else 
						GPIO_PORTB_DATA_R |= colPin;
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

uint32_t ADC0_InSeq3(void){  uint32_t result;

  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3

  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done

  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result

  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion

  return result;
}
long map (long valor, long minsensor, long limsup, long out_min, long out_max) {
	return (valor - minsensor) * (out_max - out_min) / (limsup - minsensor) + out_min;
}

	
void initSole(){
	initColumns();
	initRows();
//	setup(); 
}