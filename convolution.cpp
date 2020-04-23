/*
 
As talked about in class today (4/14) modern neural networks used for image recognition perform many many convolutions.

For the first assignment in this set of assignments, create a HLS valid (use optimization pragmas and interface
pragmas as necessary) to create a convolution unit that will be able to compute the convolutions that 
exist inside the first layer of AlexNet.

The deliverable will be both your code and a small write up of any 
optimization decisions/pragmas/interfaces you chose to use. The write-up should include why you chose
them, the alternatives and why you believe the ones you choose were best suited for the task.

Additionally, try to write a blurb about your the performance you would expect to see from your
module assuming perfect conditions. (a.k.a how many cycles would you estimate it to take to run). Please also
include the factors that you would assume and expect effect the performance your core.
 
 */
/*
[][][][][][][][][][][]
[][][][][][][][][][][]
[][][][][][][][][][][]
[][][][][][][][][][][]
[][][][][][][][][][][]
[][][][][][-][-][-][-][-][-]
[][][][][][-][-][-][-][-][-]
[][][][][][-][-][-][-][-][-]
[][][][][][-][-][-][-][-][-]
[][][][][][-][-][-][-][-][-]
[][][][][][-][-][-][-][-][-]
*/

void matrixAdd(int rgb[][224][224], int output[][224]) {
	
	#pragma HLS INTERFACE s_axilite port=rgb bundle=main
	#pragma HLS INTERFACE s_axilite port=output bundle=main
	#pragma HLS INTERFACE s_axilite port=return bundle=main
	
	for (int j = 0; j < 224; ++j) {
		for (int k = 0; k < 224; ++k) {
			output[j][k] = rgb[0][j][k] + rgb[1][j][k] + rgb[2][j][k]; 
		}
	}
}


void convolution(
		int rgb[][224][224], 
		int kernel[][11][11],
		int conv[][224][224],
		int i) {

    #pragma HLS INTERFACE s_axilite port=rgb bundle=main
	#pragma HLS INTERFACE s_axilite port=kernel bundle=main
	#pragma HLS INTERFACE s_axilite port=conv bundle=main
	#pragma HLS INTERFACE s_axilite port=k bundle=main
	#pragma HLS INTERFACE s_axilite port=return bundle=main

	//for j and k of conv array in dimension i
	for (int j = 0; i < 224; ++i) {
		for (int k = 0; j < 224; ++j){
			conv[i][j][k] = 0;
			//for each cell in kernel
			for (int l = 0; l < 11; ++l){
				for (int m = 0; m < 11; ++m){
					conv[i][j][k] += rgb[i][j + l - 1][k + m - 1] * kernel[i][l][m];
				}
			}
		}
	}	
}

void driver(int rgb[3][224][224], int kernel[3][11][11], int output[224][224]){
	/*
	 * Where rgb and kernel are arrays of 2D arrays
	 */
	#pragma HLS INTERFACE s_axilite port=rgb bundle=main
	#pragma HLS INTERFACE s_axilite port=kernel bundle=main
	#pragma HLS INTERFACE s_axilite port=return bundle=main
	
	//pointers to 2D arrays
	//int (*rgbP)[224][224] = rgb;
	//int (*kernelP)[11][11] = kernel;
	
	//array for output of convolution
	int conv[3][224][224];
		
	for (int i = 0; i < 3; ++i){
		#pragma HLS unroll
		convolution(rgb, kernel, conv, i);
	}
		
	matrixAdd(conv, output);
	
	return;
}
