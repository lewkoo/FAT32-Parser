//
//  FATTableMan.h
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-04-05.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#ifndef OS_A3_FATTableMan_h
#define OS_A3_FATTableMan_h

#include "fat32.h"
#include <inttypes.h>

#define bytesPerFatEntry 4



uint64_t getThisFatSecNum (uint32_t clusterNumber, fat32BS *boot_sector);
uint64_t getFatEntOffset (uint32_t clusterNumber, fat32BS *boot_sector);

#endif
