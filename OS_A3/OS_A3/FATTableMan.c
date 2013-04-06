//
//  FATTableMan.c
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-04-05.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#include <stdio.h>
#include "FATTableMan.h"


uint64_t getThisFatSecNum (uint32_t clusterNumber, fat32BS *boot_sector){
    uint64_t FATSz;
    uint64_t FATOffset = clusterNumber * bytesPerFatEntry;
    uint64_t ThisFatSecNum;
    
    FATSz = boot_sector->BPB_FATSz32;
    
    
    FATOffset = clusterNumber * bytesPerFatEntry; //already done
    
    ThisFatSecNum = boot_sector->BPB_RsvdSecCnt + (FATOffset / boot_sector->BPB_BytesPerSec); //sector location
    
    return ThisFatSecNum;
}
uint64_t getFatEntOffset (uint32_t clusterNumber, fat32BS *boot_sector){
    
    uint64_t ThisFATEntOffset;
    uint64_t FATOffset = clusterNumber * bytesPerFatEntry;
    
    ThisFATEntOffset = FATOffset % boot_sector->BPB_BytesPerSec; // this calculates the exact bit wise position within the sector
    
    return ThisFATEntOffset;
}