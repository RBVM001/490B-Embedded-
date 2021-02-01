/#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "init.h"
int32_t main(void){
 	//initialize UART
	initUART();
	//Initialize pins for the sole
	initSole();
	while(1){
	//reads the values from the sole
	whileUART();
	}
}





