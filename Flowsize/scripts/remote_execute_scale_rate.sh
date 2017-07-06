#!/usr/bin/env bash

ssh 1807 "cd jiangjie/1701_MLSketch/FlowSizeExp/result_scale_rate; rm 2013*"
echo "Clear result folder..."
ssh 1807 "cd jiangjie/1701_MLSketch/FlowSizeExp/; nohup sh exp_change_scale_rate.sh 2>&1 &"
echo "Execution Done."

ssh 1807 "cd jiangjie/1701_MLSketch/FlowSizeExp/; python drawing/analyze.py result_scale_rate result_syn 010 drawing/out/raw_scale_rate_result"
#rm -rf drawing/out
scp -r 1807:/home/yangtong/jiangjie/1701_MLSketch/FlowSizeExp/drawing/out /home/surlavi/Lab/MLSketch/CIfSketch/FlowSize/drawing/
echo "Copy to local..Done!"
