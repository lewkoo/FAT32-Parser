//
//  BSParser.h
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-03-19.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#ifndef OS_A3_BSParser_h
#define OS_A3_BSParser_h

#include "fat32.h"

fat32BS* parseAndReturnBS(char *diskImageLocaiton);
void parseBS(char *disk);
void staticParseBS(fat32BS *boot_sector);
uint8_t checkSignatureBytes(fat32BS *boot_sector);
uint8_t checkFAT16Descriptors(fat32BS *boot_sector);

#endif
