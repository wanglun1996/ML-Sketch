#!/usr/bin/env bash

ssh 1807 "cd jiangjie/1701_MLSketch/FlowSizeExp/result; rm *"
ssh 1807 "rm jiangjie/1701_MLSketch/FlowSizeExp/MLData/*"
ssh 1807 "rm jiangjie/1701_MLSketch/FlowSizeExp/Para/*"
echo "Clear result folder..."
ssh 1807 "cd jiangjie/1701_MLSketch/FlowSizeExp/; nohup sh exp_small.sh 2>&1 &"
#ssh 1807 "cd jiangjie/1701_MLSketch/FlowSizeExp/; nohup sh exp_syn.sh 2>&1 &"
echo "Execution Done."