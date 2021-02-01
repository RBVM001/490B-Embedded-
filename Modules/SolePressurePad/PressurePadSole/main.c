//#include <stdio.h>
//#include <stdint.h>
//#include "tm4c123gh6pm.h"
//#include "init.h"




//int main (){

//	while(1){
//		
//		
//	}
//}

/*
	Read data from UART0 and display 3 bits of it on the LEDs
*/

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "init.h"
int32_t main(void){
 
	initUART();
	while(1){
		//whileUART();
		UART0_TX( 250 );
	}
}





