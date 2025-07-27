#include <stdio.h>
#include "systemc.h"

class Adder : public sc_module {
public:
    sc_in<int> a;
    sc_in<int> b;
    sc_out<int> y;

    SC_HAS_PROCESS(Adder); // Macro to declare the process

    Adder(sc_module_name name) : sc_module(name) {
        SC_THREAD (main); // verilog-style always block
        sensitive << a << b; // Sensitivity list for the process : always@()
    }

    void main() {
        while (true) {
            y = a + b; // Perform addition
            wait(); // preemptively wait
        }
    }
};

int sc_main(int argc, char* argv[]) {
    sc_signal<int> a;
    sc_signal<int> b;
    sc_signal<int> y;

    Adder adder("Adder");
    adder.a(a);
    adder.b(b);
    adder.y(y);

    a = 10;
    b = 20;
    sc_start(100, SC_NS); // Start the simulation for 100 nanosecond

    printf("%d + %d = %d\n", a.read(), b.read(), y.read());
    return 0;
}