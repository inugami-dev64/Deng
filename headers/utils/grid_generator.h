#ifndef __GRID_GENERATOR_H
#define __GRID_GENERATOR_H

#ifdef __GRID_GENERATOR_CPP
    #include <stdlib.h>
    #include <vector>

    #include <common/base_types.h>
    #include <das/assets.h>
    #include <deng/deng_math.h>
#endif

#define DENG_DEFAULT_GRID_COLOR {0.8f, 0.8f, 0.8f, 1.0f}
#define DENG_GRID_THICKNESS 0.005f

namespace dengUtils {
    
    class GridGenerator {
    private:
        dengMath::vec4<deng_vec_t> m_color;
        deng_vec_t m_grid_len;
        deng_ui32_t m_row_c;
        deng_Asset m_asset;

    public:
        GridGenerator (
            dengMath::vec4<deng_vec_t> color, 
            deng_vec_t len,
            deng_ui32_t row_c
        );
        deng_Asset getGrid();
    };
}

#endif
