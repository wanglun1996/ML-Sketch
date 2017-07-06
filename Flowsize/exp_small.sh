#!/usr/bin/env bash

function run()
{
    local dataset=$1
    local type=$2
    local algo=$3

    result=result/${1:9}_${algo}
    resultML=result/${1:9}_${algo}_ML
    data_algo=${1:9}_${algo}
#    echo ${algo}

    ./bin/FlowSizeIP $dataset $result $type 0 ${data_algo}
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo} done.
    python FlowSizeML.py ./MLData/${data_algo}_sketch.train ./Para/${data_algo}_sketch.para
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo} Machine Learning dataset generated.
    ./bin/FlowSizeIP $dataset $resultML $type 1 ${data_algo}
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo} Machine Learning done.
}

for f in FlsmData/*
do
    echo `date +%Y-%m-%d\ %H:%M:%S` $f
    dataset=$f

    run $dataset 0 CM &
    run $dataset 1 CM_SAC &
    run $dataset 2 CU &
    run $dataset 3 CU_SAC &
    run $dataset 4 CSM &
    run $dataset 5 CSM_SAC &

done
