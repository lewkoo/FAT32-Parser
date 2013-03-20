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

void parseBS(char *disk);
void staticParseBS(fat32BS *boot_sector, unsigned char *buffer);

#endif
