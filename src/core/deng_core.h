#ifndef DENG_CORE_H
#define DENG_CORE_H

#define DENG_API_INSTANCE 1
#include "deng_core.h"

/* Pseudo code about DENG bindings */
enum CallbackResult {
    DENG_CALLBACK_SUCCESS = 0,
    DENG_CALLBACK_GENERIC_FAILIURE = 1,
    DENG_CALLBACK_CORRUPTED_INPUT_FILE = 2,
    DENG_CALLBACK_CORRUPTED_TEXTURE = 3,
    DENG_CALLBACK_CORRUPTED_ASSET = 4
};

// Asset type declaration
struct DENGAsset;
struct DENGTexture;

CallbackResult deng_LoadAsset(const char *file_name);
CallbackResult deng_AssetBindToTexture(DENGAsset *p_asset, DENGTexture p_texture);



/* Instance initialisations */

#define DENG_REND_INIT(window, camera) 
void initAsset();

#endif