#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "diskio.h"
#include "ff.h"
#include "tm4c123gh6pm.h"
#ifndef STORAGE_H
#define STORAGE_H
//setups the creation of files and mounting of SD Card
void storageSetup (char name []); 
//boots up the cared if files are existing
void bootup ();
//Mounts the SD card (FAT and FAT32)
FRESULT mountCard(void); 
//Creates the User's info file 
FRESULT createUserFile();
//Setups up the User file (Name, ID)
FRESULT setupUserFile (char id [], char name []);
FRESULT setupUsername (char name []); 
FRESULT setupID (char id []); 
//Creates the PressureLog File
FRESULT createPressureLog(void);
//Open the file 
// FIL file - the pointer to the file to be opened 
// 1 - write
// 2 - read 
FRESULT openFile (FIL file,char fileName [], char control);
//Close the file 
//	FIL file - the pointer to the file to be closed
FRESULT closeFile (FIL file);
//Write to a file 
//	FIL file - the pointer to the file to write to
FRESULT writeFile(FIL file);
//Initializes the session number
//&x - x = session amount 
//$y - y - session number
void initSession ();
// Gets the current pressure sessions recorded
char getSessionAmounts();
// Sets the session number
void setSessionAmounts();
// Gets the session number
char getSessionNumber(); 
// sets the session number
void setSessionNumber(); 
//writes to the pressure pad in decimal (demo)
// int i - the value to be written 
void writePressurePadDec(int i);
//write to the pressure pad in hex 
//int i - the value to be written
void writePressurePadHex(int i); 
//Reads all of the readings from the pressure pad 
char * readPressurePad();
//Reads a seleceted pressure pad session 
// session - the sessions to read from 
// 0-4, 5 sessions 
char * readPressurePadSession(char session);
//Read a collection of pressure sessions 
// session - the session to start at (0 - 4, 5 sessions)
// sessions - amount of sessions to record
char * readPressurePadSessionSets(char session, char sessions);
#endif