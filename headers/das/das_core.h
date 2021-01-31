#ifndef ASSET_CREATOR_CORE_H
#define ASSET_CREATOR_CORE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <vulkan/vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif

#define true 1
#define false 0

#include "../common/common.h"
#include "../common/hashmap.h"
#include "assets.h"
#include "data_loader.h"
#include "hdr_data.h"
#include "das_handler.h"

#ifdef __cplusplus
}
#endif

#endif
