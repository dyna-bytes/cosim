#include <iostream>
#include "systemc.h"

// Define the data structure for the packet
typedef struct {
    int addr;
    int data;
} packet_t;

// Define the interface for packet communication
// This class defines the pure virtual functions that the channel must implement.
class my_packet_if : public virtual sc_interface {
public:
    virtual void read(packet_t *rd_pkt) = 0;
    virtual void write(packet_t *wr_pkt) = 0;
};

// Define the channel for packet communication
// It inherits from sc_channel and implements the my_packet_if interface.
class my_channel : public sc_channel, public my_packet_if {
public:
    // This is the internal storage for the data being passed through the channel.
    packet_t my_data;

    // Constructor for the channel
    SC_CTOR(my_channel) {}

    // Implementation of the read method from the interface
    // It copies the channel's internal data to the provided packet pointer.
    virtual void read(packet_t *rd_pkt) override {
        rd_pkt->addr = my_data.addr;
        rd_pkt->data = my_data.data;
    }

    // Implementation of the write method from the interface
    // It copies the data from the provided packet pointer to the channel's internal data.
    virtual void write(packet_t *wr_pkt) override {
        my_data.addr = wr_pkt->addr;
        my_data.data = wr_pkt->data;
    }
};

// Define the sender module
class sender : public sc_module {
public:
    // The port is of type my_packet_if, not a template.
    sc_port<my_packet_if> out_port;

    int s_addr;
    int s_data;

    SC_HAS_PROCESS(sender);
    sender(sc_module_name name) : sc_module(name) {
        SC_THREAD(process);
        // Sensitivity is not needed here because the process is a free-running
        // thread controlled by wait statements, not events on the port.

        s_addr = 0x1234;
        s_data = 0xABCD;
    }

    void process() {
        packet_t pkt;

        while (true) {
            pkt.addr = s_addr;
            pkt.data = s_data;

            // Write the packet to the channel via the output port
            out_port->write(&pkt);
            std::cout << sc_time_stamp() << ": Sender: Sent packet: addr = 0x" << std::hex << pkt.addr
                      << ", data = 0x" << std::hex << pkt.data << std::endl;
            // Wait for 10 nanoseconds before sending the next packet
            wait(10, SC_NS);
        }
    }
};

// Define the receiver module
class receiver : public sc_module {
public:
    // The port is of type my_packet_if, not a template.
    sc_port<my_packet_if> in_port;

    SC_HAS_PROCESS(receiver);
    receiver(sc_module_name name) : sc_module(name) {
        SC_THREAD(process);
        // Sensitivity is not needed here. The thread will call read()
        // and then wait, controlling its own execution.
    }

    void process() {
        packet_t pkt;
        while (true) {
            // Wait for 10 nanoseconds to align with the sender's timing.
            // This ensures the receiver reads after the sender writes.
            wait(10, SC_NS);
            // Read the packet from the channel via the input port
            in_port->read(&pkt);
            std::cout << sc_time_stamp() << ": Receiver: Received packet: addr = 0x" << std::hex << pkt.addr
                      << ", data = 0x" << std::hex << pkt.data << std::endl;
        }
    }
};

// The main function to set up and run the simulation
int sc_main(int argc, char* argv[]) {
    // Instantiate the channel, sender, and receiver
    my_channel channel("channel");
    sender u_sender("sender");
    receiver u_receiver("receiver");

    // Connect the ports to the channel
    u_sender.out_port(channel);
    u_receiver.in_port(channel);

    // Run the simulation for 10 nanoseconds
    sc_start(10, SC_NS);

    u_sender.s_addr = 0x5678; // Change sender address
    u_sender.s_data = 0xEF01; // Change sender data
    sc_start(10, SC_NS); // Continue the simulation for another 10 nanoseconds

    u_sender.s_addr = 0x9ABC; // Change sender address again
    u_sender.s_data = 0xDEF0; // Change sender data again
    sc_start(10, SC_NS); // Continue the simulation for another 10 nanoseconds

    return 0;
}
