#define __DENG_MATH_CPP
#include <deng/deng_math.h>

namespace dengMath {

    /* Generic conversion methods */
    deng_vec_t Conversion::degToRad(const deng_vec_t &deg) {
        return static_cast<deng_vec_t>(deg/180 * PI);
    }

    deng_vec_t Conversion::radToDeg(const deng_vec_t &rad) {
        return static_cast<deng_vec_t>(rad/(2 * PI) * 360);
    }

    deng_px_t Conversion::vector2DSizeToPixelSize (
        const deng_vec_t &vec_size, 
        const vec2<deng_ui32_t> &window_size, 
        const deng_CoordinateAxisType &axis_type
    ) {
        switch (axis_type)
        {
        case DENG_COORD_AXIS_X:
            return (
                static_cast<deng_px_t>(vec_size) * 
                static_cast<deng_px_t>(window_size.first) / 
                2.0
            );
            break;
        
        case DENG_COORD_AXIS_Y: 
            return (
                static_cast<double>(vec_size) * 
                static_cast<double>(window_size.second) / 
                2.0
            );
            break;

        default:
            break;
        }

        return 0.0;
    }

    deng_vec_t Conversion::pixelSizeToVector2DSize (
        const deng_px_t &pixel_size, 
        const vec2<deng_ui32_t> &window_size, 
        const deng_CoordinateAxisType &axis_type
    ) {
        switch (axis_type)
        {
        case DENG_COORD_AXIS_X:
            return (
                2.0f / 
                static_cast<deng_vec_t>(window_size.first) * 
                static_cast<deng_vec_t>(pixel_size) 
            );
            break;
        
        case DENG_COORD_AXIS_Y:
            return (
                2.0f / 
                static_cast<deng_vec_t>(window_size.second) *
                static_cast<deng_vec_t>(pixel_size)
            );
            break;

        case DENG_COORD_AXIS_UNDEFINED: {
            deng_ui32_t avg_size = (window_size.first + window_size.second) / 2;
            return (
                2.0f / 
                static_cast<deng_vec_t>(avg_size) * 
                static_cast<deng_vec_t>(pixel_size)
            );   
            break;
        }

        default:
            break;
        }

        return 0.0f;
    }

    
    /* Convert child relative coordinate to absolute coordinate */
    vec2<deng_vec_t> Conversion::findAbsPosition (
        VERT_UNMAPPED_2D *vert,
        vec2<deng_vec_t> child_pos
    ) {
        dengMath::vec2<deng_vec_t> size;
        size.first = vert[1].vert_data.vert_x - vert[0].vert_data.vert_x;
        size.second = vert[3].vert_data.vert_y - vert[0].vert_data.vert_y;
       
        return (vec2<deng_vec_t>) {
            vert[0].vert_data.vert_x + (child_pos.first + 1.0f) / 2 * size.first,
            vert[0].vert_data.vert_y + (child_pos.second + 1.0f) / 2 * size.second
        };
    }


    /* Convert child relative size to absolute size */
    deng_vec_t Conversion::findAbsSize (
        deng_vec_t parent_size,
        deng_vec_t child_size
    ) { return parent_size / 2 * child_size; }


    /* Convert absolute size to relative size */
    deng_vec_t Conversion::findRelSize (
        deng_vec_t parent_size, 
        deng_vec_t abs_size
    ) { return abs_size / parent_size * 2; }


    /* Convert absolute position to relative position */
    vec2<deng_vec_t> Conversion::findRelPosition (
        VERT_UNMAPPED_2D *vert, 
        vec2<deng_vec_t> abs_pos
    ) {
        dengMath::vec2<deng_vec_t> size;
        size.first = vert[1].vert_data.vert_x - vert[0].vert_data.vert_x;
        size.second = vert[3].vert_data.vert_y - vert[0].vert_data.vert_y;

        return (vec2<deng_vec_t>) {
            2 * (abs_pos.first + vert[0].vert_data.vert_x) / size.first - 1.0f,
            2 * (abs_pos.second + vert[0].vert_data.vert_y) / size.second - 1.0f
        };

    }


    ModelMatrix::ModelMatrix() {
        setTransformation(0.0f, 0.0f, 0.0f);
        setRotation(0.0f, 0.0f, 0.0f);
        setScale(1.0f, 1.0f, 1.0f);
    }


    /* Set model rotation in radians */
    void ModelMatrix::setRotation (
        deng_vec_t x_rot, 
        deng_vec_t y_rot, 
        deng_vec_t z_rot
    ) {
        m_rot_x_mat = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, (deng_vec_t) cos(x_rot), (deng_vec_t) -sin(x_rot), 0.0f},
            {0.0f, (deng_vec_t) sin(x_rot), (deng_vec_t) cos(x_rot), 0.0f}, 
            {0.0f, 0.0f, 0.0f, 1.0f}
        };

        m_rot_y_mat = {
            {(deng_vec_t) cos(y_rot), 0.0f, (deng_vec_t) sin(y_rot), 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {(deng_vec_t) -sin(y_rot), 0.0f, (deng_vec_t) cos(y_rot), 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };

        m_rot_z_mat = {
            {(deng_vec_t) cos(z_rot), (deng_vec_t) -sin(z_rot), 0.0f, 0.0f},
            {(deng_vec_t) sin(z_rot), (deng_vec_t) cos(z_rot), 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f}, 
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
    }

    void ModelMatrix::setTransformation (
        const deng_vec_t &transform_x, 
        const deng_vec_t &transform_y, 
        const deng_vec_t &transform_z
    ) {
        m_transformation_mat = {
            {1.0f, 0.0f, 0.0f, transform_x}, 
            {0.0f, 1.0f, 0.0f, transform_y},
            {0.0f, 0.0f, 1.0f, transform_z},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
    }

    void ModelMatrix::setScale (
        const deng_vec_t &scale_x, 
        const deng_vec_t &scale_y, 
        const deng_vec_t &scale_z
    ) {
        m_scale_mat = {
            {scale_x, 0, 0, 0},
            {0, scale_y, 0, 0},
            {0, 0, scale_z, 0},
            {0, 0, 0, 1}
        };
    }


    ViewMatrix::ViewMatrix(deng_CameraType type) {
        m_x_rot = 0.0f;
        m_y_rot = 0.0f;

        m_right_side = {1.0f, 0.0f, 0.0f, 0.0f};
        m_up_side = {0.0f, -1.0f, 0.0f, 0.0f};
        m_forward_side = {0.0f, 0.0f, -1.0f, 0.0f};
        
        switch(type) {
        case DENG_CAMERA_FPP:
            m_camera_position = {
                DENG_FPP_CAMERA_DEFAULT_POS_X, 
                DENG_FPP_CAMERA_DEFAULT_POS_Y, 
                DENG_FPP_CAMERA_DEFAULT_POS_Z, 
                1.0f
            };
            break;

        case DENG_CAMERA_EDITOR:
            m_camera_position = {
                DENG_EDITOR_CAMERA_DEFAULT_POS_X,
                DENG_EDITOR_CAMERA_DEFAULT_POS_Y,
                DENG_EDITOR_CAMERA_DEFAULT_POS_Z,
                1.0f
            };
            break;

        default:
            break;
        }
    }


    void ViewMatrix::setCameraPosition(const vec4<deng_vec_t> &new_position) {
        m_camera_position = new_position;
    }


    void ViewMatrix::addToPosition (
        const vec4<deng_vec_t> &movement_speed, 
        const deng_CoordinateAxisType &movement_type
    ) {
        switch(movement_type)
        {
        case DENG_COORD_AXIS_X:
            m_camera_position.first += movement_speed.first; 
            break;

        case DENG_COORD_AXIS_Y:
            m_camera_position.second += movement_speed.second;
            break;

        case DENG_COORD_AXIS_Z:
            m_camera_position.third += movement_speed.third;
            break;

        case DENG_COORD_AXIS_UNDEFINED:
            m_camera_position.first += movement_speed.first;
            m_camera_position.second += movement_speed.second;
            m_camera_position.third += movement_speed.third;
            break;
        
        default:
            break;
        }

        #if CAMERA_LOCATION_DEBUG
            LOG (
                "x: " + 
                std::to_string(m_camera_position.first) + 
                " y: " + 
                std::to_string(m_camera_position.second) + 
                " z: " + 
                std::to_string(m_camera_position.third)
            );
        #endif
    }

    void ViewMatrix::getViewMatrix(mat4<deng_vec_t> *p_view) {
        *p_view = m_transformation_mat;
    }

    void ViewMatrix::setTransformationMatrix(deng_bool_t is_abs_cam_sys) {
        if(!is_abs_cam_sys) {
            m_transformation_mat.row1 = m_rot_x_mat * m_right_side;
            m_transformation_mat.row3 = m_rot_x_mat * m_forward_side;
            m_transformation_mat.row4 = m_rot_x_mat * m_camera_position;

            m_transformation_mat.row1 = m_rot_x_mat * m_right_side;
            m_up_side = m_rot_x_mat * m_rot_y_mat * m_transformation_mat * DENG_CAMERA_UP_SIDE;
        }
        else {
            m_transformation_mat.row1 = m_rot_x_mat * m_rot_y_mat * m_right_side;
            m_transformation_mat.row2 = DENG_CAMERA_UP_SIDE;
            m_transformation_mat.row3 = m_rot_x_mat * m_rot_y_mat * m_forward_side;
            m_transformation_mat.row4 = m_rot_x_mat * m_rot_y_mat * m_camera_position;
        }
        
        m_transformation_mat.row2 = m_up_side;
    }

    vec4<deng_vec_t> ViewMatrix::getPosition() {
        return m_camera_position;
    }


    /* Set camera rotation in radians */
    void ViewMatrix::setCameraRotation (
        deng_vec_t x_rot, 
        deng_vec_t y_rot
    ) {
        deng_vec_t delta_y_rot = m_y_rot - y_rot; 
        m_x_rot = x_rot;
        m_y_rot = y_rot;

        m_rot_x_mat = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, (deng_vec_t) cos(m_x_rot), (deng_vec_t) -sin(m_x_rot), 0.0f},
            {0.0f, (deng_vec_t) sin(m_x_rot), (deng_vec_t) cos(m_x_rot), 0.0f}, 
            {0.0f, 0.0f, 0.0f, 1.0f}
        };

        m_rot_y_mat = {
            {(deng_vec_t) cos(m_y_rot), 0.0f, (deng_vec_t) sin(m_y_rot), 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {(deng_vec_t) -sin(m_y_rot), 0.0f, (deng_vec_t) cos(m_y_rot), 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };

        // Delta rotation matrices
        m_delta_rot_y = {
            {(deng_vec_t) cos(delta_y_rot), 0.0f, (deng_vec_t) sin(delta_y_rot), 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {(deng_vec_t) -sin(delta_y_rot), 0.0f, (deng_vec_t) cos(delta_y_rot), 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };

        // Update camera bounds
        m_camera_position = m_delta_rot_y * m_camera_position;
        m_forward_side = m_delta_rot_y * m_forward_side;
        m_right_side = m_delta_rot_y * m_right_side;
    }


    /* Rotate camera around certain point */
    void ViewMatrix::setPointRotation (
        dengMath::vec3<deng_vec_t> point,
        deng_vec_t x_rot,
        deng_vec_t y_rot
    ) {
        x_rot = -x_rot;
        m_x_rot = x_rot;
        m_y_rot = y_rot;

        mat4<deng_vec_t> transform_mat;
        transform_mat.row1 = (vec4<deng_vec_t>) {1.0f, 0.0f, 0.0f, point.first + m_camera_position.first};
        transform_mat.row2 = (vec4<deng_vec_t>) {0.0f, 1.0f, 0.0f, point.second + m_camera_position.second};
        transform_mat.row3 = (vec4<deng_vec_t>) {0.0f, 0.0f, 1.0f, point.third + m_camera_position.third};
        transform_mat.row4 = (vec4<deng_vec_t>) {0.0f, 0.0f, 0.0f, 1.0f};

        m_rot_x_mat = transform_mat * (mat4<deng_vec_t>) {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, (deng_vec_t) cos(m_x_rot), (deng_vec_t) -sin(m_x_rot), 0.0f},
            {0.0f, (deng_vec_t) sin(m_x_rot), (deng_vec_t) cos(m_x_rot), 0.0f}, 
            {0.0f, 0.0f, 0.0f, 1.0f}
        } * transform_mat.inv();

        m_rot_y_mat = transform_mat * (mat4<deng_vec_t>) {
            {(deng_vec_t) cos(m_y_rot), 0.0f, (deng_vec_t) sin(m_y_rot), 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {(deng_vec_t) -sin(m_y_rot), 0.0f, (deng_vec_t) cos(m_y_rot), 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        } * transform_mat.inv();
    }


    void ViewMatrix::getSidesInWorldCoords (
        vec4<deng_vec_t> *p_right_side,
        vec4<deng_vec_t> *p_up_side,
        vec4<deng_vec_t> *p_forward_side
    ) {
        if(p_right_side)
            *p_right_side = m_right_side;
        if(p_forward_side)
            *p_forward_side = m_forward_side;
        if(p_up_side)
            *p_up_side = m_up_side;
    }


    ProjectionMatrix::ProjectionMatrix (
        const deng_vec_t &FOV, 
        const deng_vec_t &near_plane, 
        const deng_vec_t &far_plane, 
        const deng_vec_t &aspect_ratio
    ) {
        m_FOV = FOV;
        m_near = near_plane;
        m_far = far_plane;
        m_aspect_ratio = aspect_ratio;
    }

    void ProjectionMatrix::getProjectionMatrix(mat4<deng_vec_t> *matrix) {
        matrix->row1 = {m_aspect_ratio * static_cast<deng_vec_t>(1/tan(m_FOV/2)), 0, 0, 0};
        matrix->row2 = {0, static_cast<deng_vec_t>(1/tan(m_FOV/2)), 0, 0};
        matrix->row3 = {0, 0, m_far / (m_far - m_near), 1};
        matrix->row4 = {0, 0, -(m_far * m_near) / (m_far - m_near), 0};
    }

    template<typename T>
    T getSmallestElement(std::vector<T> *p_elements_vector) {
        T smallest_element = (*p_elements_vector)[0];
        for(size_t i = 0; i < p_elements_vector->size(); i++)
            if(smallest_element > (*p_elements_vector)[i]) smallest_element = (*p_elements_vector)[i];
    
        return smallest_element;
    }

    template<typename T>
    T getLargestElement(std::vector<T> *p_elements_vector) {
        T largest_element = (*p_elements_vector)[0];
        for(size_t i = 0; i < p_elements_vector->size(); i++)
            if(largest_element < (*p_elements_vector)[i]) largest_element = (*p_elements_vector)[i];
    
        return largest_element;
    }

    template<typename T>
    void sortInGrowingOrder(std::vector<T> *p_elements_vector) {
        vec2<T> sorting_buffer;

        for(size_t i = 0; i < p_elements_vector->size() - 1; i++) {
            if((*p_elements_vector)[i] > (*p_elements_vector)[i + 1]) {
                sorting_buffer.second = (*p_elements_vector)[i];
                sorting_buffer.first = (*p_elements_vector)[i + 1];

                (*p_elements_vector)[i] = sorting_buffer.first;
                (*p_elements_vector)[i + 1] = sorting_buffer.second;
            }
        }
    }

    template<typename T>
    void sortInDecliningOrder(std::vector<T> *p_elements_vector) {
        vec2<T> sorting_buffer;

        for(size_t i = 0; i < p_elements_vector->size() - 1; i++) {
            if((*p_elements_vector)[i] < (*p_elements_vector)[i + 1]) {
                sorting_buffer.second = (*p_elements_vector)[i];
                sorting_buffer.first = (*p_elements_vector)[i + 1];

                (*p_elements_vector)[i] = sorting_buffer.first;
                (*p_elements_vector)[i + 1] = sorting_buffer.second;
            }
        }
    }

    template<typename T>
    void sortVectorInGrowingOrder(std::vector<T> *p_elements_vector, deng_CoordinateAxisType coord_axis_type) {
        vec2<T> sorting_buffer;

        for(size_t i = 0; i < p_elements_vector->size() - 1; i++) {
            switch (coord_axis_type)
            {
            case DENG_COORD_AXIS_X:
                if((*p_elements_vector)[i].position_vec.first > (*p_elements_vector)[i + 1].position_vec.first) {
                    sorting_buffer.second = (*p_elements_vector)[i];
                    sorting_buffer.first = (*p_elements_vector)[i + 1];

                    (*p_elements_vector)[i] = sorting_buffer.first;
                    (*p_elements_vector)[i + 1] = sorting_buffer.second;
                }
                break;

            case DENG_COORD_AXIS_Y:
                if((*p_elements_vector)[i].position_vec.second > (*p_elements_vector)[i + 1].position_vec.second) {
                    sorting_buffer.second = (*p_elements_vector)[i];
                    sorting_buffer.first = (*p_elements_vector)[i + 1];

                    (*p_elements_vector)[i] = sorting_buffer.first;
                    (*p_elements_vector)[i + 1] = sorting_buffer.second;
                }
                break;

            case DENG_COORD_AXIS_Z:
                if((*p_elements_vector)[i].position_vec.third > (*p_elements_vector)[i + 1].position_vec.third) {
                    sorting_buffer.second = (*p_elements_vector)[i];
                    sorting_buffer.first = (*p_elements_vector)[i + 1];

                    (*p_elements_vector)[i] = sorting_buffer.first;
                    (*p_elements_vector)[i + 1] = sorting_buffer.second;
                }
                break;
            
            default:
                break;
            }
        }
    }

    template<typename T>
    void sortVectorInDecliningOrder (
        std::vector<T> *p_elements_vector, 
        deng_CoordinateAxisType coord_axis_type
    ) {
        vec2<T> sorting_buffer;

        for(size_t i = 0; i < p_elements_vector->size() - 1; i++) {
            switch (coord_axis_type)
            {
            case DENG_COORD_AXIS_X:
                if((*p_elements_vector)[i].position_vec.first < (*p_elements_vector)[i + 1].position_vec.first) {
                    sorting_buffer.second = (*p_elements_vector)[i];
                    sorting_buffer.first = (*p_elements_vector)[i + 1];

                    (*p_elements_vector)[i] = sorting_buffer.first;
                    (*p_elements_vector)[i + 1] = sorting_buffer.second;
                }
                break;

            case DENG_COORD_AXIS_Y:
                if((*p_elements_vector)[i].position_vec.second < (*p_elements_vector)[i + 1].position_vec.second) {
                    sorting_buffer.second = (*p_elements_vector)[i];
                    sorting_buffer.first = (*p_elements_vector)[i + 1];

                    (*p_elements_vector)[i] = sorting_buffer.first;
                    (*p_elements_vector)[i + 1] = sorting_buffer.second;
                }
                break;

            case DENG_COORD_AXIS_Z:
                if((*p_elements_vector)[i].position_vec.third < (*p_elements_vector)[i + 1].position_vec.third) {
                    sorting_buffer.second = (*p_elements_vector)[i];
                    sorting_buffer.first = (*p_elements_vector)[i + 1];

                    (*p_elements_vector)[i] = sorting_buffer.first;
                    (*p_elements_vector)[i + 1] = sorting_buffer.second;
                }
                break;
            
            default:
                break;
            }
        }
    }

    template<typename T>
    T *findElementByDescription (
        std::vector<T> *p_elements, 
        const char *description
    ) {
        for(size_t i = 0; i < p_elements->size(); i++) {
            LOG("findElementByDescription description is: " + std::string((*p_elements)[i].description));
            if((*p_elements)[i].description == description) {
                return &p_elements->at(i);
            } 
        }
        return &p_elements->at(0);
    }
    

    // Apply model matrix for 3d asset
    void Transformer::apply3DModelMatrix (
        deng_Asset &asset, 
        mat4<deng_vec_t> &mat
    ) {
        size_t index;
        dengMath::vec3<deng_vec_t> *p_tmp_in;
        dengMath::vec4<deng_vec_t> tmp_out;

        for(index = 0; index < asset.vertices.size; index++) {
            switch (asset.asset_mode)
            {
            case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
                p_tmp_in = (dengMath::vec3<deng_vec_t>*) &asset.vertices.p_tex_mapped_unnormalized_vert[index].vert_data;
                tmp_out = mat * (*p_tmp_in);
                asset.vertices.p_tex_mapped_unnormalized_vert[index].vert_data.vert_x = tmp_out.first;
                asset.vertices.p_tex_mapped_unnormalized_vert[index].vert_data.vert_y = tmp_out.second;
                asset.vertices.p_tex_mapped_unnormalized_vert[index].vert_data.vert_z = tmp_out.third;
                break;

            case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
                p_tmp_in = (dengMath::vec3<deng_vec_t>*) &asset.vertices.p_tex_mapped_normalized_vert[index].vert_data;
                tmp_out = mat * (*p_tmp_in);
                asset.vertices.p_tex_mapped_normalized_vert[index].vert_data.vert_x = tmp_out.first;
                asset.vertices.p_tex_mapped_normalized_vert[index].vert_data.vert_y = tmp_out.second;
                asset.vertices.p_tex_mapped_normalized_vert[index].vert_data.vert_z = tmp_out.third;
                break;

            case DENG_ASSET_MODE_3D_UNMAPPED:
                p_tmp_in = (dengMath::vec3<deng_vec_t>*) &asset.vertices.p_unmapped_unnormalized_vert[index]; 
                tmp_out = mat * (*p_tmp_in);
                asset.vertices.p_unmapped_unnormalized_vert[index].vert_data.vert_x = tmp_out.first;
                asset.vertices.p_unmapped_unnormalized_vert[index].vert_data.vert_y = tmp_out.second;
                asset.vertices.p_unmapped_unnormalized_vert[index].vert_data.vert_z = tmp_out.third;
                break;

            case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
                p_tmp_in = (dengMath::vec3<deng_vec_t>*) &asset.vertices.p_unmapped_normalized_vert[index]; 
                tmp_out = mat * (*p_tmp_in);
                asset.vertices.p_unmapped_normalized_vert[index].vert_data.vert_x = tmp_out.first;
                asset.vertices.p_unmapped_normalized_vert[index].vert_data.vert_y = tmp_out.second;
                asset.vertices.p_unmapped_normalized_vert[index].vert_data.vert_z = tmp_out.third;
                break;

            default:
                WARNME("Invalid asset vertices type in dengMath::Transformer::apply3DModelMatrix()!");
                return;
            }
        }
    }


    void Transformer::apply3DModelMatrix (
        vec2<deng_ui32_t> asset_bounds,
        std::vector<deng_Asset> *p_assets,
        mat4<deng_vec_t> &mat
    ) {
        if(asset_bounds.second > p_assets->size())
            return;
        for(size_t i = asset_bounds.first; i < asset_bounds.second; i++) {
            Transformer::apply3DModelMatrix (
                p_assets->at(i),
                mat
            );
        }
    }


    void Transformer::apply2DModelMatrix (
        deng_Asset &asset,
        mat3<deng_vec_t> &mat
    ) {
        size_t index;
        dengMath::vec2<deng_vec_t> tmp_in;
        dengMath::vec3<deng_vec_t> tmp_out;

        for(index = 0; index < asset.vertices.size; index++) {
            switch (asset.asset_mode)
            {
            case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
                tmp_in.first = asset.vertices.p_tex_mapped_vert_data_2d[index].vert_data.vert_x;
                tmp_in.second = asset.vertices.p_tex_mapped_vert_data_2d[index].vert_data.vert_y;
                tmp_out = mat * tmp_in;
                asset.vertices.p_tex_mapped_vert_data_2d[index].vert_data.vert_x = tmp_out.first;
                asset.vertices.p_tex_mapped_vert_data_2d[index].vert_data.vert_y = tmp_out.second;
                break;

            case DENG_ASSET_MODE_2D_UNMAPPED:
                tmp_in.first = asset.vertices.p_unmapped_vert_data_2d[index].vert_data.vert_x;
                tmp_in.second = asset.vertices.p_unmapped_vert_data_2d[index].vert_data.vert_y;
                tmp_out = mat * tmp_in;
                asset.vertices.p_unmapped_vert_data_2d[index].vert_data.vert_x = tmp_out.first;
                asset.vertices.p_unmapped_vert_data_2d[index].vert_data.vert_y = tmp_out.second;
                break;

            default:
                WARNME("Invalid asset vertices type in dengMath::Transformer::apply2DModelMatrix()!");
                return;
            }
        }
    }


    void Transformer::apply2DModelMatrix (
        vec2<deng_ui32_t> asset_bounds,
        std::vector<deng_Asset> *p_assets,
        mat3<deng_vec_t> &mat
    ) {
        //if(asset_bounds.second > p_assets->size())
            //return;
        for(size_t i = asset_bounds.first; i < asset_bounds.second; i++) {
            Transformer::apply2DModelMatrix (
                p_assets->at(i),
                mat
            );
        }
    }
}  
