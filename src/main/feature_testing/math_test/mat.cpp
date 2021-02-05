#include <stdlib.h>
#include <stdio.h> 
#include <unordered_map>
#include <vector>
#include <string>
#include "../../../../headers/common/base_types.h"
#include "../../../../headers/das/assets.h"
#include "../../../../headers/deng/deng_math.h"

void printmat(dengMath::mat2<deng_vec_t> mat) {
    printf (
        "%f, %f\n" \
        "%f, %f\n\n",
        mat.row1.first, mat.row1.second,
        mat.row2.first, mat.row2.second
    );
}

void printmat(dengMath::mat3<deng_vec_t> mat) {
    printf (
        "%f, %f, %f\n" \
        "%f, %f, %f\n" \
        "%f, %f, %f\n\n",
        mat.row1.first, mat.row1.second, mat.row1.third,
        mat.row2.first, mat.row2.second, mat.row2.third,
        mat.row3.first, mat.row3.second, mat.row3.third 
    );
}

void printmat(dengMath::mat4<deng_vec_t> mat) {
    printf (
        "%f, %f, %f, %f\n" \
        "%f, %f, %f, %f\n" \
        "%f, %f, %f, %f\n" \
        "%f, %f, %f, %f\n\n",
        mat.row1.first, mat.row1.second, mat.row1.third, mat.row1.fourth,
        mat.row2.first, mat.row2.second, mat.row2.third, mat.row2.fourth,
        mat.row3.first, mat.row3.second, mat.row3.third, mat.row3.fourth,
        mat.row4.first, mat.row4.second, mat.row4.third, mat.row4.fourth
    );
}

int main() {
    dengMath::mat3<deng_vec_t> test_mat3 = {
        {2.0f, -1.0f, 6.12f},
        {1.5f, 15.0f, 8.9f},
        {-9.6f, -1.2f, 18.2f}
    }; 

    dengMath::mat4<deng_vec_t> test_mat4 = {
        {2.0f, -1.0f, 6.12f, 8.69f},
        {1.5f, 15.0f, 8.9f, -17.2},
        {-9.6f, -1.2f, 18.2f, -3.14},
        {9.5f, 1.5f, 4.5f, 8.5f}
    }; 

    // Print 3x3 matrix, inverse and product of A x A^-1
    printf("DET: %f\n", dengMath::mat3<deng_vec_t>::det(test_mat3));
    printmat(test_mat3);
    dengMath::mat3<deng_vec_t> inv_mat3 = test_mat3.inv();
    printmat(inv_mat3);
    printmat(test_mat3 * inv_mat3);

    // Print 4x4 matrix, inverse and product of A x A^-1
    printf("DET: %f\n", dengMath::mat4<deng_vec_t>::det(test_mat4));
    printmat(test_mat4);
    dengMath::mat4<deng_vec_t> inv_mat4 = test_mat4.inv();
    printmat(inv_mat4);
    printmat(test_mat4 * inv_mat4);
    return EXIT_SUCCESS;
}
