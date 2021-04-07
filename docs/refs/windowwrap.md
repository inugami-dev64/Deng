# deng::WindowWrap

C++ specification
File: window.h
## Description
Wrap up all C window creation methods in one C++ class

## Constructor
Constructor for class `deng::WindowWrap` is following:  
```
deng::WindowWrap::WindowWrap (
    deng_i32_t width, 
    deng_i32_t height, 
    const char *title
);
```

* `width` specifies the window width  
* `height` specifies the window height  
* `title` specifies the window title  
 

## List of public members
`deng::WindowWrap` has following members:  

### Methods
* [getWindow](#getwindow)
* [getTitle](#gettitle)
* [getSize](#getsize)
* [getPixelSize](#getpixelsize)
* [setVCMode](#setvcmode)
* [isVCP](#isvcp)


## Method specifications

### getWindow
```
deng_SurfaceWindow *getWindow();
```
#### Description
Returns pointer to `deng_SurfaceWindow` instance used in window creation  


### getTitle
```
const char *getTitle();
``` 

#### Description
Returns the title of the window


### getSize
```
dengMath::vec2<deng_ui32_t> getSize();
```

#### Description
Returns the size of the window


### setVCMode
```
void setVCMode (
    deng_bool_t is_vcp,
    deng_bool_t change_cursor
);
```

#### Description
Toggle virtual cursor mode in deng instance.  
Read more about virtual cursors [here](../virtual-cursors.md)

#### Parameters
* `is_vcp` specifies if virtual cursor mode will be enabled or not
* `change_cursor` specifies if cursor should be changed or not


### isVCP
```
deng_bool_t isVCP();
```

#### Description
Returns true if virtual cursor mode is enabled. Otherwise returns false.  
