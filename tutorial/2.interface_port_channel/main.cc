#include <stdio.h>
#include "systemc.h"

class Adder : public sc_module {
    void process() {
        while (true) {
            y = a + b;
            wait(); // Wait for a change in inputs
        }
    }
public:
    sc_in<int> a, b;
    sc_out<int> y;

    SC_HAS_PROCESS(Adder);
    Adder(sc_module_name name) : sc_module(name) {
        SC_THREAD(process);
        sensitive << a << b;
    }
};

class Multiplier : public sc_module {
    void process() {
        while (true) {
            y = a * b;
            wait(); // Wait for a change in inputs
        }
    }
public:
    sc_in<int> a, b;
    sc_out<int> y;

    SC_HAS_PROCESS(Multiplier);
    Multiplier(sc_module_name name) : sc_module(name) {
        SC_THREAD(process);
        sensitive << a << b;
    }
};

int sc_main(int argc, char* argv[]) {
    sc_signal<int> a0, b0, y0;
    sc_signal<int> a1, b1, y1;

    sc_signal<int> mult;

    Adder u_adder0 ("adder0");
    Adder u_adder1 ("adder1");
    Multiplier u_multiplier ("multiplier");

    u_adder0.a(a0);
    u_adder0.b(b0);
    u_adder0.y(y0);

    u_adder1.a(a1);
    u_adder1.b(b1);
    u_adder1.y(y1);

    u_multiplier.a(y0);
    u_multiplier.b(y1);
    u_multiplier.y(mult);

    // Initialize inputs
    a0 = 10;
    b0 = 20;
    a1 = 2;
    b1 = 5;

    sc_start(1, SC_NS); // Start the simulation for 1 nanosecond

    printf("Adder 0 : %d + %d = %d\n", a0.read(), b0.read(), y0.read());
    printf("Adder 1 : %d + %d = %d\n", a1.read(), b1.read(), y1.read());
    printf("Multiplier : %d * %d = %d\n", y0.read(), y1.read(), mult.read());
    return 0;
}