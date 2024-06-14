#include <iostream>
#include <systemc.h>

template <int NumClk, int N>
SC_MODULE(cnn_func)
{
	//sc_in<int> XBus;
	sc_in<sc_logic> clk;
	sc_in<sc_logic> rst;
	sc_in<sc_logic> start;
	sc_out<sc_lv<3>> pattern;
	sc_out<sc_logic> done;

	sc_signal<int> val[4];

	SC_CTOR(cnn_func)
	{
		SC_THREAD(operation)
			sensitive << clk << rst;
	}
	void operation();
};


template <int NumClk, int N>
void cnn_func<NumClk, N>::operation()
{
	while (true)
	{
		float term = 1;
		float exp = 1;

		if (rst == '1')
		{
			ready = SC_LOGIC_0;
			RBus = 0;
		}

		else if ((clk == '1') && (clk->event()))
		{
			if (go == '1')
			{
				ready = SC_LOGIC_0;
				RBus = 0;

				XBus_reg = XBus->read();
				wait(clk->posedge_event());

				for (int i = 1; i < N; i++) {
					term = term * XBus_reg.read() * (1 / float(i));
					exp = exp + term;
				}

				for (int i = 0; i < NumClk - 1; i++)
					wait(clk->posedge_event());

				ready = SC_LOGIC_1;
				RBus = exp;
			}
		}
		wait();
	}
}

