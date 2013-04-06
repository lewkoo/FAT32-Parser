//
//  Util.c
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-04-06.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#include <stdio.h>
#include "Util.h"

uint64_t getDataOnClusterNum(uint32_t clusterNum, fat32BS *boot_sector){ //think about better ways to refactor this out!
    //variables set up
    long currPos; //for testing
    size_t n = 0; // number of bytes read
    FILE *source;
    size_t BUFFER_SIZE = boot_sector->BPB_BytesPerSec; //size of the sector (in bytes)
    unsigned char buffer[BUFFER_SIZE];
    memset(&buffer,0,sizeof(buffer));

    
    uint64_t RootDirSectors = ((boot_sector->BPB_RootEntCnt * 32) + (boot_sector->BPB_BytesPerSec-1)) / boot_sector->BPB_BytesPerSec;
    uint64_t FistDataSector = boot_sector->BPB_RsvdSecCnt + (boot_sector->BPB_NumFATs * boot_sector->BPB_FATSz32) + RootDirSectors;
    uint64_t FirstSectorofCluster = ((clusterNum-2) * boot_sector->BPB_SecPerClus) + FistDataSector;
    
    
    return FirstSectorofCluster;
    
}