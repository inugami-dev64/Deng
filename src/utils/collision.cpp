#define __COLLISION_CPP
#include <utils/collision.h>

namespace dengUtils {
    /*********************************************/
    /******* 2D COLLISION DETECTION METHODS ******/
    /*********************************************/

    /*
     * Find point collision with asset
     */
    deng_bool_t Collision2D::findPtCollision (
        dengMath::vec2<deng_vec_t> point,
        deng_Asset *p_asset,
        dengMath::vec2<size_t> vert_bounds
    ) {
        size_t i, j;
        deng_bool_t is_colliding = false;
        dengMath::vec2<deng_vec_t> *vc;
        dengMath::vec2<deng_vec_t> *vn;

        // Check every triangle for potential collision
        switch (p_asset->asset_mode)
        {
        case DENG_ASSET_MODE_2D_UNMAPPED:
            for(i = vert_bounds.first; i < vert_bounds.second; i++) {
                j = i + 1;
                if(j == vert_bounds.second) 
                    j = vert_bounds.first;
                
                vc = (dengMath::vec2<deng_vec_t>*) &p_asset->vertices.p_unmapped_vert_data_2d[i].vert_data;
                vn = (dengMath::vec2<deng_vec_t>*) &p_asset->vertices.p_unmapped_vert_data_2d[j].vert_data;

                if
                (
                    ((vc->second >= point.second && vn->second < point.second) ||
                    (vc->second < point.second && vn->second >= point.second)) &&
                    (point.first < (vn->first - vc->first) * (point.second - vc->second) / (vn->second - vc->second) + vc->first)
                ) is_colliding = !is_colliding;
            }
            break;

        case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
            for(i = vert_bounds.first; i < vert_bounds.second; i++) {
                j = i + 1;
                if(j == vert_bounds.second) 
                    j = vert_bounds.first;

                vc = (dengMath::vec2<deng_vec_t>*) &p_asset->vertices.p_tex_mapped_vert_data_2d[i].vert_data;
                vn = (dengMath::vec2<deng_vec_t>*) &p_asset->vertices.p_tex_mapped_vert_data_2d[j].vert_data;

                if
                (
                    ((vc->second >= point.second && vn->second < point.second) ||
                    (vc->second < point.second && vn->second >= point.second)) &&
                    (point.first < (vn->first - vc->first) * (point.second - vc->second) / (vn->second - vc->second) + vc->first)
                ) is_colliding = !is_colliding;

            }
            break;

        default:
            break;
        }

        return is_colliding;
    }


    /*
     * Find point collision with specified vertices
     */
    deng_bool_t Collision2D::findPtCollision (
        dengMath::vec2<deng_vec_t> point,
        deng_ObjVertData2D *verts,
        deng_ui32_t vert_c
    ) {
        size_t i, j;
        deng_bool_t is_colliding = false;
        dengMath::vec2<deng_vec_t> *vn, *vc;

        for(i = 0; i < vert_c; i++) {
            j = i + 1;
            if(j >= vert_c)
                j = 0;

            vc = (dengMath::vec2<deng_vec_t>*) &verts[i];
            vn = (dengMath::vec2<deng_vec_t>*) &verts[j];

            if
            (
                ((vc->second >= point.second && vn->second < point.second) ||
                (vc->second < point.second && vn->second >= point.second)) &&
                (point.first < (vn->first - vc->first) * (point.second - vc->second) / (vn->second - vc->second) + vc->first)
            ) is_colliding = !is_colliding;
        }

        return is_colliding;
    }
}
