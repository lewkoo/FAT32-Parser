//
//  FAT32Controller.h
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-04-03.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#ifndef OS_A3_FAT32Controller_h
#define OS_A3_FAT32Controller_h

#include "fat32.h"

void startCL(fat32BS *boot_sector); //starts the command line interface

//helper functions
int validateBS(fat32BS *boot_sector);

#endif
