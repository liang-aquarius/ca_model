//
// Created by chenliang on 2019/8/14.
//

#ifndef CA_FIFO_CA_FIFO_MEM_H
#define CA_FIFO_CA_FIFO_MEM_H

#include <stdint-gcc.h>
#include "ca_module.h"

template <uint32_t id, uint32_t depth, typename T>
class ca_fifo_mem : public ca_module {

    struct ca_fifo_mem_input {
        bool clk;
        bool cen;
        bool wen;
        uint16_t addr;
        T db_data;
    };

    struct ca_fifo_mem_output {
        T q_data;
    };

public:
    ca_fifo_mem() : trace(0) {}
    explicit ca_fifo_mem(bool trace_value) {
        trace = trace_value;
        if (trace_value) {
            inputs.emplace_back(ca_signal(&ca_fifo_mem_input_i.wen, &ca_fifo_mem_input_old.wen, 1, "wen"));
            inputs.emplace_back(ca_signal(&ca_fifo_mem_input_i.cen, &ca_fifo_mem_input_old.cen, 1, "cen"));
            inputs.emplace_back(ca_signal(&ca_fifo_mem_input_i.addr, &ca_fifo_mem_input_old.addr, 13, "addr"));
            inputs.emplace_back(ca_signal(&ca_fifo_mem_input_i.db_data, &ca_fifo_mem_input_old.db_data, 32, "db_data"));
            signals.emplace_back(ca_signal(&ca_fifo_mem_output_o.q_data, &ca_fifo_mem_output_old.q_data, 32, "q_data"));
            begin_init_signals(vcd_file, "ca_fifo_mem", signals,inputs);
            end_init_signals(vcd_file);
        }
    }
    bool is_trace() override {return trace;}
    void update() override;
    void dump_sigs(ofstream &file) override;
    void dump_inputs(ofstream &file) override;

    ca_fifo_mem_input ca_fifo_mem_input_i;
    ca_fifo_mem_output ca_fifo_mem_output_o;

    void run() override;

private:
    bool trace;
    vector<ca_signal> signals;
    vector<ca_signal> inputs;

    ca_fifo_mem_input ca_fifo_mem_input_old;
    ca_fifo_mem_output ca_fifo_mem_output_old;

    T mem[depth];
};

template <uint32_t id, uint32_t depth, typename T>
void ca_fifo_mem<id, depth, T> :: update () {
    ca_fifo_mem_input_old = ca_fifo_mem_input_i;
    ca_fifo_mem_output_old = ca_fifo_mem_output_o;
}

template <uint32_t id, uint32_t depth, typename T>
void ca_fifo_mem<id, depth, T> ::dump_sigs(ofstream &file){
    if (is_trace()) {
        dump_signals(file, signals, false);
    } else {
        cout << "Warning: trace is disabled in ca_fifo_mem, id: " << id << endl;
    }
}

template <uint32_t id, uint32_t depth, typename T>
void ca_fifo_mem<id, depth, T> ::dump_inputs(ofstream &file){
    if (is_trace()) {
        dump_signals(file, inputs, true);
    } else {
        cout << "Warning: trace is disabled in ca_fifo_mem, id: " << id << endl;
    }
}

template <uint32_t id, uint32_t depth, typename T>
void ca_fifo_mem<id, depth, T> :: run () {
    /////////////////////combination logic/////////////////////

    ////////////////////sequencial logic///////////////////////
    if (ca_fifo_mem_input_i.clk) {
        if (ca_fifo_mem_input_i.wen == 0 && ca_fifo_mem_input_i.cen == 0) {
            mem[ca_fifo_mem_input_i.addr] = ca_fifo_mem_input_i.db_data;
        } else if (ca_fifo_mem_input_i.cen == 0) {
            ca_fifo_mem_output_o.q_data = mem[ca_fifo_mem_input_i.addr];
        }
    }

    //////////////////updata combination output//////////////////

}

#endif //CA_FIFO_CA_FIFO_MEM_H
