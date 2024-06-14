#ifndef CNN_FUNC_H
#define CNN_FUNC_H

#include <iostream>
#include <systemc.h>
#include <fstream>

template <int NumClk>
SC_MODULE(cnn_func)
{
	sc_in<sc_logic> clk;
	sc_in<sc_logic> rst;
	sc_in<sc_logic> start;
	sc_out<sc_lv<3>> pattern;
	sc_out<sc_logic> done;

	sc_uint<8> kernel[3][9];

	sc_int<8> conv_out[3][4];
	sc_int<8> relu_out[3][4];
	sc_int<8> max_out[3];
	sc_int<8> temp;

	SC_CTOR(cnn_func)
	{
		// Initializing kernel array
		initialize_kernel();
		SC_THREAD(operation);
		sensitive << clk.pos() << rst;
	}

	void operation();

	void initialize_kernel() {
		sc_uint<8> temp_kernel[3][9] = {
			{ 0, 1, 0, 1, 1, 1, 0, 1, 0 },
			{ 1, 1, 1, 1, 0, 0, 1, 1, 1 },
			{ 1, 1, 1, 0, 1, 0, 0, 1, 0 }
		};
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 9; ++j) {
				kernel[i][j] = temp_kernel[i][j];
			}
		}
	}
};

template <int NumClk>
void cnn_func<NumClk>::operation()
{
	while (true)
	{
		if (rst == SC_LOGIC_1)
		{
			std::ifstream in("memin.txt");
			sc_uint<8>* mem = new sc_uint<8>[16];
			sc_lv<8> data;
			for (int i = 0; i < 16; i++) {
				in >> data;
				mem[i] = data.to_uint();
			}
			in.close();

			temp = 0;
			for (int i = 0; i < 3; i++) {
				max_out[i] = 0;
			}

			if (start == SC_LOGIC_1)
			{
				done = SC_LOGIC_0;
				const sc_int<8> bias[3] = { -1, -2, -2 };
				for (int k = 0; k < 3; k++) {
					for (int x = 0; x < 2; x++) {
						for (int y = 0; y < 2; y++) {
							for (int i = 0; i < 3; i++) {
								for (int j = 0; j < 3; j++) {
									temp += mem[j + 4 * i + y + 4 * x] * kernel[k][j + 3 * i];
								}
							}
							conv_out[k][y + 2 * x] = temp + bias[k];
							//std::cout << conv_out[k][y + 2 * x] << "\n";
							relu_out[k][y + 2 * x] = (conv_out[k][y + 2 * x] > 0 ? conv_out[k][y + 2 * x] : 0);
							max_out[k] = (max_out[k] > relu_out[k][y + 2 * x] ? max_out[k] : relu_out[k][y + 2 * x]);
							temp = 0;
						}
					}
					//std::cout << "max_out[" << k << "]: " << max_out[k] << "\n";
				}

				for (int i = 0; i < NumClk - 1; i++)
					wait(clk->posedge_event());

				if ((max_out[0] >= max_out[1]) && (max_out[0] >= max_out[2])) {
					pattern.write("001");
				}
				else if ((max_out[1] >= max_out[0]) && (max_out[1] >= max_out[2])) {
					pattern.write("010");
				}
				else if ((max_out[2] >= max_out[1]) && (max_out[2] >= max_out[0])) {
					pattern.write("100");
				}

				//std::cout << "pattern: " << pattern.read() << "\n";
				done.write(SC_LOGIC_1);
			}
			delete[] mem;
		}
		wait();
	}
}

#endif // CNN_FUNC_H
