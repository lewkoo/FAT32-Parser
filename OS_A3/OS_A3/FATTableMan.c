//
//  FATTableMan.c
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-04-05.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#include <stdio.h>
#include "FATTableMan.h"

extern void* memset(); //to suppress the memset warninig.
unsigned char *fatTableBuf;


uint32_t checkForNextCluster(uint32_t clusterNumber, fat32BS *boot_sector){
    
    fatEntry *tempFatEntry;
    uint32_t result = EndOfClusterResponce;
    
    fatTableBuf = malloc(boot_sector->BPB_BytesPerSec);
    uint64_t thisFatSecNum = getThisFatSecNum(clusterNumber, boot_sector);
    
    uint64_t fatEntOffset = getFatEntOffset(clusterNumber, boot_sector);
    
    readSector(thisFatSecNum, fatTableBuf);
    
    tempFatEntry = (fatEntry*)&fatTableBuf[fatEntOffset];
    
    
    
    
    if(checkIfEOC(tempFatEntry->value) == TRUE){
        result = EndOfClusterResponce;
    }else{
        //convert hex value of tempFatEntry->value to uint32_t
        //test the fuck out of this.
        result = (uint32_t)tempFatEntry->value;
        
    }
        
    return result;

    
}



//helper functions

uint64_t getThisFatSecNum (uint32_t clusterNumber, fat32BS *boot_sector){
    uint64_t FATSz;
    uint64_t FATOffset = clusterNumber * FAT_ENTRY_SIZE;
    uint64_t ThisFatSecNum;
    
    FATSz = boot_sector->BPB_FATSz32;
    
    
    FATOffset = clusterNumber * FAT_ENTRY_SIZE; //already done
    
    ThisFatSecNum = boot_sector->BPB_RsvdSecCnt + (FATOffset / boot_sector->BPB_BytesPerSec); //sector location
    
    return ThisFatSecNum;
}
uint64_t getFatEntOffset (uint32_t clusterNumber, fat32BS *boot_sector){
    
    uint64_t ThisFATEntOffset;
    uint64_t FATOffset = clusterNumber * FAT_ENTRY_SIZE;
    
    ThisFATEntOffset = FATOffset % boot_sector->BPB_BytesPerSec; // this calculates the exact bit wise position within the sector
    
    return ThisFATEntOffset;
}
bool checkIfEOC(unsigned char *rawEntryValue){
    
    bool result = FALSE;
    
    if(rawEntryValue[0] == 0xFF){
        if(rawEntryValue[1] == 0xFF){
            if(rawEntryValue[2] == 0xFF){
                if(rawEntryValue[3]==0x0F){
                    //EOC - YAY!
                    
                    result = TRUE;
                    
                }
            }
        }
    }
    
    return result;
    
}