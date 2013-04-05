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

enum FATType{
    FAT16,
    FAT32
}FATType;

enum FATType fatType;

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
    uint32_t CountofClusters;
    
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
    
    CountofClusters = DataSec / boot_sector->BPB_SecPerClus;
    
    if(CountofClusters < 4085){
        //volume is FAT12
        result = 0;
    }else if(CountofClusters < 65525){
        //volume is FAT16
        fatType = FAT16;
        result = 0;
    }else{
        //volume is FAT32
        fatType = FAT32;
        //fprintf(stderr,"%s\n\n", "Count of clusters >= 65525. It is FAT32.");
        result = 1;
    }
    
    return result;
}

void locateRootDir(fat32BS *boot_sector){
    //root dir is pointed to by BPB_RootClus
    //but that is just a cluster
    //either way, we have the cluster number (usually 2)
    //use calculateFATEntry to get the data region of Nth cluster
    
    uint32_t rootDirCLusterNum = boot_sector->BPB_RootClus;
    
    
    calculateFATEntry(rootDirCLusterNum, boot_sector);
    
    
}

uint8_t calculateFATEntry(uint32_t clusterNumber, fat32BS *boot_sector){ //calculates the location of the cluster in FAT (read on p15)
    
    //returns the first sector of a given cluster
    
    uint16_t FATSz;
    uint32_t FATOffset = clusterNumber * 4;
    uint32_t ThisFatSecNum;
    uint32_t ThisFATEntOffset;
    
    if(boot_sector->BPB_FATSz16 !=0){
        FATSz = boot_sector->BPB_FATSz16;
    }else{
        FATSz = boot_sector->BPB_FATSz32;
    }
    
    if(fatType == FAT16){
        FATOffset = clusterNumber * 2;
    }else if(fatType == FAT32){
        FATOffset = clusterNumber * 4;
    }
    
    ThisFatSecNum = boot_sector->BPB_RsvdSecCnt + (FATOffset / boot_sector->BPB_BytesPerSec);
    
    ThisFATEntOffset = FATOffset % boot_sector->BPB_BytesPerSec;
    
    
    
    
    
    
    
    
    
}