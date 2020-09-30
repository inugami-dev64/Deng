#ifndef DENG_MATH_H
#define DENG_MATH_H

#define PI 3.1415926f

namespace deng {

    template<typename T>
    struct vec2 {
        T first, second;
        size_t size() {
            return static_cast<size_t>(2);
        }

        T *getVectorElement(const size_t &i) {
            T *local_current_element;
            if(i < this->size())
                local_current_element = &this->first + i;

            else 
                ERR("index is out of vector bounds");

            return local_current_element;
        }

        vec2<T> operator+(const vec2<T> &vector) {
            vec2<T> local_vec2;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                local_vec2 = {(first + vector.first), (second + vector.second)};
            }

            return local_vec2;
        }

        vec2<T> operator-(vec2<T> vector) {
            vec2<T> local_vec2;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                local_vec2 = {(first - vector.first), (second - vector.second)};
            }
            
            return local_vec2;
        }
    };

    template<typename T>
    struct vec3 {
        T first, second, third;
        size_t size() {
            return static_cast<size_t>(3);
        }

        T *getVectorElement(const size_t &i) {
            T *local_current_element;
            if(i < this->size())
                local_current_element = &this->first + i;

            else 
                ERR("index is out of vector bounds");

            return local_current_element;
        }

        vec3<T> operator+(const vec3<T> &vector) {
            vec3<T> local_vec3;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                local_vec3 = {(first + vector.first), (second + vector.second), (third + vector.third)};
            }

            return local_vec3;
        }

        vec3<T> operator-(const vec3<T> &vector) {
            vec3<T> local_vec3;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                local_vec3 = {(first - vector.first), (second - vector.second), (third - vector.third)};
            }

            return local_vec3;
        }
    };
    
    template<typename T>
    struct vec4 {
        T first, second, third, fourth;
        size_t size() {
            return static_cast<size_t>(4);
        }

        T *getVectorElement(const size_t &i) {
            T *local_current_element;
            if(i < this->size())
                local_current_element = &this->first + i;

            else 
                ERR("index is out of vector bounds");

            return local_current_element;
        }

        vec4<T> operator+(const vec4<T> &vector) {
            vec4<T> local_vec4;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                local_vec4 = {(first + vector.first), (second + vector.second), (third + vector.third), (fourth + vector.fourth)};
            }

            return local_vec4;
        }

        vec4<T> operator-(const vec4<T> &vector) {
            vec4<T> local_vec4;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                local_vec4 = {(first - vector.first), (second - vector.second), (third - vector.third), (fourth - vector.fourth)};
            }

            return local_vec4;
        }
    };

    template<typename T>
    struct mat2 {
        vec2<T> row1;
        vec2<T> row2;
    };
    
    template<typename T>
    struct mat3x2 {
        vec2<T> row1;
        vec2<T> row2;
        vec2<T> row3;
    };

    template<typename T>
    struct mat3 {
        vec3<T> row1;
        vec3<T> row2;
        vec3<T> row3;

        mat3<T> operator*(const mat3<T> &matrix) {
            mat3<T> newMat;
            newMat.row1 = {((row1.first * matrix.row1.first) + (row1.second * matrix.row2.first) + (row1.third * matrix.row3.first)), ((row1.first * matrix.row1.second) + (row1.second * matrix.row2.second) + (row1.third * matrix.row3.second)), ((row1.first * matrix.row1.third) + (row1.second * matrix.row2.third) + (row1.third * matrix.row3.third))};
            newMat.row2 = {((row2.first * matrix.row1.first) + (row2.second * matrix.row2.first) + (row2.third * matrix.row3.first)), ((row2.first * matrix.row1.second) + (row2.second * matrix.row2.second) + (row2.third * matrix.row3.second)), ((row2.first * matrix.row1.third) + (row2.second * matrix.row2.third) + (row2.third * matrix.row3.third))};
            newMat.row3 = {((row3.first * matrix.row1.first) + (row3.second * matrix.row2.first) + (row3.third * matrix.row3.first)), ((row3.first * matrix.row1.second) + (row3.second * matrix.row2.second) + (row3.third * matrix.row3.second)), ((row3.first * matrix.row1.third) + (row3.second * matrix.row2.third) + (row3.third * matrix.row3.third))};

            return newMat;
        }
    };

    template<typename T>
    struct mat4 {
        vec4<T> row1;
        vec4<T> row2;
        vec4<T> row3;
        vec4<T> row4; 

        mat4<T> operator*(const mat4<T> &matrix) {
            mat4<T> newMat;
            newMat.row1 = {((row1.first * matrix.row1.first) + (row1.second * matrix.row2.first) + (row1.third * matrix.row3.first) + (row1.fourth * matrix.row4.first)), ((row1.first * matrix.row1.second) + (row1.second * matrix.row2.second) + (row1.third * matrix.row3.second) + (row1.fourth * matrix.row4.second)), ((row1.first * matrix.row1.third) + (row1.second * matrix.row2.third) + (row1.third * matrix.row3.third) + (row1.fourth * matrix.row4.third)), ((row1.first * matrix.row1.fourth) + (row1.second * matrix.row2.fourth) + (row1.third * matrix.row3.fourth) + (row1.fourth * matrix.row4.fourth))};
            newMat.row2 = {((row2.first * matrix.row1.first) + (row2.second * matrix.row2.first) + (row2.third * matrix.row3.first) + (row2.fourth * matrix.row4.first)), ((row2.first * matrix.row1.second) + (row2.second * matrix.row2.second) + (row2.third * matrix.row3.second) + (row2.fourth * matrix.row4.second)), ((row2.first * matrix.row1.third) + (row2.second * matrix.row2.third) + (row2.third * matrix.row3.third) + (row2.fourth * matrix.row4.third)), ((row2.first * matrix.row1.fourth) + (row2.second * matrix.row2.fourth) + (row2.third * matrix.row3.fourth) + (row2.fourth * matrix.row4.fourth))};
            newMat.row3 = {((row3.first * matrix.row1.first) + (row3.second * matrix.row2.first) + (row3.third * matrix.row3.first) + (row3.fourth * matrix.row4.first)), ((row3.first * matrix.row1.second) + (row3.second * matrix.row2.second) + (row3.third * matrix.row3.second) + (row3.fourth * matrix.row4.second)), ((row3.first * matrix.row1.third) + (row3.second * matrix.row2.third) + (row3.third * matrix.row3.third) + (row3.fourth * matrix.row4.third)), ((row3.first * matrix.row1.fourth) + (row3.second * matrix.row2.fourth) + (row3.third * matrix.row3.fourth) + (row3.fourth * matrix.row4.fourth))};
            newMat.row4 = {((row4.first * matrix.row1.first) + (row4.second * matrix.row2.first) + (row4.third * matrix.row3.first) + (row4.fourth * matrix.row4.first)), ((row4.first * matrix.row1.second) + (row4.second * matrix.row2.second) + (row4.third * matrix.row3.second) + (row4.fourth * matrix.row4.second)), ((row4.first * matrix.row1.third) + (row4.second * matrix.row2.third) + (row4.third * matrix.row3.third) + (row4.fourth * matrix.row4.third)), ((row4.first * matrix.row1.fourth) + (row4.second * matrix.row2.fourth) + (row4.third * matrix.row3.fourth) + (row4.fourth * matrix.row4.fourth))};

            return newMat;
        }

            // theoretically this operation would be impossible, because of the lenght of vector but in our case we can just add w value of 1.0f to the end
        vec4<T> operator*(const vec3<T> vector) {
            vec4<T> newVec;
            newVec.first = {(row1.first * vector.first) + (row1.second * vector.second) + (row1.third * vector.third) + row1.fourth};
            newVec.second = {(row2.first * vector.first) + (row2.second * vector.second) + (row2.third * vector.third) + row2.fourth};
            newVec.third = {(row3.first * vector.first) + (row3.second * vector.second) + (row3.third * vector.third) + row3.fourth};
            newVec.fourth = {(row4.first * vector.first) + (row4.second * vector.second) + (row4.third * vector.third) + row4.fourth};

            return newVec;
        }
    };

    class ModelMatrix {
    private:
        mat4<float> m_rot_x_mat;
        mat4<float> m_rot_y_mat;
        mat4<float> m_rot_z_mat;

        mat4<float> m_transformation_mat;
        mat4<float> m_scale_mat;

    public:
        void setRotation(const float &x_rot, const float &y_rot, const float &z_rot);
        void setTransformation(const float &transform_x, const float &transform_y, const float &transform_z);
        void setScale(const float &scale_x, const float &scale_y, const float &scale_z);
        void getModelMatrix(mat4<float> *p_model);
    };

    class ViewMatrix {
    private:
        vec4<float> m_camera_position;
        mat4<float> m_transformation_mat;

        float x_rot;
        float y_rot;
        float z_rot;

        vec4<float> m_right_side;
        vec4<float> m_up_side;
        vec4<float> m_forward_side;

        mat4<float> m_rot_x_mat;
        mat4<float> m_rot_y_mat;
        mat4<float> m_rot_z_mat;

    public:
        ViewMatrix();
        void setCameraPosition(const vec4<float> &camera_pos);
        void addToPosition(const vec4<float> &movement_speed, const dengCoordinateAxisType &movement_type, const bool &substract);

        void setRotation(const float &x_rot, const float &y_rot);
        void setTransformationMatrix();
        void getViewMatrix(mat4<float> *view_mat);
        vec4<float> getPosition();
    };

    class ProjectionMatrix {
    private:
        float m_FOV;

        float m_aspect_ratio;
        float m_near;
        float m_far;

    public:
        ProjectionMatrix(const float &FOV, const float &near, const float &far, const float &aspect_ratio);
        void getProjectionMatrix(mat4<float> *p_matrix);
    };

    struct UniformBufferData {
        mat4<float> model;
        mat4<float> view;
        mat4<float> projection;
    };

    // generic math functions
    struct Math {
        static float degToRad(const float &deg);
        static float radToDeg(const float &rad);
        static vec2<float> getCartesianCoordsPoint(const vec2<float> &centre, const int16_t &angle, const float &distance, const dengBool &inverted_y_axis);
        static float getFractionNumerator(const float &value_numerator, const float &value_denominator, const float &equivalent_denominator);
        static float getVectorLengthFromBounds(vec2<deng::vec2<float>> vector_bounds);
        static float getTriangleAnglesFromEdges(const vec3<float> &triangle_edges, const dengTriangleAngleType &triangle_angle_type);
        static float getVector2DRotation(vec2<vec2<float>> vector_bounds);
    };

    // data sorting algorithms 
    struct HandleData {
        template<typename T>
        static T getSmallestElement(std::vector<T> *p_elements_vector);
        template<typename T>
        static T getLargestElement(std::vector<T> *p_elements_vector);

        template<typename T>
        static void sortInGrowingOrder(std::vector<T> *p_elements_vector);
        template<typename T>
        static void sortInDecliningOrder(std::vector<T> *p_elements_vector);

        template<typename T>
        static void sortVectorInGrowingOrder(std::vector<T> *p_elements_vector, dengCoordinateAxisType coord_axis_type);
        template<typename T>
        static void sortVectorInDecliningOrder(std::vector<T> *p_elements_vector, dengCoordinateAxisType coord_axis_type);

        // in order to use this function the class used in std::vector must have const char *description as a public class variable
        template<typename T>
        static T *findElementByDescription(std::vector<T> *p_vector, const size_t &size, const char *description);
    };

    template<typename T>
    T HandleData::getSmallestElement(std::vector<T> *p_elements_vector) {
        T local_smallest_element = (*p_elements_vector)[0];
        for(size_t i = 0; i < p_elements_vector->size(); i++)
            if(local_smallest_element > (*p_elements_vector)[i]) local_smallest_element = (*p_elements_vector)[i];
    
        return local_smallest_element;
    }

    template<typename T>
    T HandleData::getLargestElement(std::vector<T> *p_elements_vector) {
        T local_largest_element = (*p_elements_vector)[0];
        for(size_t i = 0; i < p_elements_vector->size(); i++)
            if(local_largest_element < (*p_elements_vector)[i]) local_largest_element = (*p_elements_vector)[i];
    
        return local_largest_element;
    }

    template<typename T>
    void HandleData::sortInGrowingOrder(std::vector<T> *p_elements_vector) {
        vec2<T> local_sorting_buffer;

        for(size_t i = 0; i < p_elements_vector->size() - 1; i++) {
            if((*p_elements_vector)[i] > (*p_elements_vector)[i + 1]) {
                local_sorting_buffer.second = (*p_elements_vector)[i];
                local_sorting_buffer.first = (*p_elements_vector)[i + 1];

                (*p_elements_vector)[i] = local_sorting_buffer.first;
                (*p_elements_vector)[i + 1] = local_sorting_buffer.second;
            }
        }
    }

    template<typename T>
    void HandleData::sortInDecliningOrder(std::vector<T> *p_elements_vector) {
        vec2<T> local_sorting_buffer;

        for(size_t i = 0; i < p_elements_vector->size() - 1; i++) {
            if((*p_elements_vector)[i] < (*p_elements_vector)[i + 1]) {
                local_sorting_buffer.second = (*p_elements_vector)[i];
                local_sorting_buffer.first = (*p_elements_vector)[i + 1];

                (*p_elements_vector)[i] = local_sorting_buffer.first;
                (*p_elements_vector)[i + 1] = local_sorting_buffer.second;
            }
        }
    }

    template<typename T>
    void HandleData::sortVectorInGrowingOrder(std::vector<T> *p_elements_vector, dengCoordinateAxisType coord_axis_type) {
        vec2<T> local_sorting_buffer;

        for(size_t i = 0; i < p_elements_vector->size() - 1; i++) {
            switch (coord_axis_type)
            {
            case DENG_X:
                if((*p_elements_vector)[i].position_vec.first > (*p_elements_vector)[i + 1].position_vec.first) {
                    local_sorting_buffer.second = (*p_elements_vector)[i];
                    local_sorting_buffer.first = (*p_elements_vector)[i + 1];

                    (*p_elements_vector)[i] = local_sorting_buffer.first;
                    (*p_elements_vector)[i + 1] = local_sorting_buffer.second;
                }
                break;

            case DENG_Y:
                if((*p_elements_vector)[i].position_vec.second > (*p_elements_vector)[i + 1].position_vec.second) {
                    local_sorting_buffer.second = (*p_elements_vector)[i];
                    local_sorting_buffer.first = (*p_elements_vector)[i + 1];

                    (*p_elements_vector)[i] = local_sorting_buffer.first;
                    (*p_elements_vector)[i + 1] = local_sorting_buffer.second;
                }
                break;

            case DENG_Z:
                if((*p_elements_vector)[i].position_vec.third > (*p_elements_vector)[i + 1].position_vec.third) {
                    local_sorting_buffer.second = (*p_elements_vector)[i];
                    local_sorting_buffer.first = (*p_elements_vector)[i + 1];

                    (*p_elements_vector)[i] = local_sorting_buffer.first;
                    (*p_elements_vector)[i + 1] = local_sorting_buffer.second;
                }
                break;
            
            default:
                break;
            }
        }
    }

    template<typename T>
    void HandleData::sortVectorInDecliningOrder(std::vector<T> *p_elements_vector, dengCoordinateAxisType coord_axis_type) {
        vec2<T> local_sorting_buffer;

        for(size_t i = 0; i < p_elements_vector->size() - 1; i++) {
            switch (coord_axis_type)
            {
            case DENG_X:
                if((*p_elements_vector)[i].position_vec.first < (*p_elements_vector)[i + 1].position_vec.first) {
                    local_sorting_buffer.second = (*p_elements_vector)[i];
                    local_sorting_buffer.first = (*p_elements_vector)[i + 1];

                    (*p_elements_vector)[i] = local_sorting_buffer.first;
                    (*p_elements_vector)[i + 1] = local_sorting_buffer.second;
                }
                break;

            case DENG_Y:
                if((*p_elements_vector)[i].position_vec.second < (*p_elements_vector)[i + 1].position_vec.second) {
                    local_sorting_buffer.second = (*p_elements_vector)[i];
                    local_sorting_buffer.first = (*p_elements_vector)[i + 1];

                    (*p_elements_vector)[i] = local_sorting_buffer.first;
                    (*p_elements_vector)[i + 1] = local_sorting_buffer.second;
                }
                break;

            case DENG_Z:
                if((*p_elements_vector)[i].position_vec.third < (*p_elements_vector)[i + 1].position_vec.third) {
                    local_sorting_buffer.second = (*p_elements_vector)[i];
                    local_sorting_buffer.first = (*p_elements_vector)[i + 1];

                    (*p_elements_vector)[i] = local_sorting_buffer.first;
                    (*p_elements_vector)[i + 1] = local_sorting_buffer.second;
                }
                break;
            
            default:
                break;
            }
        }
    }

    template<typename T>
    T *HandleData::findElementByDescription(std::vector<T> *p_elements, const size_t &size, const char *description) {
        for(size_t i = 0; i < p_elements->size(); i++) {
            if((*p_elements)[i].description == description) {
                return &p_elements->at(i);
            } 
        }
        return &p_elements->at(0);
    }
}

#endif