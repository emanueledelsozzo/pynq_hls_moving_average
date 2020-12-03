# PYNQ HLS Moving Average
This repository contains an HLS design of a moving average IP for PYNQ-based systems. 

## SETUP
We relied on Vivado HLS 2019.2 to perform the High-Level Synthesis step, Vivado 2019.2 to generate the bitstream, and Pynq 2.5 to evaluate the IP. In particular, we tested our IP on three Zynq-based platforms, namely [Ultra96-V2](https://www.avnet.com/wps/portal/us/products/new-product-introductions/npi/aes-ultra96-v2/), [Pynq Z2](http://www.pynq.io/board.html), and [ZCU104 ](https://www.xilinx.com/products/boards-and-kits/zcu104.html). 

## HLS
The HLS folder contains the IP HLS files <code>moving_average.cpp</code>, <code>moving_average.hpp</code>, and a testbench <code>testbench.cpp</code>.
The current version of the IP receives an input stream of integer values and produces an output stream of float averages. The first two elements of the input stream represent the number of input elements and the window of elements to average. Currently, the IP supports moving averages from 1 to 16 elements. You can modify this parameter within the <code>moving_average.hpp</code> file.

## Vivado Design
After exporting the IP from Vivado HLS, run Vivado and create an RTL project for your target board. Then, create a block design and add the following IPs:
<ul>
<li>Zynq IP (Zynq 7000 or Zynq Ultrascale+ MPSoC according to your board)</li>
<li>AXI DMA</li>
<li>Moving Average IP</li>
</ul>
Enable the High Performance (HP) port on the Zynq IP, disable the Scatter Gather Engine on the AXI DMA. We also suggest to set the AXI DMA Buffer Length Register to the maximum value. Run the available automations and, finally, connect the Moving Average IP to the AXI DMA and to the clock and reset wires. You can also create a hierarchy containing both the AXI DMA and the Moving Average IP. At this point, validate the design, create the HDL wrapper and run the bitstream generation.<br>
Once the bitstream is ready, deploy the following files to your target board:
<ul>
<li>bitstream file: <em>projectFolder/projectName.runs/impl_1/design_1_wrapper.bit</em></li>
<li>Block design file: <em>projectFolder/projectName.srcs/sources_1/bd/design_1/hw_handoff/design_1_bd.tcl</em></li>
  <li>hwh file: <em>projectFolder/projectName.srcs/sources_1/bd/design_1/hw_handoff/design_1.hwh</em></li>
</ul>
Remember that these files should have the same name and that you cannot change the name of the bitstream file.

## PYNQ evaluation
Python folder contains two Jupyter notebooks to evaluate the Moving Average IP: <code>moving_average.ipynb</code> and <code>moving_average_with_class.ipynb</code>. These files evaluate the IP similar to the HLS testbench using PYNQ APIs. <code>moving_average_with_class.ipynb</code> is still a work-in-progress notebook. It wraps the IP usage with a Python class, which works properly on the Pynq Z2 platform, but it causes a bus error on both Ultra96-V2 and ZCU104.
