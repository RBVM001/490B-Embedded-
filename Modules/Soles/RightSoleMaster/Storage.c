#include "Storage.h"
#include <stdbool.h>
#include <stdint.h>
//Attributes 
static FATFS Fats;
FIL userHandle, pressureLogHandle; 
const char userFilename[] = "User.txt";  
const char pressureLogFilename [] = "PLog.txt"; 
FRESULT MountFresultSole;
FRESULT FresultFile;
//
char sessionNumber; 
void storageSetup (char name []) {
	mountCard(); 
	createUserFile(); 
	closeFile(userHandle);
	setupUserFile("0000", name);
	closeFile(userHandle);
	createPressureLog();
	sessionNumber = 0; 
	initSession(); 
	closeFile(pressureLogHandle);
}
FRESULT mountCard(void){
	return f_mount(&Fats, "", 0);
}
 // open the file to be written
  // Options:
  // FA_CREATE_NEW    - Creates a new file, only if it does not already exist.  If file already exists, the function fails.
  // FA_CREATE_ALWAYS - Creates a new file, always.  If file already exists, it is over-written.
  // FA_OPEN_ALWAYS   - Opens a file, always.  If file does not exist, the function creates a file.
  // FA_OPEN_EXISTING - Opens a file, only if it exists.  If the file does not exist, the function fails.
	
FRESULT createUserFile(void){
	 return f_open(&userHandle,userFilename, FA_CREATE_ALWAYS);
} 
FRESULT setupUserFile (char id [], char name []){
	UINT successfulwrites;
	f_open(&userHandle,userFilename, FA_WRITE);
	f_write(&userHandle, name, strlen(name), &successfulwrites);
	f_write(&userHandle, " ", 1, &successfulwrites);
	return setupID(id);
}
FRESULT setupUsername (char name []) {
	UINT successfulwrites;
	return f_write(&userHandle, name, strlen(name), &successfulwrites);
}
FRESULT setupID (char id []) {
	UINT successfulwrites;
	return f_write(&userHandle, id, strlen(id), &successfulwrites);
}	
FRESULT createPressureLog(void) {
	return f_open(&pressureLogHandle,pressureLogFilename, FA_CREATE_ALWAYS);
}

FRESULT openFile (FIL file,char fileName [], char control){ 
	if (control == 0) 
			return f_open(&file, userFilename, FA_WRITE|FA_OPEN_ALWAYS);
	else 
			return f_open(&file, userFilename, FA_READ);
}

FRESULT closeFile (FIL file){ 
	return f_close(&file);
}

void writePressurePadDec(int i) {
	int k = i; 
		UINT successfulwrites;
		char * value; 
		f_open(&pressureLogHandle,pressureLogFilename, FA_WRITE);
					f_lseek(&pressureLogHandle, pressureLogHandle.fsize);
					if (i == 0)
					f_write(&pressureLogHandle, "*", 1, &successfulwrites);
							  if (i/100 == 1)
									f_write(&pressureLogHandle, "1", 1, &successfulwrites);
								i= i%100; 
								if (i/10 == 0)
										f_write(&pressureLogHandle, "0", 1, &successfulwrites);
								if (i/10 == 1)
										f_write(&pressureLogHandle, "1", 1, &successfulwrites);
								if (i/10 == 2)
										f_write(&pressureLogHandle, "2", 1, &successfulwrites);
								if (i/10 == 3)
										f_write(&pressureLogHandle, "3", 1, &successfulwrites);
								if (i/10 == 4)
										f_write(&pressureLogHandle, "4", 1, &successfulwrites);
								if (i/10 == 5)
										f_write(&pressureLogHandle, "5", 1, &successfulwrites);
								if (i/10 == 6)
										f_write(&pressureLogHandle, "6", 1, &successfulwrites);
								if (i/10 == 7)
										f_write(&pressureLogHandle, "7", 1, &successfulwrites);
								if (i/10 == 8)
										f_write(&pressureLogHandle, "8", 1, &successfulwrites);
								if (i/10 == 9)
										f_write(&pressureLogHandle, "9", 1, &successfulwrites);
								if (i%10 == 0)
										f_write(&pressureLogHandle, "0", 1, &successfulwrites);
								if (i%10 == 1)
										f_write(&pressureLogHandle, "1", 1, &successfulwrites);
								if (i%10 == 2)
										f_write(&pressureLogHandle, "2", 1, &successfulwrites);
								if (i%10 == 3)
										f_write(&pressureLogHandle, "3", 1, &successfulwrites);
								if (i%10 == 4)
										f_write(&pressureLogHandle, "4", 1, &successfulwrites);
								if (i%10 == 5)
										f_write(&pressureLogHandle, "5", 1, &successfulwrites);
								if (i%10 == 6)
										f_write(&pressureLogHandle, "6", 1, &successfulwrites);
								if (i%10 == 7)
										f_write(&pressureLogHandle, "7", 1, &successfulwrites);
								if (i%10 == 8)
										f_write(&pressureLogHandle, "8", 1, &successfulwrites);
								if (i%10 == 9)
										f_write(&pressureLogHandle, "9", 1, &successfulwrites);
								f_write(&pressureLogHandle, "#", 1, &successfulwrites);
								if (k == 120)
										f_write(&pressureLogHandle, "\r\n", 2, &successfulwrites);
							f_close(&pressureLogHandle);
} 

void writePressurePadHex(); 

void initSession() {
		UINT successfulwrites;
		f_open(&pressureLogHandle,pressureLogFilename, FA_WRITE);
		f_lseek(&pressureLogHandle, pressureLogHandle.fsize);
		if (sessionNumber == 0){
			f_write(&pressureLogHandle, "&1\r\n", 4, &successfulwrites);
			f_write(&pressureLogHandle, "$0\r\n", 4, &successfulwrites);
			}
		
		else if (sessionNumber == 1){
			f_write(&pressureLogHandle, "&2\r\n", 4, &successfulwrites);
			f_write(&pressureLogHandle, "$1\r\n", 4, &successfulwrites);
			}
				
		else if (sessionNumber == 2){
			f_write(&pressureLogHandle, "&3\r\n", 4, &successfulwrites);
			f_write(&pressureLogHandle, "$2\r\n", 4, &successfulwrites);
			}
				
		else if (sessionNumber == 3){
			f_write(&pressureLogHandle, "&4\r\n", 4, &successfulwrites);
			f_write(&pressureLogHandle, "$3\r\n", 4, &successfulwrites);
			}
		sessionNumber++; 
		sessionNumber%=4;
}
