# DENG asset file format (.das) specifications  
DAS format consists of three headers INFO_HDR, VERT_HDR and INDX_HDR  
  
## INFO_HDR  
The layout for the INFO_HDR is following:  
* 8 bytes: Header name  
* 4 bytes: Header size 
* 8 bytes: Timestamp  
* 1 byte: Vertices type
* 1 bytes: Name size
* 1 - 24 bytes: Asset name  

## VERT_HDR  
The layout for the VERT_HDR is following:  
* 8 bytes: Header name  
* 4 bytes: Header size  
* 4 bytes: Vertices count  
* max(n * 32) bytes: Vertices data  
    * 12 bytes: Vertex coordinates (x, y, z: type f32)  
    * 8 bytes: Texture coordinates (x, y: type f32)  
    * 12 bytes: Vertex normal coordinates (x, y, z: type f32)  

## INDX_HDR  
The layout for the INDX_HDR is following:  
* 8 bytes: Header name  
* 4 bytes: Header size (bytes)  
* 4 bytes: Indices count  
* n * 4 bytes: Indices data (type ui32)
