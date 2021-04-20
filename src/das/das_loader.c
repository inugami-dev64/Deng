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


#define __DAS_LOADER_C
#include <das/das_loader.h>
 

/******************************************/
/******* Asset assembler functions ********/
/******************************************/

/* 
 * Assemble INFO_HDR in das file
 * This function assumes that __wfile is a valid FILE stream
 */
void __das_AssembleINFOHDR (
    char *uuid,
    das_AssetMode asset_mode
) {
    das_INFO_HDR ihdr = {0};
    strncpy (
        ihdr.hdr_name,
        __DAS_INFO_HEADER_NAME,
        8
    );
    
    ihdr.hdr_size = 54;
    strncpy (
        ihdr.uuid,
        uuid,
        36
    );

    ihdr.time_st = (deng_ui64_t) time(NULL);
    ihdr.asset_type = (deng_ui8_t) asset_mode;
    ihdr.cmpr = 0;

    fwrite(&ihdr, sizeof(das_INFO_HDR), 1, __wfile);
}


/*
 * Assemble VERT_HDR part in das file
 * This function assumes that __wfile is a valid FILE stream
 */
void __das_AssembleVERTHDR (
    das_VertDynamic vert, 
    das_AssetMode asset_mode
) {
    das_VERT_HDR vhdr = {0};
    strncpy (
        vhdr.hdr_name,
        __DAS_VERTICES_HEADER_NAME,
        8
    );

    vhdr.vert_c = vert.n;

    void *vert_data = NULL;
    size_t vert_size = 0;

    // Check for asset type used and set pointers and sizes accordingly
    switch(asset_mode) 
    {
    case DAS_ASSET_MODE_3D_UNMAPPED:
        vhdr.hdr_size = vhdr.vert_c * sizeof(VERT_UNMAPPED_UNOR) + 16;
        vert_data = (void*) vert.vuu;
        vert_size = vhdr.vert_c * sizeof(VERT_UNMAPPED_UNOR);
        break;

    case DAS_ASSET_MODE_3D_TEXTURE_MAPPED:
        vhdr.hdr_size = vhdr.vert_c * sizeof(VERT_MAPPED_UNOR) + 16;
        vert_data = (void*) vert.vmu;
        vert_size = vhdr.vert_c * sizeof(VERT_MAPPED_UNOR);
        break;

    case DAS_ASSET_MODE_3D_UNMAPPED_NORMALISED:
        vhdr.hdr_size = vhdr.vert_c * sizeof(VERT_UNMAPPED_NOR) + 16;
        vert_data = (void*) vert.vun;
        vert_size = vhdr.vert_c * sizeof(VERT_UNMAPPED_NOR);
        break;

    case DAS_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
        vhdr.hdr_size = vhdr.vert_c * sizeof(VERT_MAPPED_NOR) + 16;
        vert_data = (void*) vert.vmn;
        vert_size = vhdr.vert_c * sizeof(VERT_MAPPED_NOR);
        break;
    default:
        break;
    }

    // Write header data from stack to file
    fwrite(&vhdr, sizeof(das_VERT_HDR), 1, __wfile);

    // Write asset vertices to file
    fwrite(vert_data, 1, vert_size, __wfile);
}


/*
 * Assemble INDX_HDR part in das file
 * This function assumes that __wfile is a valid FILE stream
 */
void __das_AssembleINDXHDR(das_IndicesDynamic ind) {
    das_INDX_HDR ihdr = {0};
    strncpy (
        ihdr.hdr_name,
        __DAS_INDICES_HEADER_NAME,
        8
    );

    ihdr.hdr_size = 16 + ind.n * sizeof(deng_ui32_t);
    ihdr.ind_c = ind.n;

    // Write stack allocated INDX_HDR data to a file
    fwrite(&ihdr, sizeof(das_INDX_HDR), 1, __wfile);

    // Write all indices to a file
    fwrite(ind.indices, ind.n, sizeof(deng_ui32_t), __wfile);
}


/*
 * Assemble new custom header for das file
 * This function assumes that wfile is a valid FILE stream
 */
void __das_AssembleCustomHDR(das_HeaderInfo hdr_info) {
    strncpy (
        hdr_info.beg,
        __DAS_GENERIC_HEADER_BEG,
        7
    );

    strncpy (
        hdr_info.end,
        __DAS_GENERIC_HEADER_END,
        7
    );

    // Write header opening to asset file
    fwrite(hdr_info.beg, sizeof(char), 7, __wfile);

    // Write header size to asset file
    fwrite(&hdr_info.hdr_size, sizeof(deng_ui32_t), 1, __wfile);

    // Write main header data size to asset file
    fwrite(&hdr_info.data_size, sizeof(deng_ui32_t), 1, __wfile);

    // Write all header data to asset file
    fwrite(hdr_info.data, sizeof(char), hdr_info.data_size, __wfile);
}


/* 
 * Assemble new asset file
 * This function expects that out_file already has .das extension
 */
void das_Assemble (
    das_Asset *p_asset, 
    char *file_name,
    das_HeaderInfo *hdr_infos,
    size_t hdr_ic
) {
    __wfile = fopen (
        file_name, 
        "wb"
    );

    if(!__wfile) FILE_ERR(file_name);
    
    // Generate new uuid for the asset
    char *uuid = uuid_Generate();

    __das_AssembleINFOHDR (
        uuid,
        p_asset->asset_mode
    );

    // Write optional headers if needed
    for(size_t i = 0; i < hdr_ic; i++)
        __das_AssembleCustomHDR(hdr_infos[i]);

    __das_AssembleVERTHDR (
        p_asset->vertices,
        p_asset->asset_mode
    );
    
    __das_AssembleINDXHDR(p_asset->indices);

    fclose(__wfile);
}



/****************************************/
/********* Binary asset reader **********/
/****************************************/


/* 
 * Read .das binary asset file into das_Asset instance
 */
void das_LoadAsset (
    das_Asset *p_asset, 
    das_AssetMode dst_mode,
    das_ObjColorData color,
    char *tex_uuid,
    char *file_name
) {
    p_asset->uuid = uuid_Generate();
    p_asset->tex_uuid = tex_uuid;
    p_asset->color = color;
    p_asset->is_shown = true;
    p_asset->force_unmap = false;

    // Write all file contents into a buffer
    __das_ReadAssetFile((char*) file_name);

    das_INFO_HDR inf_hdr = {0};
    das_VERT_HDR vert_hdr = {0};
    das_INDX_HDR indx_hdr = {0};

    __das_ReadINFOHDR (
        &inf_hdr,
        file_name
    );

    __das_ReadVERTHDR (
        &vert_hdr,
        file_name
    );

    p_asset->vertices.n = vert_hdr.vert_c;

    __das_CopyVertices (
        p_asset,
        inf_hdr.asset_type,
        file_name
    );

    __das_ReadINDXHDR (
        &indx_hdr,
        file_name
    );

    p_asset->indices.n = indx_hdr.ind_c;

    __das_CopyIndices (
        &p_asset->indices.indices,
        indx_hdr.ind_c,
        file_name
    );

    __das_ReadCleanup();
}


/*
 * Read all data from asset file into buffer for reading
 */
void __das_ReadAssetFile(char *file_name) {
    FILE *file;
    __offset = 0;
    file = fopen(file_name, "rb");

    if(!file) FILE_ERR(file_name);

    fseek(file, 0, SEEK_END);
    __buf_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    __buf = (char*) calloc (
        __buf_size + 1,
        sizeof(char)
    );

    fread(__buf, sizeof(char), __buf_size, file);

    fclose(file);
}


/*
 * Free all resources allocated in asset reading 
 */
void __das_ReadCleanup() {
    free(__buf);
}


/* 
 * Read asset information from INFO_HDR
 */
void __das_ReadINFOHDR (
    das_INFO_HDR *out_hdr,
    char *file_name
) {
    if(__offset + sizeof(das_INFO_HDR) >= __buf_size)
        __DAS_CORRUPT_ERROR(file_name);


    // INFO_HDR is always the first header in .das file
    memcpy(out_hdr, __buf + __offset, sizeof(das_INFO_HDR));
    __offset += sizeof(das_INFO_HDR);

    char hdr_pad[9] = {0};
    strncpy(hdr_pad, out_hdr->hdr_name, 8);

    // Verify data correctness
    if (
        strcmp(hdr_pad, __DAS_INFO_HEADER_NAME) || 
        out_hdr->cmpr
    ) __DAS_CORRUPT_ERROR(file_name);
}


/*
 * Verify that all custom headers are skipped from reading
 */
void __das_SkipCustomHeaders(char *file_name) {
    deng_ui32_t *indices;
    deng_ui32_t ind_c = 0; 

    cm_FindAllSubstrings (
        __buf,
        __DAS_GENERIC_HEADER_BEG,
        &indices,
        &ind_c
    ); 

    if(ind_c) {
        __offset = indices[ind_c - 1];
        deng_ui32_t hdr_size = 0;
        // Buffer bounds check
        if(__offset + 7 + sizeof(deng_ui32_t) >= __buf_size)
            __DAS_CORRUPT_ERROR(file_name);

        memcpy(&hdr_size, __buf + __offset + 7, sizeof(deng_ui32_t));
        __offset += hdr_size;
    }
}


/*
 * Increment reading offset by n
 * If offset is greater than the buffer size the remainder
 * modulo will be the offset
 */
void __das_IncrementOffset(size_t n) {
    __offset += n;
    __offset %= __buf_size;
}


/*
 * Copy asset vertices from buffer to out_vert
 */
void __das_CopyVertices (
    das_Asset *p_asset,
    das_AssetMode src_mode,
    char *file_name
) {
    deng_bool_t skip_tex = false;
    deng_bool_t skip_norm = false;

    switch(p_asset->asset_mode)
    {
    case DAS_ASSET_MODE_3D_UNMAPPED:
    DAS_ASSET_MODE_3D_UNMAPPED:
        p_asset->vertices.vuu = (VERT_UNMAPPED_UNOR*) calloc (
            p_asset->vertices.n,
            sizeof(VERT_UNMAPPED_UNOR)
        );

        skip_tex = true;
        skip_norm = true;
        break;

    case DAS_ASSET_MODE_3D_UNMAPPED_NORMALISED:
    DAS_ASSET_MODE_3D_UNMAPPED_NORMALISED:
        p_asset->vertices.vun = (VERT_UNMAPPED_NOR*) calloc (
            p_asset->vertices.n,
            sizeof(VERT_UNMAPPED_NOR)
        );
        
        skip_tex = true;
        break;

    case DAS_ASSET_MODE_3D_TEXTURE_MAPPED:
    DAS_ASSET_MODE_3D_TEXTURE_MAPPED:
        p_asset->vertices.vmu = (VERT_MAPPED_UNOR*) calloc (
            p_asset->vertices.n,
            sizeof(VERT_MAPPED_UNOR)
        );

        skip_norm = true;
        break;

    case DAS_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
    DAS_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
        printf("vert_c: %ld\n", p_asset->vertices.n);
        p_asset->vertices.vmn = (VERT_MAPPED_NOR*) calloc (
            p_asset->vertices.n,
            sizeof(VERT_MAPPED_NOR)
        );
        break;

    case DAS_ASSET_MODE_DONT_CARE:
        // Allocate memory according to src_mode
        switch(src_mode)
        {
        case DAS_ASSET_MODE_3D_UNMAPPED:
            goto DAS_ASSET_MODE_3D_UNMAPPED;
            break;

        case DAS_ASSET_MODE_3D_UNMAPPED_NORMALISED:
            goto DAS_ASSET_MODE_3D_UNMAPPED_NORMALISED;
            break;

        case DAS_ASSET_MODE_3D_TEXTURE_MAPPED:
            goto DAS_ASSET_MODE_3D_TEXTURE_MAPPED;
            break;

        case DAS_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
            goto DAS_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED;
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }


    // Copy all data from buffer to out_vert
    switch(src_mode) 
    {
    case DAS_ASSET_MODE_3D_UNMAPPED:
        if(__offset + sizeof(VERT_UNMAPPED_UNOR) * p_asset->vertices.n >= __buf_size)
            __DAS_CORRUPT_ERROR(file_name);

        // Destination vertex is VERT_UNMAPPED_UNOR
        if(skip_norm && skip_tex) {
            memcpy (
                p_asset->vertices.vuu, 
                __buf + __offset, 
                sizeof(VERT_UNMAPPED_UNOR) * p_asset->vertices.n
            );
        }

        else __DAS_INVALID_ASSET_FORMAT(file_name);

        __offset += sizeof(VERT_UNMAPPED_UNOR) * p_asset->vertices.n;
        break;

    case DAS_ASSET_MODE_3D_UNMAPPED_NORMALISED:
        if(__offset + sizeof(VERT_UNMAPPED_NOR) * p_asset->vertices.n >= __buf_size)
            __DAS_CORRUPT_ERROR(file_name);
            

        // Destination vertex is VERT_UNMAPPED_NOR
        if(!skip_norm && skip_tex) {
            memcpy (
                p_asset->vertices.vun,
                __buf + __offset,
                sizeof(VERT_UNMAPPED_NOR) * p_asset->vertices.n
            );

            __offset += sizeof(VERT_UNMAPPED_NOR) * p_asset->vertices.n;
        }

        // Destination vertex is VERT_UNMAPPED_UNOR
        else if(skip_norm && skip_tex) {
            VERT_UNMAPPED_NOR *tmp = (VERT_UNMAPPED_NOR*) calloc (
                p_asset->vertices.n,
                sizeof(VERT_UNMAPPED_NOR)
            );

            memcpy (
                tmp,
                __buf + __offset,
                sizeof(VERT_UNMAPPED_NOR) * p_asset->vertices.n
            );
            
            __offset += sizeof(VERT_UNMAPPED_NOR) * p_asset->vertices.n;
            for(size_t i = 0; i < p_asset->vertices.n; i++) {
                p_asset->vertices.vuu[i] = tmp[i].vert_data;
            }

            free(tmp);
        }

        else __DAS_INVALID_ASSET_FORMAT(file_name);
        break;

    case DAS_ASSET_MODE_3D_TEXTURE_MAPPED:
        if(__offset + sizeof(VERT_MAPPED_UNOR) * p_asset->vertices.n >= __buf_size)
            __DAS_CORRUPT_ERROR(file_name);

        // Destination vertex is VERT_MAPPED_UNOR
        if(skip_norm && !skip_tex) {
            memcpy (
                p_asset->vertices.vmu,
                __buf + __offset,
                sizeof(VERT_MAPPED_UNOR) * p_asset->vertices.n
            );

            __offset += sizeof(VERT_MAPPED_UNOR) * p_asset->vertices.n;
        }

        // Destination vertex is VERT_UNMAPPED_UNOR
        else if(skip_norm && skip_tex) {
            VERT_MAPPED_UNOR *tmp = (VERT_MAPPED_UNOR*) calloc (
                p_asset->vertices.n,
                sizeof(VERT_MAPPED_UNOR)
            );

            memcpy (
                tmp,
                __buf + __offset,
                p_asset->vertices.n * sizeof(VERT_MAPPED_UNOR)
            );

            __offset += p_asset->vertices.n * sizeof(VERT_MAPPED_UNOR);

            for(size_t i = 0; i < p_asset->vertices.n; i++) {
                p_asset->vertices.vuu[i] = tmp[i].vert_data;
            }

            free(tmp);
        }

        else __DAS_INVALID_ASSET_FORMAT(file_name);
        break;

    case DAS_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
        if(__offset + sizeof(VERT_MAPPED_NOR) * p_asset->vertices.n >= __buf_size)
            __DAS_CORRUPT_ERROR(file_name);

        // Destination vertex is VERT_MAPPED_NOR
        if(!skip_norm && !skip_tex) {
            memcpy (
                p_asset->vertices.vmn,
                __buf + __offset,
                sizeof(VERT_MAPPED_NOR) * p_asset->vertices.n
            );

            __offset += sizeof(VERT_MAPPED_NOR) * p_asset->vertices.n;
        }

        else {
            // Allocate temporary buffer for storing vertices
            VERT_MAPPED_NOR *tmp = (VERT_MAPPED_NOR*) calloc (
                p_asset->vertices.n,
                sizeof(VERT_MAPPED_NOR)
            );

            memcpy (
                tmp,
                __buf + __offset,
                sizeof(VERT_MAPPED_NOR) * p_asset->vertices.n
            );

            __offset += sizeof(VERT_MAPPED_NOR) * p_asset->vertices.n;

            // Destination vertex is VERT_MAPPED_UNOR
            if(skip_norm && !skip_tex) {
                for(size_t i = 0; i < p_asset->vertices.n; i++) {
                    p_asset->vertices.vmu[i].vert_data = tmp[i].vert_data;
                    p_asset->vertices.vmu[i].tex_data = tmp[i].tex_data;
                }
            }

            // Destination vertex is VERT_UNMAPPED_NOR
            else if(!skip_norm && skip_tex) {
                for(size_t i = 0; i < p_asset->vertices.n; i++) {
                    p_asset->vertices.vun[i].vert_data = tmp[i].vert_data;
                    p_asset->vertices.vun[i].norm_data = tmp[i].norm_data;
                }
            }

            // Destination vertex is VERT_UNMAPPED_UNOR
            else if(!skip_norm && !skip_tex) {
                for(size_t i = 0; i < p_asset->vertices.n; i++) {
                    p_asset->vertices.vuu[i] = tmp[i].vert_data;
                }
            }

            free(tmp);
        }
        break;

    default: 
        break;
    }
}


/*
 * Read asset information from VERT_HDR
 */
void __das_ReadVERTHDR (
    das_VERT_HDR *out_hdr,
    char *file_name
) {
    if(__offset + sizeof(das_VERT_HDR) >= __buf_size)
        __DAS_CORRUPT_ERROR(file_name);

    // Copy the first part of the buffer into das_VERT_HDR struct
    memcpy (
        out_hdr, 
        __buf + __offset, 
        sizeof(das_VERT_HDR)
    );
    __offset += sizeof(das_VERT_HDR);

    char pad_hdr[9] = {0};
    strncpy(pad_hdr, out_hdr->hdr_name, 8);

    // Check if header name is correct
    if(strcmp(pad_hdr, __DAS_VERTICES_HEADER_NAME))
        __DAS_CORRUPT_ERROR(file_name);
}


/*
 * Copy all indices from buffer to p_out_ind
 * This function allocates memory and copies 
 * the data from file buffer to *p_out_ind
 */
void __das_CopyIndices (
    deng_ui32_t **p_out_ind,
    deng_ui32_t ind_c,
    char *file_name
) {
    if(__offset + ind_c * sizeof(deng_ui32_t) > __buf_size)
        __DAS_CORRUPT_ERROR(file_name);

    (*p_out_ind) = (deng_ui32_t*) calloc (
        ind_c,
        sizeof(deng_ui32_t)
    );

    memcpy (
        (*p_out_ind),
        __buf + __offset,
        ind_c * sizeof(deng_ui32_t) 
    );

    __offset += ind_c * sizeof(deng_ui32_t);
}


/*
 * Read asset information from INDX_HDR
 */
void __das_ReadINDXHDR (
    das_INDX_HDR *out_hdr,
    char *file_name
) {
    if(__offset + sizeof(das_INDX_HDR) >= __buf_size)
        __DAS_CORRUPT_ERROR(file_name);


    memcpy (
        out_hdr, 
        __buf + __offset, 
        sizeof(das_INDX_HDR)
    );

    __offset += sizeof(das_INDX_HDR);

    char pad_hdr[9] = {0};
    strncpy(pad_hdr, out_hdr->hdr_name, 8);

    // Check if header name is correct
    if(strcmp(pad_hdr, __DAS_INDICES_HEADER_NAME)) 
        __DAS_CORRUPT_ERROR(file_name);
}


/* 
 * Destroy all allocated assets 
 */
void das_DestroyAssets (
    das_Asset *assets,
    deng_ui32_t asset_c
) {
    // O(n)
    deng_i32_t l_index;
    for(l_index = 0; l_index < asset_c; l_index++) {
        switch (assets[l_index].asset_mode)
        {
        case DAS_ASSET_MODE_2D_UNMAPPED:
            free(assets[l_index].vertices.vu2d);
            break;

        case DAS_ASSET_MODE_2D_TEXTURE_MAPPED:
            free(assets[l_index].vertices.vm2d);
            break;

        case DAS_ASSET_MODE_3D_UNMAPPED:
            free(assets[l_index].vertices.vuu);
            break;

        case DAS_ASSET_MODE_3D_UNMAPPED_NORMALISED:
            free(assets[l_index].vertices.vun);
            break;

        case DAS_ASSET_MODE_3D_TEXTURE_MAPPED:
            free(assets[l_index].vertices.vmu);
            break;

        case DAS_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
            free(assets[l_index].vertices.vmn);
            break;
        
        default:
            break;
        }
    }
    
    free(assets[l_index].indices.indices);
    if(asset_c) free(assets);
}


/* 
 * Destroy all allocated textures 
 */
void das_DestroyTextures (
    das_Texture *textures,
    deng_ui32_t tex_c
) {
    deng_i32_t l_index;
    for(l_index = 0; l_index < tex_c; l_index++) 
        free(textures[l_index].pixel_data.p_pixel_data);

    if(tex_c) free(textures);
}