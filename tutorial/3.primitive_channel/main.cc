#include <stdio.h>
#include "systemc.h"

class Adder : public sc_module {
    void do_add() {
        while (true) {
            y.write(a.read() + b.read());
            wait();  // Wait for the next clock edge
        }
    }
public:
    sc_in_clk clk;
    sc_in<int> a, b;
    sc_fifo_out<int> y;

    SC_HAS_PROCESS(Adder);
    Adder(sc_module_name name) : sc_module(name) {
        SC_THREAD(do_add);
        sensitive << clk.pos();
    }
};

int sc_main(int argc, char* argv[]) {
    sc_signal<int> a, b;
    sc_fifo<int> y_fifo(10);  // Create a FIFO with a depth of 10
    sc_clock clk("clk", 10, SC_NS, 0.5, 0.0, SC_NS); // 10 ns clock with 50% duty cycle

    Adder u_adder("adder");

    u_adder.clk(clk);
    u_adder.a(a);
    u_adder.b(b);
    u_adder.y(y_fifo);

    a = 10;
    b = 20;
    sc_start(100, SC_NS);  // Start the simulation for 100 ns

    printf("%d + %d = %d\n", a.read(), b.read(), y_fifo.read());

    return 0;
}