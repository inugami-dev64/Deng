#ifndef DENG_MATH_H
#define DENG_MATH_H
#define PI 3.1415926f

#define DENG_CAMERA_RIGHT_SIDE(right)           (right = {1.0f, 0.0f, 0.0f, 0.0f})
#define DENG_CAMERA_UP_SIDE(up)                 (up = {0.0f, 1.0f, 0.0f, 0.0f})
#define DENG_CAMERA_FORWARD_SIDE(fwd)           (fwd = {0.0f, 0.0f, -1.0f, 0.0f})

#ifdef __DENG_MATH_CPP
    #include <stdlib.h>
    #include <math.h>
    #include <string>
    #include <vector>
    #include <common/base_types.h>
    #include <common/err_def.h>
    #include <das/assets.h>
#endif

namespace dengMath {
    
    /* vector and matrix structures */
    template<typename T>
    struct vec4 {
        T first, second, third, fourth;

        /******************************/
        /***** Operator overloads *****/
        /******************************/

        vec4<T> operator+(const vec4<T> &vec) {
            vec4<T> out = {
                first + vec.first,
                second + vec.second,
                third + vec.third
            };
            return out; 
        }

        vec4<T> operator-(const vec4<T> &vec) {
            vec4<T> out = {
                first - vec.first,
                second - vec.second,
                third - vec.third
            };

            return out;
        }

        vec4<T> operator*(const T &c) {
            vec4<T> out = {
                c * first,
                c * second,
                c * third,
                c * fourth
            };
            return out; 
        }

        void operator*=(const T &c) {
            first *= c;
            second *= c;
            third *= c;
            fourth *= c;
        }

        void operator+=(const vec4<T> &vec) {
            first += vec.first;
            second += vec.second;
            third += vec.third;
            fourth += vec.fourth;
        }

        void operator-=(const vec4<T> &vec) {
            first -= vec.first;
            second -= vec.second;
            third -= vec.third;
            fourth -= vec.fourth;
        }


        deng_bool_t operator==(const vec4<T> &vec) { 
            return (deng_bool_t) (                
                first == vec.first && 
                second == vec.second && 
                third == vec.third && 
                fourth == vec.fourth
            ); 
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

        vec3<T> operator+(const vec3<T> &vec) {
            vec3<T> out = {
                first + vec.first,
                second + vec.second,
                third + vec.third
            };
            return out; 
        }

        vec3<T> operator-(const vec3<T> &vec) {
            vec3<T> out = {
                first - vec.first,
                second - vec.second,
                third - vec.third
            };
            return out; 
        }

        vec3<T> operator*(const T &c) {
            vec3<T> out = {
                c * first,
                c * second,
                c * third,
            };
            return out; 
        }

        void operator*=(const T &c) {
            first *= c;
            second *= c;
            third *= c;
        }

        void operator+=(const vec3<T> &vec) {
            first += vec.first;
            second += vec.second;
            third += vec.third;
        }

        void operator-=(const vec3<T> &vec) {
            first -= vec.first;
            second -= vec.second;
            third -= vec.third;
        }

        deng_bool_t operator==(const vec3<T> &vec) { 
            return (deng_bool_t) (
                first == vec.first && 
                second == vec.second && 
                third == vec.third
            ); 
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
            vec2<T> out = {
                first + vec.first, 
                second + vec.second
            };
            return out; 
        }

        vec2<T> operator-(const vec2<T> &vec) {
            vec2<T> out = {
                first - vec.first,
                second - vec.second
            };
            
            return out;
        }

        vec2<T> operator*(const T &c) {
            vec2<T> out = {
                c * first,
                c * second,
            };

            return out; 
        }

        void operator*=(const T &c) {
            first *= c;
            second *= c;
        }

        void operator+=(const vec2<T> &vec) {
            first += vec.first;
            second += vec.second;
        }

        void operator-=(const vec2<T> &vec) {
            first -= vec.first;
            second -= vec.second;
        }


        deng_bool_t operator==(const vec2<T> &vec) { 
            return (deng_bool_t) (
                first == vec.first && 
                second == vec.second
            ); 
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
        static DT det(const mat2<DT> &mat) { 
            return mat.row1.first * mat.row2.second - mat.row1.second * mat.row2.first; 
        }
        /******************************/
        /***** Operator overloads *****/
        /******************************/

        /* Perform matrix multiplication on 2x2 matrix */ 
        mat2<T> operator*(const mat2<T> &mat) {
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

            out_mat.row2 = {
                (
                    (row2.first * mat.row1.first) +
                    (row2.second * mat.row2.first)
                ),
                (
                    (row2.first * mat.row1.second) +
                    (row2.second * mat.row2.second)
                )
            };

            return out_mat;
        }
        
        /* Perform multiplication with constant */
        mat2<T> operator*(const T &c) {
            mat2<T> out;
            out.row1 = {row1.first * c, row1.second * c};
            out.row2 = {row2.first * c, row2.second * c};

            return out; 
        }

        /* Peform multiplication with column vector */
        vec2<T> operator*(const vec2<T> &vec) {
            vec2<T> out = {
                (row1.first * vec.first + row1.second * vec.second),
                (row2.first * vec.first + row2.second * vec.second)
            };

            return out; 
        }

        /* Perform matrix addition */
        mat2<T> operator+(const mat2<T> &mat) {
            mat2<T> out = 
            out.row1 = {row1.first + mat.row1.first, row1.second + mat.row1.second};
            out.row2 = {row2.first + mat.row2.first, row1.second + mat.row2.second};

            return out;
        }
        
        /* Find the inverse of a matrix */
        mat2<T> inv() {
            mat2<deng_vec_t> fl_mat;
            deng_vec_t inv_det = 1 / mat2<T>::det(*this);
            fl_mat.row1.first = row2.second * inv_det;
            fl_mat.row1.second = -row2.first * inv_det;
            fl_mat.row2.first = -row1.second * inv_det;
            fl_mat.row2.second = row1.first * inv_det;

            mat2<T> out_mat;
            out_mat.row1 = {(T) fl_mat.row1.first, (T) fl_mat.row1.second};
            out_mat.row2 = {(T) fl_mat.row2.first, (T) fl_mat.row2.second};

            return out_mat;
        }
    };
    

    template<typename T>
    struct mat3 {
        vec3<T> row1;
        vec3<T> row2;
        vec3<T> row3;
        T *data() { return &row1.first; }
        
        template<typename DT>
        static DT det(const mat3<DT> &mat) {
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
            mat3<T> out;
            out.row1 = {row1.first * c, row1.second * c, row1.third * c};
            out.row2 = {row2.first * c, row2.second * c, row2.third * c};
            out.row3 = {row3.first * c, row3.second * c, row3.third * c};
            return out;
        }

        /* Perform multiplication with column vector */
        vec3<T> operator*(vec3<T> vec) {
            vec3<T> out = {
                (row1.first * vec.first + row1.second * vec.second + row1.third * vec.third),
                (row2.first * vec.first + row2.second * vec.second + row2.third * vec.third),
                (row3.first * vec.first + row3.second * vec.second + row3.third * vec.third)
            }; 
            return out;
        }

        vec3<T> operator*(const vec2<T> &vec) {
            vec3<T> out = {
                (row1.first * vec.first + row1.second * vec.second + row1.third),
                (row2.first * vec.first + row2.second * vec.second + row2.third),
                (row3.first * vec.first + row3.second * vec.second + row3.third)
            };
            return out; 
        }

        /* Perform matrix addition */
        mat3<T> operator+(const mat3<T> &mat) {
            mat3<T> out;
            out.row1 = {row1.first + mat.row1.first, row1.second + mat.row1.second, row1.third + mat.row1.third};
            out.row2 = {row2.first + mat.row2.first, row2.second + mat.row2.second, row2.third + mat.row2.third};
            out.row3 = {row3.first + mat.row3.first, row3.second + mat.row3.second, row3.third + mat.row3.third};
            return out;
        }

        /* 
         * Find the inverse of the matrix 
         */
        mat3<T> inv() {
            mat3<deng_vec_t> fl_mat;
            deng_vec_t inv_det = 1 / mat3<T>::det(*this);

            fl_mat.row1 = {
                inv_det * (row2.second * row3.third - row2.third * row3.second),
                inv_det * -(row1.second * row3.third - row1.third * row3.second),
                inv_det * (row1.second * row2.third - row1.third * row2.second) 
            };

            fl_mat.row2 = {
                inv_det * -(row2.first * row3.third - row2.third * row3.first),
                inv_det * (row1.first * row3.third - row1.third * row3.first),
                inv_det * -(row1.first * row2.third - row1.third * row2.first)
            };
            
            fl_mat.row3 = {
                inv_det * (row2.first * row3.second - row2.second * row3.first),
                inv_det * -(row1.first * row3.second - row1.second * row3.first),
                inv_det * (row1.first * row2.second - row1.second * row2.first)
            };

            mat3<T> out_mat;
            out_mat.row1 = {(T) fl_mat.row1.first, (T) fl_mat.row1.second, (T) fl_mat.row1.third}; 
            out_mat.row2 = {(T) fl_mat.row2.first, (T) fl_mat.row2.second, (T) fl_mat.row2.third};
            out_mat.row3 = {(T) fl_mat.row3.first, (T) fl_mat.row3.second, (T) fl_mat.row3.third};
            
            return out_mat;
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
        static DT det(const mat4<DT> &mat) {
            mat3<DT> adj_mat[4];
            
            adj_mat[0].row1 = {mat.row2.second, mat.row2.third, mat.row2.fourth};
            adj_mat[0].row2 = {mat.row3.second, mat.row3.third, mat.row3.fourth};
            adj_mat[0].row3 = {mat.row4.second, mat.row4.third, mat.row4.fourth};

            adj_mat[1].row1 = {mat.row2.first, mat.row2.third, mat.row2.fourth};
            adj_mat[1].row2 = {mat.row3.first, mat.row3.third, mat.row3.fourth};
            adj_mat[1].row3 = {mat.row4.first, mat.row4.third, mat.row4.fourth};

            adj_mat[2].row1 = {mat.row2.first, mat.row2.second, mat.row2.fourth};
            adj_mat[2].row2 = {mat.row3.first, mat.row3.second, mat.row3.fourth};
            adj_mat[2].row3 = {mat.row4.first, mat.row4.second, mat.row4.fourth};

            adj_mat[3].row1 = {mat.row2.first, mat.row2.second, mat.row2.third};
            adj_mat[3].row2 = {mat.row3.first, mat.row3.second, mat.row3.third};
            adj_mat[3].row3 = {mat.row4.first, mat.row4.second, mat.row4.third};

            vec4<DT> out;
            out.first = mat.row1.first * mat3<DT>::det(adj_mat[0]);
            out.second = mat.row1.second * mat3<DT>::det(adj_mat[1]);
            out.third = mat.row1.third * mat3<DT>::det(adj_mat[2]);
            out.fourth = mat.row1.fourth * mat3<DT>::det(adj_mat[3]); 
            
            return (DT) (
                out.first -
                out.second +
                out.third -
                out.fourth
            );
        };

        mat4<T> operator*(mat4<T> matrix) {
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

        /* 
         * Multiply with 3D column vector                                          
         * This operation assumes that homogenous coordinates are going to be used 
         */
        vec4<T> operator*(vec3<T> vec) {
            vec4<T> out_vec;
            out_vec.first = {
                (row1.first * vec.first) + 
                (row1.second * vec.second) + 
                (row1.third * vec.third) + 
                row1.fourth
            };

            out_vec.second = {
                (row2.first * vec.first) + 
                (row2.second * vec.second) + 
                (row2.third * vec.third) + 
                row2.fourth
            };
            out_vec.third = {
                (row3.first * vec.first) + 
                (row3.second * vec.second) + 
                (row3.third * vec.third) + 
                row3.fourth
            };

            out_vec.fourth = {
                (row4.first * vec.first) + 
                (row4.second * vec.second) + 
                (row4.third * vec.third) + 
                row4.fourth
            };

            return out_vec;
        }
        
        /* Multiply with column vector */
        vec4<T> operator*(const vec4<T> &vec) {
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

        /* 
         * Multiply matrix with constant c 
         */
        mat4<T> operator*(const T &c) {
            mat4<T> out;
            out.first = {row1.first * c, row1.second * c, row1.third * c, row1.fourth * c};
            out.second = {row2.first * c, row2.second * c, row2.third * c, row2.fourth * c};
            out.third = {row3.first * c, row3.second * c, row3.third * c, row3.fourth * c};
            out.fourth = {row4.first * c, row4.second * c, row4.third * c, row4.fourth * c}; 
            
            return out;
        }

        /* Matrix addition */
        mat4<T> operator+(const mat4<T> &mat) {
            mat4<T> out;
            out.row1 = {row1.first + mat.row1.first, row1.second + mat.row1.second, row1.third + mat.row1.third, row1.fourth + mat.row1.fourth};
            out.row2 = {row2.first + mat.row2.first, row2.second + mat.row2.second, row2.third + mat.row2.third, row2.fourth + mat.row2.fourth};
            out.row3 = {row3.first + mat.row3.first, row3.second + mat.row3.second, row3.third + mat.row3.third, row3.fourth + mat.row3.fourth};
            out.row4 = {row4.first + mat.row4.first, row4.second + mat.row4.second, row4.third + mat.row4.third, row4.fourth + mat.row4.fourth};

            return out;
        }

        /* Inverse matrix of the matrix */
        mat4<T> inv() {
            deng_vec_t inv_det = 1 / mat4<T>::det(*this);
            mat4<T> out_mat;
            mat3<deng_vec_t> adj_mat;
            
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

            return out_mat;
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
            return (
                m_transformation_mat * 
                m_rot_x_mat * 
                m_rot_y_mat * 
                m_rot_z_mat * 
                m_scale_mat
            );
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
            const deng_CoordinateAxisType &movement_type
        );

        void setCameraRotation (
            deng_vec_t x_rot, 
            deng_vec_t y_rot
        );

        void setPointRotation (
            dengMath::vec3<deng_vec_t> point,
            deng_vec_t x_rot,
            deng_vec_t y_rot
        );

        void setTransformationMatrix(deng_bool_t is_abs_cam_sys);
        void getViewMatrix(mat4<deng_vec_t> *view_mat);
        vec4<deng_vec_t> getPosition();
        void getSidesInWorldCoords (
            vec4<deng_vec_t> *p_right_side,
            vec4<deng_vec_t> *p_up_side,
            vec4<deng_vec_t> *p_forward_side
        );
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

    struct Conversion {
        static deng_f64_t degToRad(const deng_f64_t &deg);
        static deng_f64_t radToDeg(const deng_f64_t &rad);

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

        static vec2<deng_vec_t> findAbsPosition (
            VERT_UNMAPPED_2D *vert,
            vec2<deng_vec_t> child_pos
        );

        static deng_vec_t findAbsSize (
            deng_vec_t parent_size,
            deng_vec_t child_size
        );

        static deng_vec_t findRelSize (
            deng_vec_t parent_size,
            deng_vec_t abs_size
        );

        static vec2<deng_vec_t> findRelPosition (
            VERT_UNMAPPED_2D *vert,
            vec2<deng_vec_t> abs_pos
        );
    };


    struct Transformer {
        // Calculate new asset vertices according to model matrix
        static void apply3DModelMatrix (
            deng_Asset &asset, 
            mat4<deng_vec_t> &mat
        );

        static void apply3DModelMatrix (
            vec2<deng_ui32_t> asset_bounds,
            std::vector<deng_Asset> *p_assets,
            mat4<deng_vec_t> &mat
        );

        static void apply2DModelMatrix (
            deng_Asset &asset,
            mat3<deng_vec_t> &mat
        );

        static void apply2DModelMatrix (
            vec2<deng_ui32_t> asset_bounds,
            std::vector<deng_Asset> *p_assets,
            mat3<deng_vec_t> &mat
        );
    };

}

#endif
