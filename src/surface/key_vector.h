#ifndef KEY_VECTOR_H
#define KEY_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

void add_key(DENGWindow *p_window, DENGKey *p_key, DENGMouseButton *p_btn, int key_type, int array_type);
void remove_key(DENGWindow *p_window, size_t index, int key_type, int array_type);
size_t get_key_index(DENGWindow *p_window, DENGKey key, DENGMouseButton, int key_type, int array_type);
void clean_keys(DENGWindow *p_window, int key_type, int array_type);

#ifdef __cplusplus
}
#endif

#endif