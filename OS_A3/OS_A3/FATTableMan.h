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
#include "FAT32Controller.h"
#include <inttypes.h>

#define FAT_ENTRY_SIZE 4 //in bytes!
#define EoC 0xFFFFFF0F //end of cluster mark
#define EndOfClusterResponce 0

struct fatEntry_struct {
   unsigned char value[FAT_ENTRY_SIZE];
};

typedef struct fatEntry_struct fatEntry;



uint32_t checkForNextCluster(uint32_t clusterNumber, fat32BS *boot_sector);

uint64_t getThisFatSecNum (uint32_t clusterNumber, fat32BS *boot_sector);
uint64_t getFatEntOffset (uint32_t clusterNumber, fat32BS *boot_sector);

//helper methods

//converts raw entry value into a hex value (for comparing with End Of Cluster Value)
bool checkIfEOC(unsigned char *rawEntryValue);

#endif
