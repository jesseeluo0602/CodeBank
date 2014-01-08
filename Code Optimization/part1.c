#include <stdio.h>
#include <string.h>
#include <emmintrin.h>
#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
                    float* kernel)
{
    // the x coordinate of the kernel's center
    int kern_cent_X = (KERNX - 1)/2;
    // the y coordinate of the kernel's center
    int kern_cent_Y = (KERNY - 1)/2;

    // pad the matrix with 0s by creating a matrix of size x+2, y+2
    int x_padded = data_size_X + 2;
    int y_padded = data_size_Y + 2;
    float in_padded[x_padded * y_padded];
    memset(in_padded, 0, x_padded * y_padded * sizeof(float));
    for (int k = 1; k < y_padded - 1; k++) {
        // copy original matrix into padded matrix
        memcpy(&in_padded[k * x_padded + 1], &in[(k-1) * data_size_X], sizeof(float) * data_size_X);
    }

    // flip the kernel
    int kernel_size = KERNX * KERNY;
    float kernel_flipped[kernel_size];
    for (int i = 0; i < kernel_size; i++) {
        kernel_flipped[i] = kernel[kernel_size - 1 - i];
    }
    
    int x;

    // main convolution loop with adjusted padded values
    for(int y = 0; y < data_size_Y; y++){ // the y coordinate of the output location we're focusing on
        for(x = 0; x + 3 < data_size_X; x += 4){ // the x coordinate of the output location we're focusing on
            // Store the current array index in out_curr
            float* out_curr = (out + x + y *data_size_X);
            // Initialize the sum at 0
            __m128 out_value = _mm_setzero_ps();
            // Iterate through each value in the kernel
            for (int i = 0; i < kernel_size; i++) {
                // Load 4 values of the kernel
                __m128 kernel_curr = _mm_load1_ps(kernel_flipped + i);
                // Load 4 values of the padded in matrix
                __m128 in_curr = _mm_loadu_ps(in_padded + (x + i % KERNX) + (y + i / KERNY) * x_padded);
                // Calculate the dot product
                __m128 product = _mm_mul_ps(kernel_curr, in_curr);
                // out += kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X]
                out_value = _mm_add_ps(out_value, product);
            }
            // Store the sum in the array index
            _mm_storeu_ps(out_curr, out_value);
        }
    }

    // Handling matrix sizes where X is not a multiple of 4 and therefore is not calculated properly with
    // the previous for loop.
    if (data_size_X % 4 != 0) {
        for(int y = 0; y < data_size_Y; y++){ // the y coordinate of the output location we're focusing on
            for (int x_left = x; x_left < data_size_X; x_left++) {
                for (int i = 0; i < kernel_size; i++) {
                    out[x_left + y * data_size_X] += 
                        kernel_flipped[i] * in_padded[(x_left + i % KERNX) + (y + i / KERNY) * x_padded];                    
                }
            }
        }
    }

    return 1;
}

