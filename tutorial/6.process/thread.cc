#include <iostream>
#include "systemc.h"

SC_MODULE(my_module) {
public:
    sc_in_clk clk;
    sc_in<int> in;
    sc_out<int> out;

    SC_CTOR(my_module) {
        SC_THREAD(my_thread);
        sensitive << clk.pos();
    }

    void my_thread() {
        int input;
        int output;

        while (true) {
            wait(); // Wait for the clock edge
            input = in.read();
            output = input + 1;
            out.write(output);
            std::cout << "Thread executed at time: " << sc_time_stamp() << std::endl;
            std::cout << "Input: " << input << ", Output: " << output << std::endl;
        }
    }
};

int sc_main(int, char *[]) {
    sc_clock clk("clk", 5, SC_NS);
    sc_signal<int> sig_in;
    sc_signal<int> sig_out;

    my_module mod("my_module");
    mod.clk(clk);
    mod.in(sig_in);
    mod.out(sig_out);

    sig_in = 0;
    sc_start(1, SC_NS);
    sig_in = 5;
    sc_start(5, SC_NS);
    sig_in = 10;
    sc_start(5, SC_NS);

    return 0;
}