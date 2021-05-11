#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "init.h"
#include "PLL.h" 
#include "Sender.h" 
#include "Storage.h"
#include "diskio.h"
#include "ff.h"
#include "Master.h"

int32_t main(void){
 unsigned char name; 
	char begin = 0; 
	int i; 
	PLL_Init(Bus80MHz);    // 80 MHz
	initUART();
	initSole();
	initSender(); 
	initMaster();
	senderGetName(&name);
	storageSetup(&name);
	signalMasterSlave(); 
	while(1){
	senderGetName(&name);
	
	storageSetup(&name);
	
//	while (begin ==1){
//		
//		
//	}
	
}

}



