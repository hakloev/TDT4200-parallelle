#include <iostream>
#include "lodepng.h"

/* Kernel for inverting usigned char */
__global__ void invert_image( unsigned char *image ) {
    int index = threadIdx.x + blockIdx.x * blockDim.x; 
    image[index] = ~image[index]; /* Invert unsigned char */
}

# define THREADS_PER_BLOCK 512

int main( int argc, char ** argv){

    size_t pngsize;
    unsigned char *png;
    const char * filename = "lenna512x512_inv.png";
    /* Read in the image */
    lodepng_load_file(&png, &pngsize, filename);

    unsigned char *image; /* Host copy of image */
    unsigned int width, height;
    /* Decode it into a RGB 8-bit per channel vector */
    unsigned int error = lodepng_decode24(&image, &width, &height, png, pngsize);

    /* Check if read and decode of .png went well */
    if(error != 0){
        std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
	
    // Do work
	unsigned char *device_image;
	int loop_size = width * height * 3;
	size_t size = loop_size * sizeof( unsigned char );

	cudaMalloc( ( void** ) &device_image, size );
	cudaMemcpy( device_image, image, size, cudaMemcpyHostToDevice );

    invert_image<<<loop_size / THREADS_PER_BLOCK, THREADS_PER_BLOCK>>>( device_image );
	
    cudaMemcpy( image, device_image, size, cudaMemcpyDeviceToHost );

    /* Save the result to a new .png file */
    lodepng_encode24_file("lenna512x512_orig.png", image , width,height);

	free( image );
	cudaFree( device_image );

    return 0;
}

