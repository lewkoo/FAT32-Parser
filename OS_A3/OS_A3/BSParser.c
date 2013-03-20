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

void parseBS(const char *diskImageLocaiton){
    
    size_t BUFFER_SIZE = 36; //size of the BPB
    FILE *source;
    FILE *destination;
    int n;
    int count = 0;
    
    
    
    unsigned char buffer[BUFFER_SIZE];
    
    source = fopen(diskImageLocaiton, "rb");
    
    if (source) {
        
        //output testing
        destination = fopen("/Users/lewkoo/Desktop/output", "wb");
        
        n = fread(buffer, 1, BUFFER_SIZE, source);
        count += n;
        printf("n = %d\n", n);
        printf("%d bytes read from diskimage.\n", count);
        
        //output testing
        fwrite(buffer, 1, n, destination);
    }else {
        printf("fail\n");
    }
    
    fclose(source);
    
    
    
}
