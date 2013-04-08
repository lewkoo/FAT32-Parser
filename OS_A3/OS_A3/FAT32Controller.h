//
//  FAT32Controller.h
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-04-03.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#ifndef OS_A3_FAT32Controller_h
#define OS_A3_FAT32Controller_h

typedef int bool;
#define TRUE    1
#define FALSE   0

#define FOLDER_NOT_FOUND -1

#include "fat32.h"
#include "dir.h"

void startCL(); //starts the command line interface
void locateRootDir();
void printDir();
void setCurrDir(uint64_t newDirCluster); //set the current dir based on the cluster number from the FAT

//returns the first cluster number of a given dir
//by earching the current dir
uint64_t checkIfDirExists(char *dirName);

//getters
unsigned char *getBuffer();


//setters
void setDiskImageLocation(char *diskImageLocaiton);
void setBootSector(fat32BS *boot_sec);

//helper functions
void getCommand();
void cleanInputUp();
uint8_t validateBS();
int readSector(uint64_t sectorNum);

char *processFileName(char* fileName);
char *processDirName(char *dirName);

bool validateDiskImageLocation();
bool validateBootSector();
void validateBsAndImageLoc(void);
bool verifyRootDir(fatDir *rootDir);
bool isRoot(fatDir *toCheck);

void convertToUpperCase(char *sPtr);


#endif
