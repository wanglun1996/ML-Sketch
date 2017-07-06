#!/usr/bin/env bash
ssh 1807 "cd jiangjie/1701_MLSketch/FlowSizeExp/; python drawing/analyze.py result result_syn 010"
#rm -rf drawing/out
scp -r 1807:/home/yangtong/jiangjie/1701_MLSketch/FlowSizeExp/drawing/out /home/surlavi/Lab/MLSketch/CIfSketch/FlowSize/drawing/
echo "Copy to local..Done!"