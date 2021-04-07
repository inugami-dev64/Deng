#include <vector>
#include <thread>
#include <mutex>
#include <vulkan/vulkan.h>

#include <common/base_types.h>
#include <common/err_def.h>
#include <common/hashmap.h>
#include <common/common.h>
#include <common/uuid.h>
#include <das/assets.h>

#include <math/deng_math.h>
#include <deng/surface_window.h>
#include <deng/window.h>
#include <utils/shape_gen.h>
#include <utils/timer.h>
#include <utils/font.h>
#include <deng/camera.h>
#include <deng/vulkan/vulkan_sd.h>
#include <deng/vulkan/vulkan_qm.h>
#include <deng/vulkan/vulkan_resources.h>
#include <deng/vulkan/vulkan_rend_infos.h>
#include <deng/vulkan/vulkan_ic.h>
#include <deng/vulkan/vulkan_scc.h>
#include <deng/vulkan/vulkan_desc_c.h>
#include <deng/vulkan/vulkan_dc.h>
#include <deng/vulkan/vulkan_ra.h>
#include <deng/vulkan/vulkan_renderer.h>
#include <das/das_loader.h>
#include <das/tex_loader.h>
#include <deng/window.h>

/*
 * TODO: Load 4 different types (unmapped, unmapped normalised, texture mapped, 
 * texture mapped normalised) of assets[0]s into vulkan and check if the renderer works 
 * correctly 
 *
 * Cube colors: red, green, blue, yellow
 */

deng::__SharedMouseData ext_md{};


// Logging function for assets 
void assetlog(const das_Asset &asset, const char *file_name) {
    char buf[128] = {0};
    cm_OpenLogger((char*) file_name);

    for(size_t i = 0; i < asset.vertices.n; i++) {
        memset(buf, 0, 128);
        switch(asset.asset_mode)
        {
        case DAS_ASSET_MODE_2D_UNMAPPED:
            sprintf (
                buf,
                "v %f %f",
                asset.vertices.vu2d[i].vert_data.vert_x,
                asset.vertices.vu2d[i].vert_data.vert_y
            );
            cm_LogWrite(buf);
            break;

        case DAS_ASSET_MODE_2D_TEXTURE_MAPPED:
            sprintf (
                buf,
                "v %f %f | %f %f",
                asset.vertices.vu2d[i].vert_data.vert_x,
                asset.vertices.vu2d[i].vert_data.vert_y,
                asset.vertices.vm2d[i].tex_data.tex_x,
                asset.vertices.vm2d[i].tex_data.tex_y
            );
            cm_LogWrite(buf);
            break;

        case DAS_ASSET_MODE_3D_UNMAPPED:
            sprintf (
                buf,
                "v %f %f %f",
                asset.vertices.vuu[i].vert_x,
                asset.vertices.vuu[i].vert_y,
                asset.vertices.vuu[i].vert_z
            );
            cm_LogWrite(buf);
            break;

        case DAS_ASSET_MODE_3D_UNMAPPED_NORMALISED:
            sprintf (
                buf,
                "v %f %f %f | %f %f %f",
                asset.vertices.vun[i].vert_data.vert_x,
                asset.vertices.vun[i].vert_data.vert_y,
                asset.vertices.vun[i].vert_data.vert_z,
                asset.vertices.vun[i].norm_data.nor_x,
                asset.vertices.vun[i].norm_data.nor_y,
                asset.vertices.vun[i].norm_data.nor_z
            );
            cm_LogWrite(buf);
            break;

        case DAS_ASSET_MODE_3D_TEXTURE_MAPPED:
            sprintf (
                buf,
                "v %f %f %f | %f %f",
                asset.vertices.vmu[i].vert_data.vert_x,
                asset.vertices.vmu[i].vert_data.vert_y,
                asset.vertices.vmu[i].vert_data.vert_z,
                asset.vertices.vmu[i].tex_data.tex_x,
                asset.vertices.vmu[i].tex_data.tex_y
            );
            cm_LogWrite(buf);
            break;

        case DAS_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
            sprintf (
                buf,
                "v %f %f %f | %f %f | %f %f %f",
                asset.vertices.vmn[i].vert_data.vert_x,
                asset.vertices.vmn[i].vert_data.vert_y,
                asset.vertices.vmn[i].vert_data.vert_z,
                asset.vertices.vmn[i].tex_data.tex_x,
                asset.vertices.vmn[i].tex_data.tex_y,
                asset.vertices.vmn[i].norm_data.nor_x,
                asset.vertices.vmn[i].norm_data.nor_y,
                asset.vertices.vmn[i].norm_data.nor_z
            );
            cm_LogWrite(buf);
            break;
        
        default:
            break;
        }
    }
    cm_CloseLogger();
}


// Main game loop function
void gameloop (
    deng::vulkan::__vk_Renderer &rend, 
    const deng::vulkan::__vk_ConfigVars &cfg,
    deng::Window &win
) {
    while(deng_IsRunning()) {
        ext_md.mut.lock();
        deng_UpdateWindow(win.getWindow());
        ext_md.mut.unlock();

        switch(cfg.cam_type)
        {
        case DENG_CAMERA_FPP:
            ((deng::FPPCamera*) cfg.p_camera)->update();
            break;
        
        case DENG_CAMERA_EDITOR:
            ((deng::EditorCamera*) cfg.p_camera)->update();
            break;

        default:
            break;
        }
        rend.makeFrame();
        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }

    rend.idle();
    deng_DestroyWindow(win.getWindow());
}


int main() {
    deng::Window win(1200, 1000, "TEST");

    deng::FPPCamera cam (
        {0.7f, 0.7f, 0.7f}, 
        {0.2f, 0.2f}, 
        dengMath::Conversion::degToRad(65.0f), 
        0.1f, 
        -25.0f, 
        NULL, 
        NULL, 
        &win
    );

    //deng::EditorCamera cam (
        //0.1f, 
        //{0.0f, 0.0f}, 
        //{1.0, 1.0}, 
        //dengMath::Conversion::degToRad(65.0f), 
        //25.0f, 
        //0.1f,
        //&win
    //);

    // Setting up configuration variables
    deng::vulkan::__vk_ConfigVars cfg;
    cfg.background = (dengMath::vec4<deng_vec_t>) {0.0f, 0.0f, 0.0f, 1.0f};
    cfg.cam_type = DENG_CAMERA_FPP;
    cfg.p_camera = &cam;
    cfg.enable_validation_layers = true;
    cfg.enable_vsync = false;
    cfg.far_plane = -25.0f;
    cfg.near_plane = 0.1f;
    cfg.msaa_sample_count = VK_SAMPLE_COUNT_4_BIT;

    std::vector<das_Asset> assets(1);
    das_Texture tex = {0};

    das_LoadTexture (
        &tex,
        "../textures/viking_room.tga"
    );
    tex.uuid = uuid_Generate();
    tex.name = (char*) "brick";

    deng::vulkan::__vk_Renderer rend(&win, cfg);
    dengUtils::RectangleGenerator rec_gen(win.getSize());

    assets[0].asset_mode = DAS_ASSET_MODE_3D_TEXTURE_MAPPED;
    das_ReadAsset (
        &assets[0],
        DAS_ASSET_MODE_3D_TEXTURE_MAPPED,
        (char*) "assets/viking.das"
    );


    assets[0].uuid = uuid_Generate();
    assets[0].color = {1.0f, 0.0f, 0.0f, 1.0f};
    assets[0].force_unmap = false;
    assets[0].tex_uuid = tex.uuid;
    assets[0].is_shown = true;

    dengMath::Transformer3D trf;
    trf.rotate((dengMath::vec3<deng_vec_t>) {PI / 2, -PI / 2, 0.0f}, assets[0]);

    rend.submitTextures(&tex, 1);
    rend.submitAssets(assets.data(), assets.size());
    rend.setup();

    gameloop (
        rend,
        cfg,
        win
    );
    
    return EXIT_SUCCESS;
}
