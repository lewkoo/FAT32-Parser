//
//  BSParser.c
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-03-19.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#include <stdio.h>
#include <fcntl.h>
#include "BSParser.h"
#include "fat32.h"
#include <unistd.h>

void staticParseBS(fat32BS *boot_sector);

fat32BS* parseAndReturnBS(char *diskImageLocaiton){
    size_t BUFFER_SIZE = sizeof(fat32BS); //size of the BPB (NOTE: works ONLY with FAT32 (not with FAT12 or FAT16)
    int source;
    FILE *destination;
    int n;
    int count = 0;
    
    fat32BS *boot_sector = NULL;
    
    //system("stty -f /dev/rdisk1s1 115200 cs8 -cstopb -parity -icanon min 1 time 1");
    
    
    unsigned char *buffer[BUFFER_SIZE];
    memset(&buffer,0,sizeof(buffer));
    
    source = open(diskImageLocaiton, O_RDONLY); //TODO: ask Jim about this bit or figure this out
    //if you just pass it the argument, you end up reading the wrong file...
    if (source == -1)
        perror("cannot open: ");
    
    if (source) {
        
        //output testing
        //destination = fopen("/Users/lewkoo/Desktop/output", "w");
        
        n = read(source, &buffer, BUFFER_SIZE);
        count += n;
        printf("BS parser read %d bytes read from diskimage.\n\n", count);
        
        //output testing
        //fwrite(buffer, 1, n, destination);
    }else {
        printf("failed in reading the BS\n");
        exit(1);
    }
    
    
    //parse the struct
    
    boot_sector = (fat32BS*)&buffer;
    
    //checking signature bytes
    
    uint8_t result;
    
    result = checkSignatureBytes(boot_sector);
    
    //checking if FAT32
    
    result = checkFAT16Descriptors(boot_sector);
    
    //result = 1;
    
    //static parsing function here
    
    if(result == 1){
        staticParseBS(boot_sector);
    }else{
        fprintf(stderr,"%s\n\n", "Boot sector did not parse because of corrupted Boot Signature Byte");
    }
    
    
    
    close(source);
    
    return boot_sector;

}

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
    

    //checking signature bytes
    
    uint8_t result;
    
    //uint8_t result = checkSignatureBytes(boot_sector);
    
    //checking if FAT32
    
    result = checkFAT16Descriptors(boot_sector);
    
    //static parsing function here

    if(result == 1){
        //staticParseBS(boot_sector);
    }else{
        fprintf(stderr,"%s\n\n", "Boot sector did not parse because of corrupted Boot Signature Byte");
    }
    
    
    
    fclose(source);
    
    
    
}

uint8_t checkSignatureBytes(fat32BS *boot_sector){
    if(boot_sector->BS_BootSig == 0x29){
        fprintf(stderr,"%s\n", "Signature byte is okay.");
        return 1;
    }else{
        fprintf(stderr,"%s\n", "Signature byte is not set. Sorry.");
        return 0;
    }
}

uint8_t checkFAT16Descriptors(fat32BS *boot_sector){
    if(boot_sector->BPB_FATSz16 == 0x00){
        //fprintf(stderr,"%s\n\n", "FAT32 system it is.");
        return 1;
    }else{
        //fprintf(stderr,"%s\n\n", "FAT16? No, sorry.");
        return 0;
    }
}


void staticParseBS(fat32BS *boot_sector){
    
    //define a mask to get 7th bit of ExtFlags
    uint16_t bitMask = 64;
    
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
    
    //size calculaiton
    
    uint32_t size = boot_sector->BPB_SecPerClus * boot_sector->BPB_TotSec32; // sectors per cluster * total number of sectors
    uint32_t sizeMB = (size/1024)/1024; //calculate in MB
    uint32_t sizeGB = sizeMB/1024; //calculate in GB
    
    fprintf(stderr, "Size: %u bytes (%uMB, %uGB)\n", size, sizeMB, sizeGB); //not sure if the size is being calculated right
    
    //drive number
    if(boot_sector->BS_DrvNum == 0x80){
        fprintf(stderr, "Drive number: 8 (hard drive)\n");
    }else if(boot_sector->BS_DrvNum == 0x00){
        fprintf(stderr, "Drive number: 0 (floppy)\n");
    }else{
        fprintf(stderr, "Drive number: %x \n", boot_sector->BS_DrvNum);
    }
    
    fprintf(stderr, "\n---- Geometry ----\n");
    
    fprintf(stderr, "Bytes per sector: %u\n", boot_sector->BPB_BytesPerSec);
    fprintf(stderr, "Sectors per cluster: %u\n", boot_sector->BPB_SecPerClus);
    fprintf(stderr, "Total sectors: %u\n", boot_sector->BPB_TotSec32); //size
    
    
    fprintf(stderr, "\n---- FS Info ----\n");
    
    fprintf(stderr, "Volume ID: figure this out\n"); //figure this out - how did he get Transcend?
    fprintf(stderr, "Version: %d.%d\n", boot_sector->BPB_FSVerHigh, boot_sector->BPB_FSVerLow);
    fprintf(stderr, "Reserved sectors: %u\n",boot_sector->BPB_RsvdSecCnt);
    fprintf(stderr, "Number of FATs: %u\n",boot_sector->BPB_NumFATs);
    fprintf(stderr, "FAT Size: %u\n", boot_sector->BPB_FATSz32); //size
    
    //getting 7th bit of the ExtFlags
    uint16_t result = boot_sector->BPB_ExtFlags & bitMask;
    
    //BPB_ExtFlags
    if((result & bitMask) == result){
        fprintf(stderr, "Mirrored FAT: %u (yes)\n",result);
    }else{
        fprintf(stderr, "Mirrored FAT: %u (no)\n",result);
    }
    
    
   
    
    
    
    
    
    
}

