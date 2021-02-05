#include <map>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <math.h>

#include "../../headers/common/base_types.h"
#include "../../headers/das/assets.h"
#include "../../headers/deng/err_def.h"
#include "../../headers/deng/deng_math.h"

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
    deng_vec_t exp(deng_vec_t base, int exp) {
        if(exp == 0) return 1.0;
        deng_vec_t current_value = base;

        for(int i = 0; i < exp - 1; i++) {
            current_value *= base;
        }

        if(exp < 0) return 1.0 / current_value;
        else return current_value;
    }

    vec2<deng_vec_t> getCartesianCoordsPoint (
        const vec2<deng_vec_t> &centre_position, 
        const deng_vec_t &angle, 
        const deng_vec_t &distance, 
        const deng_bool_t &inverted_y_axis
    ) {
        vec2<deng_vec_t> coords;
        coords.first = (sin(angle) * distance) + centre_position.first;
        if(!inverted_y_axis) coords.second = (cos(angle) * distance) + centre_position.second;
        else coords.second = -(cos(angle) * distance) + centre_position.second;

        return coords; 
    }

    deng_vec_t getFractionNumerator (
        const deng_vec_t &value_numerator, 
        const deng_vec_t &value_denominator, 
        const deng_vec_t &equivalent_denominator
    ) {
        return (value_numerator * equivalent_denominator) / value_denominator;
    }

    deng_vec_t getVectorLengthFromBounds(vec2<vec2<deng_vec_t>> vector_bounds) {
        vec2<deng_vec_t> vector_coordinates = vector_bounds.second - vector_bounds.first;
        return static_cast<deng_vec_t>(sqrt(pow(vector_coordinates.first, 2) + pow(vector_coordinates.second, 2)));
    }

    deng_vec_t getTriangleAnglesFromEdges(const vec3<deng_vec_t> &triangle_edges, const deng_TriangleAngleType &triangle_angle_type) {
        switch (triangle_angle_type)
        {
        case DENG_TRIANGLE_ANGLE_ALPHA:
            return Conversion::radToDeg(static_cast<deng_vec_t>(asin(sqrt((triangle_edges.first + triangle_edges.second - triangle_edges.third) * (triangle_edges.first - triangle_edges.second + triangle_edges.third) * (-triangle_edges.first + triangle_edges.second + triangle_edges.third) * 
            (triangle_edges.first + triangle_edges.second + triangle_edges.third)) / 2 * triangle_edges.second * triangle_edges.third)));
            break;
        
        case DENG_TRIANGLE_ANGLE_BETA:
            return Conversion::radToDeg(static_cast<deng_vec_t>(asin(sqrt((triangle_edges.first + triangle_edges.second - triangle_edges.third) * (triangle_edges.first - triangle_edges.second + triangle_edges.third) * (-triangle_edges.first + triangle_edges.second + triangle_edges.third) * 
            (triangle_edges.first + triangle_edges.second + triangle_edges.third)) / 2 * triangle_edges.first * triangle_edges.third)));
            break;

        case DENG_TRIANGLE_ANGLE_GAMMA:
            return Conversion::radToDeg(static_cast<deng_vec_t>(asin(sqrt((triangle_edges.first + triangle_edges.second - triangle_edges.third) * (triangle_edges.first - triangle_edges.second + triangle_edges.third) * (-triangle_edges.first + triangle_edges.second + triangle_edges.third) * 
            (triangle_edges.first + triangle_edges.second + triangle_edges.third)) / 2 * triangle_edges.first * triangle_edges.second)));
            break;

        default:
            break;
        }

        return 0.0f;
    }

    deng_vec_t getVector2DRotation(vec2<vec2<deng_vec_t>> vector_bounds) {
        vec2<deng_vec_t> vector_position = vector_bounds.second - vector_bounds.first;
        deng_vec_t vector_length = getVectorLengthFromBounds(vector_bounds);
        
        // first - rotation calculated from sin, second - rotation calculated from cos
        vec2<deng_vec_t> rotations;
        rotations.first = static_cast<deng_vec_t>(asin(static_cast<double>(vector_position.first) / static_cast<double>(vector_length)));
        rotations.second = static_cast<deng_vec_t>(acos(-static_cast<double>(vector_position.second) / static_cast<double>(vector_length)));
        
        LOG("sin_rot: " + std::to_string(Conversion::radToDeg(rotations.first)) + "/cos_rot: " + std::to_string(Conversion::radToDeg(rotations.second)));

        if(rotations.first >= 0.0f) return static_cast<deng_vec_t>(round(Conversion::radToDeg(static_cast<deng_vec_t>(fabs(rotations.first)))));
        
        if(rotations.first < 0 && Conversion::radToDeg(rotations.second) >= 0 && Conversion::radToDeg(rotations.second) < 90.0f) {
            LOG("Condition rotations.first < 0 && (rotations.second >= 0 && rotations.second < 90) fulfilled!");
            return (180.0f + static_cast<deng_vec_t>(round(Conversion::radToDeg(static_cast<deng_vec_t>(fabs(rotations.first))))));
        }

        else if(rotations.first < 0 && Conversion::radToDeg(rotations.second) >= 90 && Conversion::radToDeg(rotations.second) < 180.0f) {
            LOG("Condition rotations.first < 0 && (rotations.second >= 90 && rotations.second < 180) fulfilled!");
            return (270.0f + static_cast<deng_vec_t>(round(Conversion::radToDeg(static_cast<deng_vec_t>(fabs(rotations.first))))));
        }

        return 0.0f;
    }


    /* Generic conversion methods */
    deng_vec_t Conversion::degToRad(const deng_vec_t &deg) {
        return static_cast<deng_vec_t>(deg/180 * PI);
    }

    deng_vec_t Conversion::radToDeg(const deng_vec_t &rad) {
        return static_cast<deng_vec_t>(rad/(2 * PI) * 360);
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
            return ((2.0f / static_cast<deng_vec_t>(window_size.first)) * static_cast<deng_vec_t>(pixel_size));
            break;
        
        case DENG_COORD_AXIS_Y:
            return ((2.0f / static_cast<deng_vec_t>(window_size.second)) * static_cast<deng_vec_t>(pixel_size));
            break;

        case DENG_COORD_AXIS_UNDEFINED: {
            deng_ui32_t avg_size = (window_size.first + window_size.second) / 2;
            return ((2.0f / static_cast<deng_vec_t>(avg_size)) * static_cast<deng_vec_t>(pixel_size));   
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

        default:
            break;
        }
    }

    void ViewMatrix::setCameraPosition(const vec4<deng_vec_t> &new_position) {
        m_camera_position = new_position;
    }

    void ViewMatrix::addToPosition (
        const vec4<deng_vec_t> &movement_speed, 
        const deng_CoordinateAxisType &movement_type, 
        const deng_bool_t &substract
    ) {
        switch (movement_type)
        {
        case DENG_COORD_AXIS_X: {
            if(substract) {
                m_camera_position.first -= movement_speed.first * (m_rot_x_mat * m_right_side).first; 
                m_camera_position.second -= movement_speed.second * (m_rot_x_mat * m_right_side).second;
                m_camera_position.third -= movement_speed.third * (m_rot_x_mat * m_right_side).third;
            }
            else {
                m_camera_position.first += movement_speed.first * (m_rot_x_mat * m_right_side).first; 
                m_camera_position.second += movement_speed.second * (m_rot_x_mat * m_right_side).second;
                m_camera_position.third += movement_speed.third * (m_rot_x_mat * m_right_side).third;
            }

            break;
        }

        case DENG_COORD_AXIS_Y:
            if(substract) m_camera_position.second -= movement_speed.second;
            else m_camera_position.second += movement_speed.second;
            break;

        case DENG_COORD_AXIS_Z: {
            vec2<deng_vec_t> movement = getCartesianCoordsPoint (
                {0.0f, 0.0f}, 
                m_y_rot, 
                movement_speed.third, 
                false
            );
            
            if(substract) {
                m_camera_position.first -= movement.first; 
                m_camera_position.third -= movement.second;
            }
            else {
                m_camera_position.first += movement.first; 
                m_camera_position.third += movement.second;
            }

            break;
        }
        
        default:
            break;
        }

        #if CAMERA_LOCATION_DEBUG
            LOG (
                "x: " + 
                std::to_string(m_camera_position.first) + 
                "/y: " + 
                std::to_string(m_camera_position.second) + 
                "/z: " + 
                std::to_string(m_camera_position.third)
            );
        #endif
    }

    void ViewMatrix::getViewMatrix(mat4<deng_vec_t> *p_view) {
        *p_view = m_transformation_mat;
    }

    void ViewMatrix::setTransformationMatrix() {
        m_transformation_mat.row1 = m_rot_x_mat * m_right_side;
        m_transformation_mat.row2 = DENG_FFP_CAMERA_UP_SIDE;
        m_transformation_mat.row3 = m_rot_x_mat * m_forward_side;
        m_transformation_mat.row4 = m_rot_x_mat * m_camera_position;
        
        m_up_side = m_rot_x_mat * m_rot_y_mat * m_transformation_mat *  DENG_FFP_CAMERA_UP_SIDE;
        m_transformation_mat.row2 = m_up_side;
/*
 *        m_transformation_mat.row1.first = m_right_side.first;
 *        m_transformation_mat.row2.first = m_right_side.second;
 *        m_transformation_mat.row3.first = m_right_side.third;
 *        m_transformation_mat.row4.first = m_right_side.fourth;
 *
 *        m_transformation_mat.row1.second = m_up_side.first;
 *        m_transformation_mat.row2.second = m_up_side.second;
 *        m_transformation_mat.row3.second = m_up_side.third;
 *        m_transformation_mat.row4.second = m_up_side.fourth;
 *
 *        m_transformation_mat.row1.third = m_forward_side.first;
 *        m_transformation_mat.row2.third = m_forward_side.second;
 *        m_transformation_mat.row3.third = m_forward_side.third;
 *        m_transformation_mat.row4.third = m_forward_side.fourth;
 *
 *        m_transformation_mat.row1.fourth = m_camera_position.first;
 *        m_transformation_mat.row2.fourth = m_camera_position.second;
 *        m_transformation_mat.row3.fourth = m_camera_position.third;
 *        m_transformation_mat.row4.fourth = m_camera_position.fourth;
 */
    }

    vec4<deng_vec_t> ViewMatrix::getPosition() {
        return m_camera_position;
    }


    /* Set camera rotation in radians */
    void ViewMatrix::setRotation (
        deng_vec_t x_rot, 
        deng_vec_t y_rot
    ) {
        deng_vec_t delta_x_rot = x_rot - m_x_rot;
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
        m_delta_rot_x = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, (deng_vec_t) cos(delta_x_rot), (deng_vec_t) -sin(delta_x_rot), 0.0f},
            {0.0f, (deng_vec_t) sin(delta_x_rot), (deng_vec_t) cos(delta_x_rot), 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };

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
        matrix->row1 = {m_aspect_ratio * static_cast<deng_vec_t>(1/tan(Conversion::degToRad(m_FOV/2))), 0, 0, 0};
        matrix->row2 = {0, static_cast<deng_vec_t>(1/tan(Conversion::degToRad(m_FOV/2))), 0, 0};
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
    void applyModelMatrix(deng_Asset &asset, mat4<deng_vec_t> matrix) {
        size_t index;
        dengMath::vec3<deng_vec_t> *p_tmp_in;
        dengMath::vec4<deng_vec_t> tmp_out;

        for(index = 0; index < asset.vertices.size; index++) {
            switch (asset.asset_mode)
            {
            case DENG_ASSET_MODE_3D_TEXTURE_MAPPED:
                p_tmp_in = (dengMath::vec3<deng_vec_t>*) &asset.vertices.p_tex_mapped_unnormalized_vert[index].vert_data;
                tmp_out = matrix * (*p_tmp_in);
                asset.vertices.p_tex_mapped_unnormalized_vert[index].vert_data.vert_x = tmp_out.first;
                asset.vertices.p_tex_mapped_unnormalized_vert[index].vert_data.vert_y = tmp_out.second;
                asset.vertices.p_tex_mapped_unnormalized_vert[index].vert_data.vert_z = tmp_out.third;
                break;

            case DENG_ASSET_MODE_3D_TEXTURE_MAPPED_NORMALISED:
                p_tmp_in = (dengMath::vec3<deng_vec_t>*) &asset.vertices.p_tex_mapped_normalized_vert[index].vert_data;
                tmp_out = matrix * (*p_tmp_in);
                asset.vertices.p_tex_mapped_normalized_vert[index].vert_data.vert_x = tmp_out.first;
                asset.vertices.p_tex_mapped_normalized_vert[index].vert_data.vert_y = tmp_out.second;
                asset.vertices.p_tex_mapped_normalized_vert[index].vert_data.vert_z = tmp_out.third;
                break;

            case DENG_ASSET_MODE_3D_UNMAPPED:
                p_tmp_in = (dengMath::vec3<deng_vec_t>*) &asset.vertices.p_unmapped_unnormalized_vert[index]; 
                tmp_out = matrix * (*p_tmp_in);
                asset.vertices.p_unmapped_unnormalized_vert[index].vert_data.vert_x = tmp_out.first;
                asset.vertices.p_unmapped_unnormalized_vert[index].vert_data.vert_y = tmp_out.second;
                asset.vertices.p_unmapped_unnormalized_vert[index].vert_data.vert_z = tmp_out.third;
                break;

            case DENG_ASSET_MODE_3D_UNMAPPED_NORMALISED:
                p_tmp_in = (dengMath::vec3<deng_vec_t>*) &asset.vertices.p_unmapped_normalized_vert[index]; 
                tmp_out = matrix * (*p_tmp_in);
                asset.vertices.p_unmapped_normalized_vert[index].vert_data.vert_x = tmp_out.first;
                asset.vertices.p_unmapped_normalized_vert[index].vert_data.vert_y = tmp_out.second;
                asset.vertices.p_unmapped_normalized_vert[index].vert_data.vert_z = tmp_out.third;
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
    deng_vec_t trSurface2D (
        std::array<vec2<deng_vec_t>, 3> 
        tr_verts
    ) {
        // Find triangle sides
        deng_vec_t a = 
            sqrt (
                (tr_verts[1].first - tr_verts[0].first) * (tr_verts[1].first - tr_verts[0].first) + 
                (tr_verts[1].second - tr_verts[0].second) * (tr_verts[1].second - tr_verts[0].second)
            );

        deng_vec_t b =
            sqrt (
                (tr_verts[2].first - tr_verts[1].first) * (tr_verts[2].first - tr_verts[1].first) +
                (tr_verts[2].second - tr_verts[1].second) * (tr_verts[2].second - tr_verts[1].second)
            );

        deng_vec_t c =
            sqrt (
                (tr_verts[2].first - tr_verts[0].first) * (tr_verts[2].first - tr_verts[0].first) +
                (tr_verts[2].second - tr_verts[0].second) * (tr_verts[2].second - tr_verts[0].second)
            );

        // Triangle semi perimeter
        deng_vec_t s = (a + b + c) / 2;

        return std::sqrt((s * (s - a) * (s - b) * (s - c)));
    }
}  
