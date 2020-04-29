#include <iostream>
#include "conv.cpp"

int main () {

	//int res[224][224];
	//int rgb[3][224][224];
	//int kernel[3][11][11];

	int res[50176]; //224 * 224
	int rgb[150528]; // 224 * 224 * 3
	int kernel[363]; //11 * 11 * 3

	//initializations
	for (int i = 0; i < 150528; ++i) {
		#pragma HLS unroll
		rgb[i] = 2;
	}
	for (int i = 0; i < 363; ++i) {
		#pragma HLS unroll
		kernel[i] = 2;
	}


	driver(rgb, kernel, res);

	printf("Value at (0, 0) = ", res[0][0]);

	return 0;

}
