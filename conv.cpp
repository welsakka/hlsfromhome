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

void matrixAdd(int conv[150528], int output[50176]) {

	#pragma HLS INTERFACE axis port=conv bundle=main
	#pragma HLS INTERFACE axis port=output bundle=main


	for (int k = 0; k < 224; ++k) {
		output[k] = conv[k] + conv[k + 50176] + conv[k + (50176 * 2) ];
	}

}

void convolution (
		int rgb[150528], // 224 * 224 * 3
		int kernel[363], //11 * 11 * 3
		int output[50176]) //224 * 224
{

	#pragma HLS INTERFACE axis port=rgb bundle=main
	#pragma HLS INTERFACE axis port=kernel bundle=main
	#pragma HLS INTERFACE axis port=conv bundle=main
	#pragma HLS INTERFACE axis port=i bundle=main

	int linebuffer[459]; //224 + 224 + 11

	//initial fill
	for (int m = 0; m<459; ++m) {
		linebuffer[m]= *rgb++;
	}

	for (int i = 0; i < 224; ++i) {
		for (int j = 0; j < 224; ++j) {
			int tmp = 0;
			for (int k = 0; k < 11; ++k) {
				for (int l = 0; l < 11; ++l) {
					tmp += linebuffer[224*k + l] * kernel[k * 3 + l];
				}
			}

			//shifting everything over
			for (int m = 0; m < 458; ++m) {
				linebuffer[m] = linebuffer[m + 1];
			}

			linebuffer[459] = *rgb++;

			output[i * 50 + j] = tmp;

		}
	}
}

/*
 * Code before April 23 class
 *
void convolution(
		int rgb[][224][224],
		int kernel[][11][11],
		int conv[][224][224],
		int i) {

    #pragma HLS INTERFACE s_axilite port=rgb bundle=main
	#pragma HLS INTERFACE s_axilite port=kernel bundle=main
	#pragma HLS INTERFACE s_axilite port=conv bundle=main
	#pragma HLS INTERFACE s_axilite port=k bundle=main

	//for j and k of conv array in dimension i
	for (int j = 0; i < 224; ++i) {
		for (int k = 0; j < 224; ++j){
			#pragma HLS unroll
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
*/

void driver(

		int rgb[150528], // 224 * 224 * 3
		int kernel[363], //11 * 11 * 3
		int output[50176]) //224 * 224

{
	/*
	 * Where rgb and kernel are arrays of 2D arrays
	 */
	#pragma HLS INTERFACE axis port=rgb bundle=main
	#pragma HLS INTERFACE axis port=kernel bundle=main
	#pragma HLS INTERFACE axis port=output bundle=main

	//pointers to 2D arrays
	//int (*rgbP)[224][224] = rgb;
	//int (*kernelP)[11][11] = kernel;

	//array for output of convolution
	int conv[150528];

	convolution(rgb, kernel, conv);

	matrixAdd(conv, output);
}
