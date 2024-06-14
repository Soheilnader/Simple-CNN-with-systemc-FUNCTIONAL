#include "CNN_FUNC_TB.h"

int sc_main(int argc, char* argv[]) {

	cnn_func_TB* TOP = new cnn_func_TB("TB");


	sc_trace_file* VCDFile;
	VCDFile = sc_create_vcd_trace_file("Waveform");
	sc_trace(VCDFile, TOP->rst, "rst");
	sc_trace(VCDFile, TOP->clk, "clk");
	sc_trace(VCDFile, TOP->start, "start");
	sc_trace(VCDFile, TOP->done, "done");
	sc_trace(VCDFile, TOP->pattern, "pattern");

	cout << "START\n";
	sc_start(2000, SC_NS);
	cout << "DONE\n";

	return 0;
}