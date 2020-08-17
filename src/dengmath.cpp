#include "headers/renderer.h"

namespace deng {
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


    void CameraMath::getMaxCoords(const float &currentRotationXY, const float &FOV, const float &draw_distance, vec2<float> *camera_max_centre_coords, vec2<float> *camera_max_left_corner_coord, vec2<float> *camera_max_right_corner_coord) {
        camera_max_centre_coords->x = sin(degToRad(currentRotationXY)) * draw_distance, camera_max_centre_coords->y = cos(degToRad(currentRotationXY)) * draw_distance;
        camera_max_left_corner_coord->x = sin(degToRad(currentRotationXY - (FOV/2))) * draw_distance, camera_max_left_corner_coord->y = cos(degToRad(currentRotationXY - (FOV/2))) * draw_distance;  
        camera_max_right_corner_coord->x = sin(degToRad(currentRotationXY + (FOV/2))) * draw_distance, camera_max_left_corner_coord->y = cos(degToRad(currentRotationXY + (FOV/2))) * draw_distance; 
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
        view->row2 = this->forwardSide;
        view->row3 = this->upSide;
        view->row4 = this->cameraPosition;
    }
}  