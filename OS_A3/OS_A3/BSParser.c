//
//  BSParser.c
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-03-19.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#include <stdio.h>
#include "BSParser.h"
#include "fat32.h"

void staticParseBS(fat32BS *boot_sector, unsigned char *buffer);

void parseBS(char *diskImageLocaiton){
    
    size_t BUFFER_SIZE = sizeof(fat32BS); //size of the BPB (NOTE: works ONLY with FAT32 (not with FAT12 or FAT16)
    FILE *source;
    FILE *destination;
    int n;
    int count = 0;
    
    fat32BS *boot_sector;
    
    
    
    unsigned char *buffer[BUFFER_SIZE];
    memset(&buffer,0,sizeof(buffer));
    
    source = fopen(diskImageLocaiton, "r"); //TODO: ask Jim about this bit or figure this out
    //if you just pass it the argument, you end up reading the wrong file...
    
    if (source) {
        
        //output testing
        destination = fopen("/Users/lewkoo/Desktop/output", "w");
        
        n = fread(buffer, 1, BUFFER_SIZE, source);
        count += n;
        printf("BS parser read %d bytes read from diskimage.\n\n", count);
        
        //output testing
        fwrite(buffer, 1, n, destination);
    }else {
        printf("failed in reading the BS\n");
        exit(1);
    }
    
    
    //parse the struct
    
    boot_sector = (fat32BS*)&buffer;
    
    //static parsing function here
    
    staticParseBS(boot_sector, buffer);
    
    
    fclose(source);
    
    
    
}

void staticParseBS(fat32BS *boot_sector, unsigned char *buffer){
    
    //null terminate osme of them
    
    boot_sector->BS_VolLab[BS_VolLab_LENGTH-1] = '\0';
    boot_sector->BS_FilSysType[BS_FilSysType_LENGTH-1] = '\0';
    //boot_sector->BS_OEMName[BS_OEMName_LENGTH-1] = '\0';
    
    
    fprintf(stderr, "---- Device Info ----\n");
    
    fprintf(stderr, "OEM Name: %s\n", boot_sector->BS_OEMName);
    fprintf(stderr, "Label: %s\n", boot_sector->BS_VolLab);
    fprintf(stderr, "File System Type: %s\n", boot_sector->BS_FilSysType);
    
    if(boot_sector->BPB_Media == 0xF8){
    fprintf(stderr, "Media Type: 0x%02X (fixed)\n", boot_sector->BPB_Media); //can it be anything other than fixed???
    }else{
        fprintf(stderr, "Media Type: 0x%02X (non-fixed)\n", boot_sector->BPB_Media); //catches that
    }
    
    
    
    
    
    
}

