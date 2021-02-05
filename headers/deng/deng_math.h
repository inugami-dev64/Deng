#ifndef DENG_MATH_H
#define DENG_MATH_H
#include <string>
#define PI 3.1415926f

#define DENG_FFP_CAMERA_RIGHT_SIDE      (dengMath::vec4<deng_vec_t>) {1.0f, 0.0f, 0.0f, 0.0f}
#define DENG_FFP_CAMERA_UP_SIDE         (dengMath::vec4<deng_vec_t>) {0.0f, -1.0f, 0.0f, 0.0f}
#define DENG_FFP_CAMERA_FORWARD_SIDE    (dengMath::vec4<deng_vec_t>) {0.0f, 0.0f, -1.0f, 0.0f}

namespace dengMath {
    
    /* vector and matrix structures */
    template<typename T>
    struct vec4 {
        T first, second, third, fourth;

        /******************************/
        /***** Operator overloads *****/
        /******************************/

        vec4<T> operator+(vec4<T> vec) {
            return (vec4<T>) {
                first + vec.first,
                second + vec.second,
                third + vec.third
            };
        }

        vec4<T> operator-(const vec4<T> &vec) {
            return (vec4<T>) {
                first - vec.first,
                second - vec.second,
                third - vec.third
            };
        }

        deng_bool_t operator==(vec4<T> vector) { 
            return (deng_bool_t) {
                first == vector.first && 
                second == vector.second && 
                third == vector.third && 
                fourth == vector.fourth
            }; 
        }

        T length() {
            return (T) sqrt (
                first * first +
                second * second +
                third * third +
                fourth * fourth
            );
        }

        void norm() {
            T len = length();
            first /= len;
            second /= len;
            third /= len;
            fourth /= len;
        }

        T *data() { return &first; }
    };

    template<typename T>
    struct vec3 {
        T first, second, third;
        
        /******************************/
        /***** Operator overloads *****/
        /******************************/

        vec3<T> operator+(vec3<T> vec) {
            return (vec3<T>) {
                first + vec.first,
                second + vec.second,
                third + vec.third
            };
        }

        vec3<T> operator-(vec3<T> vec) {
            return (vec3<T>) {
                first - vec.first,
                second - vec.second,
                third - vec.third
            };
        }

        
        deng_bool_t operator==(vec3<T> vector) { 
            return (deng_bool_t) {
                first == vector.first && 
                second == vector.second && 
                third == vector.third
            }; 
        }

        T length() {
            return (T) sqrt (
                first * first +
                second * second +
                third * third
            );
        }

        void norm() {
            T len = length();
            first /= len;
            second /= len;
            third /= len;
        };

        T *data() { return &first; }
    };

    template<typename T>
    struct vec2 {
        T first, second;

        /******************************/
        /***** Operator overloads *****/
        /******************************/

        vec2<T> operator+(const vec2<T> &vec) {
            return (vec2<T>) {
                first + vec.first, 
                second + vec.second
            };
        }

        vec2<T> operator-(vec2<T> vec) {
            return (vec2<T>) {
                first - vec.first,
                second - vec.second
            };
        }

        deng_bool_t operator==(vec2<T> vector) { 
            return (deng_bool_t) {
                first == vector.first && 
                second == vector.second
            }; 
        }

        T length() {
            return (T) sqrt (
                first * first +
                second * second
            );
        }

        void norm() {
            T len = length();
            first /= len;
            second /= len;
        };

        T *data() { return &first; }
    };

    template<typename T>
    struct mat2 {
        vec2<T> row1;
        vec2<T> row2;
    
        T *data() { return &row1.first; }

        /* Return the determinant of given matrix */
        template<typename DT>
        static DT det(mat2<DT> mat) { 
            return mat.row1.first * mat.row2.second - mat.row1.second * mat.row2.first; 
        }
        /******************************/
        /***** Operator overloads *****/
        /******************************/

        /* Perform matrix multiplication on 2x2 matrix */ 
        mat2<T> operator*(mat2<T> mat) {
            mat2<T> out_mat;
            out_mat.row1 = {
                (
                    (row1.first * mat.row1.first) +
                    (row1.second * mat.row2.first)
                ),
                (
                    (row1.first * mat.row1.second) +
                    (row1.second * mat.row2.second)
                )
            }; 
        }
        
        /* Perform multiplication with constant */
        mat2<T> operator*(const T &c) {
            return (mat2<T>) {
                {row1.first * c, row1.second * c},
                {row2.first * c, row2.second * c}
            };
        }

        /* Peform multiplication with column vector */
        vec2<T> operator*(vec2<T> vec) {
            return (vec2<T>) {
                (row1.first * vec.first + row1.second * vec.second),
                (row2.first * vec.first + row2.second * vec.second)
            };
        }

        /* Perform matrix addition */
        mat2<T> operator+(mat2<T> mat) {
            return (mat2<T>) {
                {row1.first + mat.row1.first, row1.second + mat.row1.second},
                {row2.first + mat.row2.first, row1.second + mat.row2.second}
            };
        }
        
        /* Find the inverse of a matrix */
        mat2<T> inv() {
            mat2<deng_vec_t> out_mat;
            deng_vec_t inv_det = 1 / (row1.first * row2.second - row1.second * row2.first); 
            out_mat.row1.first = row2.second * inv_det;
            out_mat.row1.second = -row2.first * inv_det;
            out_mat.row2.first = -row1.second * inv_det;
            out_mat.row2.second = row1.first * inv_det;

            return (mat2<T>) {
                {(T) out_mat.row1.first, (T) out_mat.row1.second},
                {(T) out_mat.row2.first, (T) out_mat.row2.second}
            };
        }
        
    };
    

    template<typename T>
    struct mat3 {
        vec3<T> row1;
        vec3<T> row2;
        vec3<T> row3;
        T *data() { return &row1.first; }
        
        template<typename DT>
        static DT det(mat3<DT> mat) {
            return (DT) (
                (mat.row1.first * mat.row2.second * mat.row3.third) +
                (mat.row1.second * mat.row2.third * mat.row3.first) + 
                (mat.row1.third * mat.row2.first * mat.row3.second) -

                (mat.row1.third * mat.row2.second * mat.row3.first) -
                (mat.row1.first * mat.row2.third * mat.row3.second) -
                (mat.row1.second * mat.row2.first * mat.row3.third) 
            );
        }

        /******************************/
        /***** Operator overloads *****/
        /******************************/

        mat3<T> operator*(const mat3<T> &matrix) {
            mat3<T> out_mat;
            out_mat.row1 = {
                (
                    (row1.first * matrix.row1.first) + 
                    (row1.second * matrix.row2.first) + 
                    (row1.third * matrix.row3.first)
                ), 
                (
                    (row1.first * matrix.row1.second) + 
                    (row1.second * matrix.row2.second) + 
                    (row1.third * matrix.row3.second)
                ), 
                (
                    (row1.first * matrix.row1.third) + 
                    (row1.second * matrix.row2.third) + 
                    (row1.third * matrix.row3.third)
                )
            };

            out_mat.row2 = {
                (
                    (row2.first * matrix.row1.first) + 
                    (row2.second * matrix.row2.first) + 
                    (row2.third * matrix.row3.first)
                ), 
                (
                    (row2.first * matrix.row1.second) + 
                    (row2.second * matrix.row2.second) + 
                    (row2.third * matrix.row3.second)
                ), 
                (
                    (row2.first * matrix.row1.third) + 
                    (row2.second * matrix.row2.third) + 
                    (row2.third * matrix.row3.third)
                )
            };

            out_mat.row3 = {
                (
                    (row3.first * matrix.row1.first) + 
                    (row3.second * matrix.row2.first) + 
                    (row3.third * matrix.row3.first)
                ), 
                (
                    (row3.first * matrix.row1.second) + 
                    (row3.second * matrix.row2.second) + 
                    (row3.third * matrix.row3.second)
                ), 
                (
                    (row3.first * matrix.row1.third) + 
                    (row3.second * matrix.row2.third) + 
                    (row3.third * matrix.row3.third)
                )
            };

            return out_mat;
        }

        /* Perform multiplication with constant c */
        mat3<T> operator*(const T &c) {
            return (mat3<T>) {
                {row1.first * c, row1.second * c, row1.third * c},
                {row2.first * c, row2.second * c, row2.third * c},
                {row3.first * c, row3.second * c, row3.third * c}
            };
        }

        /* Perform multiplication with column vector */
        vec3<T> operator*(vec3<T> vec) {
            return {
                {row1.first * vec.first + row1.second * vec.second + row1.third * vec.third},
                {row2.first * vec.first + row2.second * vec.second + row2.third * vec.third},
                {row3.first * vec.first + row3.second * vec.second + row3.third * vec.third}
            };
        }

        /* Perform matrix addition */
        mat3<T> operator+(mat3<T> mat) {
            return (mat3<T>) {
                {row1.first + mat.row1.first, row1.second + mat.row1.second, row1.third + mat.row1.third},
                {row2.first + mat.row2.first, row2.second + mat.row2.second, row2.third + mat.row2.third},
                {row3.first + mat.row3.first, row3.second + mat.row3.second, row3.third + mat.row3.third}
            };
        }

        /* Find the inverse of the matrix */
        mat3<T> inv() {
            mat3<deng_vec_t> out_mat;
            deng_vec_t inv_det = 1 / mat3<T>::det(*this);

            out_mat.row1 = {
                {inv_det * (row2.second * row3.third - row2.third * row3.second)},
                {inv_det * -(row1.second * row3.third - row1.third * row3.second)},
                {inv_det * (row1.second * row2.third - row1.third * row2.second)} 
            };

            out_mat.row2 = {
                {inv_det * -(row2.first * row3.third - row2.third * row3.first)},
                {inv_det * (row1.first * row3.third - row1.third * row3.first)},
                {inv_det * -(row1.first * row2.third - row1.third * row2.first)}
            };
            
            out_mat.row3 = {
                {inv_det * (row2.first * row3.second - row2.second * row3.first)},
                {inv_det * -(row1.first * row3.second - row1.second * row3.first)},
                {inv_det * (row1.first * row2.second - row1.second * row2.first)}
            };

            return (mat3<T>) {
                {(T) out_mat.row1.first, (T) out_mat.row1.second, (T) out_mat.row1.third},
                {(T) out_mat.row2.first, (T) out_mat.row2.second, (T) out_mat.row2.third},
                {(T) out_mat.row3.first, (T) out_mat.row3.second, (T) out_mat.row3.third}
            };
        }
    };

    template<typename T>
    struct mat4 {
        vec4<T> row1;
        vec4<T> row2;
        vec4<T> row3;
        vec4<T> row4; 

        T *data() { return &row1.first; }
        template<typename DT>
        static DT det(mat4<DT> mat) {
            return (DT) {
                mat.row1.first * mat3<DT>::det((mat3<T>) {
                    {mat.row2.second, mat.row2.third, mat.row2.fourth},
                    {mat.row3.second, mat.row3.third, mat.row3.fourth},
                    {mat.row4.second, mat.row4.third, mat.row4.fourth}
                }) -
                
                mat.row1.second * mat3<DT>::det ((mat3<T>) {
                    {mat.row2.first, mat.row2.third, mat.row2.fourth},
                    {mat.row3.first, mat.row3.third, mat.row3.fourth},
                    {mat.row4.first, mat.row4.third, mat.row4.fourth}
                }) +

                mat.row1.third * mat3<DT>::det ((mat3<T>) {
                    {mat.row2.first, mat.row2.second, mat.row2.fourth},
                    {mat.row3.first, mat.row3.second, mat.row3.fourth},
                    {mat.row4.first, mat.row4.second, mat.row4.fourth}
                }) -

                mat.row1.fourth * mat3<DT>::det ((mat3<T>) {
                    {mat.row2.first, mat.row2.second, mat.row2.third},
                    {mat.row3.first, mat.row3.second, mat.row3.third},
                    {mat.row4.first, mat.row4.second, mat.row4.third}
                })
            };
        };

        mat4<T> operator*(const mat4<T> &matrix) {
            mat4<T> out_mat;
            out_mat.row1 = {
                (
                    (row1.first * matrix.row1.first) + 
                    (row1.second * matrix.row2.first) + 
                    (row1.third * matrix.row3.first) + 
                    (row1.fourth * matrix.row4.first)
                ), 
                (
                    (row1.first * matrix.row1.second) + 
                    (row1.second * matrix.row2.second) + 
                    (row1.third * matrix.row3.second) + 
                    (row1.fourth * matrix.row4.second)
                ), 
                (
                    (row1.first * matrix.row1.third) + 
                    (row1.second * matrix.row2.third) + 
                    (row1.third * matrix.row3.third) + 
                    (row1.fourth * matrix.row4.third)
                ), 
                (
                    (row1.first * matrix.row1.fourth) + 
                    (row1.second * matrix.row2.fourth) + 
                    (row1.third * matrix.row3.fourth) + 
                    (row1.fourth * matrix.row4.fourth)
                )
            };

            out_mat.row2 = { 
                (
                    (row2.first * matrix.row1.first) + 
                    (row2.second * matrix.row2.first) + 
                    (row2.third * matrix.row3.first) + 
                    (row2.fourth * matrix.row4.first)
                ), 
                (
                    (row2.first * matrix.row1.second) + 
                    (row2.second * matrix.row2.second) + 
                    (row2.third * matrix.row3.second) + 
                    (row2.fourth * matrix.row4.second)
                ), 
                (
                    (row2.first * matrix.row1.third) + 
                    (row2.second * matrix.row2.third) + 
                    (row2.third * matrix.row3.third) + 
                    (row2.fourth * matrix.row4.third)
                ), 
                (
                    (row2.first * matrix.row1.fourth) + 
                    (row2.second * matrix.row2.fourth) + 
                    (row2.third * matrix.row3.fourth) + 
                    (row2.fourth * matrix.row4.fourth)
                )
            };

            out_mat.row3 = {
                (
                    (row3.first * matrix.row1.first) + 
                    (row3.second * matrix.row2.first) + 
                    (row3.third * matrix.row3.first) + 
                    (row3.fourth * matrix.row4.first)
                ), 
                (
                    (row3.first * matrix.row1.second) + 
                    (row3.second * matrix.row2.second) + 
                    (row3.third * matrix.row3.second) + 
                    (row3.fourth * matrix.row4.second)
                ), 
                (
                    (row3.first * matrix.row1.third) + 
                    (row3.second * matrix.row2.third) + 
                    (row3.third * matrix.row3.third) + 
                    (row3.fourth * matrix.row4.third)
                ), 
                (
                    (row3.first * matrix.row1.fourth) + 
                    (row3.second * matrix.row2.fourth) + 
                    (row3.third * matrix.row3.fourth) + 
                    (row3.fourth * matrix.row4.fourth)
                )
            };

            out_mat.row4 = {
                (
                    (row4.first * matrix.row1.first) + 
                    (row4.second * matrix.row2.first) + 
                    (row4.third * matrix.row3.first) + 
                    (row4.fourth * matrix.row4.first)
                ), 
                (
                    (row4.first * matrix.row1.second) + 
                    (row4.second * matrix.row2.second) + 
                    (row4.third * matrix.row3.second) + 
                    (row4.fourth * matrix.row4.second)
                ), 
                (
                    (row4.first * matrix.row1.third) + 
                    (row4.second * matrix.row2.third) + 
                    (row4.third * matrix.row3.third) + 
                    (row4.fourth * matrix.row4.third)
                ), 
                (
                    (row4.first * matrix.row1.fourth) + 
                    (row4.second * matrix.row2.fourth) + 
                    (row4.third * matrix.row3.fourth) + 
                    (row4.fourth * matrix.row4.fourth)
                )
            };

            return out_mat;
        }

        /* Multiply with 3D column vector                                          *
         * This operation assumes that homogenous coordinates are going to be used */
        vec4<T> operator*(const vec3<T> vector) {
            vec4<T> out_vec;
            out_vec.first = {
                (row1.first * vector.first) + 
                (row1.second * vector.second) + 
                (row1.third * vector.third) + 
                row1.fourth
            };

            out_vec.second = {
                (row2.first * vector.first) + 
                (row2.second * vector.second) + 
                (row2.third * vector.third) + 
                row2.fourth
            };
            out_vec.third = {
                (row3.first * vector.first) + 
                (row3.second * vector.second) + 
                (row3.third * vector.third) + 
                row3.fourth
            };

            out_vec.fourth = {
                (row4.first * vector.first) + 
                (row4.second * vector.second) + 
                (row4.third * vector.third) + 
                row4.fourth
            };

            return out_vec;
        }
        
        /* Multiply with column vector */
        vec4<T> operator*(vec4<T> vec) {
            vec4<T> out_vec;
            out_vec.first = {
                vec.first * row1.first + 
                vec.second * row1.second + 
                vec.third * row1.third +
                vec.fourth * row1.fourth
            };

            out_vec.second = {
                vec.first * row2.first +
                vec.second * row2.second +
                vec.third * row2.third +
                vec.fourth * row2.fourth
            };

            out_vec.third = {
                vec.first * row3.first +
                vec.second * row3.second +
                vec.third * row3.third +
                vec.fourth * row3.fourth
            };

            out_vec.fourth = {
                vec.first * row4.first +
                vec.second * row4.second +
                vec.third * row4.third +
                vec.fourth * row4.fourth
            };

            return out_vec;
        }

        /* Multiply with constant c */
        mat4<T> operator*(const T &c) {
            return (mat4<T>) {
                {row1.first * c, row1.second * c, row1.third * c, row1.fourth * c},
                {row2.first * c, row2.second * c, row2.third * c, row2.fourth * c},
                {row3.first * c, row3.second * c, row3.third * c, row3.fourth * c},
                {row4.first * c, row4.second * c, row4.third * c, row4.fourth * c}
            };
        }

        /* Matrix addition */
        mat4<T> operator+(mat4<T> mat) {
            return (mat4<T>) {
                {row1.first + mat.row1.first, row1.second + mat.row1.second, row1.third + mat.row1.third, row1.fourth + mat.row1.fourth},
                {row2.first + mat.row2.first, row2.second + mat.row2.second, row2.third + mat.row2.third, row2.fourth + mat.row2.fourth},
                {row3.first + mat.row3.first, row3.second + mat.row3.second, row3.third + mat.row3.third, row3.fourth + mat.row3.fourth},
                {row4.first + mat.row4.first, row4.second + mat.row4.second, row4.third + mat.row4.third, row4.fourth + mat.row4.fourth},
            };
        }

        /* Inverse matrix of the matrix */
        mat4<T> inv() {
            deng_vec_t inv_det = 1 / mat4<T>::det(*this);
            mat4<deng_vec_t> out_mat;
            mat3<T> adj_mat;
            
            // Row 1
            adj_mat = {
                {row2.second, row2.third, row2.fourth},
                {row3.second, row3.third, row3.fourth},
                {row4.second, row4.third, row4.fourth}
            };
            out_mat.row1.first = (T) (inv_det * mat3<T>::det(adj_mat));

            adj_mat = {
                {row1.second, row1.third, row1.fourth},
                {row3.second, row3.third, row3.fourth},
                {row4.second, row4.third, row4.fourth}
            };
            out_mat.row1.second = (T) (-inv_det * mat3<T>::det(adj_mat));

            adj_mat = {
                {row1.second, row1.third, row1.fourth},
                {row2.second, row2.third, row2.fourth},
                {row4.second, row4.third, row4.fourth}
            };
            out_mat.row1.third = (T) (inv_det * mat3<T>::det(adj_mat));
            
            adj_mat = {
                {row1.second, row1.third, row1.fourth},
                {row2.second, row2.third, row2.fourth},
                {row3.second, row3.third, row3.fourth}
            };
            out_mat.row1.fourth = (T) (-inv_det * mat3<T>::det(adj_mat)); 

            // Row 2
            adj_mat = {
                {row2.first, row2.third, row2.fourth},
                {row3.first, row3.third, row3.fourth},
                {row4.first, row4.third, row4.fourth}
            };
            out_mat.row2.first = (T) (-inv_det * mat3<T>::det(adj_mat));

            adj_mat = {
                {row1.first, row1.third, row1.fourth},
                {row3.first, row3.third, row3.fourth},
                {row4.first, row4.third, row4.fourth}
            };
            out_mat.row2.second = (T) (inv_det * mat3<T>::det(adj_mat));

            adj_mat = {
                {row1.first, row1.third, row1.fourth},
                {row2.first, row2.third, row2.fourth},
                {row4.first, row4.third, row4.fourth}
            };
            out_mat.row2.third = (T) (-inv_det * mat3<T>::det(adj_mat));

            adj_mat = {
                {row1.first, row1.third, row1.fourth},
                {row2.first, row2.third, row2.fourth},
                {row3.first, row3.third, row3.fourth}
            };
            out_mat.row2.fourth = (T) (inv_det * mat3<T>::det(adj_mat)); 

            // Row 3
            adj_mat = {
                {row2.first, row2.second, row2.fourth},
                {row3.first, row3.second, row3.fourth},
                {row4.first, row4.second, row4.fourth}
            };
            out_mat.row3.first = (T) (inv_det * mat3<T>::det(adj_mat));

            adj_mat = {
                {row1.first, row1.second, row1.fourth},
                {row3.first, row3.second, row3.fourth},
                {row4.first, row4.second, row4.fourth}
            };
            out_mat.row3.second = (T) (-inv_det * mat3<T>::det(adj_mat));

            adj_mat = {
                {row1.first, row1.second, row1.fourth},
                {row2.first, row2.second, row2.fourth},
                {row4.first, row4.second, row4.fourth}
            };
            out_mat.row3.third = (T) (inv_det * mat3<T>::det(adj_mat));

            adj_mat = {
                {row1.first, row1.second, row1.fourth},
                {row2.first, row2.second, row2.fourth},
                {row3.first, row3.second, row3.fourth}
            };
            out_mat.row3.fourth = (T) (-inv_det * mat3<T>::det(adj_mat)); 

            // Row 4
            adj_mat = {
                {row2.first, row2.second, row2.third},
                {row3.first, row3.second, row3.third},
                {row4.first, row4.second, row4.third}
            };
            out_mat.row4.first = (T) (-inv_det * mat3<T>::det(adj_mat));

            adj_mat = {
                {row1.first, row1.second, row1.third},
                {row3.first, row3.second, row3.third},
                {row4.first, row4.second, row4.third}
            };
            out_mat.row4.second = (T) (inv_det * mat3<T>::det(adj_mat));

            adj_mat = {
                {row1.first, row1.second, row1.third},
                {row2.first, row2.second, row2.third},
                {row4.first, row4.second, row4.third}
            };
            out_mat.row4.third = (T) (-inv_det * mat3<T>::det(adj_mat));

            adj_mat = {
                {row1.first, row1.second, row1.third},
                {row2.first, row2.second, row2.third},
                {row3.first, row3.second, row3.third}
            };
            out_mat.row4.fourth = (T) (inv_det * mat3<T>::det(adj_mat)); 

            return (mat4<T>) {
                {(T) out_mat.row1.first, (T) out_mat.row1.second, (T) out_mat.row1.third, (T) out_mat.row1.fourth},
                {(T) out_mat.row2.first, (T) out_mat.row2.second, (T) out_mat.row2.third, (T) out_mat.row2.fourth},
                {(T) out_mat.row3.first, (T) out_mat.row3.second, (T) out_mat.row3.third, (T) out_mat.row3.fourth},
                {(T) out_mat.row4.first, (T) out_mat.row4.second, (T) out_mat.row4.third, (T) out_mat.row4.fourth}
            };
        }
    };

    class ModelMatrix {
    private:
        mat4<deng_vec_t> m_rot_x_mat;
        mat4<deng_vec_t> m_rot_y_mat;
        mat4<deng_vec_t> m_rot_z_mat;

        mat4<deng_vec_t> m_transformation_mat;
        mat4<deng_vec_t> m_scale_mat;

    public:
        ModelMatrix();

        void setRotation (
            deng_vec_t x_rot, 
            deng_vec_t y_rot, 
            deng_vec_t z_rot
        );

        void setTransformation (
            const deng_vec_t &transform_x, 
            const deng_vec_t &transform_y, 
            const deng_vec_t &transform_z
        );

        void setScale (
            const deng_vec_t &scale_x, 
            const deng_vec_t &scale_y, 
            const deng_vec_t &scale_z
        );

        mat4<deng_vec_t> getModelMatrix() { 
            return m_transformation_mat * 
            m_rot_x_mat * 
            m_rot_y_mat * 
            m_rot_z_mat * 
            m_scale_mat;
        }
    };


    /* ViewMatrix class in DENG represents both Camera Transformation and view matrices */
    class ViewMatrix {
    private:
        vec4<deng_vec_t> m_camera_position;
        mat4<deng_vec_t> m_transformation_mat;

        deng_vec_t m_x_rot;
        deng_vec_t m_y_rot;


        vec4<deng_vec_t> m_right_side;
        vec4<deng_vec_t> m_up_side;
        vec4<deng_vec_t> m_forward_side;

        mat4<deng_vec_t> m_rot_x_mat;
        mat4<deng_vec_t> m_rot_y_mat;
        
        mat4<deng_vec_t> m_delta_rot_x;
        mat4<deng_vec_t> m_delta_rot_y;

    public:
        ViewMatrix(deng_CameraType type);
        void setCameraPosition(const vec4<deng_vec_t> &camera_pos);
        void addToPosition (
            const vec4<deng_vec_t> &movement_speed, 
            const deng_CoordinateAxisType &movement_type, 
            const deng_bool_t &substract
        );

        void setRotation (
            deng_vec_t x_rot, 
            deng_vec_t y_rot
        );

        void setTransformationMatrix();
        void getViewMatrix(mat4<deng_vec_t> *view_mat);
        vec4<deng_vec_t> getPosition();
    };

    class ProjectionMatrix {
    private:
        deng_vec_t m_FOV;

        deng_vec_t m_aspect_ratio;
        deng_vec_t m_near;
        deng_vec_t m_far;

    public:
        ProjectionMatrix (
            const deng_vec_t &FOV, 
            const deng_vec_t &near_plane, 
            const deng_vec_t &far_plane, 
            const deng_vec_t &aspect_ratio
        );
        void getProjectionMatrix(mat4<deng_vec_t> *p_matrix);
    };

    struct UniformData {
        mat4<deng_vec_t> view;
        mat4<deng_vec_t> projection;
        deng_CameraUniformFlagBits cam_flag_bits;
    };

    // generic math functions
    deng_px_t exp(deng_px_t base, int exp);
    deng_vec_t exp(deng_vec_t base, int exp);

    vec2<deng_vec_t> getCartesianCoordsPoint (
        const vec2<deng_vec_t> &centre, 
        const deng_vec_t &angle, 
        const deng_vec_t &distance, 
        const deng_bool_t &inverted_y_axis
    );

    deng_vec_t getFractionNumerator (
        const deng_vec_t &value_numerator, 
        const deng_vec_t &value_denominator, 
        const deng_vec_t &equivalent_denominator
    );

    deng_vec_t getVectorLengthFromBounds(vec2<vec2<deng_vec_t>> vector_bounds);

    deng_vec_t getTriangleAnglesFromEdges (
        const vec3<deng_vec_t> &triangle_edges, 
        const deng_TriangleAngleType &triangle_angle_type
    );

    deng_vec_t getVector2DRotation(vec2<vec2<deng_vec_t>> vector_bounds);

    struct Conversion {
        static deng_vec_t degToRad(const deng_vec_t &deg);
        static deng_vec_t radToDeg(const deng_vec_t &rad);
        static deng_px_t vector2DSizeToPixelSize (
            const deng_vec_t &vec_size, 
            const vec2<deng_ui32_t> &window_size, 
            const deng_CoordinateAxisType &axis_type
        );
        static deng_vec_t pixelSizeToVector2DSize (
            const deng_px_t &pixel_size, 
            const vec2<deng_ui32_t> &window_size, 
            const deng_CoordinateAxisType &axis_type
        );

        std::unordered_map<char, int> hex_definitions;
        
        Conversion() {
            hex_definitions['0'] = 0;
            hex_definitions['1'] = 1;
            hex_definitions['2'] = 2;
            hex_definitions['3'] = 3;
            hex_definitions['4'] = 4;
            hex_definitions['5'] = 5;
            hex_definitions['6'] = 6;
            hex_definitions['7'] = 7;
            hex_definitions['8'] = 8;
            hex_definitions['9'] = 9;
            hex_definitions['a'] = 10;
            hex_definitions['A'] = 10;
            hex_definitions['b'] = 11;
            hex_definitions['B'] = 11;
            hex_definitions['c'] = 12;
            hex_definitions['C'] = 12;
            hex_definitions['d'] = 13;
            hex_definitions['D'] = 13;
            hex_definitions['e'] = 14;
            hex_definitions['E'] = 14;
            hex_definitions['f'] = 15;
            hex_definitions['F'] = 15;
        }

        deng_ui32_t hexToDec(const std::string &hex_value);
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
    static void sortVectorInGrowingOrder (
        std::vector<T> *p_elements_vector, 
        deng_CoordinateAxisType coord_axis_type
    );
    
    template<typename T>
    static void sortVectorInDecliningOrder (
        std::vector<T> *p_elements_vector, 
        deng_CoordinateAxisType coord_axis_type
    );

    // in order to use this function the class used in std::vector must have const char *description as a public class variable
    template<typename T>
    static T *findElementByDescription (
        std::vector<T> *p_vector, 
        const char *description
    );

    // Calculate new asset vertices according to model matrix
    void applyModelMatrix (
        deng_Asset &asset, 
        mat4<deng_vec_t> matrix
    );

    // Generic trigonometric calculation functions
    deng_vec_t trSurface2D(std::array<vec2<deng_vec_t>, 3> tr_verts);
}

#endif
