#include "../core/deng_core.h"

namespace dengMath {

    // generic math methods
    double Math::exp(double base, int exp) {
        if(exp == 0) return 1.0;
        double local_current_value = base;

        for(int i = 0; i < exp - 1; i++) {
            local_current_value *= base;
        }

        if(exp < 0) return 1.0/local_current_value;
        else return local_current_value;
    }

    vec2<float> Math::getCartesianCoordsPoint(const vec2<float> &centre_position, const int16_t &angle, const float &distance, const dengBool &inverted_y_axis) {
        vec2<float> local_coords;
        local_coords.first = (sin(Conversion::degToRad(angle)) * distance) + centre_position.first;
        if(!inverted_y_axis) local_coords.second = (cos(Conversion::degToRad(angle)) * distance) + centre_position.second;
        else local_coords.second = -(cos(Conversion::degToRad(angle)) * distance) + centre_position.second;

        return local_coords; 
    }

    float Math::getFractionNumerator(const float &value_numerator, const float &value_denominator, const float &equivalent_denominator) {
        return (value_numerator * equivalent_denominator) / value_denominator;
    }

    float Math::getVectorLengthFromBounds(vec2<vec2<float>> vector_bounds) {
        vec2<float> local_vector_coordinates = vector_bounds.second - vector_bounds.first;
        return static_cast<float>(sqrt(pow(local_vector_coordinates.first, 2) + pow(local_vector_coordinates.second, 2)));
    }

    float Math::getTriangleAnglesFromEdges(const vec3<float> &triangle_edges, const dengTriangleAngleType &triangle_angle_type) {
        switch (triangle_angle_type)
        {
        case DENG_TRIANGLE_ANGLE_ALPHA:
            return Conversion::radToDeg(static_cast<float>(asin(sqrt((triangle_edges.first + triangle_edges.second - triangle_edges.third) * (triangle_edges.first - triangle_edges.second + triangle_edges.third) * (-triangle_edges.first + triangle_edges.second + triangle_edges.third) * (triangle_edges.first + triangle_edges.second + triangle_edges.third))
            / 2 * triangle_edges.second * triangle_edges.third)));
            break;
        
        case DENG_TRIANGLE_ANGLE_BETA:
            return Conversion::radToDeg(static_cast<float>(asin(sqrt((triangle_edges.first + triangle_edges.second - triangle_edges.third) * (triangle_edges.first - triangle_edges.second + triangle_edges.third) * (-triangle_edges.first + triangle_edges.second + triangle_edges.third) * (triangle_edges.first + triangle_edges.second + triangle_edges.third))
            / 2 * triangle_edges.first * triangle_edges.third)));
            break;

        case DENG_TRIANGLE_ANGLE_GAMMA:
            return Conversion::radToDeg(static_cast<float>(asin(sqrt((triangle_edges.first + triangle_edges.second - triangle_edges.third) * (triangle_edges.first - triangle_edges.second + triangle_edges.third) * (-triangle_edges.first + triangle_edges.second + triangle_edges.third) * (triangle_edges.first + triangle_edges.second + triangle_edges.third))
            / 2 * triangle_edges.first * triangle_edges.second)));
            break;

        default:
            break;
        }
    }

    float Math::getVector2DRotation(vec2<vec2<float>> vector_bounds) {
        vec2<float> local_vector_position = vector_bounds.second - vector_bounds.first;
        float local_vector_length = Math::getVectorLengthFromBounds(vector_bounds);
        
        // first - rotation calculated from sin, second - rotation calculated from cos
        vec2<float> local_rotations;
        local_rotations.first = static_cast<float>(asin(static_cast<double>(local_vector_position.first) / static_cast<double>(local_vector_length)));
        local_rotations.second = static_cast<float>(acos(-static_cast<double>(local_vector_position.second) / static_cast<double>(local_vector_length)));
        
        LOG("sin_rot: " + std::to_string(Conversion::radToDeg(local_rotations.first)) + "/cos_rot: " + std::to_string(Conversion::radToDeg(local_rotations.second)));

        if(local_rotations.first >= 0.0f) return static_cast<float>(round(Conversion::radToDeg(static_cast<float>(fabs(local_rotations.first)))));
        
        if(local_rotations.first < 0 && Conversion::radToDeg(local_rotations.second) >= 0 && Conversion::radToDeg(local_rotations.second) < 90.0f) {
            LOG("Condition local_rotations.first < 0 && (local_rotations.second >= 0 && local_rotations.second < 90) fulfilled!");
            return (180.0f + static_cast<float>(round(Conversion::radToDeg(static_cast<float>(fabs(local_rotations.first))))));
        }

        else if(local_rotations.first < 0 && Conversion::radToDeg(local_rotations.second) >= 90 && Conversion::radToDeg(local_rotations.second) < 180.0f) {
            LOG("Condition local_rotations.first < 0 && (local_rotations.second >= 90 && local_rotations.second < 180) fulfilled!");
            return (270.0f + static_cast<float>(round(Conversion::radToDeg(static_cast<float>(fabs(local_rotations.first))))));
        }

        return 0.0f;
    }

    // generic conversion methods
    float Conversion::degToRad(const float &deg) {
        return static_cast<float>(deg/180 * PI);
    }

    float Conversion::radToDeg(const float &rad) {
        return static_cast<float>(rad/(2 * PI) * 360);
    }

    uint32_t Conversion::hexToDec(const std::string &hex_value) {
        uint32_t local_dec = 0;
        for(size_t i = 0, exp_i = hex_value.size() - 1; i < hex_value.size(); i++, exp_i--)
            local_dec += (hex_definitions[hex_value[i]] * dengMath::Math::exp(16.0, exp_i));

        LOG("decimal of " + hex_value + " is " + std::to_string(local_dec));
        return local_dec;
    }

    double Conversion::vector2DSizeToPixelSize(const float &vec_size, const vec2<uint32_t> &window_size, const dengCoordinateAxisType &axis_type) {
        switch (axis_type)
        {
        case DENG_COORD_AXIS_X:
            return static_cast<double>(vec_size + 1) / (2.0 / static_cast<double>(window_size.first));
            break;
        
        case DENG_COORD_AXIS_Y: 
            return static_cast<double>(vec_size + 1) / (2.0 / static_cast<double>(window_size.second));
            break;

        default:
            break;
        }

        return 0.0;
    }

    float Conversion::pixelSizeToVector2DSize(const double &pixel_size, const vec2<uint32_t> &window_size, const dengCoordinateAxisType &axis_type) {
        switch (axis_type)
        {
        case DENG_COORD_AXIS_X:
            return ((2.0f / static_cast<float>(window_size.first)) * static_cast<float>(pixel_size));
            break;
        
        case DENG_COORD_AXIS_Y:
            return ((2.0f / static_cast<float>(window_size.second)) * static_cast<float>(pixel_size));
            break;

        default:
            break;
        }

        return 0.0f;
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

    void ModelMatrix::getModelMatrix(mat4<float> *p_model) {
        *p_model = this->m_transformation_mat * this->m_rot_x_mat * this->m_rot_y_mat * this->m_rot_z_mat * this->m_scale_mat;
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

    void ViewMatrix::addToPosition(const vec4<float> &movement_speed, const dengCoordinateAxisType &movement_type, const bool &substract) {

        switch (movement_type)
        {
        case DENG_COORD_AXIS_X: {
            vec2<float> local_movement = Math::getCartesianCoordsPoint({0.0f, 0.0f}, (this->y_rot + 90), movement_speed.first, DENG_FALSE);

            if(substract) this->m_camera_position.first -= local_movement.first, this->m_camera_position.third -= local_movement.second ;
            else this->m_camera_position.first += local_movement.first, this->m_camera_position.third += local_movement.second;
            break;
        }

        case DENG_COORD_AXIS_Y:
            if(substract) this->m_camera_position.second -= movement_speed.second;
            else this->m_camera_position.second += movement_speed.second;
            break;

        case DENG_COORD_AXIS_Z: {
            vec2<float> local_movement = Math::getCartesianCoordsPoint({0.0f, 0.0f}, this->y_rot, movement_speed.third, DENG_FALSE);
            
            if(substract) this->m_camera_position.first -= local_movement.first, this->m_camera_position.third -= local_movement.second;
            else this->m_camera_position.first += local_movement.first, this->m_camera_position.third += local_movement.second;
            break;
        }
        
        default:
            break;
        }

        #if CAMERA_LOCATION_DEBUG
            LOG("x: " + std::to_string(this->m_camera_position.first) + "/y: " + std::to_string(this->m_camera_position.second) + "/z: " + std::to_string(this->m_camera_position.third));
        #endif
    }

    void ViewMatrix::getViewMatrix(mat4<float> *view) {
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

    ProjectionMatrix::ProjectionMatrix(const float &FOV, const float &near, const float &far, const float &aspect_ratio) {
        this->m_FOV = FOV;
        this->m_near = near;
        this->m_far = far;
        this->m_aspect_ratio = aspect_ratio;
    }

    void ProjectionMatrix::getProjectionMatrix(mat4<float> *matrix) {
        matrix->row1 = {this->m_aspect_ratio * static_cast<float>(1/tan(Conversion::degToRad(this->m_FOV/2))), 0, 0, 0};
        matrix->row2 = {0, static_cast<float>(1/tan(Conversion::degToRad(this->m_FOV/2))), 0, 0};
        matrix->row3 = {0, 0, this->m_far / (this->m_far - this->m_near), 1};
        matrix->row4 = {0, 0, -(this->m_far * this->m_near) / (this->m_far - this->m_near), 0};
        // matrix->row1 = {1, 0, 0, 0};
        // matrix->row2 = {0, 1, 0, 0};
        // matrix->row3 = {0, 0, 1, 0};
        // matrix->row4 = {0, 0, 0, 1};
    }
}  