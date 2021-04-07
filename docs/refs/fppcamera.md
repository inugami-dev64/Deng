# deng::FPPCamera

C++ specification
File: camera.h
## Description
First person perspective camera class

## Constructor
Constructor for class `deng::FPPCamera` is following:  
```
deng::FPPCamera::FPPCamera (
    const dengMath::vec3<deng_vec_t> &camera_mov_speed_mul, 
    const dengMath::vec2<deng_f64_t> &mouse_sens, 
    deng_vec_t FOV, 
    deng_vec_t near_plane, 
    deng_vec_t far_plane, 
    FPPInputChangeCallback mov_disable_callback,
    FPPInputChangeCallback mov_enable_callback,
    WindowWrap *p_ww
);
```

* `camera_mov_speed_mul` specifies the multiplier that is used to calculate camera movement speed per 
17 millisecond interval 
* `mouse_sens` specifies the multiplier used to calculate mouse sensitivity
* `FOV` specifies the field of view 
* `near_plane` specifies the closest distance that could be viewed
* `far_plane` specifies the furtherest distance that could be viewed
* `mov_disable_callback` specify the callback that should be called when disabling
movement. This value is optional and can be set to `NULL`
* `mov_enable_callback` specify the callback that should be called when enabling
movement. This value is optional and can be set to `NULL`
* `*p_ww` specify current `deng::WindowWrap` instance that is used

## List of public members
`deng::FPPCamera` has following members:  

### Methods
* [moveW](#movew)
* [moveRW](#moverw)
* [moveU](#moveu)
* [moveRU](#moveru)
* [moveV](#movev)
* [moveRV](#moverv)
* [update](#update)
### Variables
* view_matrix -- This variable specifies the view matrix instance that is used
by the camera instance
* p_projection_matrix -- This variable specifies the projection matrix that is
used by the camera instance


## Method specifications

### moveW
```
void moveW();
```
#### Description
Move camera in its coordinate system on w axis by movement step


### moveRW
```
void moveW();
```

#### Description
Move camera in its coordinate system on w axis by negative movement step


### moveU
```
void moveU();
```
#### Description
Move camera in its coordinate system on u axis by movement step


### moveRU
```
void moveRU();
```
#### Description
Move camera in its coordinate system on u axis by negative movement step


### moveV
```
void moveV();
```
#### Description
Move camera in its coordinate system on v axis by movement step


### moveRV
```
void moveRV();
```
#### Description
Move camera in its coordinate system on v axis by negative movement step


### update
```
void update();
```

#### Description
Caller method for updating the camera event system
