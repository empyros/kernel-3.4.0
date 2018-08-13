#ifndef _CAM_CAL_DATA_H
#define _CAM_CAL_DATA_H


typedef struct{
    u32 u4Offset;
    u32 u4Length;
    u8 *  pu1Params;
}stCAM_CAL_INFO_STRUCT, *stPCAM_CAL_INFO_STRUCT;

typedef struct{
    u32 u4Offset;
    u32 u4Length;
    u16 *  pu1Params;
}stCAM_CAL_INFO_STRUCT2, *stPCAM_CAL_INFO_STRUCT2;
#endif //_CAM_CAL_DATA_H
