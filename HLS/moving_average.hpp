//  Authors:
//      Emanuele Del Sozzo, Davide Conficconi
//      {emanuele.delsozzo, davide.conficconi}@polimi.it
//

#ifndef MV_AVG_HPP
#define MV_AVG_HPP

#include "hls_stream.h"
#include "ap_int.h"

// Shift Register Maximum Size
#define MAX_WINDOW_SIZE 16

// Template for AXI Stream interface
template<class DT, int D,int U,int TI,int TD>
struct ap_axis{
	DT		 		data;
	ap_uint<D/8> 	keep;
	ap_uint<D/8> 	strb;
	ap_uint<U> 		user;
	ap_uint<1> 		last;
	ap_uint<TI> 	id;
	ap_uint<TD> 	dest;
};

// Definition of input/output AXI Stream types
typedef ap_axis<int, 32, 1, 1, 1> my_ap_in;
typedef ap_axis<float, 32, 1, 1, 1> my_ap_out;

// input/output types wrapped into hls_stream
typedef hls::stream<my_ap_in> my_stream_in;
typedef hls::stream<my_ap_out> my_stream_out;

// Top function prototype
void moving_average(my_stream_in &in, my_stream_out &out);

#endif

