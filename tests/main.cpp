#include <iostream>

#include "ca_fifo_top.h"
#include "driver_monitor.h"

ca_fifo_top<0,1024,uint32_t> * u_ca_fifo_top;
driver_monitor<0,1024,uint32_t> * u_driver_monitor;

using namespace std;

void connect_modules() {
    u_ca_fifo_top->ca_fifo_top_input_i.clk = u_driver_monitor->driver_monitor_output_o.clk;
    u_ca_fifo_top->ca_fifo_top_input_i.reset_n = u_driver_monitor->driver_monitor_output_o.reset_n;
    u_ca_fifo_top->ca_fifo_top_input_i.read_en = u_driver_monitor->driver_monitor_output_o.read_en;
    u_ca_fifo_top->ca_fifo_top_input_i.write_en = u_driver_monitor->driver_monitor_output_o.write_en;
    u_ca_fifo_top->ca_fifo_top_input_i.data_in = u_driver_monitor->driver_monitor_output_o.data_write;
    //cout << "cycle=" << cycle << " driver_monitor_output_o.data_write=" << u_driver_monitor.driver_monitor_output_o.data_write << endl;

    u_driver_monitor->driver_monitor_input_i.full = u_ca_fifo_top->ca_fifo_top_output_o.full_w;
    u_driver_monitor->driver_monitor_input_i.empty = u_ca_fifo_top->ca_fifo_top_output_o.empty_w;
    u_driver_monitor->driver_monitor_input_i.data_valid = u_ca_fifo_top->ca_fifo_top_output_o.data_valid_w;
    u_driver_monitor->driver_monitor_input_i.data_out = u_ca_fifo_top->ca_fifo_top_output_o.data_out_w;
}

int main() {

////////////////////vcd file init////////////////////////////////////
    begin_init_vcd_file("main_test.vcd", vcd_file);
    u_ca_fifo_top = new ca_fifo_top<0,1024,uint32_t>(true);
    u_driver_monitor = new driver_monitor<0,1024,uint32_t>;
    end_init_vcd_file(vcd_file);

    int num_cycles = 2048;

    for(int i=0; i< num_cycles; i++){
        //////////////////combinational logic & connect modules/////////////////

        connect_modules();
        /////////////////////////sequential logic/////////////////
        u_ca_fifo_top->run();
        u_driver_monitor->run();

        /////////////////////////////dump vcd file /////////////////////////
        if (u_ca_fifo_top->is_trace()) {
            u_ca_fifo_top->dump_inputs(vcd_file);
            time_record = false;

            u_ca_fifo_top->dump_sigs(vcd_file);
            time_record = false;
        }

        /////////////////////////////update values////////////////////////////
        u_ca_fifo_top->update();
        u_driver_monitor->update();

        cycle++;
    }
}