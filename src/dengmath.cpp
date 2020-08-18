#include "headers/renderer.h"

namespace deng {

    float degToRad(const uint16_t &deg)  {
        return (deg/360) * 2 * PI;
    }

    void ModelMatrix::setRotation(const uint16_t &Rx, const uint16_t &Ry, const uint16_t &Rz) {
        this->RxMat = {{1.0f, 0.0f, 0.0f, 0.0f},
                       {0.0f, cos(degToRad(Rx)), -(sin(degToRad(Rx))), 0.0f},
                       {0.0f, sin(degToRad(Rx)), cos(degToRad(Rx)), 0.0f}, 
                       {0.0f, 0.0f, 0.0f, 1.0f}};

        this->RyMat = {{cos(degToRad(Ry)), 0.0f, sin(degToRad(Ry)), 0.0f},
                       {0.0f, 1.0f, 0.0f, 0.0f},
                       {-(sin(degToRad(Ry))), 0.0f, cos(degToRad(Ry)), 0.0f},
                       {0.0f, 0.0f, 0.0f, 1.0f}};

        this->RzMat = {{cos(degToRad(Rz)), -(sin(degToRad(Rz))), 0.0f, 0.0f},
                       {sin(degToRad(Rz)), cos(degToRad(Rz)), 0.0f, 0.0f},
                       {0.0f, 0.0f, 1.0f, 0.0f}, 
                       {0.0f, 0.0f, 0.0f, 1.0f}};
    }

    void ModelMatrix::setTransformation(const float &Tx, const float &Ty, const float &Tz) {
        this->transformMat = {{1.0f, 0.0f, 0.0f, Tx}, 
                              {0.0f, 1.0f, 0.0f, Ty},
                              {0.0f, 0.0f, 1.0f, Tz},
                              {0.0f, 0.0f, 0.0f, 1.0f}};
    }

    void ModelMatrix::setScale(const float &Sx, const float &Sy, const float &Sz) {
        this->scaleMat = {{Sx, 0, 0, 0},
                          {0, Sy, 0, 0},
                          {0, 0, Sz, 0},
                          {0, 0, 0, 0}};
    }

    void ModelMatrix::getMatrix(mat4<float> *model) {
        *model = this->transformMat * this->RxMat * this->RyMat * this->RzMat * this->scaleMat;
    }

    void ViewMatrix::setAxes(const vec4<float> &right, const vec4<float> &up, const vec4<float> &forward) {
        this->rightSide = right;
        this->upSide = up;
        this->forwardSide = forward;
    }

    void ViewMatrix::setPosition(const vec4<float> &newPos) {
        this->cameraPosition = newPos;
    }

    void ViewMatrix::addToPosition(const vec4<float> &addition, const CoordinateType &type, const bool &substract) {
        switch (type)
        {
        case DENG_X:
            if(substract) this->cameraPosition.x -= addition.x;
            else this->cameraPosition.x += addition.x;
            break;

        case DENG_Y:
            if(substract) this->cameraPosition.y -= addition.y;
            else this->cameraPosition.y += addition.y;
            break;

        case DENG_Z:
            if(substract) this->cameraPosition.z -= addition.z;
            else this->cameraPosition.z += addition.z;
            break;
        
        default:
            break;
        }
    }

    void ViewMatrix::getViewMatrix(mat4<float> *view) {
        view->row1 = this->rightSide;
        view->row2 = this->upSide;
        view->row3 = this->forwardSide;
        view->row4 = this->cameraPosition;
    }

    ProjectionMatrix::ProjectionMatrix(const float &FOV, const float &near, const float &far, const float &aspect_ratio) {
        this->m_FOV = FOV;
        this->m_near = near;
        this->m_far = far;
        this->m_aspectRatio = aspect_ratio;

        this->m_top = (this->m_near * tan(PI/180 * this->m_FOV/2));
        this->m_bottom = -(this->m_near * tan(PI/180 * this->m_FOV/2));
        this->m_right = (this->m_near * tan(PI/180 * this->m_FOV/2) * this->m_aspectRatio);
        this->m_left = -(this->m_near * tan(PI/180 * this->m_FOV/2) * this->m_aspectRatio);

    }

    void ProjectionMatrix::getProjectionMatrix(mat4<float> *matrix) {
        matrix->row1 = {2*this->m_near/(this->m_right - this->m_left), 0, (this->m_right + this->m_left) / (this->m_right - this->m_left), 0};
        matrix->row2 = {0, 2*this->m_near/(this->m_top - this->m_bottom), (this->m_top + this->m_bottom) / (this->m_top - this->m_bottom), 0};
        matrix->row3 = {0, 0, -(this->m_far + this->m_near) / (this->m_far = this->m_near), -(2*this->m_far*this->m_near) / (this->m_far - this->m_near)};
        matrix->row4 = {0, 0, -1, 0};
    }
}  