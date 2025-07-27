#include <iostream>
#include "systemc.h"

using namespace std;

class my_if : virtual public sc_interface {
public:
    virtual void display() = 0;
};

class my_channel : public sc_channel, public my_if {
public:
    SC_CTOR(my_channel) {}
    void display() override {
        cout << sc_time_stamp() << " my_channel display method called." << endl;
    }
};

// Module B: Export channel my_interface through my_if
// : A module 에서 sc_export 를 통해 my_if 인터페이스를 내보냄
SC_MODULE(B) {
public:
    sc_export<my_if> my_export;
    SC_CTOR(B) : my_export("my_export") {
        my_channel* channel = new my_channel("channel");
        my_export.bind(*channel);
    }
};

// Module A: Module A is connected to the export of module B
// : A 모듈은 B 모듈의 export에 연결됨
SC_MODULE(A) {
public:
    sc_port<my_if> my_port;

    SC_CTOR(A) {
        SC_THREAD(run);
    }
    void run() {
        while (true) {
            wait(10, SC_NS);
            my_port->display();
        }
    }
};

int sc_main(int argc, char* argv[]) {
    A my_A("my_A");
    B my_B("my_B");

    // port->export 연결
    my_A.my_port(my_B.my_export);
    sc_start(100, SC_NS);
    return 0;
}