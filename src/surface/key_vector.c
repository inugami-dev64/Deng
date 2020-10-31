#include "deng_surface_core.h"

// Kb key methods
void add_key(DENGWindow *p_window, DENGKey *p_key, DENGMouseButton *p_btn, int key_type, int array_type) {
    switch (key_type)
    {
    case MOUSE_BUTTON:
        if(array_type & ACTIVE_KEYS) {
            // printf("%s\n", "Adding btn to active keys!");
            p_window->active_keys.btn_count++;
            p_window->active_keys.p_btn = realloc(p_window->active_keys.p_btn, p_window->active_keys.btn_count * sizeof(DENGMouseButton));
        
            p_window->active_keys.p_btn[p_window->active_keys.btn_count - 1] = *p_btn;
        }
        
        if(array_type & RELEASE_KEYS) {
            // printf("%s\n", "Adding btn to release keys!");
            p_window->released_keys.btn_count++;
            p_window->released_keys.p_btn = realloc(p_window->released_keys.p_btn, p_window->released_keys.btn_count * sizeof(DENGMouseButton));

            p_window->released_keys.p_btn[p_window->released_keys.btn_count - 1] = *p_btn;
        }

        break;

    case KB_KEY:
        if(array_type & ACTIVE_KEYS) {
            // printf("%s\n", "Adding key to active keys!");
            p_window->active_keys.key_count++;
            p_window->active_keys.p_keys = realloc(p_window->active_keys.p_keys, p_window->active_keys.key_count * sizeof(DENGKey));

            p_window->active_keys.p_keys[p_window->active_keys.key_count - 1] = *p_key;
        }

        if(array_type & RELEASE_KEYS) {
            // printf("%s\n", "Adding key to release keys!");
            p_window->released_keys.key_count++;
            p_window->released_keys.p_keys = realloc(p_window->released_keys.p_keys, p_window->released_keys.key_count * sizeof(DENGKey));

            p_window->released_keys.p_keys[p_window->released_keys.key_count - 1] = *p_key;
        }

        break;
    
    default:
        break;
    }
    
}

size_t get_key_index(DENGWindow *p_window, DENGKey key, DENGMouseButton btn, int key_type, int array_type) {
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
        break;
    }
}

void remove_key(DENGWindow *p_window, size_t index, int key_type, int array_type) {
    int l_index, r_index;

    switch (key_type)
    {
    case MOUSE_BUTTON: {
        if(array_type & ACTIVE_KEYS) {
            DENGMouseButton tmp[p_window->active_keys.btn_count--];

            for(l_index = 0, r_index = 0; l_index < p_window->active_keys.btn_count; l_index++, r_index++) {
                if(r_index != index) tmp[l_index] = p_window->active_keys.p_btn[r_index];
                else {
                    r_index++;
                    tmp[l_index] = p_window->active_keys.p_btn[r_index];
                }
            }   

            free(p_window->active_keys.p_btn);
            p_window->active_keys.p_btn = malloc(p_window->active_keys.btn_count * sizeof(DENGMouseButton));

            for(l_index = 0; l_index < p_window->active_keys.btn_count; l_index++)
                p_window->active_keys.p_btn[l_index] = tmp[l_index];
        }

        if(array_type & RELEASE_KEYS) {
            DENGMouseButton tmp[p_window->released_keys.btn_count--];

            for(l_index = 0, r_index = 0; l_index < p_window->released_keys.btn_count; l_index++, r_index++) {
                if(r_index != index) tmp[l_index] = p_window->released_keys.p_btn[r_index];
                else {
                    r_index++;
                    tmp[l_index] = p_window->released_keys.p_btn[r_index];
                }
            }   

            free(p_window->released_keys.p_btn);
            p_window->released_keys.p_btn = malloc(p_window->released_keys.btn_count * sizeof(DENGMouseButton));

            for(l_index = 0; l_index < p_window->released_keys.btn_count; l_index++)
                p_window->released_keys.p_btn[l_index] = tmp[l_index];
        }
        break;
    }

    case KB_KEY: {
        if(array_type & ACTIVE_KEYS) {
            DENGKey tmp[p_window->active_keys.key_count--];

            for(l_index = 0, r_index = 0; l_index < p_window->active_keys.key_count; l_index++, r_index++) {
                if(r_index != index) tmp[l_index] = p_window->active_keys.p_keys[r_index];
                else { 
                    r_index++;
                    tmp[l_index] = p_window->active_keys.p_keys[r_index];
                }
            }

            free(p_window->active_keys.p_keys);
            p_window->active_keys.p_keys = malloc(p_window->active_keys.key_count * sizeof(DENGKey));

            for(l_index = 0; l_index < p_window->active_keys.key_count; l_index++)
                p_window->active_keys.p_keys[l_index] = tmp[l_index];
        }

        if(array_type & RELEASE_KEYS) {
            DENGKey tmp[p_window->released_keys.key_count--];

            for(l_index = 0, r_index = 0; l_index < p_window->released_keys.key_count; l_index++, r_index++) {
                if(r_index != index) tmp[l_index] = p_window->released_keys.p_keys[r_index];
                else { 
                    r_index++;
                    tmp[l_index] = p_window->released_keys.p_keys[r_index];
                }
            }

            free(p_window->released_keys.p_keys);
            p_window->released_keys.p_keys = malloc(p_window->released_keys.key_count * sizeof(DENGKey));

            for(l_index = 0; l_index < p_window->released_keys.key_count; l_index++)
                p_window->released_keys.p_keys[l_index] = tmp[l_index];
        }
        break;
    }
    
    default:
        break;
    }
}

void clean_keys(DENGWindow *p_window, int key_type, int array_type) {
    switch (key_type)
    {
    case MOUSE_BUTTON:
        if(array_type & ACTIVE_KEYS) {
            free(p_window->active_keys.p_btn);
            p_window->active_keys.p_btn = (DENGMouseButton*) malloc(sizeof(DENGMouseButton));
            p_window->active_keys.btn_count = 0;
        }

        if(array_type & RELEASE_KEYS) {
            free(p_window->released_keys.p_btn);
            p_window->released_keys.p_btn = (DENGMouseButton*) malloc(sizeof(DENGMouseButton));
            p_window->released_keys.btn_count = 0;
        }
        break;
    
    case KB_KEY:
        if(array_type & ACTIVE_KEYS) {
            free(p_window->active_keys.p_keys);
            p_window->active_keys.p_keys = (DENGKey*) malloc(sizeof(DENGKey));
            p_window->active_keys.key_count = 0;
        }   

        if(array_type & RELEASE_KEYS) {
            free(p_window->released_keys.p_keys);
            p_window->released_keys.p_keys = (DENGKey*) malloc(sizeof(DENGKey));
            p_window->released_keys.key_count = 0;
        }
        break;
    default:
        break;
    }
}