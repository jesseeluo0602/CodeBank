#include <stdio.h>
#include <string.h>
#include <emmintrin.h>
#include <math.h>
#include <omp.h>
#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
                    float* kernel)
{
    omp_set_num_threads(16);
    // the x coordinate of the kernel's center
    int kern_cent_X = (KERNX - 1)/2;
    // the y coordinate of the kernel's center
    int kern_cent_Y = (KERNY - 1)/2;

    // pad the matrix with 0s by creating a matrix of size x+2, y+2
    int x_padded = data_size_X + 2;
    int y_padded = data_size_Y + 2;
    float in_padded[x_padded * y_padded];
    //set first row of padded matrix equal to 0
    #pragma omp parallel for firstprivate(x_padded)
    for (int k = 0; k < x_padded/4*4;k+=4){
        in_padded[k] = 0;
        in_padded[k+1] = 0;
        in_padded[k+2] = 0;
        in_padded[k+3] = 0;
    }
    for (int k = x_padded/4*4; k < x_padded;k++){
        in_padded[k] = 0;
    }
    //set last row of padded matrix equal to 0
    #pragma omp parallel for firstprivate(x_padded,y_padded)
    for (int k = 0; k < x_padded/4*4;k+=4){
        in_padded[k+x_padded*(y_padded-1)] = 0;
        in_padded[k+1+x_padded*(y_padded-1)] = 0;
        in_padded[k+2+x_padded*(y_padded-1)] = 0;
        in_padded[k+3+x_padded*(y_padded-1)] = 0;
    }
    for (int k=x_padded/4*4; k < x_padded;k++){
        in_padded[k+x_padded*(y_padded-1)] = 0;
    }
    //set first and last column of padded matrix equal to 0
    #pragma omp parallel for firstprivate(x_padded)
    for (int k = 0; k < y_padded-1; k++){
        in_padded[k*x_padded] = 0;
        in_padded[x_padded-1+(k+1)*x_padded] = 0;
    }
    // copy original matrix into padded matrix
    #pragma omp parallel for firstprivate(data_size_X, data_size_Y,x_padded)
    for (int y = 0; y < data_size_Y; y++){
        for(int x = 1; x < x_padded - 1; x++) {
            in_padded[x+(y+1)*x_padded]=in[(x-1)+y*data_size_X];
        }
    }

    // flip the kernel
   
    float kernel_flipped[KERNX*KERNY];
    for (int i = 0; i < KERNX*KERNY; i++) {
        kernel_flipped[i] = kernel[KERNX*KERNY - 1 - i];
    }
    
    int x,y;
    __m128 out_value1, kernel_curr0, kernel_curr1, kernel_curr2, in_curr0, in_curr1, in_curr2, product0, product1, product2;

    // main convolution loop with adjusted padded values
    #pragma omp parallel for firstprivate(kernel_flipped,x_padded,x,data_size_X, out_value1, kernel_curr0, kernel_curr1, kernel_curr2, in_curr0, in_curr1, in_curr2, product0, product1, product2, KERNX*KERNY)
    for(y = 0; y < data_size_Y; y++){ // the y coordinate of the output location we're focusing on
        for(x = 0; x < data_size_X - 3; x += 4){ // the x coordinate of the output location we're focusing on
            // Store the current array index in out_curr
            //float* out_curr2 = (out + 4 + x + y *data_size_X);
            // Initialize the sum at 0
            __m128 out_value1 = _mm_setzero_ps();
            //__m128 out_value2 = _mm_setzero_ps();
            // Iterate through each value in the kernel
            for (int i = 0; i < KERNX*KERNY; i+=3) {
                // Load 4 values of the kernel
                __m128 kernel_curr0 = _mm_load1_ps(kernel_flipped + i);
                __m128 kernel_curr1 = _mm_load1_ps(kernel_flipped + i + 1);
                __m128 kernel_curr2 = _mm_load1_ps(kernel_flipped + i + 2);
                // Load 4 values of the padded in matrix
                __m128 in_curr0 = _mm_loadu_ps(in_padded + (x + i% KERNX) + (y + i/ KERNY) * x_padded);
                __m128 in_curr1 = _mm_loadu_ps(in_padded + (x + (i + 1)% KERNX) + (y + (i + 1)/ KERNY) * x_padded);
                __m128 in_curr2 = _mm_loadu_ps(in_padded + (x + (i + 2)% KERNX) + (y + (i + 2)/ KERNY) * x_padded);
                // Calculate the dot product
                __m128 product0 = _mm_mul_ps(kernel_curr0, in_curr0);
                __m128 product1 = _mm_mul_ps(kernel_curr1, in_curr1);
                __m128 product2 = _mm_mul_ps(kernel_curr2, in_curr2);
                // out += kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X]
                out_value1 = _mm_add_ps(out_value1, product0);
                out_value1 = _mm_add_ps(out_value1, product1);
                out_value1 = _mm_add_ps(out_value1, product2);
            }
            // Store the sum in the array index
            _mm_storeu_ps(out + x + y *data_size_X, out_value1);
            //_mm_storeu_ps(out_curr, out_value2);

        }
    }

    // Handling matrix sizes where X is not a multiple of 4 and therefore is not calculated properly with
    // the previous for loop.
    if (data_size_X % 4 != 0) {
        for(int y = 0; y < data_size_Y; y++){ // the y coordinate of the output location we're focusing on
            for (int x_left = x; x_left < data_size_X; x_left++) {
                for (int i = 0; i < KERNX*KERNY; i++) {
                    out[x_left + y * data_size_X] += 
                        kernel_flipped[i] * in_padded[(x_left + i % KERNX) + (y + i / KERNY) * x_padded];                    
                }
            }
        }
    }

    return 1;
}