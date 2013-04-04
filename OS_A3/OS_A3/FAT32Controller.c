//
//  FAT32Controller.c
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-04-03.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#include <stdio.h>
#include "FAT32Controller.h"
#include "BSParser.h"

void startCL(fat32BS *boot_sector){
    
}

uint8_t validateBS(fat32BS *boot_sector){
    
    uint8_t result = checkFAT16Descriptors(boot_sector);
    result = checkSignatureBytes(boot_sector);
    
    //perform calculations on P14
    //instanciate variables
    uint16_t FATSz;
    uint32_t TotSec;
    uint32_t DataSec;
    
    uint16_t RootDirSectors = ((boot_sector->BPB_RootEntCnt *32) +(boot_sector->BPB_BytesPerSec -1)) / boot_sector->BPB_BytesPerSec;
    

    
    if(boot_sector->BPB_FATSz16 != 0){
        FATSz = boot_sector->BPB_FATSz16;
    }else{
        FATSz = boot_sector->BPB_FATSz32;
    }
    
    if(boot_sector->BPB_TotSec16 != 0){
        TotSec = boot_sector->BPB_TotSec16;
    }else{
        TotSec = boot_sector->BPB_TotSec32;
    }
    
    DataSec = TotSec - (boot_sector->BPB_RsvdSecCnt + (boot_sector->BPB_NumFATs * FATSz) + RootDirSectors );
    
    //continue coding from P14
    
}