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

#include "fat32.h"
#include "dir.h"

void startCL(); //starts the command line interface
void locateRootDir();
//void printDir(fatDir *dir);

//setters
void setDiskImageLocation(char *diskImageLocaiton);
void setBootSector(fat32BS *boot_sec);

//helper functions
uint8_t validateBS();

bool validateDiskImageLocation();
bool validateBootSector();


#endif
