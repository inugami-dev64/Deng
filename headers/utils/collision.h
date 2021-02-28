#ifndef COLLISION_H
#define COLLISION_H

namespace dengUtils {
    /*
     * Collision2D
     * This struct stores methods for 2D collision detection 
     */
    struct Collision2D {
        deng_bool_t findPtCollision (
            dengMath::vec2<deng_vec_t> point,
            deng_Asset *p_asset,
            dengMath::vec2<size_t> vert_bounds
        ); 

        deng_bool_t findPtCollision ( 
            dengMath::vec2<deng_vec_t> point,
            deng_ObjVertData2D *verts,
            deng_ui32_t vert_c
        );
    };
}

#endif
