#include "CNN_FUNC_TB.h"

void cnn_func_TB::resetting()
{
	while (true)
	{
		wait(7, SC_NS);
		rst = SC_LOGIC_0;
		wait(11, SC_NS);
		rst = SC_LOGIC_1;
		wait(58, SC_NS);
		rst = SC_LOGIC_0;
		wait();
	}
}

void cnn_func_TB::clocking()
{
	while (true)
	{
		wait(17, SC_NS);
		clk = SC_LOGIC_0;
		wait(17, SC_NS);
		clk = SC_LOGIC_1;
	}
}

void cnn_func_TB::inGenerating()
{
	while (true)
	{
		start = SC_LOGIC_0;
		wait(60, SC_NS);
		start = SC_LOGIC_1;
		wait(60, SC_NS);
		start = SC_LOGIC_0;

		wait();
	}
}

void cnn_func_TB::displaying()
{
	while (true){
		if (done == '1')
			cout << "Pattern: " << pattern <<"   in: " << sc_time_stamp() << "\n" << endl;
		wait();
	}
}

