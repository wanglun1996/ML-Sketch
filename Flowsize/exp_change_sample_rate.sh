#!/usr/bin/env bash

function run()
{
    local dataset=$1
    local type=$2
    local algo=${4}_${3}
    local sr=$4

    result=result_sample_rate/${1:9}_${algo}
    resultML=result_sample_rate/${1:9}_${algo}_ML
    data_algo=${1:9}_${algo}

    ./bin/FlowSizeIP $dataset $result $type 0 ${data_algo} ${sr}
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo} done.
    python FlowSizeML.py ./MLData/${data_algo}_sketch.train ./Para/${data_algo}_sketch.para
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo} Machine Learning dataset generated.
    ./bin/FlowSizeIP $dataset $resultML $type 1 ${data_algo} ${sr}
    echo `date +%Y-%m-%d\ %H:%M:%S` ${data_algo} Machine Learning done.
}

sr=(5 10 20 40 100 200 500)

for f in FlsmData/*
do
    echo `date +%Y-%m-%d\ %H:%M:%S` $f
    dataset=$f

    for r in ${sr[@]}
    do
        run $dataset 0 CM $r &
        run $dataset 2 CU $r &
        run $dataset 4 CSM $r &
    done

    r=1000
    run $dataset 0 CM $r &
    run $dataset 2 CU $r &
    run $dataset 4 CSM $r
done