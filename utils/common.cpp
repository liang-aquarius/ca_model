//
// Created by chenliang on 2019/8/14.
//

#include "common.h"

//////global variables
int cycle = 0;
ofstream vcd_file;
bool time_record = false;

void dsptime(const struct tm * ptm, ofstream &file) {
    file << "\t" << ptm->tm_year + 1900 << "/" << std::setw(2) << setfill('0') << ptm->tm_mon + 1 << "/" << std::setw(2) << setfill('0') << ptm->tm_mday << " ";
    file << ptm->tm_hour << ":" << ptm->tm_min << ":" << ptm->tm_sec << " " << endl;
}

void begin_init_vcd_file(const string &file_name, ofstream &file){
    file.open(file_name);
    if (file.is_open()) {
        time_t nowtime;
        nowtime = time(NULL);
        struct tm *local;
        local=localtime(&nowtime);  //获取当前系统时间



        file << "$date" << endl;
        dsptime(local,file);
        file << "$end" << endl << endl;

        file << "$version" << endl;
        file << "\tCycle accurate model by Liang" << endl;
        file << "$end" << endl << endl;

        file << "$timescale" << endl;
        file << "\t1 ps" << endl;
        file << "$end" << endl << endl;

        file.flush();

    } else {
        cout << "can not open vcd file: " << file_name << endl;
        exit(1);
    }
}
void begin_init_signals(ofstream &file, const string &module_name, vector<ca_signal> signals, vector<ca_signal> inputs){
    if (file.is_open()) {
        file << endl << "$scope module " << module_name << " $end" << endl;
        for (auto iter = inputs.cbegin(); iter != inputs.cend(); iter++) {
            file << "$var reg\t" << (int) ((*iter).len) << "\t" << (*iter).name << "\t\t" << (*iter).name << "\t\t"
            << "$end" << endl;
        }
        for (auto iter = signals.cbegin(); iter != signals.cend(); iter++) {
            file << "$var reg\t" << (int) ((*iter).len) << "\t" << (*iter).name << "\t\t" << (*iter).name << "\t\t"
                 << "$end" << endl;
        }
        //file << "$upscope $end" << endl << endl;
        //file.flush();
    } else {
        cout << "can not open vcd file in begin_init_signal" << endl;
        exit(1);
    }
}
void end_init_signals(ofstream &file){
    if (file.is_open()) {
        file << "$upscope $end" << endl << endl;
        file.flush();
    } else {
        cout << "can not open vcd file in end_init_signal" << endl;
        exit(1);
    }
}

void end_init_vcd_file(ofstream &file) {
    if (file.is_open()) {
        file << "$enddefinitions  $end" << endl << endl;
    } else {
        cout << "In end_init_vcd_file, vcd file is not opened" << endl;
        exit(1);
    }
}

void dump_signals(ofstream& file, vector<ca_signal> signals, bool is_input){
    //time_record=false;
    for (auto iter = signals.cbegin(); iter != signals.cend(); iter++) {
        int len = (*iter).len;
        string name = (*iter).name;
        auto data_ptr = (uint64_t*) (*iter).data;
        auto old_data_ptr = (uint64_t*) (*iter).old_data;

        uint64_t data_tmp = (*data_ptr) % ( ( (uint64_t) 1) << ((uint8_t) len) );
        uint64_t old_data_tmp =  (*old_data_ptr) % ( ( uint64_t (1)) << ((uint8_t) len) );


        if ( data_tmp != old_data_tmp || cycle == 0) {
            int tmp_cycle = (cycle != 0 && is_input) ? cycle - 1 : cycle;
            if (!time_record)
                file << "#" << tmp_cycle << endl;

            file << "b";
            if (len == 0) {
                file << (data_tmp&(uint64_t)0x1);
            } else {
                for (int i = len-1; i >= 0 ; i--) {
                    bool bit = (data_tmp & ((uint64_t)0x01 << (uint32_t)i)) != 0;
                    file << bit;
                }
            }
            file << " " << (*iter).name <<endl;
            time_record = true;
        }
    }
}