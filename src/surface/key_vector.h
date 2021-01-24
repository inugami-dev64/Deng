#ifndef KEY_VECTOR_H
#define KEY_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

// method declarations
void deng_InitKeyData(deng_SurfaceWindow *p_window);

void deng_RegisterKeyEvent(deng_SurfaceWindow *p_window, deng_Key *p_key, deng_MouseButton *p_btn, int key_type, int array_type);
void deng_ClearKeyEvent(deng_SurfaceWindow *p_window, size_t index, int key_type, int array_type);
size_t deng_FindKeyIndex(deng_SurfaceWindow *p_window, deng_Key key, deng_MouseButton, int key_type, int array_type);
void deng_CleanKeyEvents(deng_SurfaceWindow *p_window, int key_type, int array_type);

int deng_IsKeyActive(deng_SurfaceWindow *p_window, deng_Key key);
int deng_IsMouseBtnActive(deng_SurfaceWindow *p_window, deng_MouseButton btn);
int deng_IsKeyReleased(deng_SurfaceWindow *p_window, deng_Key key);
int deng_IsMouseBtnReleased(deng_SurfaceWindow *p_window, deng_MouseButton btn);

void deng_FreeKeyData(deng_SurfaceWindow *p_window);

#ifdef __cplusplus
}
#endif

#endif