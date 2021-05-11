#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "init.h"
#include "PLL.h" 
#include "Storage.h"
#include "diskio.h"
#include "ff.h"
#include "Slave.h"

int32_t main(void){
 unsigned char name; 
	char begin = 0; 
	int i; 
	PLL_Init(Bus80MHz);    // 80 MHz
	initUART();
	initSole();
	initSlave();
	GPIO_PORTF_DATA_R = 0x02;
	slaveInChar();
  //slaveGetName(&name); 
		GPIO_PORTF_DATA_R = 0x08;
	storageSetup(&name);
	GPIO_PORTF_DATA_R = 0x08;
	while(1){
	
//	while (begin ==1){
//		
//		
//	}
	
}

}



