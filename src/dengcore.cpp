#include "headers/renderer.h"

namespace deng {

    float degToRad(const float &deg) {
        return (float) deg/180 * PI;
    }

    void getCirclePointCoords(const float &centre_x, const float &centre_y, const uint16_t &angle, const float &radius, float *out_x, float *out_y) {
        if(out_x != nullptr) *out_x = radius * static_cast<float>(sin(degToRad(angle))) + centre_x;
        if(out_x != nullptr) *out_y = radius * static_cast<float>(cos(degToRad(angle))) + centre_y;
    }

    float getFractionNumerator(const float &value_numerator, const float &value_denominator, const float &equivalent_denominator) {
        return (value_numerator * equivalent_denominator) / value_denominator;
    }

    void ModelMatrix::setRotation(const float &x_rot, const float &y_rot, const float &z_rot) {
        this->m_rot_x_mat = {{1.0f, 0.0f, 0.0f, 0.0f},
                        {0.0f, static_cast<float>(cos(degToRad(x_rot))), static_cast<float>(-sin(degToRad(x_rot))), 0.0f},
                        {0.0f, static_cast<float>(sin(degToRad(x_rot))), static_cast<float>(cos(degToRad(x_rot))), 0.0f}, 
                        {0.0f, 0.0f, 0.0f, 1.0f}};

        this->m_rot_y_mat = {{static_cast<float>(cos(degToRad(y_rot))), 0.0f, static_cast<float>(sin(degToRad(y_rot))), 0.0f},
                        {0.0f, 1.0f, 0.0f, 0.0f},
                        {static_cast<float>(-sin(degToRad(y_rot))), 0.0f, static_cast<float>(cos(degToRad(y_rot))), 0.0f},
                        {0.0f, 0.0f, 0.0f, 1.0f}};

        this->m_rot_z_mat = {{static_cast<float>(cos(degToRad(z_rot))), static_cast<float>(-sin(degToRad(z_rot))), 0.0f, 0.0f},
                        {static_cast<float>(sin(degToRad(z_rot))), static_cast<float>(cos(degToRad(z_rot))), 0.0f, 0.0f},
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

    void ViewMatrix::addToPosition(const vec4<float> &movement_speed, const dengCoordinateType &movement_type, const bool &substract) {
        float movement_X, movement_Z;
        switch (movement_type)
        {
        case DENG_X:
            getCirclePointCoords(0.0f, 0.0f, (this->y_rot + 90), movement_speed.first, &movement_X, &movement_Z);

            if(substract) this->m_camera_position.first -= movement_X, this->m_camera_position.third -= movement_Z;
            else this->m_camera_position.first += movement_X, this->m_camera_position.third += movement_Z;
            break;

        case DENG_Y:
            if(substract) this->m_camera_position.second -= movement_speed.second;
            else this->m_camera_position.second += movement_speed.second;
            break;

        case DENG_Z:
            getCirclePointCoords(0.0f, 0.0f, this->y_rot, movement_speed.third, &movement_X, &movement_Z);
            
            if(substract) this->m_camera_position.first -= movement_X, this->m_camera_position.third -= movement_Z;
            else this->m_camera_position.first += movement_X, this->m_camera_position.third += movement_Z;
            break;
        
        default:
            break;
        }
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
        this->x_rot = static_cast<float>(cos(degToRad(y_rot)) * x_rot);
        this->z_rot = static_cast<float>(-sin(degToRad(y_rot)) * x_rot);

        this->m_rot_x_mat = {{1.0f, 0.0f, 0.0f, 0.0f},
                       {0.0f, static_cast<float>(cos(degToRad(this->x_rot))), static_cast<float>(-sin(degToRad(this->x_rot))), 0.0f},
                       {0.0f, static_cast<float>(sin(degToRad(this->x_rot))), static_cast<float>(cos(degToRad(this->x_rot))), 0.0f}, 
                       {0.0f, 0.0f, 0.0f, 1.0f}};

        this->m_rot_y_mat = {{static_cast<float>(cos(degToRad(this->y_rot))), 0.0f, static_cast<float>(sin(degToRad(this->y_rot))), 0.0f},
                       {0.0f, 1.0f, 0.0f, 0.0f},
                       {static_cast<float>(-sin(degToRad(this->y_rot))), 0.0f, static_cast<float>(cos(degToRad(this->y_rot))), 0.0f},
                       {0.0f, 0.0f, 0.0f, 1.0f}};

        this->m_rot_z_mat = {{static_cast<float>(cos(degToRad(this->z_rot))), static_cast<float>(-sin(degToRad(this->z_rot))), 0.0f, 0.0f},
                       {static_cast<float>(sin(degToRad(this->z_rot))), static_cast<float>(cos(degToRad(this->z_rot))), 0.0f, 0.0f},
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
        matrix->row1 = {this->m_aspect_ratio * static_cast<float>(1/tan(degToRad(this->m_FOV/2))), 0, 0, 0};
        matrix->row2 = {0, static_cast<float>(1/tan(degToRad(this->m_FOV/2))), 0, 0};
        matrix->row3 = {0, 0, this->m_far / (this->m_far - this->m_near), 1};
        matrix->row4 = {0, 0, -(this->m_far * this->m_near) / (this->m_far - this->m_near), 0};
        // matrix->row1 = {1, 0, 0, 0};
        // matrix->row2 = {0, 1, 0, 0};
        // matrix->row3 = {0, 0, 1, 0};
        // matrix->row4 = {0, 0, 0, 1};
    }
}  