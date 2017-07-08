# ML-Sketch

This repository contains the system implementation of ML-Sketch and Experimental Codes.

There are mainly three subdirectories under this repository: Sketch, sketch-dpdk, and Python.

## Sketch

This directory contains the implementation of all the sketches we mentioned in our paper (link) and their machine learning versions. There are three subdirectories: FlowSize, TopK and FlowNum corresponding to three case studies in the paper. You can enter the 'make' command to create the executable 'main' file. You can fill the main file with any code you want with our interface.

## dpdk-sketch

This repository contains the network-level implementation of our ML-Sketch using DPDK. There are two subdirectories: dpdk-sender and dpdk-receiver. You can link two computers with a switch and compile the sender and receiver end on each of them. Then it can do the packet sending and receiving job when monitoring the network metrics with our ML-Sketch Framework.

## Python

This repository mainly contains the python machine learning code for out experiments.

## Contribution

The main contributor of these codes are Tong Yang, PKU, Lun Wang, PKU, Jie Jiang, PKU, Junzhi Gong, PKU and Lingtong Liu, XiDian University. For any further questions, please contact Lun Wang by lun.wang@pku.edu.cn or wanglun@berkeley.edu.

