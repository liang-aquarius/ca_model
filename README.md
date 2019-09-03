# ca_fifo

ca_fifo is an example of the cycle accurate modle.

The cycle accurate model is a C++ class based model for RTL and hardware modeling. It can be used as a software simulator before Verilog/VHDL coding. Although called “Cycle Accurate”, it can also describe transection level model, if you take “cycle” as “phase” in SystemC. If you do not want to use SystemC or any commercial EDA simulation tools such as VCS, this model is a good start for modeling and simulating your hardware design.
