#include <iostream>
#include <systemc.h>

using namespace std;

SC_MODULE(Counter) { //class
    sc_in<bool> clk; 
    sc_in<bool> rst;
    sc_in<bool> en;
    sc_out<sc_uint<4>> cnt_out;

    sc_uint<4> cnt;//local var

    void count() { //method to count
        if (rst.read()) {
            cnt = sc_uint<4> (0);
            cnt_out = cnt;
        }
        else if (en.read()) {
            cnt += (sc_uint < 4> (1));
            cnt_out.write(cnt);
            cout << "counter @ "<<sc_time_stamp() << "val: " << cnt_out.read();
        }
    }
    //constructor
    SC_CTOR(Counter) {
        cnt = sc_uint <4> (0);
        SC_METHOD(count);
        sensitive << rst << clk.pos();
    }
};

int sc_main(int argc, char* argv[]) //testbench
{
    
    sc_signal<bool> clk;
    sc_signal<bool> rst;
    sc_signal<bool> en;
    sc_signal<sc_uint<4>> cnt_out;

    //instantiate
    Counter counter("counter");
    //bind the ports
    counter.clk(clk);
    counter.rst(rst);
    counter.en(en);
    counter.cnt_out(cnt_out);

    //create a trace file to capture w/f
    sc_start(1, SC_NS);

    // Open VCD file
    sc_trace_file* wf = sc_create_vcd_trace_file("counter");
    // Dump the desired signals
    sc_trace(wf, clk, "clock");
    sc_trace(wf, rst, "reset");
    sc_trace(wf, en, "enable");
    sc_trace(wf, cnt_out, "count");

    rst = 0;
    en = 0;
    for (int i = 0; i < 5; i++) {
       clk = 0;
       sc_start(1, SC_NS);
       clk = 1;
       sc_start(1, SC_NS);
    }

    rst = 1;
    for (int i = 0; i < 10; i++) {
       clk = 0;
       sc_start(1, SC_NS);
       clk = 1;
       sc_start(1, SC_NS);
    }
    rst = 0;

    for (int i = 0;i < 5;i++) {
       clk = 0;
       sc_start(1, SC_NS);
       clk = 1;
       sc_start(1, SC_NS);
    }

    en = 1;  // Assert enable
    for (int i = 0;i < 20;i++) {
       clk = 0;
       sc_start(1, SC_NS);
       clk = 1;
       sc_start(1, SC_NS);
    }
    en = 0; // De-assert enable
    sc_close_vcd_trace_file(wf);
    sc_stop();

    cout << "Hello";

    return 0;
}



///code by chat gpt//////////////////////////////

#include <systemc.h>

SC_MODULE(counter) {
    sc_in_clk clk;
    sc_in<bool> reset;
    sc_in<bool> enable;
    sc_out<int> clk_out;

    int count;

    void count_process() {
        while (true) {
            if (reset.read() == true) {
                count = 0;
                clk_out.write(count);
            } else if (enable.read() == true) {
                count++;
                if (count == 11) {
                    count = 1;
                }
                clk_out.write(count);
            }
            wait();
        }
    }

    SC_CTOR(counter) {
        count = 0;
        SC_THREAD(count_process);
        sensitive << clk.pos();
    }
};

int sc_main(int argc, char* argv[]) {
    sc_signal<bool> reset;
    sc_signal<bool> enable;
    sc_signal<int> clk_out;
    sc_clock clk("clk", 10, SC_NS);

    counter my_counter("my_counter");
    my_counter.clk(clk);
    my_counter.reset(reset);
    my_counter.enable(enable);
    my_counter.clk_out(clk_out);

    reset.write(false);
    enable.write(false);
    sc_start(0, SC_NS);

    reset.write(true);
    sc_start(10, SC_NS);

    reset.write(false);
    enable.write(true);

    for (int i = 0; i < 20; i++) {
        sc_start(10, SC_NS);
    }

    reset.write(true);
    enable.write(false);
    sc_start(10, SC_NS);

    reset.write(false);
    sc_start(10, SC_NS);

    return 0;
}
