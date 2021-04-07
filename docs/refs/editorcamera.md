# deng::EditorCamera

C++ specification
File: camera.h
## Description


## Constructor
Constructor for class `deng::EditorCamera` is following:  
```
deng::EditorCamera::EditorCamera (
    deng_vec_t zoom_step,
    const dengMath::vec3<deng_vec_t> &origin,
    const dengMath::vec2<deng_f64_t> &mouse_sens,
    deng_vec_t FOV,
    deng_vec_t near_plane,
    deng_vec_t far_plane,
    WindowWrap *p_ww
);
```
 
* `zoom_step` specifies the amount of movement that is made by zooming in or out
during one scroll event
* `origin` specifies the origin of the editor camera rotation
* `mouse_sens` specifies the multiplier used to calculate mouse sensitivity
* `FOV` specifies the field of view 
* `near_plane` specifies the closest distance that could be viewed
* `far_plane` specifies the furtherest distance that could be viewed
* `*p_ww` specify current `deng::WindowWrap` instance that is used


## List of public members
`deng::EditorCamera` has following members:  

### Methods
* [update](#update)

### Variables
* view_matrix -- This variable specifies the view matrix instance that is used
by the camera instance
* p_projection_matrix -- This variable specifies the projection matrix that is
used by the camera instance

## Method specifications

### update
```
void update()
```
#### Description
Caller method for updating the camera event system
