//
// Created by chenliang on 2019/8/14.
//

#ifndef CA_FIFO_SC_MODULE_H
#define CA_FIFO_SC_MODULE_H

#ifndef __SYNTHESIS__
#include "common.h"
#endif

class  ca_module {
public:
    virtual void run() = 0;
    virtual void update() = 0;

    virtual bool is_trace() = 0;
    virtual void dump_sigs(ofstream &file) = 0;
    virtual void dump_inputs(ofstream &file) = 0;

};
#endif //CA_FIFO_SC_MODULE_H
