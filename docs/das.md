---
title: DAS file format
date: 2021-04-07
---

# DENG asset file format (.das) specifications  
DAS is a file format implementation for storing 3D models by DENG project. 
This file format uses .das extension and can be read header by header. 
DENG project includes a tool for translating various 3D model formats into 
DENG asset format, but if you want to create your own implementation of the 
format translator then read about the file structure below.  
DAS format consists of mainly three headers INFO_HDR, VERT_HDR and INDX_HDR.  
Additional headers are specified with following tags BEG_HDR and END_HDR and can
contain various meta information.  
**NOTE: Custom headers can only be placed in das file between INFO_HDR and VERT_HDR!**

### Magic number 
DENG asset format has three magic numbers which help to identify the specific
use case for given file. The magic numbers in little endian are following:  
* Static asset format `0x5453544553535144`  
* Animation format `0x4e41544553535144` (reserved for future)  
* Map data format `0x504d544553535144` (reserved for future)  


### INFO_HDR  
The layout for the INFO_HDR is following:  
* 8 bytes: Magic number (in little endian: 0x5453544553535144)
* 8 bytes: Header name  
* 4 bytes: Header size 
* 32 bytes: Asset UUID
* 8 bytes: Timestamp  
* 1 byte: Asset type
* 1 byte Compression level (default 0, reservered for future)

List of valid asset types are following:  
* 0 - Texture mapped normalised asset
* 1 - Unmapped normalised asset
* 2 - Texture mapped unormalised asset
* 3 - Unmapped unnormalised asset  

Currently compression algorithm specifying byte has to be 0 and is reserved for future use.

### VERT_HDR  
The layout for the VERT_HDR is following:  
* 8 bytes: Header name  
* 4 bytes: Header size  

#### VPOS_HDR
VPOS_HDR is a subheader of VERT_HDR, which contains information about vertex positions. This header is necessary at 
all cases otherwise the asset is considered as corrupted.  
The layout of VPOS_HDR is following:  
* 8 bytes: Header name
* 4 bytes: Header size
* 4 bytes: Vertices count  
* n * 12 bytes: Vertex coordinates (x, y, z: type f32)

#### VTEX_HDR
VTEX_HDR is a subheader of VERT_HDR, which contains information about texture positions. This header is not necessary if
the asset mode is any of the following: `DAS_ASSET_MODE_UNMAPPED`, `__DAS_ASSET_MODE_UNMAPPED_UNOR`  
The layout of VTEX_HDR is following:  
* 8 bytes: Header name  
* 4 bytes: Header size  
* 4 bytes: Vertices count  
* n * 8 bytes: Texture coordinates (x, y: type f32)  

#### VNOR_HDR
VNOR_HDR is a subheader of VERT_HDR, which contains information about vertex normals. This header is not necessary if
the asset mode is any of the following: `__DAS_ASSET_MODE_UNMAPPED_UNOR`, `__DAS_ASSET_MODE_TEXTURE_MAPPED_UNOR`  
The layout of VNOR_HDR is following:  
* 8 bytes: Header name  
* 4 bytes: Header size  
* 4 bytes: Vertices count  
* n * 12 bytes: Vertex normal coordinates (x, y, z: type f32)

### INDX_HDR  
The layout for the INDX_HDR is following:  
* 8 bytes: Header name  
* 4 bytes: Header size (bytes)  
* 4 bytes: Indices count  
* m * n * 4 bytes: Indices data (type ui32)  

Where m represents the vertex elements count which can be determined from the asset mode as follows:
`__DAS_ASSET_MODE_3D_UNMAPPED_UNOR` - 1
`DAS_ASSET_MODE_3D_UNMAPPED` - 2
`__DAS_ASSET_MODE_3D_TEXTURE_MAPPED_UNOR` - 2
`DAS_ASSET_MODE_3D_TEXTURE_MAPPED` - 3

### META_HDR
Meta headers can be used to store some additional information
or they can be optionally ignored as well.  
The layout for META_HDR is following:  
* 8 bytes: Header beginning (META_HDR)
* 4 bytes: Header size
* 4 bytes: Data size
* n bytes: Data
