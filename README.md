# ca_model

The cycle accurate model is a C++ class based model for RTL and hardware modeling. It can be used as a software simulator before Verilog/VHDL coding. Although called “Cycle Accurate”, it can also describe transection level model, if you take “cycle” as “phase” in SystemC. If you do not want to use SystemC or any commercial EDA simulation tools such as VCS, this model is a good start for modeling and simulating your hardware design.

The basic of the model is in utils. An cycle accurate FIFO design as example is in designs/ca_fifo. The test for the FIFO design is in tests/ca_fifo.
