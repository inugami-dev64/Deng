#include "headers/renderer.h"

namespace deng {

    float degToRad(uint16_t deg) {
        return (float) deg/180 * PI;
    }

    void ModelMatrix::setRotation(const uint16_t &Rx, const uint16_t &Ry, const uint16_t &Rz) {
        this->m_RxMat = {{1.0f, 0.0f, 0.0f, 0.0f},
                       {0.0f, cos(degToRad(Rx)), -(sin(degToRad(Rx))), 0.0f},
                       {0.0f, sin(degToRad(Rx)), cos(degToRad(Rx)), 0.0f}, 
                       {0.0f, 0.0f, 0.0f, 1.0f}};

        

        this->m_RyMat = {{cos(degToRad(Ry)), 0.0f, sin(degToRad(Ry)), 0.0f},
                       {0.0f, 1.0f, 0.0f, 0.0f},
                       {-(sin(degToRad(Ry))), 0.0f, cos(degToRad(Ry)), 0.0f},
                       {0.0f, 0.0f, 0.0f, 1.0f}};

        this->m_RzMat = {{cos(degToRad(Rz)), -(sin(degToRad(Rz))), 0.0f, 0.0f},
                       {sin(degToRad(Rz)), cos(degToRad(Rz)), 0.0f, 0.0f},
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

    void ViewMatrix::setAxes(const vec4<float> &right, const vec4<float> &up, const vec4<float> &forward) {
        this->m_rightSide = right;
        this->m_upSide = up;
        this->m_forwardSide = forward;
    }

    void ViewMatrix::setPosition(const vec4<float> &newPos) {
        this->m_cameraPosition = newPos;
    }

    void ViewMatrix::addToPosition(const vec4<float> &addition, const CoordinateType &type, const bool &substract) {
        switch (type)
        {
        case DENG_X:
            if(substract) this->m_cameraPosition.x -= addition.x;
            else this->m_cameraPosition.x += addition.x;
            break;

        case DENG_Y:
            if(substract) this->m_cameraPosition.y -= addition.y;
            else this->m_cameraPosition.y += addition.y;
            break;

        case DENG_Z:
            if(substract) this->m_cameraPosition.z -= addition.z;
            else this->m_cameraPosition.z += addition.z;
            break;
        
        default:
            break;
        }

        LOG("Camera X: " + std::to_string(this->m_cameraPosition.x) + "/Camera Y: " + std::to_string(this->m_cameraPosition.y) + "/Camera Z: " + std::to_string(this->m_cameraPosition.z) + "/Camera W: " + std::to_string(this->m_cameraPosition.w));
    }

    void ViewMatrix::getViewMatrix(mat4<float> *view) {
        view->row1 = this->m_rightSide;
        view->row2 = this->m_upSide;
        view->row3 = this->m_forwardSide;
        view->row4 = this->m_cameraPosition;
        // view->row1 = {1, 0, 0, 0};
        // view->row2 = {0, 1, 0, 0};
        // view->row3 = {0, 0, 1, 0};
        // view->row4 = {0, 0, 0, 1};
    }

    vec4<float> ViewMatrix::getPosition() {
        return this->m_cameraPosition;
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