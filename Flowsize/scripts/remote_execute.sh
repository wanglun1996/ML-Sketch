#!/usr/bin/env bash

ssh 1807 "cd jiangjie/1701_MLSketch/FlowSizeExp/; nohup sh exp_raw.sh 2>&1 &"
ssh 1807 "cd jiangjie/1701_MLSketch/FlowSizeExp/; nohup sh exp_syn.sh 2>&1 &"
echo "Execution Done."