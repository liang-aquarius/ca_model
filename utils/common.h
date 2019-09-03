//
// Created by chenliang on 2019/8/14.
//

#ifndef CA_FIFO_COMMON_H
#define CA_FIFO_COMMON_H
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <time.h>
using namespace std;

extern int cycle;
extern ofstream vcd_file;
extern bool time_record;

class ca_signal{
public:
    void* data;
    void* old_data;
    uint8_t len;
    string name;
    ca_signal() = default;
    ca_signal(void* d, void* o, uint8_t l, string na) :
        data(d), old_data(o), len(l), name(move(na)){}
};


void begin_init_vcd_file(const string &file_name, ofstream &vcd_file); // open file and write header
void begin_init_signals(ofstream &file, const string &module_name, vector<ca_signal> signals, vector<ca_signal> inputs);
void end_init_signals(ofstream &file);
void end_init_vcd_file(ofstream &vcd_file); // open file and write header
void dump_signals(ofstream& vcd_file, vector<ca_signal> signals, bool is_input);



#endif //CA_FIFO_COMMON_H
