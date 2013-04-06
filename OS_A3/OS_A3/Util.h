//
//  Util.h
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-04-06.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#ifndef OS_A3_Util_h
#define OS_A3_Util_h

#include <inttypes.h>
#include "fat32.h"

uint64_t getDataOnClusterNum(uint32_t clusterNum, fat32BS *boot_sector);

#endif
