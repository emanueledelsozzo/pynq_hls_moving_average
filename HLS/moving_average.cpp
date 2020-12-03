//  Authors:
//	Emanuele Del Sozzo, Davide Conficconi
//	{emanuele.delsozzo, davide.conficconi}@polimi.it
//

#include "moving_average.hpp"

void moving_average(my_stream_in &in, my_stream_out &out){
// AXI Stream interfaces for the top function pointer arguments
#pragma HLS INTERFACE axis register both port=in
#pragma HLS INTERFACE axis register both port=out
// The kernel is always active, no start signal required
#pragma HLS INTERFACE ap_ctrl_none port=return

// Shift Register
	int local_window[MAX_WINDOW_SIZE];
#pragma HLS ARRAY_PARTITION variable=local_window complete dim=1

// Variables to store the average and the oldest value
	int avg = 0;
	int prev = 0;
// Read input data  and window size
	int size = in.read().data;
	int window = in.read().data;


	for(int i = 0; i < size; i++){
#pragma HLS PIPELINE
// Read oldest value from shift register
		prev = local_window[0];
// shift data
		for(int j = 0; j < MAX_WINDOW_SIZE-1; j++){
			local_window[j] = local_window[j+1];
		}
// Read input data and store it into the shift register
		my_ap_in tmpIn = in.read();
		int val = tmpIn.data;
		local_window[window-1] = val;
// Compute average
		int sub_val = i < window ? 0 : prev;
		avg = avg + val - sub_val;
// Produce output
		if(i >= window - 1){
			float out_val = (float)avg / window;
			my_ap_out tmpOut;
			tmpOut.data = out_val;
			tmpOut.keep = tmpIn.keep;
			tmpOut.strb = tmpIn.strb;
			tmpOut.user = tmpIn.user;
			tmpOut.last = tmpIn.last;
			tmpOut.id = tmpIn.id;
			tmpOut.dest = tmpIn.dest;
			out.write(tmpOut);
		}
	}
}

