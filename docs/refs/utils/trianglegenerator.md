# dengUtils::TriangleGenerator
C++ specification  
File: shapes.h
## Description
Class for generating triangles in vertices or assets form

## Constructor
Constructor for class `dengUtils::TriangleGenerator` is following:  
```
TriangleGenerator(dengMath::vec2<deng_ui32_t> draw_bounds);
```
 
* `draw_bounds` specify the window size to draw at  

## List of public members
`dengUtils::TriangleGenerator` has following members:  

### Methods
* [generateAbsTriangle](#generateabstriangle)
* [generateRelTriangle](#generatereltriangle)
* [makeAbsTriangleAsset](#makeabstriangleasset)
* [makeRelTriangleAsset](#makereltriangleasset)


## Method specifications


### generateAbsTriangle
```
static void generateAbsTriangle (
    std::vector<VERT_UNMAPPED_2D> &vert,
    dengMath::vec2<deng_vec_t> tri_rec_pos,
    dengMath::vec2<deng_vec_t> tri_rec_size,
    dengMath::vec2<deng_vec_t> tri_rec_origin,
    dengMath::vec4<deng_vec_t> color,
    deng_ui32_t hier_level,
    std::array<dengMath::vec2<deng_vec_t>, 3> tri_rec_triangle_pos 
);
```

#### Description
Generate absolutely positioned triangle vertices

#### Parameters
* `vert` Output vertices vector 
* `tri_rec_pos` Triangle surrounding rectangle position
* `tri_rec_size` Triangle surrounding rectangle size
* `tri_rec_origin` Triangle surrounding rectangle origin
* `color` Triangle color
* `hier_level` Triangle hierarchy level within 2D assets
* `tri_rec_triangle_pos` Triangle vertices position relative to its surrounding rectangle


### generateRelTriangle
```
static void generateRelTriangle (
    std::vector<VERT_UNMAPPED_2D> &vert,
    VERT_UNMAPPED_2D *outer_rec,  
    dengMath::vec2<deng_vec_t> tri_rec_pos,
    dengMath::vec2<deng_vec_t> tri_rec_size,
    dengMath::vec2<deng_vec_t> tri_rec_origin,
    dengMath::vec4<deng_vec_t> color,
    deng_bool_t is_abs_size,
    deng_ui32_t hier_level,
    std::array<dengMath::vec2<deng_vec_t>, 3> tri_rec_triangle_pos
);
```

#### Description
Generate relatively positioned triangle vertices  
NOTE: This method uses heap allocation for asset vertices and indices!
Manual cleanup in necessary.  

#### Parameters
* `vert` Output vertices vector 
* `outer_rec` Must be a valid pointer to `VERT_UNMAPPED_2D` array where are at least
4 array elements
* `tri_rec_pos` Triangle surrounding rectangle relative position
* `tri_rec_size` Triangle surrounding rectangle relative size
* `tri_rec_origin` Triangle surrounding rectangle relative origin
* `color` Triangle color
* `is_abs_size` Specify if outer rectangle size should be absolute
* `hier_level` Triangle hierarchy level within 2D assets
* `tri_rec_triangle_pos` Triangle vertices position relative to its surrounding rectangle


### makeAbsTriangleAsset
```
das_Asset makeAbsTriangleAsset (
    const char *asset_id,
    dengMath::vec2<deng_vec_t> tri_rec_pos,
    dengMath::vec2<deng_vec_t> tri_rec_size,
    dengMath::vec2<deng_vec_t> tri_rec_origin,
    dengMath::vec4<deng_vec_t> color,
    std::array<dengMath::vec2<deng_vec_t>, 3> tri_rec_triangle_pos,
    deng_px_t border_width,
    deng_ui32_t hier_level,
    dengMath::vec4<deng_vec_t> border_color
);
```

#### Description
Generate absolutely positioned triangle asset  
NOTE: This method uses heap allocation for asset vertices and indices!
Manual cleanup in necessary.  

#### Parameters
* `asset_id` Specifies the asset id that is going to be used by generated asset. 
Note that it is recommended to generate UUID for each asset in order to avoid id collisions.
For more info about UUIDs in DENG read [this](../../uuids.md)
* `tri_rec_pos` Triangle surrounding rectangle position
* `tri_rec_size` Triangle surrounding rectangle size
* `tri_rec_origin` Triangle surrounding rectangle origin
* `color` Specify the triangle color
* `tri_rec_triangle_pos` Triangle vertices position relative to its surrounding rectangle
* `border_width` Specify the width of the border
* `hier_level` Triangle hierarchy level within 2D assets
* `border_color` Specify the border color


### makeRelTriangleAsset
```
das_Asset makeRelTriangleAsset (
    const char *asset_id,
    VERT_UNMAPPED_2D *outer_rec,
    dengMath::vec2<deng_vec_t> tri_rec_pos,
    dengMath::vec2<deng_vec_t> tri_rec_size,
    dengMath::vec2<deng_vec_t> tri_rec_origin,
    dengMath::vec4<deng_vec_t> color,
    std::array<dengMath::vec2<deng_vec_t>, 3> tri_rec_triangle_pos,
    deng_px_t border_width,
    dengMath::vec4<deng_vec_t> border_color,
    deng_ui32_t hier_level,
    deng_bool_t is_abs_size
);
```

#### Description
Generate relatively positioned triangle asset  
NOTE: This method uses heap allocation for asset vertices and indices!
Manual cleanup in necessary.  

#### Parameters
* `asset_id` Specifies the asset id that is going to be used by generated asset. 
Note that it is recommended to generate UUID for each asset in order to avoid id collisions.
For more info about UUIDs in DENG read [this](../../uuids.md)
* `outer_rec` Must be a valid pointer to `VERT_UNMAPPED_2D` array where are at least
4 array elements
* `tri_rec_pos` Triangle surrounding rectangle relative position
* `tri_rec_size` Triangle surrounding rectangle relative size
* `tri_rec_origin` Triangle surrounding rectangle relative origin
* `color` Specify the triangle color
* `tri_rec_triangle_pos` Triangle vertices position relative to its surrounding rectangle
* `border_width` Specify the width of the border
* `border_color` Specify the border color
* `hier_level` Triangle hierarchy level within 2D assets
* `is_abs_size` Specify if triangle surrounding rectangle size should be absolute
