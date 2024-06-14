#include "CNN_FUNC.h"

SC_MODULE(cnn_func_TB)
{
	sc_signal<sc_logic> clk, rst, start, done;
	sc_signal<sc_lv<3>> pattern;

	cnn_func<12>* cnn;

	SC_CTOR(cnn_func_TB)
	{
		
		cnn = new cnn_func<12>("CNN_Instance");
		cnn->clk(clk);
		cnn->rst(rst);
		cnn->start(start);
		cnn->done(done);
		cnn->pattern(pattern);
		

		SC_THREAD(resetting);
		SC_THREAD(clocking);
		SC_THREAD(inGenerating);
		SC_THREAD(displaying);
		sensitive << done;

	}
	void resetting();
	void clocking();
	void inGenerating();
	void displaying();
};
