/*    ____         ________     __      _        ______  
 *   |  _ \_      |  ______|   |  \    | |     _/  ____\
 *   | | \_ \_    |  |         |   \   | |    /  _/   
 *   | |   \  |   |  |_____    | |\ \  | |   |  |   _____ 
 *   | |    | |   |  ______|   | | \ \ | |   |  |  |___  | 
 *   | |  _/ _|   |  |         | |  \ \| |    \  \    / /
 *   | |_/ _/     |  |_____    | |   \   |     \_ \__/ /
 *   |____/       |________|   |_|    \__|       \____/
 *               
 * __________________________________________________________
 * __________________________________________________________
 *                       Project DENG 
 *
 *
 * Copyright (C) 2020 - 2021
 * This Software is licensed under Apache License as described 
 * in the LICENSE.md file, which you should have recieved with 
 * this distribution.
 * 
 * You may reproduce and distribute copies of the
 * Work or Derivative Works thereof in any medium, with or without
 * modifications, and in Source or Object form, provided that You
 * meet the following conditions:
 *
 * (a) You must give any other recipients of the Work or
 *     Derivative Works a copy of this License; and
 *
 * (b) You must cause any modified files to carry prominent notices
 *     stating that You changed the files; and
 *
 * (c) You must retain, in the Source form of any Derivative Works
 *     that You distribute, all copyright, patent, trademark, and
 *     attribution notices from the Source form of the Work,
 *     excluding those notices that do not pertain to any part of
 *     the Derivative Works; and
 *
 * (d) If the Work includes a "NOTICE" text file as part of its
 *     distribution, then any Derivative Works that You distribute must
 *     include a readable copy of the attribution notices contained
 *     within such NOTICE file, excluding those notices that do not
 *     pertain to any part of the Derivative Works, in at least one
 *     of the following places: within a NOTICE text file distributed
 *     as part of the Derivative Works; within the Source form or
 *     documentation, if provided along with the Derivative Works; or,
 *     within a display generated by the Derivative Works, if and
 *     wherever such third-party notices normally appear. The contents
 *     of the NOTICE file are for informational purposes only and
 *     do not modify the License. You may add Your own attribution
 *     notices within Derivative Works that You distribute, alongside
 *     or as an addendum to the NOTICE text from the Work, provided
 *     that such additional attribution notices cannot be construed
 *     as modifying the License.
 *
 * You may add Your own copyright statement to Your modifications and
 * may provide additional or different license terms and conditions
 * for use, reproduction, or distribution of Your modifications, or
 * for any such Derivative Works as a whole, provided Your use,
 * reproduction, and distribution of the Work otherwise complies with
 * the conditions stated in this License.
 */ 



#define __SC_ED_CPP
#include <imgui-layer/scene_editor/sc_ed.h>


namespace dengEditor {

    SceneEditor3D::SceneEditor3D(deng_RendererHintBits hints) : 
        m_win(WIDTH, HEIGHT, "DENG - Scene editor"),
        m_cam(DENG_CAMERA_TYPE_EDITOR, static_cast<deng_vec_t>(dengMath::Conversion::degToRad(65.0)),
            {-0.1f, -25.0f}, {0.7f, 0.7f, 0.7f}, {0.3f, 0.3f}, false, &m_win),
        m_rend(hints, dengMath::vec4<deng_vec_t>{0.0f, 0.0f, 0.0f, 1.0f})
    {
        deng::Camera3DBindings bindings = {};
        bindings.mov_w = deng_CreateInputMask(1, DENG_MOUSE_SCROLL_UP);
        bindings.mov_nw = deng_CreateInputMask(1, DENG_MOUSE_SCROLL_UP);
        bindings.ch_vcp = deng_CreateInputMask(1, DENG_MOUSE_BTN_2);
        m_cam.setBindings(bindings);

        // Setup the renderer
        m_rend.setup(m_cam, m_win);
    }


    void SceneEditor3D::run() {
        m_ui_man = std::make_unique<deng::UIManager>(m_rend);

        // Main loop
        while(deng_IsRunning()) {
            // Start measuring time for FPS counter 
            auto t1 = std::chrono::high_resolution_clock::now();

            // Update the UI manager io data
            m_ui_man->updateIO(m_win);

            ImGui::NewFrame();
                SceneEditor3DEntityPanel::spawnEntityPanel(m_rend);
            ImGui::EndFrame();
            ImGui::Render();

            ImDrawData *p_draw_data = ImGui::GetDrawData();
            m_ui_man->render(p_draw_data, m_win);
            m_rend.update();

            auto t2 = std::chrono::high_resolution_clock::now();
            m_ui_man->setTime(t1, t2);
        }
    }
}


int main() {
    dengEditor::SceneEditor3D sc(DENG_RENDERER_HINT_MSAA_2 | DENG_RENDERER_HINT_API_VULKAN);
    sc.run();
    return 0;
}
