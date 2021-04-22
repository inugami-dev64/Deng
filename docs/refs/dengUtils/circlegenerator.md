---
title: CircleGenerator
date: 2021-04-07
---

# dengUtils::CircleGenerator

C++ specification  
File: shapes.h
## Description
Class for circle generation in vertices or asset form  

## Constructor
Constructor for class `dengUtils::CircleGenerator` is following:  
```
CircleGenerator(dengMath::vec2<deng_ui32_t> draw_bounds);
```
 
* `draw_bounds` Specify the size of the window to draw in 

## List of public members
`dengUtils::CircleGenerator` has following members:  

### Methods
* [generateAbsCircle](#generateabscircle)
* [generateRelCircle](#generaterelcircle)
* [makeAbsCircleAsset](#makeabscircleasset)
* [makeRelCircleAsset](#makerelcircleasset)

## Method specifications


### generateAbsCircle
```
static void generateAbsCircle (
    std::vector<VERT_UNMAPPED_2D> &vert,
    std::vector<deng_ui32_t> &indices,
    dengMath::vec2<deng_vec_t> pos,
    deng_vec_t radius,
    deng_ui32_t hier_level,
    dengMath::vec4<deng_vec_t> color
);
```

#### Description
Generate absolutely positioned circle

#### Parameters
* `vert` Output vertices vector 
* `indices` Output indices vector
* `pos` Position of the circle
* `radius` Size of the circle radius
* `hier_level` Circle hierarchy level within 2D assets
* `color` Circle color


### generateRelCircle
```
static void generateRelCircle (
    std::vector<VERT_UNMAPPED_2D> &vert,
    std::vector<deng_ui32_t> &indices,
    dengMath::vec2<deng_vec_t> pos,
    deng_vec_t radius,
    dengMath::vec4<deng_vec_t> color,
    deng_ui32_t hier_level,
    VERT_UNMAPPED_2D *outer_rec
);
```

#### Description
Generate relatively positioned circle

#### Parameters
* `vert` Output vertices vector 
* `indices` Output indices vector
* `pos` Relative position of the circle
* `radius` Relative size of the circle radius
* `color` Circle color
* `hier_level` Circle hierarchy level within 2D assets
* `outer_rec` Must be a valid pointer to `VERT_UNMAPPED_2D` array that has
at least 4 members


### makeAbsCircleAsset
```
das_Asset makeAbsCircleAsset (
    const char *asset_id,
    dengMath::vec2<deng_vec_t> pos,
    deng_vec_t radius,
    dengMath::vec4<deng_vec_t> color,
    deng_px_t border_width,
    deng_ui32_t hier_level,
    dengMath::vec4<deng_vec_t> border_color
);
```

#### Description
Generate absolutely positioned circle asset

#### Parameters
* `asset_id` Specifies the asset id that is going to be used by generated asset. 
Note that it is recommended to generate UUID for each asset in order to avoid id collisions.
For more info about UUIDs in DENG read [this](../../uuids.md)
* `pos` Position of the circle
* `radius` Size of the circle radius
* `color` Circle color
* `border_width` Width of the border in pixels
* `hier_level` Circle hierarchy level within 2D assets
* `border_color` Color of the border


### makeRelCircleAsset
```
das_Asset makeRelCircleAsset (
    const char *asset_id,
    dengMath::vec2<deng_vec_t> pos,
    deng_vec_t radius,
    dengMath::vec4<deng_vec_t> color,
    VERT_UNMAPPED_2D *outer_rec,
    deng_px_t border_width,
    deng_ui32_t hier_level,
    dengMath::vec4<deng_vec_t> border_color
);
```

#### Description
Generate relatively positioned circle asset

#### Parameters
* `asset_id` Specifies the asset id that is going to be used by generated asset. 
Note that it is recommended to generate UUID for each asset in order to avoid id collisions.
For more info about UUIDs in DENG read [this](../../uuids.md)
* `pos` Position of the circle
* `radius` Size of the circle radius
* `color` Circle color
* `outer_rec` Must be a valid pointer to `VERT_UNMAPPED_2D` array that has
at least 4 members
* `border_width` Width of the border in pixels
* `hier_level` Circle hierarchy level within 2D assets
* `border_color` Color of the border
