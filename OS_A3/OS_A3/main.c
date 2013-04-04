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

int main(int argc, const char * argv[])
{
    
    fat32BS *boot_sector;
 
    //usage check up here
    char* fileSystemPath = malloc(sizeof(argv[0])+1);
    fileSystemPath = (char *)argv[1];
    fprintf(stderr,"Reading from: %s\n" ,fileSystemPath);
    
    
    boot_sector = parseAndReturnBS(fileSystemPath);
    
    if(boot_sector != NULL){
        //inicialise FAT32 controller
        printf("\nInitializing FAT 32 controller\n");
        
        
    }else{
        //report an error, quit
    }
                      
    
    printf("\nExecution finished\n");
    return 0;
}

