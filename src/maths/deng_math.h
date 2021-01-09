#ifndef DENG_MATH_H
#define DENG_MATH_H

#define PI 3.1415926f

namespace dengMath {
    
    /* vector and matrix structures */
    
    template<typename T>
    struct vec4 {
        T first, second, third, fourth;
        size_t size() {
            return static_cast<size_t>(4);
        }

        T *getVectorElement(const size_t &i) {
            T *current_element;
            if(i < this->size())
                current_element = &this->first + i;

            else 
                MATH_ERR("index is out of vector bounds");

            return current_element;
        }

        void add(const T &element, const size_t &index) {
            *this->getVectorElement(index) = element;
        }

        template<typename EndCast>
        vec4<EndCast> vecCast() {
            vec4<EndCast> end_cast;
            end_cast.first = static_cast<EndCast>(this->first);
            end_cast.second = static_cast<EndCast>(this->second);
            end_cast.third = static_cast<EndCast>(this->third);
            end_cast.fourth = static_cast<EndCast>(this->fourth);
            return end_cast;
        }

        vec4<T> operator+(const vec4<T> &vector) {
            vec4<T> vec4;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                vec4 = {(this->first + vector.first), (this->second + vector.second), (this->third + vector.third), (this->fourth + vector.fourth)};
            }

            return vec4;
        }

        vec4<T> operator-(const vec4<T> &vector) {
            vec4<T> vec4;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                vec4 = {(this->first - vector.first), (this->second - vector.second), (this->third - vector.third), (this->fourth - vector.fourth)};
            }

            return vec4;
        }

        bool operator<(vec4<T> vector) { return this->first < vector.first && this->second < vector.second && this->third < vector.third && this->fourth < vector.fourth; }
        bool operator>(vec4<T> vector) { return this->first > vector.first && this->second > vector.second && this->third > vector.third && this->fourth > vector.fourth; }
        bool operator==(vec4<T> vector) { return this->first == vector.first && this->second == vector.second && this->third == vector.third && this->fourth == vector.fourth; }

        T *data() { return &this->first; }
    };

    template<typename T>
    struct vec3 {
        T first, second, third;
        size_t size() {
            return static_cast<size_t>(3);
        }

        void fit();

        T *getVectorElement(const size_t &i) {
            T *current_element;
            if(i < this->size())
                current_element = &this->first + i;

            else 
                MATH_ERR("index is out of vector bounds");

            return current_element;
        }

        void add(const T &element, const size_t &index) {
            *this->getVectorElement(index) = element;
        }

        template<typename EndCast>
        vec3<EndCast> vecCast() {
            vec3<EndCast> end_cast;
            end_cast.first = static_cast<EndCast>(this->first);
            end_cast.second = static_cast<EndCast>(this->second);
            end_cast.third = static_cast<EndCast>(this->third);
            return end_cast;
        }

        vec3<T> operator+(const vec3<T> &vector) {
            vec3<T> vec3;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                vec3 = {(first + vector.first), (second + vector.second), (third + vector.third)};
            }

            return vec3;
        }

        vec3<T> operator-(const vec3<T> &vector) {
            vec3<T> vec3;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                vec3 = {(first - vector.first), (second - vector.second), (third - vector.third)};
            }

            return vec3;
        }

        vec4<T> operator>>(vec4<T> vector) {
            vector = {first, second, third, vector.fourth};
            return vector;
        }

        bool operator<(vec3<T> vector) { return first < vector.first && second < vector.second && third < vector.third; }
        bool operator>(vec3<T> vector) { return first > vector.first && second > vector.second && third > vector.third; }
        bool operator==(vec3<T> vector) { return this->first == vector.first && this->second == vector.second && this->third == vector.third; }

        T *data() { return &this->first; }
    };

    template<typename T>
    struct vec2 {
        T first, second;
        size_t size() {
            return static_cast<size_t>(2);
        }

        T *getVectorElement(const size_t &i) {
            T *current_element;
            if(i < this->size())
                current_element = &this->first + i;

            else 
                MATH_ERR("index is out of vector bounds");

            return current_element;
        }

        void add(const T &element, const size_t &index) {
            *this->getVectorElement(index) = element;
        }

        template<typename EndCast>
        vec2<EndCast> vecCast() {
            vec2<EndCast> end_cast;
            end_cast.first = static_cast<EndCast>(this->first);
            end_cast.second = static_cast<EndCast>(this->second);
            return end_cast;
        }

        vec2<T> operator+(const vec2<T> &vector) {
            vec2<T> vec2;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                vec2 = {(first + vector.first), (second + vector.second)};
            }

            return vec2;
        }

        vec2<T> operator-(vec2<T> vector) {
            vec2<T> vec2;
            if
            (
                typeid(T).name() == typeid(float).name() || 
                typeid(T).name() == typeid(uint8_t).name() || 
                typeid(T).name() == typeid(uint16_t).name() || 
                typeid(T).name() == typeid(uint32_t).name() || 
                typeid(T).name() == typeid(uint64_t).name() || 
                typeid(T).name() == typeid(int).name() || 
                typeid(T).name() == typeid(double).name()
            ) vec2 = {(first - vector.first), (second - vector.second)};
            
            return vec2;
        }
        
        vec3<T> operator>>(vec3<T> vector) {
            vector = {first, second, vector.third};
            return vector;
        }

        vec4<T> operator>>(vec4<T> vector) {
            vector = {first, second, vector.third, vector.fourth};
            return vector;
        }

        bool operator<(vec2<T> vector) { return first < vector.first && second < vector.second; }
        bool operator>(vec2<T> vector) { return first > vector.first && second > vector.second; }
        bool operator==(vec2<T> vector) { return this->first == vector.first && this->second == vector.second; }
        T *data() { return &this->first; }
    };

    template<typename T>
    struct mat2 {
        vec2<T> row1;
        vec2<T> row2;
        T *data() { return &this->row1.first; }
    };
    
    template<typename T>
    struct mat3x2 {
        vec2<T> row1;
        vec2<T> row2;
        vec2<T> row3;

        T *data() { return &this->row1.first; }
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

        T *data() { return &this->row1.first; }
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

        // theoretically this operation would be impossible, because of the length of vector but in our case we can just add w value of 1.0f to the end
        vec4<T> operator*(const vec3<T> vector) {
            vec4<T> newVec;
            newVec.first = {(row1.first * vector.first) + (row1.second * vector.second) + (row1.third * vector.third) + row1.fourth};
            newVec.second = {(row2.first * vector.first) + (row2.second * vector.second) + (row2.third * vector.third) + row2.fourth};
            newVec.third = {(row3.first * vector.first) + (row3.second * vector.second) + (row3.third * vector.third) + row3.fourth};
            newVec.fourth = {(row4.first * vector.first) + (row4.second * vector.second) + (row4.third * vector.third) + row4.fourth};

            return newVec;
        }

        T *data() { return &this->row1.first; }
    };

    class ModelMatrix {
    private:
        mat4<float> m_rot_x_mat;
        mat4<float> m_rot_y_mat;
        mat4<float> m_rot_z_mat;

        mat4<float> m_transformation_mat;
        mat4<float> m_scale_mat;

    public:
        ModelMatrix();

        void setRotation(const float &x_rot, const float &y_rot, const float &z_rot);
        void setTransformation(const float &transform_x, const float &transform_y, const float &transform_z);
        void setScale(const float &scale_x, const float &scale_y, const float &scale_z);
        mat4<float> getModelMatrix() { 
            return m_transformation_mat * m_rot_x_mat * m_rot_y_mat * m_rot_z_mat * m_scale_mat;
        }
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
        void addToPosition(const vec4<float> &movement_speed, const deng_CoordinateAxisType &movement_type, const bool &substract);

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
        ProjectionMatrix(const float &FOV, const float &near_plane, const float &far_plane, const float &aspect_ratio);
        void getProjectionMatrix(mat4<float> *p_matrix);
    };

    struct UniformData {
        mat4<float> view;
        mat4<float> projection;
        deng_CameraUniformFlagBits cam_flag_bits;
    };

    // generic math functions
    double exp(double base, int exp);
    float exp(float base, int exp);

    vec2<float> getCartesianCoordsPoint(const vec2<float> &centre, const int16_t &angle, const float &distance, const bool &inverted_y_axis);
    float getFractionNumerator(const float &value_numerator, const float &value_denominator, const float &equivalent_denominator);
    float getVectorLengthFromBounds(vec2<vec2<float>> vector_bounds);
    float getTriangleAnglesFromEdges(const vec3<float> &triangle_edges, const deng_TriangleAngleType &triangle_angle_type);
    float getVector2DRotation(vec2<vec2<float>> vector_bounds);

    struct Conversion {
        static float degToRad(const float &deg);
        static float radToDeg(const float &rad);
        static double vector2DSizeToPixelSize(const double &vec_size, const vec2<uint32_t> &window_size, const deng_CoordinateAxisType &axis_type);
        static float pixelSizeToVector2DSize(const double &pixel_size, const vec2<uint32_t> &window_size, const deng_CoordinateAxisType &axis_type);

        std::unordered_map<char, int> hex_definitions;
        
        Conversion() {
            this->hex_definitions['0'] = 0;
            this->hex_definitions['1'] = 1;
            this->hex_definitions['2'] = 2;
            this->hex_definitions['3'] = 3;
            this->hex_definitions['4'] = 4;
            this->hex_definitions['5'] = 5;
            this->hex_definitions['6'] = 6;
            this->hex_definitions['7'] = 7;
            this->hex_definitions['8'] = 8;
            this->hex_definitions['9'] = 9;
            this->hex_definitions['a'] = 10;
            this->hex_definitions['A'] = 10;
            this->hex_definitions['b'] = 11;
            this->hex_definitions['B'] = 11;
            this->hex_definitions['c'] = 12;
            this->hex_definitions['C'] = 12;
            this->hex_definitions['d'] = 13;
            this->hex_definitions['D'] = 13;
            this->hex_definitions['e'] = 14;
            this->hex_definitions['E'] = 14;
            this->hex_definitions['f'] = 15;
            this->hex_definitions['F'] = 15;
        }

        uint32_t hexToDec(const std::string &hex_value);
    };

    // data handling methods 
    template<typename T>
    static T getSmallestElement(std::vector<T> *p_elements_vector);
    template<typename T>
    static T getLargestElement(std::vector<T> *p_elements_vector);

    template<typename T>
    static void sortInGrowingOrder(std::vector<T> *p_elements_vector);
    template<typename T>
    static void sortInDecliningOrder(std::vector<T> *p_elements_vector);

    template<typename T>
    static void sortVectorInGrowingOrder(std::vector<T> *p_elements_vector, deng_CoordinateAxisType coord_axis_type);
    template<typename T>
    static void sortVectorInDecliningOrder(std::vector<T> *p_elements_vector, deng_CoordinateAxisType coord_axis_type);

    // in order to use this function the class used in std::vector must have const char *description as a public class variable
    template<typename T>
    static T *findElementByDescription(std::vector<T> *p_vector, const char *description);

    // Calculate new asset vertices according to model matrix
    void applyModelMatrix(deng_Asset &asset, mat4<float> matrix);

    // Generic trigonometric calculation functions
    float trSurface2D(std::array<vec2<float>, 3> tr_verts);
}

#endif