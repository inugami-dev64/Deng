/*    ____         ________     __      _        ______  
 *   |  _ \_      |  ______|   |  \    | |     _/  ____\
 *   | | \_ \_    |  |         |   \   | |    /  _/   
 *   | |   \  |   |  |_____    | |\ \  | |   |  |   _____ 
 *   | |    | |   |  ______|   | | \ \ | |   |  |  |___  | 
 *   | |  _/ _|   |  |         | |  \ \| |    \  \    / /
 *   | |_/ _/     |  |_____    | |   \   |     \_ \__/ /
 *   |____/       |________|   |_|    \__|       \____/
 *               
 * __________________________________________________________
 * __________________________________________________________
 *                       Project DENG 
 *
 *
 * Copyright (C) 2020 - 2021
 * This Software is licensed under Apache License as described 
 * in the LICENSE.md file, which you should have recieved with 
 * this distribution.
 * 
 * You may reproduce and distribute copies of the
 * Work or Derivative Works thereof in any medium, with or without
 * modifications, and in Source or Object form, provided that You
 * meet the following conditions:
 *
 * (a) You must give any other recipients of the Work or
 *     Derivative Works a copy of this License; and
 *
 * (b) You must cause any modified files to carry prominent notices
 *     stating that You changed the files; and
 *
 * (c) You must retain, in the Source form of any Derivative Works
 *     that You distribute, all copyright, patent, trademark, and
 *     attribution notices from the Source form of the Work,
 *     excluding those notices that do not pertain to any part of
 *     the Derivative Works; and
 *
 * (d) If the Work includes a "NOTICE" text file as part of its
 *     distribution, then any Derivative Works that You distribute must
 *     include a readable copy of the attribution notices contained
 *     within such NOTICE file, excluding those notices that do not
 *     pertain to any part of the Derivative Works, in at least one
 *     of the following places: within a NOTICE text file distributed
 *     as part of the Derivative Works; within the Source form or
 *     documentation, if provided along with the Derivative Works; or,
 *     within a display generated by the Derivative Works, if and
 *     wherever such third-party notices normally appear. The contents
 *     of the NOTICE file are for informational purposes only and
 *     do not modify the License. You may add Your own attribution
 *     notices within Derivative Works that You distribute, alongside
 *     or as an addendum to the NOTICE text from the Work, provided
 *     that such additional attribution notices cannot be construed
 *     as modifying the License.
 *
 * You may add Your own copyright statement to Your modifications and
 * may provide additional or different license terms and conditions
 * for use, reproduction, or distribution of Your modifications, or
 * for any such Derivative Works as a whole, provided Your use,
 * reproduction, and distribution of the Work otherwise complies with
 * the conditions stated in this License.
 */ 


#ifndef __DAS_READER_H
#define __DAS_READER_H

#ifdef __cplusplus
extern "C" {
#endif
#if defined(__DAS_READER_C) || defined(__DAM_C)
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    #include <common/base_types.h>
    #include <common/common.h>
    #include <common/cerr_def.h>

    #include <data/assets.h>
    #include <data/das_file.h>
    #include <data/das_check.h>
    #include <data/das_runtime.h>
    
    /**********************************************/
    /********** Header reading functions **********/
    /**********************************************/

    /*
     * Read generic of data from __sbuf with bounds checking
     */
    void __das_DataRead(void *buf, size_t chunk_size, size_t n, char *file_name);

    /*
     * Read asset information from INFO_HDR
     */
    void __das_ReadINFO_HDR (
        das_INFO_HDR *out_hdr,
        char *file_name
    );


    /*
     * Verify that all custom headers are skipped from reading
     */
    void __das_SkipMetaHeaders();
    

    /*
     * Read meta information contained between BEG_HDR and END_HDR
     */
    void __das_ReadMeta(char *file_name);


    /*
     * Read information from VERT_HDR
     */
    void __das_ReadVERT_HDR (
        das_VERT_HDR *out_hdr,
        char *file_name
    );

    
    /*
     * Read information about one vertex element header type
     */
    void __das_ReadGenVertHdr (
        __das_VertTemplate *out_hdr,
        char *hdr_name,
        char *file_name
    );


    /*
     * Read information from INDX_HDR
     */
    void __das_ReadINDX_HDR (
        das_INDX_HDR *out_hdr,
        char *file_name
    );


    /**************************************************/
    /******** File stream initialiser functions *******/
    /**************************************************/

    /*
     * Read all data from asset file into buffer for reading
     */
    void __das_ReadAssetFile(char *file_name); 


    /*
    * Free all resources allocated in asset reading 
    */
    void __das_ReadCleanup();


    /*******************************************/
    /******** Vertex copying functions *********/
    /*******************************************/


    /*
     * Copy asset vertices from buffer to out_vert
     * This functions assumes that the p_asset has correct vertices count set and
     * the offset is at the beginning of vertex data sequence
     */
    static void __das_CopyVertices (
        das_Asset *p_asset,
        char *file_name
    );


    /*
     * Copy all indices from buffer to p_out_ind
     */
    static void __das_CopyIndices (
        das_Asset *p_asset,
        deng_ui32_t ind_c,
        char *file_name
    );


    /*
     * Increment the reading offset by n bytes
     */
    void __das_IncrementOffset(size_t n);


    /*******************************************************************/
    /*********** Vertex normal calculation algorithm functions *********/
    /*******************************************************************/
    
    /*
     * Create vertex normals for vertices that are not normalised
     * and add them to closest 
     */
    static void __das_VertNormalise(das_Asset *p_asset);


    
    // Global static variables for DAS reader
    static FILE *__sfile = NULL;
    static char *__cbuf = NULL;
    static deng_ui64_t __buf_size = 0;
    static deng_ui64_t __offset = 0;

    static size_t __meta_c = 0;
    static char **__meta_infos = NULL;
#endif


/* 
 * Read .das binary asset file into das_Asset instance
 */
void das_LoadAsset (
    das_Asset *p_asset, 
    das_AssetMode dst_mode,
    das_ObjColorData color,
    deng_bool_t read_meta,
    char *tex_uuid,
    char *file_name
);


/*
 * Get all the metadata which was read from the last read_meta specified
 * das_LoadAsset call
 */
char **das_GetMeta(size_t *p_meta_c);


/*
 * Clean all memory allocated for meta data
 */
void das_CleanMeta();

#ifdef __cplusplus
}
#endif
#endif
