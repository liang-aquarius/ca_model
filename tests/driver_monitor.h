//
// Created by chenliang on 2019/8/16.
//

#ifndef CA_FIFO_DRIVER_MONITOR_H
#define CA_FIFO_DRIVER_MONITOR_H

#include <stdint-gcc.h>
#include "ca_module.h"


template <uint32_t id, uint32_t depth, typename T>
class driver_monitor : ca_module {
    //////////define output struct/////////////////
    struct driver_monitor_input {
        bool empty;
        bool full;
        bool data_valid;
        T data_out;
    };

    //////////define output struct/////////////////
    struct driver_monitor_output {
        bool clk;
        bool reset_n;

        bool read_en;
        bool write_en;
        T data_write;
    };

public:
    driver_monitor() = default;

    driver_monitor_input driver_monitor_input_i;
    driver_monitor_output driver_monitor_output_o;
    //////////////////interface functions///////////////////
    bool is_trace() override {}
    void run() override;
    void dump_sigs(ofstream &file) override {};
    void dump_inputs(ofstream &file) override {};
    void update() override ;
};
template <uint32_t id, uint32_t depth, typename T>
void driver_monitor<id, depth, T> :: run () {
    /////////////////////combination logic/////////////////////
    if (driver_monitor_input_i.data_valid)
        cout << "cycle= " << cycle << "\t" << "data_out= " << driver_monitor_input_i.data_out << endl;

        if (driver_monitor_input_i.full)
            cout << "at cycle " << cycle << " fifo full" << endl;

    if (driver_monitor_input_i.empty)
        cout << "at cycle " << cycle << " fifo empty" << endl;

    ////////////////////sequencial logic///////////////////////
    driver_monitor_output_o.clk = 1;
    if (cycle < 10) driver_monitor_output_o.reset_n = 0;
    else  driver_monitor_output_o.reset_n = 1;

    if (cycle < 1024) {
        driver_monitor_output_o.write_en = 1;
        driver_monitor_output_o.read_en = 0;
    } else {
        driver_monitor_output_o.write_en = 0;
        driver_monitor_output_o.read_en = 1;
    }

    driver_monitor_output_o.data_write = cycle;


    //////////////////updata combination output//////////////////
}


template <uint32_t id, uint32_t depth, typename T>
void driver_monitor<id, depth, T> :: update () {

}

#endif //CA_FIFO_DRIVER_MONITOR_H
