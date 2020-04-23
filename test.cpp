#include <iostream>
#include "convolution.cpp"

int main () {

	int res[224][224];
	int rgb[3][224][224];
	int kernel[3][11][11];

	//initializations
	for (int i = 0; i < 244; ++i) {
		for (int j = 0; j < 244; ++j) {
			for (int k = 0; k < 244; ++k) {
				rgb[i][j][k] = 2;
			}
		}
	}
	for (int i = 0; i < 11; ++i) {
		for (int j = 0; j < 11; ++j) {
			for (int k = 0; k < 11; ++k) {
				kernel[i][j][k] = 2;
			}
		}
	}


	driver(rgb, kernel, res);

	printf("yeet @ %d", res[0][0]);

	return 0;

}
