//
//  main.c
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-03-19.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#include <stdio.h>
#include "BSParser.h"

int main(int argc, const char * argv[])
{
 
    //usage check up here
    char* fileSystemPath = malloc(sizeof(argv[0])+1);
    fileSystemPath = (char *)argv[1];
    fprintf(stderr,"Reading from: %s\n" ,fileSystemPath);
    
    
    parseBS(fileSystemPath); //probably should return the struct
                      //really depends on how I want to structure this
    
    printf("\nExecution finished\n");
    return 0;
}

