//  Authors:
//      Emanuele Del Sozzo, Davide Conficconi
//      {emanuele.delsozzo, davide.conficconi}@polimi.it
//

#include <stdio.h>
#include <random>
#include "moving_average.hpp"

// Input elements
#define NUM_ELEM 10000

// my_ap_in creator
my_ap_in createMyApIn(int data, int bitwidth, bool last){
	my_ap_in tmp;
	tmp.data = data;
	tmp.dest = 0;
	tmp.id = 0;
	tmp.keep = ( 1<<(bitwidth/8) ) - 1;
	tmp.last = last;
	tmp.strb = ( 1<<(bitwidth/8) ) - 1;
	tmp.user = 0;

	return tmp;
}

int main(int argc, char* argv[]){

	int in[NUM_ELEM];
	float outLocal[NUM_ELEM], outFPGA[NUM_ELEM];
// Input/output streams
	my_stream_in inFPGAstream;
	my_stream_out outFPGAstream;
// random generator seed and distribution
	int myseed = 1234;
	std::default_random_engine rng(myseed);
	std::uniform_int_distribution<int> rng_dist(-1000, 1000);

// generation of input random from the distribution
	for(int i = 0; i < NUM_ELEM; i++){
		in[i] = static_cast<int>(rng_dist(rng));
	}

// Loop on supported window sizes
	for(int i = 1; i <= MAX_WINDOW_SIZE; i++){
// Software results
		for(int j = 0; j < NUM_ELEM - i + 1; j++){
			int acc = 0;
			for(int k = 0; k < i; k++){
				acc += in[j + k];
			}
			outLocal[j] = (float)acc / i;
		}

// Push data into the streams
		inFPGAstream.write(createMyApIn(NUM_ELEM, 32, 0));
		inFPGAstream.write(createMyApIn(i, 32, 0));
		for(int j = 0; j < NUM_ELEM; j++){

			inFPGAstream.write(createMyApIn(in[j], 32, j == NUM_ELEM - 1));
		}

// Invoke top function
		moving_average(inFPGAstream, outFPGAstream);

		for(int j = 0; j < NUM_ELEM - i + 1; j++){
// Read data from output stream
			my_ap_out tmpOut = outFPGAstream.read();
// Check results
			if(fabs(outLocal[j] - tmpOut.data) > 0.0000001){
				printf("Window size: %d - Error at index %d! %.20f != %.20f\n", i, j, outLocal[j], tmpOut.data);
				return 1;
			}
		}
	}
	return 0;
}

