#include <iostream>
#include "systemc.h"

SC_MODULE(my_module) {
public:
    sc_in<int> in;
    sc_out<int> out;

    SC_CTOR(my_module) {
        SC_METHOD(process_method);
        sensitive << in;
    }

    void process_method() {
        int input;
        int output;

        input = in;
        output = input + 1;
        out = output;
        std::cout << "Process method called at time: " << sc_time_stamp() << std::endl;
        std::cout << "Input: " << input << ", Output: " << output << std::endl;
    }
};

int sc_main(int argc, char* argv[]) {

    sc_signal<int> sig_in;
    sc_signal<int> sig_out;

    my_module mod("my_module");
    mod.in(sig_in);
    mod.out(sig_out);

    sig_in = 0;
    sc_start(1, SC_NS);
    sig_in = 5;
    sc_start(1, SC_NS);
    sig_in = 10;
    sc_start(1, SC_NS);

    return 0;
}