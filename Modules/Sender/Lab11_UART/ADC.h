#include "tm4c123gh6pm.h"

// This initialization function sets up the ADC 
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source: channel 1
// SS3 interrupts: enabled but not promoted to controller
void ADC0_Init(void){
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x10;				// 1) activate clock for port E
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_DIR_R &= ~0x04;			 	// 2) make PE2 input
	GPIO_PORTE_AFSEL_R |= 0x04; 			// 3) enable alternate function on PE4
	GPIO_PORTE_DEN_R &= ~0x04; 			// 4) disable digital I/O on PE4
	GPIO_PORTE_AMSEL_R |= 0x04; 		// 5) enable analog function on PE4
	SYSCTL_RCGCADC_R |= 0x01; 				// 6) activate ADC0
	delay = SYSCTL_RCGCADC_R;					// delay to stabilize
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	ADC0_PC_R = 0x01; 												// 7) configure for 125K
	ADC0_SSPRI_R = 0x0123; 								// 8) Sequencer 3 is highest priority
	ADC0_ACTSS_R &= ~0x0008; 					// 9) disable sample sequencer 3
	ADC0_EMUX_R &= ~0xF000; 						// 10) seq3 is software trigger
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0) 	+ 1; // 11) clear SS3 field; set channel Ain1
	ADC0_SSCTL3_R = 0x0006; 							// 12) no TS0 D0, yes IE0 END0
	ADC0_SAC_R = 6; 											// samples 64 returns average
	ADC0_IM_R &= ~0x0008; 									// 13) disable SS3 interrupts
	ADC0_ACTSS_R |= 0x0008; 							// 14) enable sample sequencer 3
}



//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
unsigned long ADC0_In(void){
	long data;
	ADC0_PSSI_R = 0x0008;										// 1) initiate SS3
	while((ADC0_RIS_R&0x08)==0){};				// 2) wait for conversion done
	data = ADC0_SSFIFO3_R&0xFFF;			// 3) read 12-bit result
	ADC0_ISC_R = 0x0008;											// 4) acknowledge completion
	return data;
}
