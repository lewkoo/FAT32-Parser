//
//  FAT32Controller.c
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-04-03.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#include <stdio.h>
#include "FAT32Controller.h"
#include "FATTableMan.h"
#include "BSParser.h"
#include "dir.h"
#include "Util.h"

#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN    0x02
#define ATTR_SYSTEM    0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE   0x20

//DIR_Name constants

#define DIR_FREE_KEEP_GOING 0xE5
#define DIR_FREE_STOP       0x00

//volume ID array

char *vol_ID;

unsigned char *newBuf;
unsigned char *printBuf;

extern void* memset(); //to suppress the memset warninig.

enum FATType{
    FAT16,
    FAT32
}FATType;

enum Arguments{
    DIR,
    CD,
    INFO,
    GET
};


enum FATType fatType;
char *diskImageLocaiton = NULL;
fat32BS *boot_sector = NULL;
uint64_t currDirClusterNum;


//for command-line interface
char *line;
char *origLine;
char command[20];
unsigned argumentsCount = 0;
char **arguments;


void startCL(){
    locateRootDir();
    
    while(1){
        fprintf(stderr,"\n%s", "> ");
        
        getCommand();
        
        if(strcmp(command, "cd") == 0){
            //do cd
            
            
            
        }else if(strcmp(command, "info") == 0){
            //do info command
            staticParseBS(boot_sector);
        }else if(strcmp(command, "dir") == 0){
            //do dir command
            printDir();
        }else if(strcmp(command, "get") == 0){
            //do get command
            
        }else if(strcmp(command, "quit") == 0){
            exit(1);
        }
        else{
            fprintf(stderr,"Incorrect command: %s \n Usage: \n -info : prints information about this drive \n -dir : lists the current directory \n -cd 'directory name' : sets current directory to 'directory name' \n -get 'file name' copies the file name to local system \n -quit : quites the application \n ",origLine);
        }
        
        cleanInputUp();
    }

    


}

void getCommand(){
    
    line = malloc(128);
    origLine = line;
    
    fgets(line,128,stdin);
    
    
    sscanf(line,"%20s ", command );
    
    line = strchr(line, ' ');
    
    argumentsCount = 0;
    arguments = malloc(sizeof(char *));
    
    while (1)
    {
        char arg[20];
        if (line && (sscanf(++line, "%20s", arg) == 1))
        {
            arguments[argumentsCount] = malloc(sizeof(char) * 20);
            strncpy(arguments[argumentsCount], arg, 20);
            
            argumentsCount++;
            
            arguments = realloc(arguments, sizeof(char *) * argumentsCount + 1);
            line = strchr(line, ' ');
        }
        else {
            break;
        }
    }
}

void cleanInputUp(){
    for (int i = 0; i < argumentsCount; i++) {
        //printf("Argument %i is: %s\n", i, arguments[i]);
    }
    
    for (int i = 0; i < argumentsCount; i++) {
        free(arguments[i]);
    }
    
    free(arguments);
    free(origLine);
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
    
    
        FATSz = boot_sector->BPB_FATSz32;

        TotSec = boot_sector->BPB_TotSec32;

    
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

void setCurrDir(uint64_t newDirCluster){
    uint64_t FirstSectorofCluster = getDataOnClusterNum(newDirCluster, boot_sector); //get the first cluster
    currDirClusterNum = newDirCluster; //updating the current dir
    //read the contents of a sector into the buffer
    readSector(FirstSectorofCluster);
}

void locateRootDir(){
    validateBsAndImageLoc();
    //size_t BUFFER_SIZE = boot_sector->BPB_BytesPerSec; //size of the sector (in bytes)
    //unsigned char buffer[BUFFER_SIZE];
    //memset(&buffer,0,sizeof(buffer));
    
    //The White paper Way
     
    uint32_t rootDirCLusterNum = boot_sector->BPB_RootClus;
    uint64_t FirstSectorofCluster = getDataOnClusterNum(rootDirCLusterNum, boot_sector);
    
    currDirClusterNum = rootDirCLusterNum; //setting the curDir to rootDir
    
    //read the contents of a sector into the buffer
    readSector(FirstSectorofCluster);
    //processing
    fatDir *tstStruct = (fatDir*)&newBuf[0];
    
    bool result = verifyRootDir(tstStruct);
    
    if(result != TRUE){
        fprintf(stderr,"%s\n", "Root dir corrupted");
    }
    
    //setting the volume name
    vol_ID = tstStruct->DIR_Name;
    vol_ID[DIR_NAME_LENGTH] = '\0';

    //move to dir command
    //printDir(tstStruct);
    
    uint32_t nextClusterNum = checkForNextCluster(rootDirCLusterNum, boot_sector);
    
    if(nextClusterNum == EndOfClusterResponce){
        //do nothing
    }else{
        //parse next cluster cluster
    }
    
    
    
    //changes current directory given a cluster number
    //setCurrDir(5);
    //setCurrDir(2);
    //setCurrDir(5);
    //setCurrDir(2);
}

/*int readFatTableEntry(uint64_t sectorNum, uint64_t fatEntOffset, unsigned char *buffer){
    int returnVal = 0;
    size_t n = 0; // number of bytes read
    FILE *source;
    size_t BUFFER_SIZE = boot_sector->BPB_BytesPerSec; //size of the sector (in bytes)
    
    
    source = fopen(diskImageLocaiton, "r");
    
    
    
    
    fclose(source);
    
}*/


int readSector(uint64_t sectorNum){
    int returnVal = 0;
    size_t n = 0; // number of bytes read
    FILE *source;
    size_t BUFFER_SIZE = boot_sector->BPB_BytesPerSec; //size of the sector (in bytes)
    newBuf = malloc(BUFFER_SIZE);
    
    source = fopen(diskImageLocaiton, "r");
    
    int result = fseek(source, (long)sectorNum*boot_sector->BPB_BytesPerSec, SEEK_SET);
    
    if(result == 0){
        //success
    }else{
        fprintf(stderr, "Failed to seek.");
    }
    
    
    if(source){
        n = fread(newBuf, 1, BUFFER_SIZE, source);
    }

    if(n != BUFFER_SIZE){
        returnVal = 1;
    }
    
    fclose(source);
    //free(newBuf);
    
    return returnVal;
}

void printDir(){
    
    int i = 0;
    
    setCurrDir(currDirClusterNum); //load content of dir cluster into memory
    
    fatDir *dir = (fatDir*)&newBuf[0];
    
    bool result = verifyRootDir(dir);
    
    if(result != TRUE){
        fprintf(stderr,"%s\n", "Root dir corrupted");
    }

    
    //size_t BUFFER_SIZE = boot_sector->BPB_BytesPerSec; //size of the sector (in bytes)
    //printBuf = malloc(BUFFER_SIZE); //consider using a different buffer for printing
    
    fprintf(stderr,"\n%s\n", "DIRECTORY LISTING");
    fprintf(stderr,"%s: %s\n", "VOL_ID", vol_ID);
    
    //if root dir, skip over the first entry
    if(isRoot(dir) == TRUE){
        i = sizeof(fatDir);
    }
    
    for(; i < 16*sizeof(fatDir); i+=sizeof(fatDir)){
        dir = (fatDir*)&newBuf[i];
        
        if(dir->DIR_Name[0] == DIR_FREE_STOP){
            break;
        }
        
        if( (dir->DIR_Attr & ATTR_DIRECTORY) == ATTR_DIRECTORY){
            
            char *temp = dir->DIR_Name;
            
            temp[DIR_NAME_LENGTH] = '\0';
            
            fprintf(stderr,"<%s>        %d\n",temp, dir->DIR_FileSize);
        }else if( (dir->DIR_Attr & ATTR_HIDDEN) != ATTR_HIDDEN){
            char *temp = dir->DIR_Name;
            
            temp = processFileName(temp);
            
            temp[DIR_NAME_LENGTH] = '\0';
            
            fprintf(stderr,"%s        %d\n",temp, (dir->DIR_FileSize/1024));
        }
        
        
        
        
        
    }

    
}

bool verifyRootDir(fatDir *rootDir){
    if( (rootDir->DIR_Attr & ATTR_VOLUME_ID) != ATTR_VOLUME_ID){
        return FALSE;
    }else{
        return TRUE;
    }
}



void setDiskImageLocation(char *diskImageLoc){
    diskImageLocaiton = diskImageLoc;
}

void setBootSector(fat32BS *boot_sec){
    boot_sector = boot_sec;
}

bool validateDiskImageLocation(){
    if(diskImageLocaiton != NULL){
        return TRUE;
    }else{
        return FALSE;
    }
}
bool validateBootSector(){
    if(boot_sector != NULL){
        return TRUE;
    }else{
        return FALSE;
    }
}

void validateBsAndImageLoc(void) {
    bool testResult = FALSE;
    
    testResult = validateBootSector();
    
    if(testResult == FALSE){
        fprintf(stderr, "Failed to validate Boot Sector. Exit.");
        //break;
    }
    
    testResult = validateDiskImageLocation();
    
    if(testResult == FALSE){
        fprintf(stderr, "Failed to validate Disk Image Location. Exit.");
        //break;
    }
}



//takes the FAT32 formatted file name
//removes whitespaces, adds a dot between
//filename and extension

char *processFileName(char *fileName){
    
    char *output = fileName;
    bool dotInserted = FALSE;
    int j = 0;
    int i = 0;
    for(; i < strlen(fileName); i++,j++){
        if(fileName[i]!=' '){
                output[j] = fileName[i];
        }
        else{
            if(dotInserted == FALSE){
                output[j] = '.';
                output[j++] = fileName[i];
                dotInserted = TRUE;
            }
            j--;
            
        }
            
        
        
    }
    output[j]='\0';
    
    return output;
}

bool isRoot(fatDir *toCheck){
    if( (toCheck->DIR_Attr & ATTR_VOLUME_ID) != ATTR_VOLUME_ID){
        return TRUE;
    }else{
        return FALSE;
    }
}

unsigned char *getBuffer(){
    return newBuf;
}



