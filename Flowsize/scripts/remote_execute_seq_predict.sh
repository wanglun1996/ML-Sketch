#!/usr/bin/env bash

ssh 1807 "cd jiangjie/1701_MLSketch/FlowSizeExp/result_seq; rm x*; rm u*"
echo "Clear result folder..."
ssh 1807 "cd jiangjie/1701_MLSketch/FlowSizeExp/; nohup sh exp_seq.sh 2>&1 &"
echo "Execution Done."

ssh 1807 "cd jiangjie/1701_MLSketch/FlowSizeExp/; python drawing/analyze.py result_seq result_syn 010 drawing/out/raw_seq_result"
scp -r 1807:/home/yangtong/jiangjie/1701_MLSketch/FlowSizeExp/drawing/out /home/surlavi/Lab/MLSketch/CIfSketch/FlowSize/drawing/
echo "Copy to local..Done!"
