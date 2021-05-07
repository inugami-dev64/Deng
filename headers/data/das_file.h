

#ifndef __DAS_FILE_H
#define __DAS_FILE_H

#define __DAS_INFO_HEADER_NAME              "INFO_HDR"
#define __DAS_INFO_HEADER_NAME_NN           { 'I', 'N', 'F', 'O', '_', 'H', 'D', 'R' }
#define __DAS_INFO_HEADER_SIZE              62
#define __DAS_VERTICES_HEADER_NAME          "VERT_HDR"
#define __DAS_VERTICES_HEADER_NAME_NN       { 'V', 'E', 'R', 'T', '_', 'H', 'D', 'R' }
#define __DAS_VERT_POSITION_HEADER_NAME     "VPOS_HDR"    
#define __DAS_VERT_POSITION_HEADER_NAME_NN  { 'V', 'P', 'O', 'S', '_', 'H', 'D', 'R' }    
#define __DAS_TEX_POSITION_HEADER_NAME      "VTEX_HDR"    
#define __DAS_TEX_POSITION_HEADER_NAME_NN   { 'V', 'T', 'E', 'X', '_', 'H', 'D', 'R' }    
#define __DAS_NORM_POSITION_HEADER_NAME     "VNOR_HDR"    
#define __DAS_NORM_POSITION_HEADER_NAME_NN  { 'V', 'N', 'O', 'R', '_', 'H', 'D', 'R' }    
#define __DAS_INDICES_HEADER_NAME           "INDX_HDR"
#define __DAS_INDICES_HEADER_NAME_NN        { 'I', 'N', 'D', 'X', '_', 'H', 'D', 'R' }
#define __DAS_META_HEADER_NAME              "META_HDR"
#define __DAS_META_HEADER_NAME_NN           { 'M', 'E', 'T', 'A', '_', 'H', 'D', 'R' }
#define __DAS_STATIC_MAGIC_NUMBER           0x5453544553534144
#define __DAS_ANIMATION_MAGIC_NUMBER        0x4e41544553534144
#define __DAS_MAP_MAGIC_NUMBER              0x504d544553534144
#define __DAS_UUID_LEN                      32

typedef struct das_META_HDR {
    char beg[8]; // always has to be BEG_HDR
    deng_ui32_t hdr_size;
    deng_ui32_t data_size;
    char *data;
} das_META_HDR;


typedef struct das_INFO_HDR {
    deng_ui64_t magic_number;
    char hdr_name[8];
    deng_ui32_t hdr_size;
    char uuid[__DAS_UUID_LEN];
    deng_ui64_t time_st;
    deng_ui8_t asset_type;
    deng_ui8_t cmpr;
} das_INFO_HDR;


typedef struct das_VERT_HDR {
    char hdr_name[8];
    deng_ui32_t hdr_size;
} das_VERT_HDR;


typedef struct __das_VertTemplate {
    char hdr_name[8];
    deng_ui32_t hdr_size;
    deng_ui32_t vert_c;
} __das_VertTemplate;


typedef __das_VertTemplate das_VPOS_HDR;
typedef __das_VertTemplate das_VTEX_HDR;
typedef __das_VertTemplate das_VNOR_HDR;


typedef struct das_INDX_HDR {
    char hdr_name[8];
    deng_ui32_t hdr_size;
    deng_ui32_t ind_c;
} das_INDX_HDR;

#endif
