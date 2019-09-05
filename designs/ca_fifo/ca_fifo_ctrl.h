//
// Created by chenliang on 2019/8/14.
//

#ifndef CA_FIFO_CA_FIFO_CTRL_H
#define CA_FIFO_CA_FIFO_CTRL_H

#include <stdint-gcc.h>
#include "ca_module.h"

template <uint32_t id, uint32_t depth, typename T>   /////n for instance id
class ca_fifo_ctrl : ca_module {

    //////////define input struct/////////////////
    struct ca_fifo_ctrl_input {
        bool clk;
        bool reset_n;
        bool read_en;
        bool write_en;
        T db_data;
        T q_data;
    };

    //////////define output struct/////////////////
    struct ca_fifo_ctrl_output {
        bool empty_w;
        bool full_w;
        bool data_valid_r;
        T data_out_r;
        bool wen_r;
        bool cen_r;
        uint16_t addr_r;
        T db_data;
    };

public:

    /////////////////constructors//////////////
    ca_fifo_ctrl() : trace(0) {}

    explicit ca_fifo_ctrl<id,depth,T>(bool trace_value){
        trace = trace_value;
        if (trace_value) {    ////////////add signals for dump wave/////////////
            signals.emplace_back(ca_signal(&rptr_r, &rptr_old, 13, "rptr_r"));
            signals.emplace_back(ca_signal(&wptr_r, &wptr_old, 13, "wptr_r"));

            signals.emplace_back(ca_signal(&ca_fifo_ctrl_input_i.read_en, &ca_fifo_ctrl_input_old.read_en, 1, "read_en_i"));
            signals.emplace_back(ca_signal(&ca_fifo_ctrl_input_i.write_en, &ca_fifo_ctrl_input_old.write_en, 1, "write_en_i"));
            signals.emplace_back(ca_signal(&ca_fifo_ctrl_input_i.reset_n, &ca_fifo_ctrl_input_old.reset_n, 1, "reset_n"));

            signals.emplace_back(ca_signal(&ca_fifo_ctrl_input_i.db_data, &ca_fifo_ctrl_input_old.db_data, 32, "db_data_in"));


            signals.emplace_back(ca_signal(&ca_fifo_ctrl_output_o.db_data, &ca_fifo_ctrl_output_old.db_data, 32, "db_data_out"));
            signals.emplace_back(ca_signal(&ca_fifo_ctrl_output_o.data_valid_r, &ca_fifo_ctrl_output_old.data_valid_r, 1, "data_valid_r"));
            signals.emplace_back(ca_signal(&ca_fifo_ctrl_output_o.data_out_r, &ca_fifo_ctrl_output_old.data_out_r, 32, "data_out_r"));
            begin_init_signals(vcd_file, "ca_fifo_ctrl", signals);
            end_init_signals(vcd_file);
        }
    }

    ca_fifo_ctrl_input ca_fifo_ctrl_input_i;
    ca_fifo_ctrl_output ca_fifo_ctrl_output_o;

    //////////////////interface functions///////////////////
    void connect_submod() override {};
    void run() override;
    void update() override;

    bool is_trace() override {return trace;}
    void dump_sigs(ofstream &file) override;

    ///////////////local variables, _r for regs and _w for wires////////////////
private:
    uint16_t rptr_r;
    uint16_t rptr_old;

    uint16_t wptr_r;
    uint16_t wptr_old;

    bool data_valid_b1_r;
    bool data_valid_b1_old;

    ca_fifo_ctrl_output ca_fifo_ctrl_output_old;
    ca_fifo_ctrl_input ca_fifo_ctrl_input_old;

    bool full_w;
    bool empty_w;

    bool full_w_old;
    bool empty_w_old;

    ///////////////local variables for dump vcd///////////////
    bool trace;
    vector<ca_signal> signals;
};

template <uint32_t id, uint32_t depth, typename T>
void ca_fifo_ctrl<id, depth, T> :: run () {

    /////////////////////cycle n-1 combination logic/////////////////////
    empty_w = rptr_r == wptr_r;
    full_w = (rptr_r / depth != wptr_r / depth) & (rptr_r % depth == wptr_r % depth);

    ////////////////////sequencial logic///////////////////////
    bool clk = ca_fifo_ctrl_input_i.clk;
    bool reset_n = ca_fifo_ctrl_input_i.reset_n;

    if (clk || !reset_n) {
        if (!reset_n)
            rptr_r = 0;
        else if (ca_fifo_ctrl_input_i.read_en)
            rptr_r = (rptr_r  == 2 * depth - 1) ? 0 : rptr_r + 1;

        if (!reset_n) {
            wptr_r = 0;
            ca_fifo_ctrl_output_o.wen_r = true;
        } else if (ca_fifo_ctrl_input_i.write_en) {
            wptr_r = (wptr_old == 2 * depth - 1) ? 0 : wptr_old + 1;
            ca_fifo_ctrl_output_o.wen_r = false;
        } else {
            ca_fifo_ctrl_output_o.wen_r = true;
        }

        if (!reset_n)
            ca_fifo_ctrl_output_o.cen_r = true;
        else if (ca_fifo_ctrl_input_i.read_en || ca_fifo_ctrl_input_i.write_en)
            ca_fifo_ctrl_output_o.cen_r = false;
        else
            ca_fifo_ctrl_output_o.cen_r = true;

        if (!reset_n)
            ca_fifo_ctrl_output_o.addr_r = 0;
        else if (ca_fifo_ctrl_input_i.write_en)
            ca_fifo_ctrl_output_o.addr_r = wptr_old%depth;
        else
            ca_fifo_ctrl_output_o.addr_r = rptr_old%depth;

        if (!reset_n)
            ca_fifo_ctrl_output_o.db_data = 0;
        else if (ca_fifo_ctrl_input_i.write_en)
            ca_fifo_ctrl_output_o.db_data = ca_fifo_ctrl_input_i.db_data;


        if (!reset_n)
            data_valid_b1_r = 0;
        else if (!ca_fifo_ctrl_output_old.cen_r && ca_fifo_ctrl_output_old.wen_r)
            data_valid_b1_r = 1;
        else
            data_valid_b1_r = 0;

        if (!reset_n)
            ca_fifo_ctrl_output_o.data_valid_r = 0;
        else
            ca_fifo_ctrl_output_o.data_valid_r = data_valid_b1_old;

        if (!reset_n)
            ca_fifo_ctrl_output_o.data_out_r = 0;
        else if (data_valid_b1_old)
            ca_fifo_ctrl_output_o.data_out_r = ca_fifo_ctrl_input_i.q_data;
    }


    //////////////////cycle n combination logic, just for dump wave////////////////////s
    empty_w = rptr_r == wptr_r;
    full_w = (rptr_r / depth != wptr_r / depth) & (rptr_r % depth == wptr_r % depth);

    //////////////////updata combination output//////////////////
    ca_fifo_ctrl_output_o.empty_w = empty_w;
    ca_fifo_ctrl_output_o.full_w = full_w;

}

template <uint32_t id, uint32_t depth, typename T>
void ca_fifo_ctrl<id, depth, T> ::dump_sigs(ofstream &file){
    if (is_trace()) {
        dump_signals(file, signals);
    } else {
        cout << "Warning: trace is disabled" << endl;
    }
}
template <uint32_t id, uint32_t depth, typename T>
void ca_fifo_ctrl<id, depth, T> :: update () {
    full_w_old = full_w;
    empty_w_old = empty_w;

    rptr_old = rptr_r;
    wptr_old = wptr_r;
    data_valid_b1_old = data_valid_b1_r;

    ca_fifo_ctrl_output_old = ca_fifo_ctrl_output_o;
    ca_fifo_ctrl_input_old = ca_fifo_ctrl_input_i;
}


#endif //CA_FIFO_CA_FIFO_CTRL_H
