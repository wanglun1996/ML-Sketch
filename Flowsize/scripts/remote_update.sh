#!/usr/bin/env bash

git push origin master
ssh 1807 "cd jiangjie/1701_MLSketch/FlowSizeExp/; git pull origin master; cd build; cmake ..; make"
echo "Update done."
