//
//  main.c
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-03-19.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#include <stdio.h>
#include "BSParser.h"
#include "fat32.h"
#include "FAT32Controller.h"

int main(int argc, const char * argv[])
{
    
    fat32BS *boot_sector;
 
    //usage check up here
    char* fileSystemPath = malloc(sizeof(argv[0])+1);
    fileSystemPath = (char *)argv[1];
    fprintf(stderr,"Reading from: %s\n" ,fileSystemPath);
    
    
    boot_sector = parseAndReturnBS(fileSystemPath);//gets the BS data
    uint8_t result = validateBS(boot_sector);//tests if it is FAT32
    
    setDiskImageLocation(fileSystemPath);
    setBootSector(boot_sector);

    
    if(boot_sector != NULL && result == 1){
        //inicialise FAT32 controller
        
        setDiskImageLocation(fileSystemPath);
        setBootSector(boot_sector);
        
        
        printf("\nInitializing FAT 32 controller\n");
        //staticParseBS(boot_sector);
        
        startCL(boot_sector);
                
        
    }else{
        //report an error, quit
    }
                      
    
    printf("\nExecution finished\n");
    return 0;
}

