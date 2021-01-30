#include <stdio.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"




int main (){
	SYSCTL_RCGCADC_R |= 0x02;
  SYSCTL_RCGC2_R |= 0x00000010;   							// 1) activate clock for Port E
  GPIO_PORTE_DIR_R &= ~0x06;      							// 2) make PE1 input
  GPIO_PORTE_AFSEL_R |= 0x06;    								// 3) enable alternate function on PE1,PE2
  GPIO_PORTE_DEN_R &= ~0x06;      							// 4) disable digital I/O on PE1, PE2
  GPIO_PORTE_AMSEL_R |= 0x06;     							// 5) enable analog function on PE1, PE2
  SYSCTL_RCGC0_R |= 0x00020000;   							// 6) enable ADC1 (pg 458)
  SYSCTL_RCGC0_R &= ~0x00000300;  							// 7) configure for 125K  (pg458)
  ADC1_SSPRI_R = 0x0123;          							// 8) Sequencer 2 is highest priority (pg841)
																								//  [3:0] - disable(0) enable(1) ASEN0-ASEN3
  ADC1_EMUX_R |= 0x0F00;         							// 9) seq2 is always (continuosly sampling) (pg 833)
																								// [15:12]- seq3, [11:8] - seq2, [7:4]-seq1, [3:0] - seq0
  ADC1_SSMUX2_R |= 	0x0000021;									// 10) channel Ain2 (PE1, 2)  Ain1 (PE2, 1) pg801, pg875
  ADC1_SSCTL2_R |= 0x0060;         						  // 11) disable TS0 D0, enable IE0 END0, pg876
  ADC1_ACTSS_R |= 0x0004;         							// 12) enable sample sequencer 2 (pg821)
	ADC1_IM_R = 0x00000004;  											// enable interrupt ss2, pg825
	NVIC_PRI4_R = ( NVIC_PRI4_R & 0xFFFF0FFF ) | 0x00002000;	// Set interrupt priority to 1
	NVIC_EN1_R          |=  0x00040000;           // pg 104, 134, 142
}