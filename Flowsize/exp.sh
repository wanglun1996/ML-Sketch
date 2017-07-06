#!/usr/bin/env bash

g++ main.cpp -std=c++11

dataset=FlowData/tmp_data
result=result/tmp_data
resultML=result/tmp_data_ml

./a.out $dataset $result 0 0
python FlowSizeML.py ./MLData/sketch.train ./Para/sketch.para
./a.out $dataset $resultML 0 1
