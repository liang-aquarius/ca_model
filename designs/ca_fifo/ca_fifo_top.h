//
// Created by chenliang on 2019/8/16.
//
#ifndef CA_FIFO_CA_FIFO_TOP_H
#define CA_FIFO_CA_FIFO_TOP_H

#include "ca_fifo_ctrl.h"
#include "ca_fifo_mem.h"

template <uint32_t id, uint32_t depth, typename T>
class ca_fifo_top : ca_module {
    //////////define input struct/////////////////
    struct ca_fifo_top_input {
        bool clk;
        bool reset_n;
        bool read_en;
        bool write_en;
        T data_in;
    };

    //////////define output struct/////////////////
    struct ca_fifo_top_output {
        bool empty_w;
        bool full_w;
        bool data_valid_w;
        T data_out_w;

    };

public:
    /////////////////constructors//////////////
    ca_fifo_top() : trace(false) {
        u_ca_fifo_ctrl = new ca_fifo_ctrl<id, depth, T>(false);
        u_ca_fifo_mem = new ca_fifo_mem<id, depth, T>(false);
    }
    explicit ca_fifo_top (bool trace_value){
        trace = trace_value;
        if (trace_value) {    ////////////add signals for dump wave/////////////
            signals.emplace_back(ca_signal(&ca_fifo_top_input_i.read_en, &ca_fifo_top_input_old.read_en, 1, "read_en_i"));
            signals.emplace_back(ca_signal(&ca_fifo_top_input_i.write_en, &ca_fifo_top_input_old.write_en, 1, "write_en_i"));
            signals.emplace_back(ca_signal(&ca_fifo_top_input_i.reset_n, &ca_fifo_top_input_old.reset_n, 1, "reset_n"));
            signals.emplace_back(ca_signal(&ca_fifo_top_input_i.data_in, &ca_fifo_top_input_old.data_in, 32, "data_in"));

            signals.emplace_back(ca_signal(&ca_fifo_top_output_o.empty_w, &ca_fifo_top_output_old.empty_w, 1, "empty_w"));
            signals.emplace_back(ca_signal(&ca_fifo_top_output_o.full_w, &ca_fifo_top_output_old.full_w, 1, "full_w"));
            signals.emplace_back(ca_signal(&ca_fifo_top_output_o.data_valid_w, &ca_fifo_top_output_old.data_valid_w, 1, "data_valid_w"));
            signals.emplace_back(ca_signal(&ca_fifo_top_output_o.data_out_w, &ca_fifo_top_output_old.data_out_w, 32, "data_out_w"));


            /////////////////////add sub module signals///////////////////////////
            begin_init_signals(vcd_file, "ca_fifo_top", signals);
            u_ca_fifo_ctrl = new ca_fifo_ctrl<id, depth, T>(trace_value);
            u_ca_fifo_mem = new ca_fifo_mem<id, depth, T>(trace_value);
            end_init_signals(vcd_file);
        } else {
            u_ca_fifo_ctrl = new ca_fifo_ctrl<id, depth, T>(false);
            u_ca_fifo_mem = new ca_fifo_mem<id, depth, T>(false);
        }
    }

    ca_fifo_top_output ca_fifo_top_output_o;
    ca_fifo_top_input ca_fifo_top_input_i;

    //////////////////interface functions///////////////////
    bool is_trace() override {return trace;}
    void run() override;
    void dump_sigs(ofstream &file) override;
    //void dump_inputs(ofstream &file) override;
    void update() override;
    void connect_submod() override;

private:
    //////////////////sub modules/////////////
    ca_fifo_ctrl<id, depth, T> *u_ca_fifo_ctrl;
    ca_fifo_mem<id, depth, T> *u_ca_fifo_mem;


    ///////////////local variables, _r for regs and _w for wires////////////////
    ca_fifo_top_output ca_fifo_top_output_old;
    ca_fifo_top_input ca_fifo_top_input_old;

    ///////////////local variables for dump vcd///////////////
    bool trace;
    vector<ca_signal> signals;
};


template <uint32_t id, uint32_t depth, typename T>
void ca_fifo_top<id, depth, T> ::connect_submod(){
    u_ca_fifo_mem->ca_fifo_mem_input_i.clk = ca_fifo_top_input_i.clk;
    u_ca_fifo_mem->ca_fifo_mem_input_i.db_data = u_ca_fifo_ctrl->ca_fifo_ctrl_output_o.db_data;
    u_ca_fifo_mem->ca_fifo_mem_input_i.addr = u_ca_fifo_ctrl->ca_fifo_ctrl_output_o.addr_r;
    u_ca_fifo_mem->ca_fifo_mem_input_i.wen = u_ca_fifo_ctrl->ca_fifo_ctrl_output_o.wen_r;
    u_ca_fifo_mem->ca_fifo_mem_input_i.cen = u_ca_fifo_ctrl->ca_fifo_ctrl_output_o.cen_r;

    u_ca_fifo_ctrl->ca_fifo_ctrl_input_i.clk = ca_fifo_top_input_i.clk;
    u_ca_fifo_ctrl->ca_fifo_ctrl_input_i.reset_n = ca_fifo_top_input_i.reset_n;
    u_ca_fifo_ctrl->ca_fifo_ctrl_input_i.db_data = ca_fifo_top_input_i.data_in;

    u_ca_fifo_ctrl->ca_fifo_ctrl_input_i.write_en = ca_fifo_top_input_i.write_en;
    u_ca_fifo_ctrl->ca_fifo_ctrl_input_i.read_en = ca_fifo_top_input_i.read_en;
    u_ca_fifo_ctrl->ca_fifo_ctrl_input_i.q_data = u_ca_fifo_mem->ca_fifo_mem_output_o.q_data;

    u_ca_fifo_ctrl->connect_submod();
    u_ca_fifo_mem->connect_submod();
}

template <uint32_t id, uint32_t depth, typename T>
void ca_fifo_top<id, depth, T> :: run () {
    /////////////////////combination logic/////////////////////


    ////////////////////sequencial logic///////////////////////

    ///////////////////////sub-modules///////////////////////////
    u_ca_fifo_mem->run();
    u_ca_fifo_ctrl->run();

    //////////////////cycle n combination logic, just for dump wave////////////////////

    //////////////////updata combination output//////////////////

    ca_fifo_top_output_o.empty_w = u_ca_fifo_ctrl->ca_fifo_ctrl_output_o.empty_w;
    ca_fifo_top_output_o.full_w = u_ca_fifo_ctrl->ca_fifo_ctrl_output_o.full_w;
    ca_fifo_top_output_o.data_out_w = u_ca_fifo_ctrl->ca_fifo_ctrl_output_o.data_out_r;
    ca_fifo_top_output_o.data_valid_w = u_ca_fifo_ctrl->ca_fifo_ctrl_output_o.data_valid_r;
}

template <uint32_t id, uint32_t depth, typename T>
void ca_fifo_top<id, depth, T> :: dump_sigs(ofstream &file){
    if (is_trace()) {
        dump_signals(file, signals);
    } else {
        cout << "Warning: trace is disabled in ca_fifo_top, id: " << id << endl;
    }

    if (u_ca_fifo_ctrl->is_trace()) {
        u_ca_fifo_ctrl->dump_sigs(vcd_file);
    }
    if (u_ca_fifo_mem->is_trace()) {
        u_ca_fifo_mem->dump_sigs(vcd_file);
    }
}

template <uint32_t id, uint32_t depth, typename T>
void ca_fifo_top<id, depth, T> :: update () {
    ca_fifo_top_output_old = ca_fifo_top_output_o;
    ca_fifo_top_input_old = ca_fifo_top_input_i;
    u_ca_fifo_ctrl->update();
    u_ca_fifo_mem->update();
}
#endif //CA_FIFO_CA_FIFO_TOP_H
