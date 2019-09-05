# ca_model

The cycle accurate model is a very simple C++ class based model for RTL and hardware modeling. It can be used as a perfomance model or software simulator before Verilog/VHDL coding, or golden C for rtl verification. Although called “Cycle Accurate”, it can also describe transection level model(TLM). This model is a good start for modeling and simulating your hardware design if you do not want to coding rtl/SystemC, nor to use any commercial EDA simulation tools.

The fundation of the model is in libs. A cycle accurate FIFO design as an example is in designs/ca_fifo. The test for the FIFO design is in tests/ca_fifo.
