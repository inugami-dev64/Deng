#include <stdlib.h>
#include <stdio.h> 
#include <unordered_map>
#include <vector>
#include <string>
#include <random>
#include <common/base_types.h>
#include <das/assets.h>
#include <deng/deng_math.h>

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
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<deng_vec_t> num(0.0f, 100.0f); 
    dengMath::mat4<deng_vec_t> test_mat4 = {
        {num(mt), num(mt), 1.0f, num(mt)},
        {num(mt), num(mt), num(mt), 1.0f},
        {num(mt), 1.0f, num(mt), num(mt)},
        {num(mt), num(mt), num(mt), num(mt)},
    };

    dengMath::mat3<deng_vec_t> test_mat3 = {
        {num(mt), num(mt), num(mt)},
        {num(mt), 1.0f, num(mt)},
        {num(mt), num(mt), num(mt)},
    }; 

    dengMath::mat2<deng_vec_t> test_mat2 = {
        {num(mt), num(mt)},
        {num(mt), 1.0f},
    };

    dengMath::mat4<deng_vec_t> det_mat = {
        {2, 4, 6, 5},
        {9, 2, 1, 7},
        {52, 45, 6, 9},
        {1, 2, 4, 7}
    };

    // Print 4x4 matrix, inverse and product of A x A ^ (-1)
    printf("DET: %f\n", dengMath::mat4<deng_vec_t>::det(det_mat));
    printmat(test_mat4);
    dengMath::mat4<deng_vec_t> inv_mat4 = test_mat4.inv();
    printmat(inv_mat4);
    printmat(test_mat4 * inv_mat4);
    printf("_______________________________\n");

    // Print 3x3 matrix, inverse and product of A x A ^(-1)
    printmat(test_mat3);
    dengMath::mat3<deng_vec_t> inv_mat3 = test_mat3.inv();
    printmat(inv_mat3);
    printmat(test_mat3 * inv_mat3);
    printf("_______________________________\n");

    // Print 2x2 matrix, inverse and product of A x A ^(-1)
    printmat(test_mat2);
    dengMath::mat2<deng_vec_t> inv_mat2 = test_mat2.inv();
    printmat(inv_mat2);
    printmat(test_mat2 * inv_mat2);
    printf("_______________________________\n");

    return EXIT_SUCCESS;
}
