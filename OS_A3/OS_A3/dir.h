//
//  dir.h
//  OS_A3
//
//  Created by Levko Ivanchuk on 2013-04-03.
//  Copyright (c) 2013 Levko Ivanchuk. All rights reserved.
//

#ifndef OS_A3_dir_h
#define OS_A3_dir_h
#include <inttypes.h>

/* dir info constants */
#define DIR_NAME_LENGTH 11



#pragma pack(push)
#pragma pack(1)
struct fatDir_struct {
    char DIR_Name[DIR_NAME_LENGTH];
    uint8_t DIR_Attr;
    uint8_t DIR_NTRes;
    uint16_t DIR_CrtTime;
    uint16_t DIR_CrtDate;
    uint16_t DIR_LstAccDate;
    uint16_t DIR_FstClusHI;
    uint16_t DIR_WrtTime;
    uint16_t DIR_WrtDate;
    uint16_t DIR_FstClusLO;
    uint32_t DIR_FileSize;
    
};
#pragma pack(pop)

typedef struct fatDir_struct fatDir;

#endif
