---
title: RectangleGenerator
date: 2021-04-07
---

# dengUtils::RectangleGenerator

C++ specification  
File: shapes.h  
## Description
Generate rectangles in vertices and assets form

## Constructor
Constructor for class `dengUtils::RectangleGenerator` is following:  
```
RectangleGenerator(dengMath::vec2<deng_ui32_t> draw_bounds);
```
 
* `draw_bounds` specifies the window to draw at  

## List of public members
`dengUtils::RectangleGenerator` has following members:  

### Methods
* [generateUnmappedAbsRec](#generateunmappedabsrec)
* [generateUnmappedRelRec](#generateunmappedrelrec)
* [generateMappedAbsRec](#generatemappedabsrec)
* [generateMappedRelRec](#generatemappedrelrec)
* [makeUnmappedAbsRecAsset](#makeunmappedabsrecasset)
* [makeMappedAbsRecAsset](#makemappedabsrecasset)
* [makeUnmappedRelRecAsset](#makeunmappedrelrecasset)
* [makeMappedRelRecAsset](#makemappedrelrecasset)

## Method specifications

### generateUnmappedAbsRec
```
static void generateUnmappedAbsRec (
    dengMath::vec2<deng_vec_t> pos,
    dengMath::vec2<deng_vec_t> size,
    dengMath::vec2<deng_vec_t> origin,
    dengMath::vec4<deng_vec_t> color,
    deng_ui32_t hier_level,
    std::vector<VERT_UNMAPPED_2D> &vert   
);
```

#### Description
Generate vertices for absolutely positioned unmapped rectangle 

#### Parameters
* `pos` Specifies the position of the rectangle
* `size` Specifies the size of the rectangle
* `origin` Specifies the origin point of the rectangle
* `color` Specifies the color of the unmapped rectangle
* `hier_level` Specifies the hierarchy level of the rectangle between 2D assets
* `vert` Is the output vector for all the vertices generated


### generateUnmappedRelRec
```
static void generateUnmappedRelRec (
    dengMath::vec2<deng_vec_t> pos,
    dengMath::vec2<deng_vec_t> size,
    deng_bool_t is_abs_size,
    dengMath::vec2<deng_vec_t> origin,
    VERT_UNMAPPED_2D *outer_rec,
    dengMath::vec4<deng_vec_t> color,
    deng_ui32_t hier_level,
    std::vector<VERT_UNMAPPED_2D> &vert
);
```

#### Description
Generate rectangle that has its position, size and origin related to another rectangle

#### Parameters
* `pos` Specifies the relative position of the rectangle  
* `size` Specifies the relative or absolut size of the rectangle  
* `is_abs_size` Specify if size should be relative to outer rectangle or not  
* `origin` Specify the relative origin of the rectangle  
* `outer_rec` Must be a valid pointer to `VERT_UNMAPPED_2D` array instance that must contain  
at least 4 elements  
* `color` Specifies the unmapped rectangle color in float RGB  
* `hier_level` Specifies the hierarchy level of the rectangle between 2D assets  
* `vert` Is the output vector for all the vertices generated  


### generateMappedAbsRec
```
static void generateMappedAbsRec (
    dengMath::vec2<deng_vec_t> pos,
    dengMath::vec2<deng_vec_t> size,
    dengMath::vec2<deng_vec_t> origin,
    deng_ui32_t hier_level,
    std::vector<VERT_MAPPED_2D> &vert
);
```

#### Description
Generate vertices for absolutely positioned texture mapped rectangle.  
NOTE: This method does not declare any texture positions!  

#### Parameters
* `pos` Specifies the position of the rectangle
* `size` Specifies the size of the rectangle
* `origin` Specifies the origin of the rectangle
* `hier_level` Specifies the hierarchy level of the rectangle between 2D assets  
* `vert` Is the output vector for all the vertices generated  


### generateMappedRelRec
```
static void generateMappedRelRec (
    dengMath::vec2<deng_vec_t> pos,
    dengMath::vec2<deng_vec_t> size,
    deng_bool_t is_abs_size,
    dengMath::vec2<deng_vec_t> origin,
    VERT_UNMAPPED_2D *outer_rec,
    deng_ui32_t hier_level,
    std::vector<VERT_MAPPED_2D> &vert
);
```

#### Description
Generate vertices for relatively positioned texture mapped rectangle  
NOTE: This method does not declare any texture positions!  

#### Parameters
* `pos` Specifies the relative position of the rectangle
* `size` Specifies the relative or absolute size of the rectangle
* `is_abs_size` Specify if absolute size should be used in generating the rectangle
* `origin` Specifies the relative origin of the rectangle
* `outer_rec` Must be a valid pointer to `VERT_UNMAPPED_2D` array instance that must contain  
at least 4 elements  
* `hier_level` Specifies the hierarchy level of the rectangle between 2D assets  
* `vert` Is the output vector for all the vertices generated  


### makeUnmappedAbsRecAsset
```
das_Asset makeUnmappedAbsRecAsset (
    const char *asset_id,
    dengMath::vec2<deng_vec_t> pos,
    dengMath::vec2<deng_vec_t> size,
    dengMath::vec2<deng_vec_t> origin,
    dengMath::vec4<deng_vec_t> color,
    deng_ui32_t hier_level,
    deng_px_t border_width,
    dengMath::vec4<deng_vec_t> border_color
);
```

#### Description
Generate absolutely positioned unmapped rectangle asset.
NOTE: This method uses heap allocation for asset vertices and indices!
Manual cleanup is necessary.

#### Parameters
* `asset_id` Specifies the asset id that is going to be used by generated asset. 
Note that it is recommended to generate UUID for each asset in order to avoid id collisions.
For more info about UUIDs in DENG read [this](../../uuids.md)
* `pos` Specifies the position of the rectangle asset
* `size` Specifies the size of the rectangle asset
* `origin` Specifies the origin of the rectangle asset
* `color` Specifies the color of the rectangle asset
* `hier_level` Specifies the hierarchy level of the rectangle asset in 2D assets
* `border_width` Specifies the border width that is going to be generated for the rectangle
asset
* `border_color` Specifies the border color


### makeMappedAbsRecAsset
```
das_Asset makeMappedAbsRecAsset (
    const char *asset_id,
    const char *tex_id,
    dengMath::vec2<deng_vec_t> pos,
    dengMath::vec2<deng_vec_t> size,
    deng_ui32_t hier_level,
    dengMath::vec2<deng_vec_t> origin
);
```

#### Description
Generate absolutely positioned texture mapped rectangle asset.  
NOTE: This method uses heap allocation for asset vertices and indices!
Manual cleanup in necessary.  

#### Parameters
* `asset_id` Specifies the asset id that is going to be used by generated asset. 
Note that it is recommended to generate UUID for each asset in order to avoid id collisions.
For more info about UUIDs in DENG read [this](../../uuids.md)
* `tex_id` Specifies the texture ID that this asset is going to use
* `pos` Specifies the position of the rectangle asset
* `size` Specifies the size of the rectangle asset
* `hier_level` Specifies the hierarchy level of the rectangle asset in 2D assets
* `origin` Specifies the origin of the rectangle asset


### makeUnmappedRelRecAsset
```
das_Asset makeUnmappedRelRecAsset (
    const char *asset_id,
    dengMath::vec2<deng_vec_t> pos,
    dengMath::vec2<deng_vec_t> size,
    deng_bool_t is_abs_size,
    dengMath::vec2<deng_vec_t> origin,
    dengMath::vec4<deng_vec_t> color,
    VERT_UNMAPPED_2D *outer_rec,
    deng_px_t border_width,
    deng_ui32_t hier_level,
    dengMath::vec4<deng_vec_t> border_color
);
```

#### Description
Generate relatively positioned unmapped rectangle asset.  
NOTE: This method uses heap allocation for asset vertices and indices!
Manual cleanup in necessary.  

#### Parameters
* `asset_id` Specifies the asset id that is going to be used by generated asset. 
Note that it is recommended to generate UUID for each asset in order to avoid id collisions.
For more info about UUIDs in DENG read [this](../../uuids.md)
* `pos` Specifies the relative position of the rectangle asset
* `size` Specifies the relative or absolute size of the rectangle asset
* `is_abs_size` Specifies if size should be absolute or not
* `origin` Specifies the relative origin of the rectangle asset
* `color` Specifies the color of the rectangle asset
* `outer_rec` must be a valid pointer to `VERT_UNMAPPED_2D` array instance that must contain  
at least 4 elements  
* `border_width` Specifies the border width that is going to be generated for the rectangle
asset
* `hier_level` Specifies the hierarchy level of the rectangle between 2D assets  
* `border_color` Specifies the border color


### makeMappedRelRecAsset
```
das_Asset makeMappedRelRecAsset (
    const char *asset_id,
    const char *tex_id,
    dengMath::vec2<deng_vec_t> pos,
    dengMath::vec2<deng_vec_t> size,
    deng_bool_t is_abs_size,
    dengMath::vec2<deng_vec_t> origin,
    dengMath::vec4<deng_vec_t> color,
    deng_ui32_t hier_level,
    VERT_UNMAPPED_2D *outer_rec
);
```

#### Description
Generate relatively positioned texture mapped rectangle asset  
NOTE: This method uses heap allocation for asset vertices and indices!
Manual cleanup in necessary.  

#### Parameters
* `asset_id` Specifies the asset id that is going to be used by generated asset. 
Note that it is recommended to generate UUID for each asset in order to avoid id collisions.
For more info about UUIDs in DENG read [this](../../uuids.md)
* `tex_id` Specifies the texture ID that this asset is going to use
* `pos` Specifies the relative position of the rectangle asset
* `size` Specifies the relative or absolute size of the rectangle asset
* `is_abs_size` Specifies if size should be absolute or not
* `origin` Specifies the relative origin of the rectangle asset
* `color` Specifies the color of the rectangle asset
* `hier_level` Specifies the hierarchy level of the rectangle between 2D assets  
* `outer_rec` must be a valid pointer to `VERT_UNMAPPED_2D` array instance that must contain  
at least 4 elements  

