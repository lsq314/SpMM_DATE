#include <hls_stream.h>
#include <ap_int.h>
#include "test.h"


void producer(uint32_t* source, hls::stream<uint32_t>* from){

	for (uint32_t i=0; i<1000; i++) {
//#pragma HLS PIPELINE
		from->write(source[i]);
	}

	from->write(0xFFFFFFFF);
}

void consumer(hls::stream<uint32_t>* to, hls::stream<uint32_t>* from){

	for (bool enable=false; !enable; ){
		uint32_t temp = from->read();
		enable = temp == 0xFFFFFFFF;

		if (!enable) {
			to->write(temp);
		}
	}

	to->write(0xFFFFFFFF);
}

void consumer_two(uint32_t* to, hls::stream<uint32_t>* from){

	uint32_t count = 0;

	for (bool enable=false; !enable; ){
		uint32_t temp = from->read();
		enable = temp == 0xFFFFFFFF;

		if (!enable) {
			to[count] = temp;
			count += 1;
		}
	}
}

void test(uint32_t* source, uint32_t* dest){

#pragma HLS INTERFACE mode=m_axi bundle=dest depth=1000 port=dest
#pragma HLS INTERFACE mode=m_axi bundle=source depth=1000 port=source

#pragma HLS INTERFACE mode=s_axilite port=return

#pragma HLS DATAFLOW

	hls::stream<uint32_t, 2> from, to;

	producer(source, &from);
	consumer(&to, &from);
	consumer_two(dest, &to);
}

void reader(uint32_t* from, hls::stream<uint32_t>* to){

	uint32_t sum = 0;

	for (uint32_t i=0; i<22; i++) {
#pragma HLS PIPELINE
		sum += from[i];
	}

	to->write(sum);
}

void merger_sum(hls::stream<uint32_t>* from1, hls::stream<uint32_t>* from2, hls::stream<uint32_t>* from3,
		hls::stream<uint32_t>* from4, hls::stream<uint32_t>* from5, hls::stream<uint32_t>* from6,
		hls::stream<uint32_t>* from7, hls::stream<uint32_t>* from8){

	uint32_t sum1, sum2, sum3, sum4, sum5, sum6, sum7, sum8;
	sum1 = from1->read();
	sum2 = from2->read();
	sum3 = from3->read();
	sum4 = from4->read();
	sum5 = from5->read();
	sum6 = from6->read();
	sum7 = from7->read();
	sum8 = from8->read();
}

void fixchannel(uint32_t* source1, uint32_t* source2, uint32_t* source3, uint32_t* source4,
		uint32_t* source5, uint32_t* source6, uint32_t* source7, uint32_t* source8){

#pragma HLS INTERFACE mode=m_axi depth=22 port=source1 bundle=source1
#pragma HLS INTERFACE mode=m_axi depth=22 port=source2 bundle=source2
#pragma HLS INTERFACE mode=m_axi depth=22 port=source3 bundle=source3
#pragma HLS INTERFACE mode=m_axi depth=22 port=source4 bundle=source4
#pragma HLS INTERFACE mode=m_axi depth=22 port=source5 bundle=source5
#pragma HLS INTERFACE mode=m_axi depth=22 port=source6 bundle=source6
#pragma HLS INTERFACE mode=m_axi depth=22 port=source7 bundle=source7
#pragma HLS INTERFACE mode=m_axi depth=22 port=source8 bundle=source8

#pragma HLS INTERFACE mode=s_axilite bundle=ctr port=return

	hls::stream<uint32_t> to1, to2, to3, to4, to5, to6, to7, to8;

	for (uint32_t i=0; i<1000; i++){
#pragma HLS DATAFLOW
		reader(source1, &to1);
		reader(source2, &to2);
		reader(source3, &to3);
		reader(source4, &to4);
		reader(source5, &to5);
		reader(source6, &to6);
		reader(source7, &to7);
		reader(source8, &to8);

		merger_sum(&to1, &to2, &to3, &to4, &to5, &to6, &to7, &to8);
	}
}

void channel_reader(uint32_t* from, hls::stream<uint32_t>* to, hls::stream<uint32_t>* channel_idle,
		hls::stream<uint32_t>* job){

	channel_idle->write(0);

	for (uint32_t i=0; i<2; i++) {
#pragma HLS PIPELINE off
		uint32_t tj = job->read();
		uint32_t sum = 0;
		for (uint32_t j=tj*22; j<(22+tj*22); j++){
#pragma HLS PIPELINE
			sum += from[j];
		}
		to->write(sum);
		if (i==0)
			channel_idle->write(0);
	}
}

void allocator(hls::stream<uint32_t>* channel_idle1, hls::stream<uint32_t>* channel_idle2, hls::stream<uint32_t>* channel_idle3,
		hls::stream<uint32_t>* channel_idle4, hls::stream<uint32_t>* job1, hls::stream<uint32_t>* job2, hls::stream<uint32_t>* job3,
		hls::stream<uint32_t>* job4){

	for (uint32_t i=0; i<8; ){
		if (!channel_idle1->empty() || !channel_idle2->empty() || !channel_idle3->empty() || !channel_idle4->empty()) {

			if (!channel_idle1->empty()) {
				channel_idle1->read();
				job1->write(i);
			} else if (!channel_idle2->empty()) {
				channel_idle2->read();
				job2->write(i);
			} else if (!channel_idle3->empty()) {
				channel_idle3->read();
				job3->write(i);
			} else if (!channel_idle4->empty()) {
				channel_idle4->read();
				job4->write(i);
			}
			i++;
		}
#ifndef __SYNTHESIS__
		i++;
#endif
	}
}

void merger_allo(hls::stream<uint32_t>* from1, hls::stream<uint32_t>* from2, hls::stream<uint32_t>* from3,
		hls::stream<uint32_t>* from4){

	for (uint32_t i=0; i<2; i++){
		from1->read();
		from2->read();
		from3->read();
		from4->read();
	}
}

void allocchannel(uint32_t* source1, uint32_t* source2, uint32_t* source3, uint32_t* source4){

#pragma HLS INTERFACE mode=m_axi depth=22 port=source1 bundle=source1
#pragma HLS INTERFACE mode=m_axi depth=22 port=source2 bundle=source2
#pragma HLS INTERFACE mode=m_axi depth=22 port=source3 bundle=source3
#pragma HLS INTERFACE mode=m_axi depth=22 port=source4 bundle=source4

#pragma HLS INTERFACE mode=s_axilite bundle=ctr port=return

	hls::stream<uint32_t> to1, to2, to3, to4;
	hls::stream<uint32_t> idle1, idle2, idle3, idle4;
	hls::stream<uint32_t> job1, job2, job3, job4;

	for (uint32_t i=0; i<1000; i++){
#pragma HLS DATAFLOW
		allocator(&idle1, &idle2, &idle3, &idle4, &job1, &job2, &job3, &job4);
		channel_reader(source1, &to1, &idle1, &job1);
		channel_reader(source2, &to2, &idle2, &job2);
		channel_reader(source3, &to3, &idle3, &job3);
		channel_reader(source4, &to4, &idle4, &job4);
		merger_allo(&to1, &to2, &to3, &to4);
	}
}


