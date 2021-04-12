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
contain various information.  
**NOTE: Custom headers can only be placed in das file between INFO_HDR and VERT_HDR!**

#### INFO_HDR  
The layout for the INFO_HDR is following:  
* 8 bytes: Header name  
* 4 bytes: Header size 
* 32 bytes: Asset UUID
* 8 bytes: Timestamp  
* 1 byte: Asset type
* 1 byte Compression algorithm (default 0, reservered for future)

List of valid asset types are following:  
* 0 - Texture mapped normalised asset
* 1 - Unmapped normalised asset
* 2 - Texture mapped unormalised asset
* 3 - Unmapped unnormalised asset  

Currently compression algorithm specifying byte has to be 0 and is reserved for future use.

#### VERT_HDR  
The layout for the VERT_HDR is following:  
* 8 bytes: Header name  
* 4 bytes: Header size  
* 4 bytes: Vertices count  
* max(n * 32) bytes: Vertices data  
    * 12 bytes: Vertex coordinates (x, y, z: type f32)  
    * 8 bytes: Texture coordinates (x, y: type f32)  
    * 12 bytes: Vertex normal coordinates (x, y, z: type f32)  

#### INDX_HDR  
The layout for the INDX_HDR is following:  
* 8 bytes: Header name  
* 4 bytes: Header size (bytes)  
* 4 bytes: Indices count  
* n * 4 bytes: Indices data (type ui32)

#### Custom headers 
Custom headers can be used to store some loader specific information
or they can be optionally ignored as well.  
The layout for custom header is following:  
* 7 bytes: Header begginning (BEG_HDR)
* 4 bytes: Header size
* 4 bytes: Data size
* n bytes: Data
* 7 bytes: Header ending (END_HDR)
