#include "../core/api_core.h"

namespace dengMath {

    // generic math methods
    // Expand base by exp 
    double exp(double base, int exp) {
        if(exp == 0) return 1.0;
        double current_value = base;

        for(int i = 0; i < exp - 1; i++) {
            current_value *= base;
        }

        if(exp < 0) return 1.0/current_value;
        else return current_value;
    }

    // Expand base by exp
    float exp(float base, int exp) {
        if(exp == 0) return 1.0;
        float current_value = base;

        for(int i = 0; i < exp - 1; i++) {
            current_value *= base;
        }

        if(exp < 0) return 1.0/current_value;
        else return current_value;
    }

    vec2<float> getCartesianCoordsPoint(const vec2<float> &centre_position, const deng_i16_t &angle, const float &distance, const bool &inverted_y_axis) {
        vec2<float> coords;
        coords.first = (sin(Conversion::degToRad(angle)) * distance) + centre_position.first;
        if(!inverted_y_axis) coords.second = (cos(Conversion::degToRad(angle)) * distance) + centre_position.second;
        else coords.second = -(cos(Conversion::degToRad(angle)) * distance) + centre_position.second;

        return coords; 
    }

    float getFractionNumerator(const float &value_numerator, const float &value_denominator, const float &equivalent_denominator) {
        return (value_numerator * equivalent_denominator) / value_denominator;
    }

    float getVectorLengthFromBounds(vec2<vec2<float>> vector_bounds) {
        vec2<float> vector_coordinates = vector_bounds.second - vector_bounds.first;
        return static_cast<float>(sqrt(pow(vector_coordinates.first, 2) + pow(vector_coordinates.second, 2)));
    }

    float getTriangleAnglesFromEdges(const vec3<float> &triangle_edges, const deng_TriangleAngleType &triangle_angle_type) {
        switch (triangle_angle_type)
        {
        case DENG_TRIANGLE_ANGLE_ALPHA:
            return Conversion::radToDeg(static_cast<float>(asin(sqrt((triangle_edges.first + triangle_edges.second - triangle_edges.third) * (triangle_edges.first - triangle_edges.second + triangle_edges.third) * (-triangle_edges.first + triangle_edges.second + triangle_edges.third) * 
            (triangle_edges.first + triangle_edges.second + triangle_edges.third)) / 2 * triangle_edges.second * triangle_edges.third)));
            break;
        
        case DENG_TRIANGLE_ANGLE_BETA:
            return Conversion::radToDeg(static_cast<float>(asin(sqrt((triangle_edges.first + triangle_edges.second - triangle_edges.third) * (triangle_edges.first - triangle_edges.second + triangle_edges.third) * (-triangle_edges.first + triangle_edges.second + triangle_edges.third) * 
            (triangle_edges.first + triangle_edges.second + triangle_edges.third)) / 2 * triangle_edges.first * triangle_edges.third)));
            break;

        case DENG_TRIANGLE_ANGLE_GAMMA:
            return Conversion::radToDeg(static_cast<float>(asin(sqrt((triangle_edges.first + triangle_edges.second - triangle_edges.third) * (triangle_edges.first - triangle_edges.second + triangle_edges.third) * (-triangle_edges.first + triangle_edges.second + triangle_edges.third) * 
            (triangle_edges.first + triangle_edges.second + triangle_edges.third)) / 2 * triangle_edges.first * triangle_edges.second)));
            break;

        default:
            break;
        }

        return 0.0f;
    }

    float getVector2DRotation(vec2<vec2<float>> vector_bounds) {
        vec2<float> vector_position = vector_bounds.second - vector_bounds.first;
        float vector_length = getVectorLengthFromBounds(vector_bounds);
        
        // first - rotation calculated from sin, second - rotation calculated from cos
        vec2<float> rotations;
        rotations.first = static_cast<float>(asin(static_cast<double>(vector_position.first) / static_cast<double>(vector_length)));
        rotations.second = static_cast<float>(acos(-static_cast<double>(vector_position.second) / static_cast<double>(vector_length)));
        
        LOG("sin_rot: " + std::to_string(Conversion::radToDeg(rotations.first)) + "/cos_rot: " + std::to_string(Conversion::radToDeg(rotations.second)));

        if(rotations.first >= 0.0f) return static_cast<float>(round(Conversion::radToDeg(static_cast<float>(fabs(rotations.first)))));
        
        if(rotations.first < 0 && Conversion::radToDeg(rotations.second) >= 0 && Conversion::radToDeg(rotations.second) < 90.0f) {
            LOG("Condition rotations.first < 0 && (rotations.second >= 0 && rotations.second < 90) fulfilled!");
            return (180.0f + static_cast<float>(round(Conversion::radToDeg(static_cast<float>(fabs(rotations.first))))));
        }

        else if(rotations.first < 0 && Conversion::radToDeg(rotations.second) >= 90 && Conversion::radToDeg(rotations.second) < 180.0f) {
            LOG("Condition rotations.first < 0 && (rotations.second >= 90 && rotations.second < 180) fulfilled!");
            return (270.0f + static_cast<float>(round(Conversion::radToDeg(static_cast<float>(fabs(rotations.first))))));
        }

        return 0.0f;
    }


    /* Generic conversion methods */
    float Conversion::degToRad(const float &deg) {
        return static_cast<float>(deg/180 * PI);
    }

    float Conversion::radToDeg(const float &rad) {
        return static_cast<float>(rad/(2 * PI) * 360);
    }

    deng_ui32_t Conversion::hexToDec(const std::string &hex_value) {
        deng_ui32_t dec = 0;
        for(size_t i = 0, exp_i = hex_value.size() - 1; i < hex_value.size(); i++, exp_i--)
            dec += (hex_definitions[hex_value[i]] * dengMath::exp(16.0, exp_i));

        LOG("decimal of " + hex_value + " is " + std::to_string(dec));
        return dec;
    }

    deng_px_t Conversion::vector2DSizeToPixelSize (
        const deng_vec_t &vec_size, 
        const vec2<deng_ui32_t> &window_size, 
        const deng_CoordinateAxisType &axis_type
    ) {
        switch (axis_type)
        {
        case DENG_COORD_AXIS_X:
            return static_cast<double>(vec_size) * static_cast<double>(window_size.first) / 2;
            break;
        
        case DENG_COORD_AXIS_Y: 
            return (static_cast<double>(vec_size) * static_cast<double>(window_size.second)) / 2;
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
            return ((2.0f / static_cast<float>(window_size.first)) * static_cast<float>(pixel_size));
            break;
        
        case DENG_COORD_AXIS_Y:
            return ((2.0f / static_cast<float>(window_size.second)) * static_cast<float>(pixel_size));
            break;

        case DENG_COORD_AXIS_UNDEFINED: {
            deng_ui32_t avg_size = (window_size.first + window_size.second) / 2;
            return ((2.0f / static_cast<float>(avg_size)) * static_cast<float>(pixel_size));   
            break;
        }

        default:
            break;
        }

        return 0.0f;
    }

    ModelMatrix::ModelMatrix() {
        setTransformation(0.0f, 0.0f, 0.0f);
        setRotation(0.0f, 0.0f, 0.0f);
        setScale(1.0f, 1.0f, 1.0f);
    }

    void ModelMatrix::setRotation(const float &x_rot, const float &y_rot, const float &z_rot) {
        this->m_rot_x_mat = {{1.0f, 0.0f, 0.0f, 0.0f},
                        {0.0f, static_cast<float>(cos(Conversion::degToRad(x_rot))), static_cast<float>(-sin(Conversion::degToRad(x_rot))), 0.0f},
                        {0.0f, static_cast<float>(sin(Conversion::degToRad(x_rot))), static_cast<float>(cos(Conversion::degToRad(x_rot))), 0.0f}, 
                        {0.0f, 0.0f, 0.0f, 1.0f}};

        this->m_rot_y_mat = {{static_cast<float>(cos(Conversion::degToRad(y_rot))), 0.0f, static_cast<float>(sin(Conversion::degToRad(y_rot))), 0.0f},
                        {0.0f, 1.0f, 0.0f, 0.0f},
                        {static_cast<float>(-sin(Conversion::degToRad(y_rot))), 0.0f, static_cast<float>(cos(Conversion::degToRad(y_rot))), 0.0f},
                        {0.0f, 0.0f, 0.0f, 1.0f}};

        this->m_rot_z_mat = {{static_cast<float>(cos(Conversion::degToRad(z_rot))), static_cast<float>(-sin(Conversion::degToRad(z_rot))), 0.0f, 0.0f},
                        {static_cast<float>(sin(Conversion::degToRad(z_rot))), static_cast<float>(cos(Conversion::degToRad(z_rot))), 0.0f, 0.0f},
                        {0.0f, 0.0f, 1.0f, 0.0f}, 
                        {0.0f, 0.0f, 0.0f, 1.0f}};
    }

    void ModelMatrix::setTransformation(const float &transform_x, const float &transform_y, const float &transform_z) {
        this->m_transformation_mat = {{1.0f, 0.0f, 0.0f, transform_x}, 
                                {0.0f, 1.0f, 0.0f, transform_y},
                                {0.0f, 0.0f, 1.0f, transform_z},
                                {0.0f, 0.0f, 0.0f, 1.0f}};
    }

    void ModelMatrix::setScale(const float &scale_x, const float &scale_y, const float &scale_z) {
        this->m_scale_mat = {{scale_x, 0, 0, 0},
                            {0, scale_y, 0, 0},
                            {0, 0, scale_z, 0},
                            {0, 0, 0, 1}};
    }


    ViewMatrix::ViewMatrix() {
        this->x_rot = 0.0f;
        this->y_rot = 0.0f;
        this->z_rot = 0.0f;

        this->m_right_side = {1.0f, 0.0f, 0.0f, 0.0f};
        this->m_up_side = {0.0f, 1.0f, 0.0f, 0.0f};
        this->m_forward_side = {0.0f, 0.0f, 1.0f, 0.0f};

        this->m_camera_position = {DENG_CAMERA_DEFAULT_X, DENG_CAMERA_DEFAULT_Y, DENG_CAMERA_DEFAULT_Z, 1.0f};
    }

    void ViewMatrix::setCameraPosition(const vec4<float> &new_position) {
        this->m_camera_position = new_position;
    }

    void ViewMatrix::addToPosition(const vec4<float> &movement_speed, const deng_CoordinateAxisType &movement_type, const bool &substract) {
        switch (movement_type)
        {
        case DENG_COORD_AXIS_X: {
            vec2<float> movement = getCartesianCoordsPoint({0.0f, 0.0f}, (this->y_rot + 90), movement_speed.first, false);

            if(substract) this->m_camera_position.first -= movement.first, this->m_camera_position.third -= movement.second ;
            else this->m_camera_position.first += movement.first, this->m_camera_position.third += movement.second;
            break;
        }

        case DENG_COORD_AXIS_Y:
            if(substract) this->m_camera_position.second -= movement_speed.second;
            else this->m_camera_position.second += movement_speed.second;
            break;

        case DENG_COORD_AXIS_Z: {
            vec2<float> movement = getCartesianCoordsPoint({0.0f, 0.0f}, this->y_rot, movement_speed.third, false);
            
            if(substract) this->m_camera_position.first -= movement.first, this->m_camera_position.third -= movement.second;
            else this->m_camera_position.first += movement.first, this->m_camera_position.third += movement.second;
            break;
        }
        
        default:
            break;
        }

        #if CAMERA_LOCATION_DEBUG
            LOG("x: " + std::to_string(this->m_camera_position.first) + "/y: " + std::to_string(this->m_camera_position.second) + "/z: " + std::to_string(this->m_camera_position.third));
        #endif
    }

    void ViewMatrix::getViewMatrix(mat4<deng_vec_t> *view) {
        *view = this->m_transformation_mat * this->m_rot_x_mat * this->m_rot_z_mat * this->m_rot_y_mat;
    }

    void ViewMatrix::setTransformationMatrix() {
        this->m_transformation_mat.row1 = this->m_right_side;
        this->m_transformation_mat.row2 = this->m_up_side;
        this->m_transformation_mat.row3 = this->m_forward_side;
        this->m_transformation_mat.row4 = this->m_camera_position;
    }

    vec4<float> ViewMatrix::getPosition() {
        return this->m_camera_position;
    }

    void ViewMatrix::setRotation(const float &x_rot, const float &y_rot) {
        this->y_rot = static_cast<float>(y_rot);
        this->x_rot = static_cast<float>(cos(Conversion::degToRad(y_rot)) * x_rot);
        this->z_rot = static_cast<float>(-sin(Conversion::degToRad(y_rot)) * x_rot);

        this->m_rot_x_mat = {{1.0f, 0.0f, 0.0f, 0.0f},
                            {0.0f, static_cast<float>(cos(Conversion::degToRad(this->x_rot))), static_cast<float>(-sin(Conversion::degToRad(this->x_rot))), 0.0f},
                            {0.0f, static_cast<float>(sin(Conversion::degToRad(this->x_rot))), static_cast<float>(cos(Conversion::degToRad(this->x_rot))), 0.0f}, 
                            {0.0f, 0.0f, 0.0f, 1.0f}};

        this->m_rot_y_mat = {{static_cast<float>(cos(Conversion::degToRad(this->y_rot))), 0.0f, static_cast<float>(sin(Conversion::degToRad(this->y_rot))), 0.0f},
                            {0.0f, 1.0f, 0.0f, 0.0f},
                            {static_cast<float>(-sin(Conversion::degToRad(this->y_rot))), 0.0f, static_cast<float>(cos(Conversion::degToRad(this->y_rot))), 0.0f},
                            {0.0f, 0.0f, 0.0f, 1.0f}};

        this->m_rot_z_mat = {{static_cast<float>(cos(Conversion::degToRad(this->z_rot))), static_cast<float>(-sin(Conversion::degToRad(this->z_rot))), 0.0f, 0.0f},
                            {static_cast<float>(sin(Conversion::degToRad(this->z_rot))), static_cast<float>(cos(Conversion::degToRad(this->z_rot))), 0.0f, 0.0f},
                            {0.0f, 0.0f, 1.0f, 0.0f}, 
                            {0.0f, 0.0f, 0.0f, 1.0f}};

    }

    ProjectionMatrix::ProjectionMatrix(const float &FOV, const float &near_plane, const float &far_plane, const float &aspect_ratio) {
        this->m_FOV = FOV;
        this->m_near = near_plane;
        this->m_far = far_plane;
        this->m_aspect_ratio = aspect_ratio;
    }

    void ProjectionMatrix::getProjectionMatrix(mat4<deng_vec_t> *matrix) {
        matrix->row1 = {this->m_aspect_ratio * static_cast<float>(1/tan(Conversion::degToRad(this->m_FOV/2))), 0, 0, 0};
        matrix->row2 = {0, static_cast<float>(1/tan(Conversion::degToRad(this->m_FOV/2))), 0, 0};
        matrix->row3 = {0, 0, this->m_far / (this->m_far - this->m_near), 1};
        matrix->row4 = {0, 0, -(this->m_far * this->m_near) / (this->m_far - this->m_near), 0};
        // matrix->row1 = {1, 0, 0, 0};
        // matrix->row2 = {0, 1, 0, 0};
        // matrix->row3 = {0, 0, 1, 0};
        // matrix->row4 = {0, 0, 0, 1};
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
    void sortVectorInDecliningOrder(std::vector<T> *p_elements_vector, deng_CoordinateAxisType coord_axis_type) {
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
    T *findElementByDescription(std::vector<T> *p_elements, const char *description) {
        for(size_t i = 0; i < p_elements->size(); i++) {
            LOG("findElementByDescription description is: " + std::string((*p_elements)[i].description));
            if((*p_elements)[i].description == description) {
                return &p_elements->at(i);
            } 
        }
        return &p_elements->at(0);
    }
    

    // Apply model matrix for 3d asset
    void applyModelMatrix(deng_Asset &asset, mat4<deng_vec_t> matrix) {
        size_t index;
        dengMath::vec3<float> *p_tmp_in;
        dengMath::vec4<float> tmp_out;

        for(index = 0; index < asset.vertices.size; index++) {
            switch (asset.asset_mode)
            {
            case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
                p_tmp_in = (dengMath::vec3<float>*) &asset.vertices.p_texture_mapped_vert_data[index].vert_data;
                tmp_out = matrix * (*p_tmp_in);
                asset.vertices.p_texture_mapped_vert_data[index].vert_data.vert_x = tmp_out.first;
                asset.vertices.p_texture_mapped_vert_data[index].vert_data.vert_y = tmp_out.second;
                asset.vertices.p_texture_mapped_vert_data[index].vert_data.vert_z = tmp_out.third;
                break;

            case DENG_ASSET_MODE_3D_UNMAPPED:
                p_tmp_in = (dengMath::vec3<float>*) &asset.vertices.p_unmapped_vert_data[index]; 
                tmp_out = matrix * (*p_tmp_in);
                asset.vertices.p_unmapped_vert_data[index].vert_data.vert_x = tmp_out.first;
                asset.vertices.p_unmapped_vert_data[index].vert_data.vert_y = tmp_out.second;
                asset.vertices.p_unmapped_vert_data[index].vert_data.vert_z = tmp_out.third;
                break;

            case DENG_ASSET_MODE_2D_UNMAPPED:
            case DENG_ASSET_MODE_2D_TEXTURE_MAPPED:
                WARNME("2D assets are not supported in applyModelMatrix()!");
                WARNME("Use applyModelMatrix2D() instead!");
                return;
            default:
                break;
            }
        }
    }

    
    /* Calculate triangle surface area based on triangle vertices */
    float trSurface2D(std::array<vec2<float>, 3> tr_verts) {
        // Find triangle sides
        float a = 
            sqrt (
                (tr_verts[1].first - tr_verts[0].first) * (tr_verts[1].first - tr_verts[0].first) + 
                (tr_verts[1].second - tr_verts[0].second) * (tr_verts[1].second - tr_verts[0].second)
            );

        float b =
            sqrt (
                (tr_verts[2].first - tr_verts[1].first) * (tr_verts[2].first - tr_verts[1].first) +
                (tr_verts[2].second - tr_verts[1].second) * (tr_verts[2].second - tr_verts[1].second)
            );

        float c =
            sqrt (
                (tr_verts[2].first - tr_verts[0].first) * (tr_verts[2].first - tr_verts[0].first) +
                (tr_verts[2].second - tr_verts[0].second) * (tr_verts[2].second - tr_verts[0].second)
            );

        // Triangle semi perimeter
        float s = (a + b + c) / 2;

        return std::sqrt((s * (s - a) * (s - b) * (s - c)));
    }
}  