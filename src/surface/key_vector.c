#include "deng_key_definitions.h"
#include "deng_surface_core.h"

void deng_InitKeyData(deng_SurfaceWindow *p_window) {
    p_window->active_keys.key_count = 0;
    p_window->active_keys.p_keys = (deng_Key*) malloc (
        sizeof(deng_Key)
    );
    
    p_window->active_keys.btn_count = 0;
    p_window->active_keys.p_btn = (deng_MouseButton*) malloc (
        sizeof(deng_MouseButton)
    );

    p_window->released_keys.key_count = 0;
    p_window->released_keys.p_keys = (deng_Key*) malloc (
        sizeof(deng_Key)
    );

    p_window->released_keys.btn_count = 0;
    p_window->released_keys.p_btn = (deng_MouseButton*) malloc (
        sizeof(deng_MouseButton)
    );
}

void deng_RegisterKeyEvent (
    deng_SurfaceWindow *p_window, 
    deng_Key *p_key, 
    deng_MouseButton *p_btn, 
    int key_type, 
    int array_type
) {
    switch (key_type)
    {
    case MOUSE_BUTTON:
        if(array_type & ACTIVE_KEYS) {
            p_window->active_keys.btn_count++;
            p_window->active_keys.p_btn = realloc (
                p_window->active_keys.p_btn, 
                p_window->active_keys.btn_count * sizeof(deng_MouseButton)
            );
        
            p_window->active_keys.p_btn[p_window->active_keys.btn_count - 1] = *p_btn;
        }
        
        if(array_type & RELEASE_KEYS) {
            p_window->released_keys.btn_count++;
            p_window->released_keys.p_btn = realloc (
                p_window->released_keys.p_btn, 
                p_window->released_keys.btn_count * sizeof(deng_MouseButton)
            );

            p_window->released_keys.p_btn[p_window->released_keys.btn_count - 1] = *p_btn;
        }

        break;

    case KB_KEY:
        if(array_type & ACTIVE_KEYS) {
            p_window->active_keys.key_count++;
            p_window->active_keys.p_keys = realloc (
                p_window->active_keys.p_keys, 
                p_window->active_keys.key_count * sizeof(deng_Key)
            );

            p_window->active_keys.p_keys[p_window->active_keys.key_count - 1] = *p_key;
        }

        if(array_type & RELEASE_KEYS) {
            p_window->released_keys.key_count++;
            p_window->released_keys.p_keys = realloc (
                p_window->released_keys.p_keys, 
                p_window->released_keys.key_count * sizeof(deng_Key)
            );

            p_window->released_keys.p_keys[p_window->released_keys.key_count - 1] = *p_key;
        }

        break;
    
    default:
        break;
    }
    
}

size_t deng_FindKeyIndex (
    deng_SurfaceWindow *p_window, 
    deng_Key key, 
    deng_MouseButton btn, 
    int key_type, 
    int array_type
) {
    size_t index;

    switch (key_type)
    {
    case MOUSE_BUTTON:
        if(array_type & ACTIVE_KEYS) {
            for(index = 0; index < p_window->active_keys.btn_count; index++)
                if(p_window->active_keys.p_btn[index] == btn) return index;

            index = p_window->active_keys.btn_count;
            return index;
        }

        else if(array_type & RELEASE_KEYS) {
            for(index = 0; index < p_window->released_keys.btn_count; index++)
                if(p_window->released_keys.p_btn[index] == btn) return index;

            index = p_window->released_keys.btn_count;
            return index;
        }
        break;
    
    case KB_KEY: {
        if(array_type & ACTIVE_KEYS) {
            for(index = 0; index < p_window->active_keys.key_count; index++) 
                if(p_window->active_keys.p_keys[index] == key) return index;

            index = p_window->active_keys.key_count;
            return index;
        }

        else if(array_type & RELEASE_KEYS) {
            for(index = 0; index < p_window->released_keys.key_count; index++) 
                if(p_window->released_keys.p_keys[index] == key) return index;

            index = p_window->released_keys.key_count;
            return index;
        }
        break;
    }

    default:
        perror("Arguments to function call deng_FindKeyIndex are invalid!");
        return (size_t) -1;
    }

    return (size_t) -1;
}

void deng_ClearKeyEvent ( 
    deng_SurfaceWindow *p_window, 
    size_t index, 
    int key_type, 
    int array_type
) {
    int l_index, r_index;

    switch (key_type)
    {
    case MOUSE_BUTTON: {
        if(array_type & ACTIVE_KEYS) {
            // it's fine i'm using gcc :)))
            deng_MouseButton tmp[p_window->active_keys.btn_count--];

            for(l_index = 0, r_index = 0; l_index < p_window->active_keys.btn_count; l_index++, r_index++) {
                if(r_index != index) tmp[l_index] = p_window->active_keys.p_btn[r_index];
                else {
                    r_index++;
                    tmp[l_index] = p_window->active_keys.p_btn[r_index];
                }
            }   

            free(p_window->active_keys.p_btn);
            p_window->active_keys.p_btn = (deng_MouseButton*) malloc (
                p_window->active_keys.btn_count * 
                sizeof(deng_MouseButton)
            );

            for(l_index = 0; l_index < p_window->active_keys.btn_count; l_index++)
                p_window->active_keys.p_btn[l_index] = tmp[l_index];
        }

        if(array_type & RELEASE_KEYS) {
            // it's fine i'm using gcc :)))
            deng_MouseButton tmp[p_window->released_keys.btn_count--];

            for(l_index = 0, r_index = 0; l_index < p_window->released_keys.btn_count; l_index++, r_index++) {
                if(r_index != index) tmp[l_index] = p_window->released_keys.p_btn[r_index];
                else {
                    r_index++;
                    tmp[l_index] = p_window->released_keys.p_btn[r_index];
                }
            }   

            free(p_window->released_keys.p_btn);
            p_window->released_keys.p_btn = (deng_MouseButton*) malloc (
                p_window->released_keys.btn_count * 
                sizeof(deng_MouseButton)
            );

            for(l_index = 0; l_index < p_window->released_keys.btn_count; l_index++)
                p_window->released_keys.p_btn[l_index] = tmp[l_index];
        }
        break;
    }

    case KB_KEY: {
        if(array_type & ACTIVE_KEYS) {
            // it's fine i'm using gcc :)))
            deng_Key tmp[p_window->active_keys.key_count--];

            for(l_index = 0, r_index = 0; l_index < p_window->active_keys.key_count; l_index++, r_index++) {
                if(r_index != index) tmp[l_index] = p_window->active_keys.p_keys[r_index];
                else { 
                    r_index++;
                    tmp[l_index] = p_window->active_keys.p_keys[r_index];
                }
            }

            free(p_window->active_keys.p_keys);
            p_window->active_keys.p_keys = (deng_Key*) malloc (
                p_window->active_keys.key_count * 
                sizeof(deng_Key)
            );

            for(l_index = 0; l_index < p_window->active_keys.key_count; l_index++)
                p_window->active_keys.p_keys[l_index] = tmp[l_index];
        }

        if(array_type & RELEASE_KEYS) {
            // it's fine i'm using gcc :)))
            deng_Key tmp[p_window->released_keys.key_count--];

            for(l_index = 0, r_index = 0; l_index < p_window->released_keys.key_count; l_index++, r_index++) {
                if(r_index != index) 
                    tmp[l_index] = p_window->released_keys.p_keys[r_index];
                else { 
                    r_index++;
                    tmp[l_index] = p_window->released_keys.p_keys[r_index];
                }
            }

            free(p_window->released_keys.p_keys);
            p_window->released_keys.p_keys = (deng_Key*) malloc (
                p_window->released_keys.key_count * 
                sizeof(deng_Key)
            );

            for(l_index = 0; l_index < p_window->released_keys.key_count; l_index++)
                p_window->released_keys.p_keys[l_index] = tmp[l_index];
        }
        break;
    }
    
    default:
        break;
    }
}

void deng_CleanKeyEvents (
    deng_SurfaceWindow *p_window, 
    int key_type, 
    int array_type
) {
    switch (key_type)
    {
    case MOUSE_BUTTON:
        if(array_type & ACTIVE_KEYS) {
            free(p_window->active_keys.p_btn);
            p_window->active_keys.p_btn = (deng_MouseButton*) malloc (
                sizeof(deng_MouseButton)
            );
            p_window->active_keys.btn_count = 0;
        }

        if(array_type & RELEASE_KEYS) {
            free(p_window->released_keys.p_btn);
            p_window->released_keys.p_btn = (deng_MouseButton*) malloc (
                sizeof(deng_MouseButton)
            );
            p_window->released_keys.btn_count = 0;
        }
        break;
    
    case KB_KEY:
        if(array_type & ACTIVE_KEYS) {
            free(p_window->active_keys.p_keys);
            p_window->active_keys.p_keys = (deng_Key*) malloc (
                sizeof(deng_Key)
            );
            p_window->active_keys.key_count = 0;
        }   

        if(array_type & RELEASE_KEYS) {
            free(p_window->released_keys.p_keys);
            p_window->released_keys.p_keys = (deng_Key*) malloc (
                sizeof(deng_Key)
            );
            p_window->released_keys.key_count = 0;
        }
        break;
    default:
        break;
    }
}

int deng_IsKeyActive (
    deng_SurfaceWindow *p_window, 
    deng_Key key
) {
    size_t index;
    for(index = 0; index < p_window->active_keys.key_count; index++) 
        if(key == p_window->active_keys.p_keys[index]) return true;

    return false;
}

int deng_IsMouseBtnActive (
    deng_SurfaceWindow *p_window, 
    deng_MouseButton btn
) {
    size_t index;
    for(index = 0; index < p_window->active_keys.btn_count; index++)
        if(btn == p_window->active_keys.p_btn[index]) return true;
    
    return false;
}

int deng_IsKeyReleased (
    deng_SurfaceWindow *p_window, 
    deng_Key key
) {
    size_t index; 
    for(index = 0; index < p_window->released_keys.key_count; index++) 
        if(key == p_window->released_keys.p_keys[index]) return true;
    
    return false;
}

int deng_IsMouseBtnReleased (
    deng_SurfaceWindow *p_window, 
    deng_MouseButton key
) {
    size_t index; 
    for(index = 0; index < p_window->released_keys.btn_count; index++) 
        if(key == p_window->released_keys.p_btn[index]) return true;
    
    return false;
}

void deng_FreeKeyData(deng_SurfaceWindow *p_window) {
    if(p_window->active_keys.btn_count != 0)
        free(p_window->active_keys.p_btn);
    
    if(p_window->active_keys.key_count != 0)
        free(p_window->active_keys.p_keys);

    if(p_window->released_keys.key_count != 0)
        free(p_window->released_keys.p_keys);

    if(p_window->released_keys.btn_count != 0)
        free(p_window->released_keys.p_btn);
}
