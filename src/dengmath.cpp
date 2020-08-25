#include "headers/renderer.h"

namespace deng {

    float degToRad(const float &deg) {
        return (float) deg/180 * PI;
    }

    void getCirclePointCoords(const float &centre_x, const float &centre_y, const uint16_t &angle, const float &radius, float *out_x, float *out_y) {
        if(out_x != nullptr) *out_x = radius * sin(degToRad(angle)) + centre_x;
        if(out_x != nullptr) *out_y = radius * cos(degToRad(angle)) + centre_y;
    }

    float getFractionNumerator(const float &valNum, const float &valDenom, const float &equivalentDenom) {
        return (valNum * equivalentDenom) / valDenom;
    }

    void ModelMatrix::setRotation(const float &x_rot, const float &y_rot, const float &z_rot) {
        this->m_RxMat = {{1.0f, 0.0f, 0.0f, 0.0f},
                       {0.0f, cos(degToRad(x_rot)), -(sin(degToRad(x_rot))), 0.0f},
                       {0.0f, sin(degToRad(x_rot)), cos(degToRad(x_rot)), 0.0f}, 
                       {0.0f, 0.0f, 0.0f, 1.0f}};

        this->m_RyMat = {{cos(degToRad(y_rot)), 0.0f, sin(degToRad(y_rot)), 0.0f},
                       {0.0f, 1.0f, 0.0f, 0.0f},
                       {-(sin(degToRad(y_rot))), 0.0f, cos(degToRad(y_rot)), 0.0f},
                       {0.0f, 0.0f, 0.0f, 1.0f}};

        this->m_RzMat = {{cos(degToRad(z_rot)), -(sin(degToRad(z_rot))), 0.0f, 0.0f},
                       {sin(degToRad(z_rot)), cos(degToRad(z_rot)), 0.0f, 0.0f},
                       {0.0f, 0.0f, 1.0f, 0.0f}, 
                       {0.0f, 0.0f, 0.0f, 1.0f}};
    }

    void ModelMatrix::setTransformation(const float &Tx, const float &Ty, const float &Tz) {
        this->m_transformMat = {{1.0f, 0.0f, 0.0f, Tx}, 
                              {0.0f, 1.0f, 0.0f, Ty},
                              {0.0f, 0.0f, 1.0f, Tz},
                              {0.0f, 0.0f, 0.0f, 1.0f}};
    }

    void ModelMatrix::setScale(const float &Sx, const float &Sy, const float &Sz) {
        this->m_scaleMat = {{Sx, 0, 0, 0},
                          {0, Sy, 0, 0},
                          {0, 0, Sz, 0},
                          {0, 0, 0, 1}};
    }

    void ModelMatrix::getModelMatrix(mat4<float> *model) {
        *model = this->m_transformMat * this->m_RxMat * this->m_RyMat * this->m_RzMat * this->m_scaleMat;
    }

    ViewMatrix::ViewMatrix() {
        this->x_rot = 0.0f;
        this->y_rot = 0.0f;
        this->z_rot = 0.0f;

        this->m_rightSide = {1.0f, 0.0f, 0.0f, 0.0f};
        this->m_upSide = {0.0f, 1.0f, 0.0f, 0.0f};
        this->m_forwardSide = {0.0f, 0.0f, 1.0f, 0.0f};

        this->m_cameraPosition = {DENG_CAMERA_DEFAULT_X, DENG_CAMERA_DEFAULT_Y, DENG_CAMERA_DEFAULT_Z, 1.0f};
    }

    void ViewMatrix::setCameraPosition(const vec4<float> &newPos) {
        this->m_cameraPosition = newPos;
    }

    void ViewMatrix::addToPosition(const vec4<float> &movement_speed, const CoordinateType &movementType, const bool &substract) {
        float movement_X, movement_Y, movement_Z;
        switch (movementType)
        {
        case DENG_X:
            getCirclePointCoords(0.0f, 0.0f, (this->y_rot + 90), movement_speed.x, &movement_X, &movement_Z);

            if(substract) this->m_cameraPosition.x -= movement_X, this->m_cameraPosition.z -= movement_Z;
            else this->m_cameraPosition.x += movement_X, this->m_cameraPosition.z += movement_Z;
            break;

        case DENG_Y:
            if(substract) this->m_cameraPosition.y -= movement_speed.y;
            else this->m_cameraPosition.y += movement_speed.y;
            break;

        case DENG_Z:
            getCirclePointCoords(0.0f, 0.0f, this->y_rot, movement_speed.z, &movement_X, &movement_Z);
            
            if(substract) this->m_cameraPosition.x -= movement_X, this->m_cameraPosition.z -= movement_Z;
            else this->m_cameraPosition.x += movement_X, this->m_cameraPosition.z += movement_Z;
            break;
        
        default:
            break;
        }
    }

    void ViewMatrix::getViewMatrix(mat4<float> *view) {
        *view = this->m_transformationMat * this->m_RxMat * this->m_RzMat * this->m_RyMat;
    }

    void ViewMatrix::setTransformationMatrix() {
        this->m_transformationMat.row1 = this->m_rightSide;
        this->m_transformationMat.row2 = this->m_upSide;
        this->m_transformationMat.row3 = this->m_forwardSide;
        this->m_transformationMat.row4 = this->m_cameraPosition;
    }

    vec4<float> ViewMatrix::getPosition() {
        return this->m_cameraPosition;
    }

    void ViewMatrix::setRotation(const float &x_rot, const float &y_rot) {
        this->y_rot = y_rot;
        getCirclePointCoords(0.0f, 0.0f, this->y_rot, x_rot, &this->z_rot, &this->x_rot); 
        this->z_rot = -this->z_rot;

        this->m_RxMat = {{1.0f, 0.0f, 0.0f, 0.0f},
                       {0.0f, cos(degToRad(this->x_rot)), -(sin(degToRad(this->x_rot))), 0.0f},
                       {0.0f, sin(degToRad(this->x_rot)), cos(degToRad(this->x_rot)), 0.0f}, 
                       {0.0f, 0.0f, 0.0f, 1.0f}};

        this->m_RyMat = {{cos(degToRad(this->y_rot)), 0.0f, sin(degToRad(this->y_rot)), 0.0f},
                       {0.0f, 1.0f, 0.0f, 0.0f},
                       {-(sin(degToRad(this->y_rot))), 0.0f, cos(degToRad(this->y_rot)), 0.0f},
                       {0.0f, 0.0f, 0.0f, 1.0f}};

        this->m_RzMat = {{cos(degToRad(this->z_rot)), -(sin(degToRad(this->z_rot))), 0.0f, 0.0f},
                       {sin(degToRad(this->z_rot)), cos(degToRad(this->z_rot)), 0.0f, 0.0f},
                       {0.0f, 0.0f, 1.0f, 0.0f}, 
                       {0.0f, 0.0f, 0.0f, 1.0f}};

    }

    ProjectionMatrix::ProjectionMatrix(const float &FOV, const float &near, const float &far, const float &aspect_ratio) {
        this->m_FOV = FOV;
        this->m_near = near;
        this->m_far = far;
        this->m_aspectRatio = aspect_ratio;

        this->m_top = this->m_near * tan(degToRad(this->m_FOV/2));
        this->m_bottom = -this->m_near * tan(degToRad(this->m_FOV/2));
        this->m_right = this->m_near * tan(degToRad(this->m_FOV/2)) * this->m_aspectRatio;
        this->m_left = -this->m_near * tan(degToRad(this->m_FOV/2)) * this->m_aspectRatio;

    }

    void ProjectionMatrix::getProjectionMatrix(mat4<float> *matrix) {
        matrix->row1 = {this->m_aspectRatio * 1/tan(degToRad(this->m_FOV/2)), 0, 0, 0};
        matrix->row2 = {0, 1/tan(degToRad(this->m_FOV/2)), 0, 0};
        matrix->row3 = {0, 0, this->m_far / (m_far - m_near), 1};
        matrix->row4 = {0, 0, -(this->m_far * this->m_near) / (this->m_far - this->m_near), 0};
        // matrix->row1 = {1, 0, 0, 0};
        // matrix->row2 = {0, 1, 0, 0};
        // matrix->row3 = {0, 0, 1, 0};
        // matrix->row4 = {0, 0, 0, 1};
    }
}  